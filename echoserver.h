#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include <QtNetwork>

class EchoServer : public QUdpSocket
{
    Q_OBJECT
public:
    explicit EchoServer(QObject *parent = nullptr);

    void setPort(quint16 pPort) {
        mPort = pPort;
    }

    void setClientAddress(const QString &pAddress) {
        mClientAddress = pAddress;
    }

    void setClientPort(quint16 pPort) {
        mClientPort = pPort;
    }

public slots:
    void start();

private slots:
    void readPendingDatagrams();

private:
    Q_DISABLE_COPY(EchoServer)

    quint16 mPort;
    QString mClientAddress;
    quint16 mClientPort;
    QUdpSocket *mClient;

    QByteArray mSendReceiveBuffer;

    int mErrorCnt;
};

#endif // ECHOSERVER_H
