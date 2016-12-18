#-------------------------------------------------
#
# Project created by QtCreator 2016-11-17T20:44:13
#
#-------------------------------------------------

CONFIG(debug, debug|release) {
    CONFIG -= debug release
    CONFIG += debug
}
CONFIG(release, debug|release) {
        CONFIG -= debug release
        CONFIG += release
}

QT       -= core gui

TARGET = tidoplib
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

INCLUDEPATH += ../../src/ \
               /home/esteban/desarrollo/libs/opencv/include/

SOURCES += \
    ../../src/core/console.cpp \
    ../../src/core/mathutils.cpp \
    ../../src/core/messages.cpp \
    ../../src/core/utils.cpp \
    ../../src/experimental/events.cpp \
    ../../src/experimental/experimental.cpp \
    ../../src/geometric_entities/lineString.cpp \
    ../../src/geometric_entities/operations.cpp \
    ../../src/geometric_entities/point.cpp \
    ../../src/geometric_entities/segment.cpp \
    ../../src/geometric_entities/window.cpp \
    ../../src/graphic_entities/color.cpp \
    ../../src/graphic_entities/graphic_entity.cpp \
    ../../src/stereo/anaglyph.cpp \
    ../../src/fourier.cpp \
    ../../src/img_processing.cpp \
    ../../src/linedetector.cpp \
    ../../src/matching.cpp \
    ../../src/transform.cpp \
    ../../src/VideoStream.cpp \
    ../../src/core/exception.cpp

HEADERS += \
    ../../src/core/config.h \
    ../../src/core/console.h \
    ../../src/core/core.h \
    ../../src/core/defs.h \
    ../../src/core/flags.h \
    ../../src/core/mathutils.h \
    ../../src/core/messages.h \
    ../../src/core/utils.h \
    ../../src/experimental/events.h \
    ../../src/experimental/experimental.h \
    ../../src/geometric_entities/bbox.h \
    ../../src/geometric_entities/lineString.h \
    ../../src/geometric_entities/operations.h \
    ../../src/geometric_entities/point.h \
    ../../src/geometric_entities/polygon.h \
    ../../src/geometric_entities/segment.h \
    ../../src/geometric_entities/window.h \
    ../../src/graphic_entities/color.h \
    ../../src/graphic_entities/graphic_entity.h \
    ../../src/stereo/anaglyph.h \
    ../../src/fourier.h \
    ../../src/img_processing.h \
    ../../src/LineDetector.h \
    ../../src/matching.h \
    ../../src/transform.h \
    ../../src/VideoStream.h \
    ../../src/core/exception.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

debug{
    DEFINES += _DEBUG
}else{
    DEFINES += NDEBUG
}
