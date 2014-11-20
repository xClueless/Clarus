TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS +=-std=c++1y

SOURCES += main.cpp \
    VideoDevice.cpp \
    RemoteVideoFeed.cpp \
    VideoServer.cpp \
    AudioUtil.cpp \
    AudioBouncer.cpp

LIBS += -lopencv_highgui -lopencv_core
LIBS += -L/usr/local/lib -lrtaudio
LIBS += -L/usr/local/lib/clews/ -lsock

HEADERS += \
    VideoDevice.hpp \
    RemoteVideoFeed.hpp \
    VideoServer.hpp \
    AudioUtil.hpp \
    AudioBouncer.hpp


