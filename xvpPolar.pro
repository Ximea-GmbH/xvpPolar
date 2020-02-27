#-------------------------------------------------
#
# Project created by QtCreator 2020-02-19T16:18:46
#
#-------------------------------------------------

QT       += widgets xml

TARGET = xvpPolar
TEMPLATE = lib
CONFIG += plugin c++17

SOURCES += polarplugin.cpp \
     polartohsvchnbl.cpp \
    degreeofpolarizationchnbl.cpp \
    splitchannelschnbl.cpp \
    abstractpolarchnbl.cpp

HEADERS += polarplugin.h \
     polartohsvchnbl.h \
    degreeofpolarizationchnbl.h \
    splitchannelschnbl.h \
    abstractpolarchnbl.h

INCLUDEPATH += $$PWD/include/xiCore $$PWD/include/xiCoreGui

QMAKE_CXXFLAGS += -fopenmp

LIBS += -fopenmp

linux {
  LIBS += -L/opt/XIMEA/CamTool -lxiCore -lxiCoreGui
  QMAKE_CXXFLAGS += -ffast-math -ftree-vectorize -march=znver1
}


unix {
    target.path = /opt/XIMEA/CamTool
    INSTALLS += target
}
