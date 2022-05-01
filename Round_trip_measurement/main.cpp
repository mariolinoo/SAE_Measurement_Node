/**
 * UDP echo application/ Measurement node for SAE project.
 *
 * @ UDP echo application:
 * Client sends echo packet to server which sends it back to the client.
 * Time measurement functionality in client to calculate round trip time.
 *
 * @ Measurement node for SAE project:
 * TODO
 */
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

// If this build flag is set the UDP echo application is built
// deprecated: use the dedicated project sae_measurement_node
// (sae_measurement_node.pro) for it.
#define BUILD_ROUND_TRIP_MEAS_APP

#ifdef BUILD_ROUND_TRIP_MEAS_APP

#include "echoclient.h"
#include "echoserver.h"

#else // !BUILD_ROUND_TRIP_MEAS_APP

#include "measurementnode.h"

#endif // BUILD_ROUND_TRIP_MEAS_APP

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser lParser;
    const QCommandLineOption lcHelpOption = lParser.addHelpOption();
    const QCommandLineOption lcVersionOption = lParser.addVersionOption();

    #ifdef BUILD_ROUND_TRIP_MEAS_APP
    lParser.setApplicationDescription("UDP echo application");

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

    const QCommandLineOption lcClientNoWaitForResponseOption(QStringList() << "w" << "no-wait",
            "Don't wait in the client application for a response of the previous sent echo request.");
    lParser.addOption(lcClientNoWaitForResponseOption);

    const QCommandLineOption lcSetCounterStartValueOption(QStringList() << "s" << "counter-start",
            "Set the start value of the counter to send in the echo request.", "counter_value");
    lParser.addOption(lcSetCounterStartValueOption);

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
    bool lPortValid = false, lRemotePortValid = false, lAddressValid = false,
            lDontWaitForResponse = false;
    quint32 lCounter = 0;
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
    if (lParser.isSet(lcClientNoWaitForResponseOption)) {
        lDontWaitForResponse = true;
    }
    if (lParser.isSet(lcSetCounterStartValueOption)) {
        bool lValid;
        lCounter = static_cast<quint32>(lParser.value(lcSetCounterStartValueOption).toUInt(&lValid));
        if (!lValid) {
            qDebug() << "Invalid counter value given. Using 0 as default.";
            lCounter = 0;
        }
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
        lClient->setWaitForServerResponse(!lDontWaitForResponse);
        lClient->setCountValue(lCounter);
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

#else  // !BUILD_ROUND_TRIP_MEAS_APP

    lParser.setApplicationDescription("Measurement node for SAE project");

    const QCommandLineOption lcPortOption(QStringList() << "p" << "port",
            "The UDP port to use for this measurement node.", "port");
    lParser.addOption(lcPortOption);

    const QCommandLineOption lcMasterAddressOption(QStringList() << "m" << "master",
            "The ip address of the master node.", "ip");
    lParser.addOption(lcMasterAddressOption);

    const QCommandLineOption lcMasterPortOption(QStringList() << "r" << "remote-port",
            "The remote port of the master node.", "port");
    lParser.addOption(lcMasterPortOption);

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
    if (lParser.isSet(lcMasterAddressOption)) {
        lAddress = lParser.value(lcMasterAddressOption);
        lAddressValid = true;
    }
    if (lParser.isSet(lcPortOption)) {
        lPort = static_cast<quint16>(lParser.value(lcPortOption).toUInt(&lPortValid));
    }
    if (lParser.isSet(lcMasterPortOption)) {
        lRemotePort = static_cast<quint16>(lParser.value(lcMasterPortOption).toUInt(&lRemotePortValid));
    }

    QScopedPointer<QObject> lObject;
    MeasurementNode *node = new MeasurementNode();
    lObject.reset(node);

    if (lPortValid) {
        node->setClientPort(lPort);
    }
    if (lAddressValid) {
        node->setMasterNodeAddress(lAddress);
    }
    if (lRemotePortValid) {
        node->setMasterNodePort(lRemotePort);
    }

#endif // BUILD_ROUND_TRIP_MEAS_APP

    QMetaObject::invokeMethod(lObject.data(), "start", Qt::QueuedConnection);

    return a.exec();
}
