#ifndef CONSTANTS_H
#define CONSTANTS_H

// Default maximum size of an UDP datagram handled by the measurement node.
#define DEFAULT_DATAGRAM_MAX_SIZE    5

// To enable verbose debug logging, shall be disabled in production.
#define VERBOSE_DEBUG_PRINTING

// UDP ports and IP of master node
#define MEASUREMENT_NODE_PORT        4242
#define MASTER_NODE_PORT             8080
#define MASTER_NODE_IP               "192.168.1.10"

// The node id to use for communication with the master
#define NODE_ID                      1

#endif // CONSTANTS_H
