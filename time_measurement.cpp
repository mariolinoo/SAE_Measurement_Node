#include "time_measurement.h"
#include <QtGlobal>
#include <iostream>

using namespace std;

#define PIN RPI_BPLUS_GPIO_J8_05

void TimeMeasurement::init()
{
    //Init GPIO 
    cout<<"Init start"<<"\n";
    if(!bcm2835_init())
        cout<<"Failed Init"<<"\n";
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_DOWN);

    //Time GPIO
    this->values.start.tv_sec  = 0;
    this->values.start.tv_nsec = 0;
    this->values.end.tv_sec  = 0;
    this->values.end.tv_nsec = 0;

    this->time_delay_comm   = 0;
    this->time_delay_master = 0;

    this->initialized = 1;

    cout<<"Init end"<<"\n";
    return;
}

void TimeMeasurement::timesync_communication(long long measured_delay)
{
    this->time_delay_comm = measured_delay;

    return;
}

long long TimeMeasurement::measurement(void)
{
    cout<<"Measurement start"<<"\n";
    long long tmp; 
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    this->values.start = time; 
    bcm2835_gpio_hen(PIN);
    while(1)
    {
        if(bcm2835_gpio_eds(PIN))
        {
            bcm2835_gpio_set_eds(PIN);
            break;
        }
        //cout<<"measuring"<<"\n";
    }
    cout<<"sound detected"<<"\n";
    measurementinterrupt();
    //Measured time calc - prototype
    tmp = (((this->values.start.tv_sec * 1000000000L) + this->values.start.tv_nsec) - this->time_delay_comm);
    this->values.elapsed = ((this->values.start.tv_sec * 1000000000L) + this->values.start.tv_nsec) - ((this->values.end.tv_sec * 1000000000L) + this->values.end.tv_nsec);

    return this->values.elapsed;
}

void TimeMeasurement::measurementinterrupt()
{
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    this->values.end = time;
}
