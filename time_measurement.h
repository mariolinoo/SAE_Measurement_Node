#ifndef TIME_MEASUREMENT_H
#define TIME_MEASUREMENT_H


#include <time.h>   //Time library 
#include <bcm2835.h> //GPIO lib


class TimeMeasurement {
    private:
        //Time delay on the communication
        long time_delay_comm;
        //Time delay to master cpu
        long time_delay_master; 
        //General indicator
        bool initialized = false; 
        //Measurment
        typedef struct{
            struct timespec start;
            struct timespec end; 
            long elapsed;
        }measurement_struct; 

        measurement_struct values;

        //Interrupt for sensor measurement
        void measurementinterrupt();

    public: 


        //Initializes all variables and ports
        void init();

        //Measurment 
        long measurement(void);
    
};
#endif