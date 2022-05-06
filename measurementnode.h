#ifndef MEASUREMENTNODE_H
#define MEASUREMENTNODE_H

#include <QObject>
#include <QScopedPointer>
#include <QHostAddress>

#include "protocol.h"

class QUdpSocket;
class QNetworkDatagram;
//class TimeMeasurement;

class MeasurementNode : public QObject
{
    Q_OBJECT
public:
    explicit MeasurementNode(QObject *parent = nullptr);
    ~MeasurementNode();

    void setClientPort(quint16 pClientPort) { mClientPort = pClientPort; }
    void setMasterNodeAddress(const QString &pAddress) {
        mMasterNodeAddress = QHostAddress(pAddress);
    }
    void setMasterNodePort(quint16 pPort) {
        mMasterNodePort = pPort;
    }

    /**
     * @brief sendToMasterNode Sends a message to the master node.
     * @return In case of error this function returns -1 same as QUdpSocket::writeDatagram().
     */
    qint64 sendToMasterNode();

signals:

public slots:
    void start();

private slots:
    void readPendingDatagrams();

private:
    Q_DISABLE_COPY(MeasurementNode)

    QScopedPointer<QUdpSocket> mLocalSocket;   // for bind, to receive (read) data
    QScopedPointer<QUdpSocket> mRemoteSocket;  // to write data to remote hosts

    QScopedPointer <QNetworkDatagram> mMasterDatagram;

    quint16 mClientPort;

    QByteArray mSendReceiveBuffer;

    QHostAddress mMasterNodeAddress;
    quint16 mMasterNodePort;

//    QScopedPointer<TimeMeasurement> mTimeMeasurement;
};

#endif // MEASUREMENTNODE_H
