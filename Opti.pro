#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T03:07:04
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Opti
TEMPLATE = app


SOURCES += main.cpp\
        mw.cpp \
        calc.cpp \
        gw.cpp \
        simplex.cpp \
    pw.cpp

HEADERS += mw.h \
        calc.h \
        gw.h \
        simplex.h \
    pw.h

FORMS += mw.ui \
      gw.ui \
    pw.ui
