#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T15:39:51
#
#-------------------------------------------------

QT       += widgets network core

TARGET = KurssinPuoli
TEMPLATE = app
CONFIG += staticlib
CONFIG += c++11

#INCLUDEPATH += $$PWD/../OmaPuoli
#DEPENDPATH += $$PWD/../OmaPuoli
#LIBS += $$PWD/../OmaPuoli/peliikkuna.o

SOURCES += \
    matkustaja.cc \
    nysse.cc \
    sijainti.cc \
    pysakki.cc \
    logiikka.cc \
    offlinelukija.cc \
    main.cc \
    konffiikkuna.cc \
    alustusvirhe.cc \
    pelivirhe.cc

HEADERS += \
    toimijarp.hh \
    kaupunkirp.hh \
    matkustaja.hh \
    nysse.hh \
    sijainti.hh \
    pysakki.hh \
    offlinelukija.hh \
    logiikka.hh \
    matkustajarp.hh \
    kulkuneuvorp.hh \
    tilastorp.hh \
    luopeli.hh \
    pysakkirp.hh \
    doxygeninfo.hh \
    konffiikkuna.hh \
    alustusvirhe.hh \
    pelivirhe.hh \
    alykaupunkirp.hh
unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    konffiikkuna.ui

RESOURCES += \
    offlinedata.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../OmaPuoli/release/ -lOmaPuoli
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../OmaPuoli/debug/ -lOmaPuoli
else:unix: LIBS += -L$$OUT_PWD/../OmaPuoli/ -lOmaPuoli

INCLUDEPATH += $$PWD/../OmaPuoli
DEPENDPATH += $$PWD/../OmaPuoli

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OmaPuoli/release/libOmaPuoli.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OmaPuoli/debug/libOmaPuoli.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OmaPuoli/release/OmaPuoli.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OmaPuoli/debug/OmaPuoli.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../OmaPuoli/libOmaPuoli.a
