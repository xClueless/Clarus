TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    VideoDevice.cpp \
    RemoteVideoFeed.cpp \
    VideoServer.cpp

LIBS += -lopencv_highgui -lopencv_core
LIBS += -L/usr/local/lib/clews/ -lsock

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    VideoDevice.hpp \
    RemoteVideoFeed.hpp \
    VideoServer.hpp


