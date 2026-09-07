#-------------------------------------------------
# Project created for Starry Audio Extractor
#-------------------------------------------------

QT       += core gui widgets

CONFIG += c++17

TARGET = StarryAudioExtractor
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/src

MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = uic

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/starrybackground.cpp \
    src/starrytitlebar.cpp \
    src/starryformatsdialog.cpp \
    src/ffmpeghelper.cpp \
    src/urlextractorhelper.cpp

HEADERS += \
    src/mainwindow.h \
    src/starrybackground.h \
    src/starrytitlebar.h \
    src/starryformatsdialog.h \
    src/ffmpeghelper.h \
    src/urlextractorhelper.h \
    src/style.h

FORMS += \
    src/mainwindow.ui

win32: LIBS += -luser32