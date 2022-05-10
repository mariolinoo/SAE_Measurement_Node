#include <sys/time.h>   //Time library 
#include <wiringPi.h>   //Gpio ports 


class TimeMeasurement {
    private:
        //CPU time
        struct timespec cpu_time;
        //Time delay on the communication
        long long time_delay_comm;
        //Time delay to master cpu
        long long time_delay_master; 
        //General indicator
        bool initialized = false; 
        //Measurment
        typedef struct{
            struct timespec start;
            struct timespec end; 
            long long elapsed;
            bool    ready_to_calc;
        }measurement; 

        measurement values;

        //Interrupt for sensor measurement
        void measurementinterrupt();

    public: 

        //Initializes all variables and ports
        void init();

        //Calculates time delay to master
        void timesync_master(unsigned int time_master); //whsl egal?

        //Calculates time delay on bus 
        void timesync_communication(unsigned int measured_delay);

        //Measurment 
        unsigned int measurement();
    
};
