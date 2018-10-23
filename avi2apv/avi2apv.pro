#-------------------------------------------------
#
# Project created by QtCreator 2018-10-03T23:12:16
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = avi2apv
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    codec.cpp \
    codec_btree8.cpp \
    codec_btree4.cpp \
    codec_btree2.cpp \
    codec_btree1.cpp

HEADERS += \
        mainwindow.h \
    codec.h \
    codec_btree8.h \
    codec_btree4.h \
    codec_btree2.h \
    codec_btree1.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Include the configuration file below in the QT .pro file, and modify the path accordingly.

# ##############################################################################
# ##############################################################################
# FFMPEG: START OF CONFIGURATION BELOW ->
# Copy these lines into your own project
# Make sure to set the path variables for:
# 1) QTFFmpegWrapper sources (i.e. where the QVideoEncoder.cpp and QVideoDecoder.cpp lie),
# 2) FFMPEG include path (i.e. where the directories libavcodec, libavutil, etc. lie),
# 3) the binary FFMPEG libraries (that must be compiled separately).
# Under Linux path 2 and 3 may not need to be set as these are usually in the standard include and lib path.
# Under Windows, path 2 and 3 must be set to the location where you placed the FFMPEG includes and compiled binaries
# Note that the FFMPEG dynamic librairies (i.e. the .dll files) must be in the PATH
# ##############################################################################
# ##############################################################################

# ##############################################################################
# Modify here: set FFMPEG_LIBRARY_PATH and FFMPEG_INCLUDE_PATH
# ##############################################################################

# Set QTFFMPEGWRAPPER_SOURCE_PATH to point to the directory containing the QTFFmpegWrapper sources
QTFFMPEGWRAPPER_SOURCE_PATH = ./QTFFmpegWrapper

# Set FFMPEG_LIBRARY_PATH to point to the directory containing the FFmpeg import libraries (if needed - typically for Windows), i.e. the dll.a files
FFMPEG_LIBRARY_PATH = D:/ffmpeg/lib

# Set FFMPEG_INCLUDE_PATH to point to the directory containing the FFMPEG includes (if needed - typically for Windows)
FFMPEG_INCLUDE_PATH = ./QTFFmpegWrapper

# ##############################################################################
# Do not modify: FFMPEG default settings
# ##############################################################################
# Sources for QT wrapper
SOURCES += $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoEncoder.cpp \
    $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoDecoder.cpp
HEADERS += $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoEncoder.h \
    $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoDecoder.h

# Set list of required FFmpeg libraries
LIBS += -lavutil \
    -lavcodec \
    -lavformat \
    -lswscale

# Add the path
LIBS += -L$$FFMPEG_LIBRARY_PATH
INCLUDEPATH += QVideoEncoder
INCLUDEPATH += $$FFMPEG_INCLUDE_PATH

# Requied for some C99 defines
DEFINES += __STDC_CONSTANT_MACROS

# ##############################################################################
# FFMPEG: END OF CONFIGURATION
# ##############################################################################
