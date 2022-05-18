#include <stdio.h>
#include <QCoreApplication>
#include <iostream>

#include "measurementnode.h"

int main(int argc, char *argv[])
{    
    QCoreApplication a(argc, argv);

    QScopedPointer<MeasurementNode> lObject(new MeasurementNode);

    QMetaObject::invokeMethod(lObject.data(), "start", Qt::QueuedConnection);
    
    // Blocking call to event loop - required for Qt network functionality
    // no separate while(true) endless loop needed.
    // Function calls to time_measurement.h functions are directly performed within
    // the measurement node (each time a UDP request arrives).
    return a.exec();

    /*
    //Startet komm funktionen
    while(true)
    {
        //fragt komm funktionen ab (flag/funktionsaufruf/public varibale oder struct)
        //ggf werden die zeitfunktionen dann aufgerufen
        INFO_PRINT("Hello from the main loop");

    }
    return result;
    */
}
