#include "measurementnode.h"
#include "time_measurement.h"

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

    mTimeMeasurement.reset(new TimeMeasurement());
}

MeasurementNode::~MeasurementNode() = default;

void MeasurementNode::start() {
    INFO_PRINT("Starting Measurement Node.");
    INFO_PRINT("Listening on UDP port " << mClientPort);
    INFO_PRINT("Server connection info: " << mMasterNodeAddress << ":" << mMasterNodePort);

    const bool lcClientSocketBindSuccessful = mLocalSocket->bind(QHostAddress::Any, mClientPort);
    Q_ASSERT(lcClientSocketBindSuccessful);
    connect(mLocalSocket.data(), &QUdpSocket::readyRead, this, &MeasurementNode::readPendingDatagrams);
}

void MeasurementNode::readPendingDatagrams() {
    unsigned int ts_time_master = 0;

    while ( mLocalSocket->hasPendingDatagrams()) {
        const qint64 lcRet = mLocalSocket->readDatagram(mSendReceiveBuffer.data(), DEFAULT_DATAGRAM_MAX_SIZE);
        if (lcRet != -1) {

            DEBUG_PRINT("Received " << qFromBigEndian<quint32>(mSendReceiveBuffer.data()));
            if (lcRet == 0) {
                WARN_PRINT("Received datagram of length 0.");
                continue;
            }

            // dispatch the protocol
            // Note: beware of network byte order (big endian) where first byte is at the last index
            switch (static_cast<int>(mSendReceiveBuffer.at(DEFAULT_DATAGRAM_MAX_SIZE-1))) {
                case MasterToSlaveMessage::SYNC_TIME:
                    DEBUG_PRINT("TimeSync message received");
                    // TODO update the timestamp
                    // Not yet clear how a timestamp sent by the master looks like
                    // e.g. 1 byte will be most likely not enough for the timestamp

                    // TODO get the timestamp from the UDP datagram and put it int ts_time_master
                    mTimeMeasurement->timesync_master(ts_time_master);

                    break;
                case MasterToSlaveMessage::START_MEAS:
                    DEBUG_PRINT("Start Measurement message received");

                    // TODO what does this function do - it may not block the event loop
                    mTimeMeasurement->measurement();

                    break;
                // TODO other cases - maybe in first version just ignore all other messages
                default:
                    // TODO handle error
                    DEBUG_PRINT("Unknown message received: " << static_cast<int>(mSendReceiveBuffer.at(0)));
                    break;
            }
        }
    }
}

qint64 MeasurementNode::sendToMasterNode(SlaveToMasterMessage::SlaveToMasterMessage msg, quint8 byte1) {
    QDataStream(&mSendReceiveBuffer, QIODevice::WriteOnly) << static_cast<quint8>(msg) << byte1;

    return mRemoteSocket->writeDatagram(mSendReceiveBuffer.data(), mSendReceiveBuffer.size(), mMasterNodeAddress, mMasterNodePort);
}
