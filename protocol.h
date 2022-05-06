#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "constants.h"

namespace MasterToSlaveMessage {
    enum MasterToSlaveMessage {
        START_ROUNDTRIP_MEAS = 1,
        START_TIME_MEAS = 2
    };
}

#if 0
namespace ErrorMessage {
    enum ErrorMessage {
        GENERAL_ERROR = 0
    };
}
#endif // 0

namespace SlaveToMasterMessage {
    // Note: in case nodeid is fixed at compile time it
    //       could be directly included here
    enum SlaveToMasterMessage {
        HELLO_MSG = (NODE_ID << 4) | 1,
        ACK_ROUNDTRIP = (NODE_ID << 4) | 2,
        TIME_MEA = (NODE_ID << 4) | 3
    };
}

#endif // PROTOCOL_H
