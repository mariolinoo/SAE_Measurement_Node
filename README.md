# SAE_Measurement_Node

## Build instructions on Raspberry Pi
```
cd <project-dir>
mkdir build
cd build
qmake ..
make -j 1
```
  
## Build with QtCreator
- Just open the .pro file in QtCreator.
- Most of the configuration should be done automatically by QtCreator, e.g. you get your three profiles "Debug", "Release" and "Profile" you can choose from.

## Testing the application
For testing the sae_measurement_node application the qt_udp_sockets_echo application found in the Round_trip_measurement subfolder can be used:
```
./qt_udp_sockets_echo -cws 100000000
```
- Using -c the client is selected
- With -w it is not waited for answers of the udp echo server (we don't have in this case)
- The -s option configures the inital value of the counter (100000000 == 100 * 10**6, e.g. start with 100 sec).
