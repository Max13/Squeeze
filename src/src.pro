#-------------------------------------------------
#
# Project created by QtCreator 2016-07-15T12:17:29
#
#-------------------------------------------------

QT          += core gui widgets concurrent

TARGET      =  Squeeze
TEMPLATE    =  app


SOURCES     += main.cpp \
               MainWidget.cpp

HEADERS     += MainWidget.hpp

RESOURCES   += resources/resources.qrc

OTHER_FILES += resources/icon.icns \
               resources/icon.ico \
               resources/icon.svg

osx:ICON    =  resources/icon.icns
win32:RC_ICONS = resources/icon.ico
