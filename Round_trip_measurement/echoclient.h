#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QString>
#include <QScopedPointer>

#include <vector>

class QTimer;
class QUdpSocket;

struct SteadyClockTimePointWrapper;

class EchoClient : public QObject
{
    Q_OBJECT
public:
    explicit EchoClient(QObject *parent = nullptr);

    ~EchoClient();

    void setAddress(const QString &pAddress) {
        mServerAddress = QHostAddress(pAddress);
    }

    void setPort(quint16 pPort) {
        mServerPort = pPort;
    }

    void setClientPort(quint16 pPort) {
        mClientPort = pPort;
    }

    void setTimerInterval(int pInterval) {
        mTimerInterval = pInterval;
    }

    void setMaxNumPackets(unsigned int pMaxNumPackets);

    void setWaitForServerResponse(bool pWaitForResponse) {
        mWaitForServerResponse = pWaitForResponse;
    }

    void setCountValue(quint32 pCounter) {
        mCounter = pCounter;
    }

public slots:
    void start();

private slots:
    void sendUdpEchoDatagram();

    void readPendingDatagrams();

private:
    Q_DISABLE_COPY(EchoClient)

    QHostAddress mServerAddress;
    quint16 mServerPort;
    QTimer *mTimer;
    int mTimerInterval;
    QUdpSocket *mClientSocket;
    quint16 mClientPort;
    QUdpSocket *mServerSocket;

    quint32 mCounter;
    QByteArray mSendReceiveBuffer;

    unsigned int mMaxNumPackets;
    bool mWaitForServerResponse;

    QScopedPointer<SteadyClockTimePointWrapper> mStart;
    QScopedPointer<SteadyClockTimePointWrapper> mStop;

    std::vector<int> mDurations;
    int mErrorCnt;
    bool mFirstValue;
};

#endif // ECHOCLIENT_H
