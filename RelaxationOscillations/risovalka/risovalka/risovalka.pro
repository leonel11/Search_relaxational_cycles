#-------------------------------------------------
#
# Project created by QtCreator 2014-05-19T19:49:21
#
#-------------------------------------------------

QT       += core gui

TARGET = risovalka
TEMPLATE = app
INCLUDEPATH += C:/qwt-5.2.0/include

LIBS += -LC:/qwt-5.2.0/lib -lqwt

SOURCES += main.cpp\
        mainwindow.cpp \
    taskparameters.cpp

HEADERS  += mainwindow.h \
    types.h \
    taskparameters.h
