QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    measurementnode.cpp \
    time_measurement.cpp

HEADERS += \
    constants.h \
    logging.h \
    measurementnode.h \
    protocol.h \
    time_measurement.h
    