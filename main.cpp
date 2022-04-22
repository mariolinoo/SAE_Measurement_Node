/**
 * UDP echo application.
 * Client sends echo packet to server which sends it back to the client.
 * Time measurement functionality in client to calculate round trip time.
 */
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "echoclient.h"
#include "echoserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser lParser;
    lParser.setApplicationDescription("UDP echo application");
    const QCommandLineOption lcHelpOption = lParser.addHelpOption();
    const QCommandLineOption lcVersionOption = lParser.addVersionOption();

    const QCommandLineOption lcClientApplicationOption(QStringList() << "c" << "client",
            "Run the client application.");
    lParser.addOption(lcClientApplicationOption);

    const QCommandLineOption lcAddressOption(QStringList() << "a" << "address",
            "The ip address to use.", "ip");
    lParser.addOption(lcAddressOption);

    const QCommandLineOption lcPortOption(QStringList() << "p" << "port",
            "The port to use.", "port");
    lParser.addOption(lcPortOption);

    const QCommandLineOption lcRemotePortOption(QStringList() << "r" << "remote-port",
            "The remote port to use.", "port");
    lParser.addOption(lcRemotePortOption);

    const QCommandLineOption lcIntervalOption(QStringList() << "i" << "interval",
            "The interval to use for the timer sending UDP packets.", "interval");
    lParser.addOption(lcIntervalOption);

    const QCommandLineOption lcMaxNumPacketsToSendOption(QStringList() << "m" << "max-packets",
            "Maximum number of UDP packets to send.", "max_packets");
    lParser.addOption(lcMaxNumPacketsToSendOption);

    if (!lParser.parse(QCoreApplication::arguments())) {
        lParser.showHelp();
        return -1;
    }

    if (lParser.isSet(lcHelpOption)) {
        lParser.showHelp();
        return 0;
    }
    if (lParser.isSet(lcVersionOption)) {
        lParser.showVersion();
        return 0;
    }

    quint16 lPort = 0, lRemotePort = 0;
    QString lAddress;
    bool lPortValid = false, lRemotePortValid = false, lAddressValid = false;
    if (lParser.isSet(lcAddressOption)) {
        lAddress = lParser.value(lcAddressOption);
        lAddressValid = true;
    }
    if (lParser.isSet(lcPortOption)) {
        lPort = static_cast<quint16>(lParser.value(lcPortOption).toUInt(&lPortValid));
    }
    if (lParser.isSet(lcRemotePortOption)) {
        lRemotePort = static_cast<quint16>(lParser.value(lcRemotePortOption).toUInt(&lRemotePortValid));
    }

    int lTimerIntervalMs = 3000;
    unsigned int lMaxNumPackets = 50000;
    if (lParser.isSet(lcIntervalOption)) {
      lTimerIntervalMs = lParser.value(lcIntervalOption).toInt();
    }
    if (lParser.isSet(lcMaxNumPacketsToSendOption)) {
      lMaxNumPackets = lParser.value(lcMaxNumPacketsToSendOption).toInt();
    }

    QScopedPointer<QObject> lObject;
    if (lParser.isSet(lcClientApplicationOption)) {
        qDebug() << "Starting Client application";
        EchoClient *lClient = new EchoClient();
        lObject.reset(lClient);
        if (lAddressValid) {
            lClient->setAddress(lAddress);
        }
        if (lPortValid) {
            lClient->setClientPort(lPort);
        }
        if (lRemotePortValid) {
            lClient->setPort(lRemotePort);
        }
        lClient->setTimerInterval(lTimerIntervalMs);
        lClient->setMaxNumPackets(lMaxNumPackets);
    } else {
        qDebug() << "Starting Server application";
        EchoServer *lServer = new EchoServer();
        lObject.reset(lServer);
        if (lAddressValid) {
            lServer->setClientAddress(lAddress);
        }
        if (lPortValid) {
            lServer->setPort(lPort);
        }
        if (lRemotePortValid) {
            lServer->setClientPort(lRemotePort);
        }
    }
    QMetaObject::invokeMethod(lObject.data(), "start", Qt::QueuedConnection);

    return a.exec();
}
