#ifndef CONSTANTS_H
#define CONSTANTS_H

// Default maximum size of an UDP datagram handled by the measurement node.
#define DEFAULT_DATAGRAM_MAX_SIZE    5

// To enable verbose debug logging, shall be disabled in production.
#define VERBOSE_DEBUG_PRINTING

// UDP ports and IP of master node
#define MEASUREMENT_NODE_PORT        4242
#define MEASUREMENT_NODE_IP          "127.0.0.1"  // should be also possible to get this programatically
#define MASTER_NODE_PORT             7000
#define MASTER_NODE_IP               "127.0.0.1"

#define NODE_ID                      1

#endif // CONSTANTS_H
