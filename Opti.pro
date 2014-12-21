#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T03:07:04
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Opti
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mw.cpp \
        calc.cpp \
        gw.cpp \
        simplex.cpp \
    pw.cpp \
    powell.cpp \
    method.cpp \
    fletcherreeves.cpp

HEADERS += mw.h \
        calc.h \
        gw.h \
        simplex.h \
    pw.h \
    powell.h \
    method.h \
    fletcherreeves.h

FORMS += mw.ui \
      gw.ui \
    pw.ui
