QT += core gui dbus widgets xml quick
QT += multimedia multimediawidgets
QT += positioning
QT += location
QT += serialport

QTPLUGIN += gstreamercamerabin

TARGET = arcomivi
CONFIG += console
#CONFIG -= app_bundle

#QMAKE_LFLAGS+=--sysroot=/opt/poky/2.0/sysroots/i586-poky-linux
#QMAKE_LFLAGS+=--sysroot=/opt/poky/2.0/sysroots/i586-poky-linux
#QMAKE_LFLAGS+=--sysroot=/opt/poky/2.0/sysroots/x86_64-pokysdk-linux

CONFIG += c++11

#LIBS += -L/opt/poky/2.0/sysroots/x86_64-pokysdk-linux/lib
#comment open cv out temporary
#LIBS += -L/usr/lib/x86_64-linux-gnu/
#LIBS += -lopencv_video \
#        -lopencv_videostab \
#        -lopencv_highgui \
#        -lopencv_core

TEMPLATE = app
target.path=/usr/bin
INSTALLS=target

aciconfigxml.path = /usr/share/arcomivi/
aciconfigxml.files = aciconfig.xml
INSTALLS += aciconfigxml

qml.path = /usr/share/arcomivi/qml
qml.files = qml/*.qml
INSTALLS += qml

css.path = /usr/share/arcomivi/css
css.files = css/*
INSTALLS += css

DEFINES     += QT_NO_DEBUG

SOURCES += main.cpp \
    aciconfig.cpp \
    acilogger.cpp \
    acimainview.cpp \
    aciusbcontroller.cpp \
#    qfoo.cpp \
#    acicamera.cpp \
    acilistmodel.cpp \
    acimedia.cpp \
    acimusicplayer.cpp \
    acivideoview.cpp \
    acisettings.cpp \
    acisteerings.cpp

HEADERS += \
    globalincludes.h \
    aciconfig.h \
    acilogger.h \
    acimainview.h \
    aciusbcontroller.h \
#    qfoo.h \
#    acicamera.h \
    acilistmodel.h \
    acimedia.h \
    acimusicplayer.h \
    acivideoview.h \
    acisettings.h \
    acisteerings.h

DISTFILES += \
    css/* \
    aciconfig.xml \
    qml/MainView.qml \
    qml/MainMenu.qml \
    qml/StatusBar.qml \
    qml/ViewsSwitcher.qml \
    qml/Steerings.qml \
    qml/SteerMusic.qml \
    qml/SteerSwitcher.qml \
    qml/SteerEmpty.qml \
    qml/ACIButton.qml \
    qml/ACIProgressBar.qml \
    qml/ACIStandardListItem.qml \
    qml/ACIHomeView.qml \
    qml/ACIMediaView.qml \
    qml/ACISettingsView.qml \
    qml/ACIVideoView.qml \
    qml/ACINaviView.qml \
    qml/ACISelectScreen.qml

OTHER_FILES += \
    css/*
