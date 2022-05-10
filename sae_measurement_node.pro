
QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle
+

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
    

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bcm2835-1.71/src/release/ -lbcm2835
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bcm2835-1.71/src/debug/ -lbcm2835
else:unix: LIBS += -L$$PWD/../bcm2835-1.71/src/ -lbcm2835

INCLUDEPATH += $$PWD/../bcm2835-1.71/src
DEPENDPATH += $$PWD/../bcm2835-1.71/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bcm2835-1.71/src/release/libbcm2835.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bcm2835-1.71/src/debug/libbcm2835.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bcm2835-1.71/src/release/bcm2835.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bcm2835-1.71/src/debug/bcm2835.lib
else:unix: PRE_TARGETDEPS += $$PWD/../bcm2835-1.71/src/libbcm2835.a
0
