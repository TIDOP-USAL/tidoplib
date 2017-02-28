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

unix {
    CONFIG += c++14
} else {
    CONFIG += c++11
}

INCLUDEPATH += ../../src/ \
               $$PWD/../../../Libs/opencv/include \
               $$PWD/../../../Libs/eigen/include/eigen3

SOURCES += \
    ../../src/core/console.cpp \
    ../../src/core/mathutils.cpp \
    ../../src/core/messages.cpp \
    ../../src/core/utils.cpp \
    ../../src/experimental/experimental.cpp \
    ../../src/geometric_entities/operations.cpp \
    ../../src/geometric_entities/segment.cpp \
    ../../src/geometric_entities/window.cpp \
    ../../src/graphic_entities/color.cpp \
    ../../src/graphic_entities/graphic_entity.cpp \
    ../../src/stereo/anaglyph.cpp \
    ../../src/fourier.cpp \
    ../../src/matching.cpp \
    ../../src/transform.cpp \
    ../../src/VideoStream.cpp \
    ../../src/core/exception.cpp \
    ../../src/img_process/filters.cpp \
    ../../src/img_process/img_processing.cpp \
    ../../src/img_process/skeleton.cpp \
    ../../src/img_process/white_balance.cpp \
    ../../src/feature_detection/linedetector.cpp \
    ../../src/feature_detection/shapedetection.cpp \
    ../../src/core/datamodel.cpp \
    ../../src/img/imgio.cpp \
    ../../src/vect/vectio.cpp

HEADERS += \
    ../../src/core/config.h \
    ../../src/core/console.h \
    ../../src/core/defs.h \
    ../../src/core/flags.h \
    ../../src/core/mathutils.h \
    ../../src/core/messages.h \
    ../../src/core/utils.h \
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
    ../../src/matching.h \
    ../../src/transform.h \
    ../../src/VideoStream.h \
    ../../src/core/exception.h \
    ../../src/feature_detection/linedetector.h \
    ../../src/feature_detection/shapedetection.h \
    ../../src/img_process/filters.h \
    ../../src/img_process/img_processing.h \
    ../../src/img_process/skeleton.h \
    ../../src/img_process/white_balance.h \
    ../../src/core/datamodel.h \
    ../../src/img/imgio.h \
    ../../src/vect/vectio.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

debug{
    DEFINES += _DEBUG
}else{
    DEFINES += NDEBUG
}
