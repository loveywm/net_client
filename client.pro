#-------------------------------------------------
#
# Project created by QtCreator 2013-05-13T23:47:27
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    client.cpp \
    net.cpp \
    arp.cpp \
    arp_attack.cpp \
    eth_arp_show.cpp \
    ping.cpp

HEADERS += \
    client.h \
    net.h
