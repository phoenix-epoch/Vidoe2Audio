#-------------------------------------------------
# Project created for Starry Audio Extractor
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = StarryAudioExtractor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/starrybackground.cpp \
    src/ffmpeghelper.cpp

HEADERS += \
    src/mainwindow.h \
    src/starrybackground.h \
    src/ffmpeghelper.h \
    src/style.h

# Windows 配置
win32 {
    RC_ICONS = 
}
