#-------------------------------------------------
#
# Project created by QtCreator 2018-10-01T09:30:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FitModelQt
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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    gui/dialoginfo.ui \
    gui/dialoglauncher.ui \
    gui/dialogsnippets.ui \
    gui/dialogviewer.ui \
    gui/mainwindow.ui \
    gui/dialogtreeselector.ui \
    gui/dialogstatsselector.ui

HEADERS += \
    fitmodel/config.h \
    fitmodel/draw.h \
    fitmodel/eigen.h \
    fitmodel/eigenmb.h \
    fitmodel/fit_launcher.h \
    fitmodel/free.h \
    fitmodel/lk.h \
    fitmodel/models.h \
    fitmodel/optimiz.h \
    fitmodel/options.h \
    fitmodel/utilities.h \
    gui/dialoginfo.h \
    gui/dialoglauncher.h \
    gui/dialogsnippets.h \
    gui/dialogviewer.h \
    gui/fitmodeltreewrapper.h \
    gui/mainwindow.h \
    gui/phylotreewidget.h \
    gui/queryparser.h \
    gui/dialogtreeselector.h \
    gui/dialogstatsselector.h

SOURCES += \
    gui/dialoginfo.cpp \
    gui/dialoglauncher.cpp \
    gui/dialogsnippets.cpp \
    gui/dialogviewer.cpp \
    gui/fitmodeltreewrapper.cpp \
    gui/main.cpp \
    gui/mainwindow.cpp \
    gui/phylotreewidget.cpp \
    gui/queryparser.cpp \
    fitmodel/draw.c \
    fitmodel/eigen.c \
    fitmodel/eigenmb.c \
    fitmodel/fit_main.c \
    fitmodel/free.c \
    fitmodel/lk.c \
    fitmodel/models.c \
    fitmodel/optimiz.c \
    fitmodel/options.c \
    fitmodel/utilities.c \
    gui/dialogtreeselector.cpp \
    gui/dialogstatsselector.cpp

RESOURCES += \
    resources.qrc

