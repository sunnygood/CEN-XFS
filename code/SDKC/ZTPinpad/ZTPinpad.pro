
TEMPLATE = lib
TARGET = ZTPinpad
DEFINES = 
VERSION = 3.5
QT -= core gui
QMAKE_LFLAGS += -Wl,-rpath,./ -Wl,--as-needed 
CONFIG += release warn_off
DESTDIR = ../../../lib
DEPENDPATH += ../../../Interface  ../include ../common
INCLUDEPATH += ../../../Interface ../include ../common
LIBS += -L../../../lib -L../lib -lSystemBase -lPortBase -lAlgBase


OBJECTS_DIR = tmp/


# Input
HEADERS += errorcode.h \
           ISystem.h \
           IPort.h \
           IDriver.h \
           IPinpad.h \
           IAlg.h \
           CCommandBase.h \
           CPinpadFace.h \
           CPinpadBase.h \
           CPinpadHxx.h \
           CPinpadEPP.h \
           CPinpadEPP_R.h \
           CPinpadVISA.h \
           CPinpadVISA_R.h \
           CPinpadWOSA.h \
           CPinpadWOSA_R.h \  
           CPinpadSimulate.h \         
           CEccPCI.h \
           CEccVISA.h


SOURCES += CCommandBase.cpp \
           CPinpadFace.cpp \
           CPinpadBase.cpp \
           CPinpadHxx.cpp \
           CPinpadEPP.cpp \
           CPinpadEPP_R.cpp \
           CPinpadVISA.cpp \
           CPinpadVISA_R.cpp \
           CPinpadWOSA.cpp \
           CPinpadWOSA_R.cpp \
           CPinpadSimulate.cpp \
           CEccPCI.cpp \
           CEccVISA.cpp \
           ModulePinpad.cpp
