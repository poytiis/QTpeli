#-------------------------------------------------
#
# Project created by QtCreator 2015-11-06T08:59:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OmaPuoli
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib

DEFINES += QT_NO_DEBUG_OUTPUT

# Lisätty kurssin puolen tavaroiden käyttämiseksi
INCLUDEPATH += $$PWD/../KurssinPuoli
DEPENDPATH += $$PWD/../KurssinPuoli
#LIBS += $$PWD/../KurssinPuoli/sijainti.o


SOURCES += \
    luopeli.cc \
    kaupunki.cpp \
    drooni.cpp \
    peliikkuna.cpp \
    tilasto.cpp \
    ammus.cpp \
    rynnakkokivaari.cpp \
    nyssekuva.cpp \
    miina.cpp \
    laser.cpp \
    pelialue.cpp \
    lopetusdialog.cpp

HEADERS  += \
    kaupunki.hh \
    drooni.hh \
    peliikkuna.hh \
    tilasto.hh \
    ammus.hh \
    rynnakkokivaari.hh \
    nyssekuva.hh \
    miina.hh \
    laser.hh \
    pelialue.hh \
    lopetusdialog.hh
FORMS += \
    peliikkuna.ui \
    lopetusdialog.ui

RESOURCES += images.qrc

DISTFILES += \
    tekstitiedostot
