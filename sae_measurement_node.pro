QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
#    time_measurement.cpp \
    measurementnode.cpp


HEADERS += \
    constants.h \
    logging.h \
#    time_measurement.h \
    measurementnode.h \
    protocol.h
    
