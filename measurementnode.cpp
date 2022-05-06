#include "measurementnode.h"
//#include "time_measurement.h"

#include <QtNetwork>

#include "constants.h"
#include "logging.h"

MeasurementNode::MeasurementNode(QObject *parent)
    : QObject{parent},
      mLocalSocket(nullptr),
      mRemoteSocket(nullptr),
      mClientPort(MEASUREMENT_NODE_PORT),
      mMasterNodeAddress(QHostAddress(MASTER_NODE_IP)),
      mMasterNodePort(MASTER_NODE_PORT)
{
    mLocalSocket.reset(new QUdpSocket(this));
    mRemoteSocket.reset(new QUdpSocket(this));

    mSendReceiveBuffer.resize(DEFAULT_DATAGRAM_MAX_SIZE);

    mMasterDatagram.reset(new QNetworkDatagram(mSendReceiveBuffer, mMasterNodeAddress, mMasterNodePort));
    mMasterDatagram->setSender(QHostAddress(MEASUREMENT_NODE_IP), MEASUREMENT_NODE_PORT);

//    mTimeMeasurement.reset(new TimeMeasurement());
}

MeasurementNode::~MeasurementNode() = default;

void MeasurementNode::start() {
    INFO_PRINT("Starting Measurement Node.");
    INFO_PRINT("Listening on UDP port " << mClientPort);
    INFO_PRINT("Server connection info: " << mMasterNodeAddress << ":" << mMasterNodePort);

    const bool lcClientSocketBindSuccessful = mLocalSocket->bind(QHostAddress::Any, mClientPort);
    Q_ASSERT(lcClientSocketBindSuccessful);
    connect(mLocalSocket.data(), &QUdpSocket::readyRead, this, &MeasurementNode::readPendingDatagrams);
    // not working with QUdpSocket which is not bound to a HostAddress:Port
//    connect(mRemoteSocket.data(), &QUdpSocket::readyRead, this, &MeasurementNode::readPendingDatagrams);

    // finally say hello to the master
    mSendReceiveBuffer[0] = SlaveToMasterMessage::SlaveToMasterMessage::HELLO_MSG;
//    mRemoteSocket->writeDatagram(mSendReceiveBuffer.data(), mSendReceiveBuffer.size(), mMasterNodeAddress, mMasterNodePort);
    sendToMasterNode();
}

void MeasurementNode::readPendingDatagrams() {
//    unsigned int ts_time_master = 0;
//    quint32 ts = 0, ts_sec = 0, ts_usec = 0;

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
                // TODO for production disable this
                QString data;
                QTextStream ss(&data);
                ss << "0x " << Qt::hex;
                for (int i = 0; i < DEFAULT_DATAGRAM_MAX_SIZE; i++) {
                    //DEBUG_PRINT("i=" << i << ": " << static_cast<qint8>(mSendReceiveBuffer.at(i)));
                    ss << static_cast<qint8>(mSendReceiveBuffer.at(i)) << " ";
                }
                DEBUG_PRINT("Received (" << lcPendingDatagramSize << "): " << data);
            }

            // dispatch the protocol
            // Note: beware of network byte order (big endian) where first byte is at the last index
            // TODO seems to be in first byte from C application (master), but maybe because it only sends 1 byte --> check received length
            switch (static_cast<int>(mSendReceiveBuffer.at(0))) {
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
//                    mRemoteSocket->writeDatagram(mSendReceiveBuffer.data(), mSendReceiveBuffer.size(), mMasterNodeAddress, mMasterNodePort);
                    sendToMasterNode();

                    break;
                case MasterToSlaveMessage::START_TIME_MEAS:
                    DEBUG_PRINT("Start Measurement message received");

                    // TODO trigger the measurement and sent result back to master
//                    mTimeMeasurement->measurement();

                    break;
                default:
                    ERROR_PRINT("Unknown message received: " << static_cast<int>(mSendReceiveBuffer.at(0)));
                    break;
            }
        }
    }
}

qint64 MeasurementNode::sendToMasterNode() {
//    QDataStream(&mSendReceiveBuffer, QIODevice::WriteOnly) << static_cast<quint8>(msg) << byte1;

    mMasterDatagram->setData(mSendReceiveBuffer);
    DEBUG_PRINT("Sending data to master node: senderAddress=" << mMasterDatagram->senderAddress() << ", senderPort=" << mMasterDatagram->senderPort());
    return mRemoteSocket->writeDatagram(*mMasterDatagram);
}
