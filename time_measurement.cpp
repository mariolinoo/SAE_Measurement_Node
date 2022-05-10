#include "time_measurement.h"
#include <QtGlobal>

using namespace std;

void TimeMeasurement::init()
{
    //Init GPIO 
    wiringPiSetup();
    pinMode(0, INPUT);

    //Time GPIO
    this->values.start.tv_sec  = 0;
    this->values.start.tv_nsec = 0;
    this->values.end.tv_sec  = 0;
    this->values.end.tv_nsec = 0;

    this->values.ready_to_calc = FALSE;
    this->values.done = FALSE;

    this->time_delay_comm   = 0;
    this->time_delay_master = 0;

    this->initialized = TRUE;
}

void TimeMeasurement::timesync_master(timespec time_master)
{
    clock_gettime(CLOCK_REALTIME, &cpu_time);
    this->time_delay_master = ((cpu_time.tv_sec * 1000000000L) + cpu_time.tv_nsec) - ((time_master.tv_sec * 1000000000L) + time_master.tv_nsec);

    return;
}

void TimeMeasurement::timesync_communication(long long measured_delay)
{
    this->time_delay_comm = measured_delay;

    return;
}

long long TimeMeasurement::measurement()
{
    long long tmp; 
    clock_gettime(CLOCK_REALTIME, &cpu_time);
    this->value.start = cpu_time; 
    wiringPiISR(0, INT_EDGE_RISING, this->measurementinterrupt)
    while(this->value.ready_to_calc != TRUE);
    
    //Measured time calc - prototype
    tmp = (((this->values.start.tv_sec * 1000000000L) + this->values.start.tv_nsec) - this->time_delay_comm);
    this->values.elapsed = ((this->values.start.tv_sec * 1000000000L) + this->values.start.tv_nsec) - ((this->values.end.tv_sec * 1000000000L) + this->values.end.tv_nsec);

    return this->values.elapsed;
}

void TimeMeasurement::measurementinterrupt()
{
    clock_gettime(CLOCK_REALTIME, &cpu_time);
    this->values.end = cpu_time;
    this->values.ready_to_calc = TRUE;
}
