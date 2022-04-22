#include "echoserver.h"
#include "common.h"

EchoServer::EchoServer(QObject *parent)
    : QUdpSocket(parent),
      mPort(4242),
      mClientAddress("127.0.0.1"),
      mClientPort(7000),
      mClient(nullptr),
      mSendReceiveBuffer(),
      mErrorCnt(0)
{
    mClient = new QUdpSocket(this);
    mSendReceiveBuffer.resize(sizeof(quint32));
}

void EchoServer::start() {
    qDebug() << "Listening on UDP port " << mPort;
    qDebug() << "Client connection info: " << mClientAddress << ":" << mClientPort;
    const bool lcSocketBindSuccessful = bind( QHostAddress::Any, mPort );
    Q_ASSERT(lcSocketBindSuccessful);
    connect( this, &QUdpSocket::readyRead, this, &EchoServer::readPendingDatagrams );
}

void EchoServer::readPendingDatagrams() {
    while ( hasPendingDatagrams()) {

#ifdef ENABLE_DEBUG_PRINTING2
        const quint64 lcSize = pendingDatagramSize();
#endif // ENABLE_DEBUG_PRINTING2

        qint64 lcRet = readDatagram(mSendReceiveBuffer.data(), mSendReceiveBuffer.size());

#ifdef ENABLE_DEBUG_PRINTING2
        qDebug() << "Data: " << mSendReceiveBuffer <<  ", pendingDatagramSize()=" << lcSize;
#endif // ENABLE_DEBUG_PRINTING2

        // echo back
        if (lcRet != -1) {
            mClient->writeDatagram(mSendReceiveBuffer.data(), QHostAddress(mClientAddress), mClientPort);
        }
        else {
           // handle the error
           mErrorCnt++;
#ifdef ENABLE_DEBUG_PRINTING2
           qDebug() << "EchoServer::readPendingDatagrams error count: " << mErrorCnt;
#endif // ENABLE_DEBUG_PRINTING2
        }
    }
}
