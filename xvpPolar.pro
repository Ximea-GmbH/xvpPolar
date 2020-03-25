#-------------------------------------------------
#
# Project created by QtCreator 2020-02-19T16:18:46
#
#-------------------------------------------------

QT       += widgets xml

TARGET = xvpPolar
TEMPLATE = lib
CONFIG += plugin C++17

SOURCES += polarplugin.cpp \
     polartohsvchnbl.cpp \
    degreeofpolarizationchnbl.cpp \
    splitchannelschnbl.cpp \
    abstractpolarchnbl.cpp \
    cxremovereflectionschnbl.cpp

HEADERS += polarplugin.h \
     polartohsvchnbl.h \
    degreeofpolarizationchnbl.h \
    splitchannelschnbl.h \
    abstractpolarchnbl.h \
    cxremovereflectionschnbl.h

INCLUDEPATH += $$PWD/include/xiCore $$PWD/include/xiCoreGui

windows {
  LIBS += -LC:\XIMEA\Examples\CamTool\xvpSample\lib\win\x64 -lxiCore -lxiCoreGui -openmp
  QMAKE_CXXFLAGS += -fp:fast -arch:AVX2 -Ox -openmp -Qvec-report:1
}

linux {
  LIBS += -L/opt/XIMEA/CamTool -lxiCore -lxiCoreGui -fopenmp
  QMAKE_CXXFLAGS += -ffast-math -ftree-vectorize -march=znver1 -fopenmp
}


unix {
    target.path = /opt/XIMEA/CamTool
    INSTALLS += target
}
