#include "echoclient.h"
#include "common.h"

#include <QFile>
#include <QtNetwork>
#include <QTimer>
#include <QTextStream>

#include <sys/time.h>

struct SteadyClockTimePointWrapper {
    explicit SteadyClockTimePointWrapper() {
      gettimeofday(&mTimePoint, nullptr);
    }

    struct timeval mTimePoint;
};

EchoClient::EchoClient(QObject *parent)
    : QObject(parent),
      mServerAddress(QHostAddress("127.0.0.1")),
      mServerPort(4242),
      mTimer(nullptr),
      mTimerInterval(3000),
      mClientSocket(nullptr),
      mClientPort(7000),
      mServerSocket(nullptr),
      mCounter(0),
      mSendReceiveBuffer(),
      mMaxNumPackets(50000),
      mStart(nullptr),
      mStop(nullptr),
      mDurations(),
      mErrorCnt(0),
      mFirstValue(true)
{
    mTimer = new QTimer(this);
    mTimer->setTimerType(Qt::PreciseTimer);
    mClientSocket = new QUdpSocket(this);
    mServerSocket = new QUdpSocket(this);
    mSendReceiveBuffer.reserve(sizeof(quint32));

    //std::chrono::steady_clock::time_point p2 = std::chrono::high_resolution_clock::now();
    //std::chrono::duration_cast<std::chrono::microseconds>(p2 - p1);
}

EchoClient::~EchoClient() = default;

void EchoClient::setMaxNumPackets(unsigned int pMaxNumPackets) {
    mMaxNumPackets = pMaxNumPackets;
    const auto lcNewSize = pMaxNumPackets + static_cast<unsigned int>(0.1 * pMaxNumPackets);
#ifdef ENABLE_DEBUG_PRINTING
    qDebug() << "Reserving " << lcNewSize << " entries in mDurations.";
#endif // ENABLE_DEBUG_PRINTING
    mDurations.reserve(lcNewSize);
}

void EchoClient::start() {
    qDebug() << "Starting Client UDP send timer with interval " << mTimerInterval << " ms and maxNumPackets " << mMaxNumPackets;
    qDebug() << "Listening on UDP port " << mClientPort;
    qDebug() << "Server connection info: " << mServerAddress << ":" << mServerPort;

    const bool lcClientSocketBindSuccessful = mClientSocket->bind(QHostAddress::Any, mClientPort);
    Q_ASSERT(lcClientSocketBindSuccessful);
    connect(mClientSocket, &QUdpSocket::readyRead, this, &EchoClient::readPendingDatagrams);

    mTimer->setInterval(mTimerInterval);
    connect( mTimer, &QTimer::timeout, this, &EchoClient::sendUdpEchoDatagram );
    mTimer->start();
}

void EchoClient::sendUdpEchoDatagram() {
#ifdef ENABLE_DEBUG_PRINTING
    qDebug() << "EchoClient::sendUdpEchoDatagram";
#endif // ENABLE_DEBUG_PRINTING

    if (!mStart.isNull()) {
      // still waiting on answer, skipping this timer
      qDebug() << "Waiting on server response, skipping this timer.";
      return;
    }

    // prepare data to be sent to server
    QDataStream(&mSendReceiveBuffer, QIODevice::WriteOnly) << mCounter;
    mCounter++;
#ifdef ENABLE_DEBUG_PRINTING
    qDebug() << "mSendReceiveBuffer: " << mSendReceiveBuffer;
#endif // ENABLE_DEBUG_PRINTING

    // !!! Time Measurement START !!!
    mStart.reset(new SteadyClockTimePointWrapper);
    const qint64 lcRet = mServerSocket->writeDatagram(mSendReceiveBuffer.data(), mSendReceiveBuffer.size(), mServerAddress, mServerPort);
    if (lcRet == -1) {
      // handle the error
      mErrorCnt++;
#ifdef ENABLE_DEBUG_PRINTING2
      qDebug() << "EchoClient::sendUdpEchoDatagram error count: " << mErrorCnt;
#endif // ENABLE_DEBUG_PRINTING
    }
}

void EchoClient::readPendingDatagrams() {
    if (mClientSocket->hasPendingDatagrams()) {

        const qint64 lcRet = mClientSocket->readDatagram(mSendReceiveBuffer.data(), mSendReceiveBuffer.size());
        if (lcRet != -1) {
          // !!! Time Measurement STOP !!!
          mStop.reset(new SteadyClockTimePointWrapper);

          const int lcDuration = static_cast<int>((mStop->mTimePoint.tv_sec * 1000000 + mStop->mTimePoint.tv_usec) -
                    (mStart->mTimePoint.tv_sec * 1000000 + mStart->mTimePoint.tv_usec));

#ifdef ENABLE_DEBUG_PRINTING
          qDebug() << "Received " << qFromBigEndian<quint32>(mSendReceiveBuffer.data());
          //qDebug() << "Duration: " << duration_cast<microseconds>(mStop->mTimePoint - mStart->mTimePoint).count() << " microseconds.";
          qDebug() << "Duration: " << lcDuration << " microseconds";
#endif // ENABLE_DEBUG_PRINTING

          mStart.reset();
          mStop.reset();

          if (mFirstValue) {
            // skip first value since measurement is off by more than 2 times compared to other value
            // most likely there some memory allocation etc. happens
            mFirstValue = false;
            return;
          }

          mDurations.push_back(lcDuration);
          if (mDurations.size() == mMaxNumPackets) {
            // dump the data to a file
            qDebug() << "Dumping " << mDurations.size() << " values to csv file. Had " << mErrorCnt << " errors.";
            QString lOutputData;
            QTextStream lsStream(&lOutputData);
            for (size_t i = 0; i< mDurations.size(); i++) {
              lsStream << mDurations[i] << "\n";
            }

            QFile lFile("measurement.csv");
            if (lFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
              QTextStream lStream(&lFile);

              lStream << lOutputData;

              lFile.close();

              qDebug() << "Terminating the program ...";
              QCoreApplication::quit();
            }
            else {
              qWarning() << "Error: Opening the file 'measurement.csv' failed. Dumping data to log ...\n" << lOutputData;
            }
          }
          else {
#ifdef ENABLE_DEBUG_PRINTING
            qDebug() << mDurations.size() << "/" << mMaxNumPackets;
#endif // ENABLE_DEBUG_PRINTING
          }
        }
        else {
          // handle the error
          mErrorCnt++;
#ifdef ENABLE_DEBUG_PRINTING
          qDebug() << "EchoClient::readPendingDatagrams error cnt: " << mErrorCnt;
#endif // ENABLE_DEBUG_PRINTING
        }
    }
}
