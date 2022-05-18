#include "measurementnode.h"
//#include "time_measurement.h"

#include <QtNetwork>

#include "constants.h"
#include "logging.h"

MeasurementNode::MeasurementNode(QObject *parent)
    : QObject{parent},
      mLocalSocket(nullptr),
      mClientPort(MEASUREMENT_NODE_PORT),
      mMasterNodeAddress(QHostAddress(MASTER_NODE_IP)),
      mMasterNodePort(MASTER_NODE_PORT)
{
    mLocalSocket.reset(new QUdpSocket(this));

    mSendReceiveBuffer.resize(DEFAULT_DATAGRAM_MAX_SIZE);

//    mTimeMeasurement.reset(new TimeMeasurement());
}

MeasurementNode::~MeasurementNode() = default;

void MeasurementNode::start() {
    INFO_PRINT("Starting Measurement Node.");
    INFO_PRINT("Listening on UDP port " << mClientPort);
    INFO_PRINT("Server connection info: " << mMasterNodeAddress << ":" << mMasterNodePort);

    sound_measurement.init();

    const bool lcClientSocketBindSuccessful = mLocalSocket->bind(QHostAddress::Any, mClientPort);
    Q_ASSERT(lcClientSocketBindSuccessful);
    connect(mLocalSocket.data(), &QUdpSocket::readyRead, this, &MeasurementNode::readPendingDatagrams);

    // finally say hello to the master
    mSendReceiveBuffer[0] = SlaveToMasterMessage::SlaveToMasterMessage::HELLO_MSG;
    mLocalSocket->writeDatagram(mSendReceiveBuffer.data(), mSendReceiveBuffer.size(), mMasterNodeAddress, mMasterNodePort);
}

void MeasurementNode::readPendingDatagrams() {
//    unsigned int ts_time_master = 0;
//    quint32 ts = 0, ts_sec = 0, ts_usec = 0;
    long int measured_time;

//    mSendReceiveBuffer[DEFAULT_DATAGRAM_MAX_SIZE-1] = 1; // only for testing, remove it in production

    while ( mLocalSocket->hasPendingDatagrams()) {
        const qint64 lcPendingDatagramSize = mLocalSocket->pendingDatagramSize();
        const qint64 lcRet = mLocalSocket->readDatagram(mSendReceiveBuffer.data(), DEFAULT_DATAGRAM_MAX_SIZE);
        if (lcRet != -1) {

            //DEBUG_PRINT("Received " << qFromBigEndian<quint32>(mSendReceiveBuffer.data()));
            if (lcRet == 0) {
                WARN_PRINT("Received datagram of length 0.");
                continue;
            }
            else {
#ifdef VERBOSE_DEBUG_PRINTING
                QString data;
                QTextStream ss(&data);
                ss << "0x " << Qt::hex;
                for (int i = 0; i < DEFAULT_DATAGRAM_MAX_SIZE; i++) {
                    //DEBUG_PRINT("i=" << i << ": " << static_cast<qint8>(mSendReceiveBuffer.at(i)));
                    ss << static_cast<qint8>(mSendReceiveBuffer.at(i)) << " ";
                }
                DEBUG_PRINT("Received (" << lcPendingDatagramSize << "): " << data);
#endif // VERBOSE_DEBUG_PRINTING
            }

            // dispatch the protocol
            // Note: beware of network byte order (big endian) where first byte is at the last index
            switch (static_cast<int>(mSendReceiveBuffer.at(lcPendingDatagramSize-1))) {
                case MasterToSlaveMessage::START_ROUNDTRIP_MEAS:
                    DEBUG_PRINT("Start roundtrip measurement message received");
#if 0
                    // get the timestamp received via network packet,
                    // it is assumed that timestamp is a 32 bit signed integer
                    ts = qFromBigEndian<quint32>(mSendReceiveBuffer.data());

                    // decode the timestamp containing seconds and microseconds
                    // e.g. as returned by get time of day
                    ts_sec  = ts / 1000000;
                    ts_usec = ts % 1000000;

                    DEBUG_PRINT("Received the timestamp: " << ts << " (sec=" << ts_sec << ", usec=" << ts_usec << ")");

                    // TODO get the timestamp from the UDP datagram and put it int ts_time_master
//                    mTimeMeasurement->timesync_master(ts_time_master);
#endif // 0
                    // send ack back
                    mSendReceiveBuffer.fill(0, DEFAULT_DATAGRAM_MAX_SIZE);
                    mSendReceiveBuffer[0] = SlaveToMasterMessage::SlaveToMasterMessage::ACK_ROUNDTRIP;
                    mLocalSocket->writeDatagram(mSendReceiveBuffer.data(), mSendReceiveBuffer.size(), mMasterNodeAddress, mMasterNodePort);
//                    sendToMasterNode();

                    break;
                case MasterToSlaveMessage::START_TIME_MEAS:
                    DEBUG_PRINT("Start Measurement message received");

                    // TODO trigger the measurement and sent result back to master
                    //measured_time = sound_measurement.measurement()/1000;
                    measured_time = 2000000000;
                    DEBUG_PRINT("Measurement: " << measured_time);
                    QDataStream(&mSendReceiveBuffer, QIODevice::WriteOnly) << static_cast<quint8>(SlaveToMasterMessage::TIME_MEA) << static_cast<qint32>(measured_time); 
                    mLocalSocket->writeDatagram(mSendReceiveBuffer.data(), mSendReceiveBuffer.size(), mMasterNodeAddress, mMasterNodePort);
//                    mTimeMeasurement->measurement();

                    break;
                default:
                    ERROR_PRINT("Unknown message received: " << static_cast<int>(mSendReceiveBuffer.at(0)));
                    break;
            }
        }
    }
}
