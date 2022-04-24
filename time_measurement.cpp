#include "time_measurement.h"

#include <QtGlobal>

void TimeMeasurement::timesync_master(unsigned int time_master)
{
    //Welche Zeit soll dafür genommen werden -> zu welchen Zeitpunkt soll die Zeit lokal festgestellt werden?
    Q_UNUSED(time_master);

    return;
}

void TimeMeasurement::timesync_communication(unsigned int measured_delay)
{
    time_delay_comm = measured_delay;

    return;
}

unsigned int TimeMeasurement::measurement()
{
    return 0;
}
