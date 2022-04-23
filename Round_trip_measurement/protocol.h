#ifndef PROTOCOL_H
#define PROTOCOL_H

namespace MasterToSlaveMessage {
    enum MasterToSlaveMessage {
        SYNC_TIME = 1,
        START_MEAS = 2,
        END_MEAS = 3,
        ERROR_MSG = 4,
        ERROR_SOLVED = 5
    };
}

namespace ErrorMessage {
    enum ErrorMessage {
        GENERAL_ERROR = 0
    };
}

namespace SlaveToMasterMessage {
    // Note: in case nodeid is fixed at compile time it
    //       could be directly included here
    enum SlaveToMasterMessage {
        ERROR_MSG = 1,
        MEASUREMENT = 2,
        TIMEOUT = 3,
        ERROR_SOLVED = 4
    };
}

#endif // PROTOCOL_H
