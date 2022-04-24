#include<time.h>

class TimeMeasurement {
    private:
        //CPU time
        struct timespec cpu_time;
        //Time delay on the communication
        unsigned int time_delay_comm;
        //Time delay to master cpu
        unsigned int time_delay_master; 
        //General indicator
        bool initialized = false; 
        //Measurment
        typedef struct{
            struct timespec start;
            struct timespec end; 
            unsigned int result;
            bool    done;
        }measurment; 

    public: 
        //Calculates time delay to master
        void timesync_master(unsigned int time_master);

        //Calculates time delay on bus 
        void timesync_communication(unsigned int measured_delay);

        //Measurment 
        unsigned int measurement();
    
};
