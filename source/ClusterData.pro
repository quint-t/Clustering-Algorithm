#-------------------------------------------------
#
# Project created by QtCreator 2020-07-30T20:10:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ClusterData
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    plotmodel.cpp \
    plotpresenter.cpp \
    kmeans.cpp \
    rundialog.cpp \
    cluster.cpp \
    point.cpp \
    gendialog.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    plotmodel.h \
    plotpresenter.h \
    cluster.h \
    kmeans.h \
    point.h \
    rundialog.h \
    gendialog.h

FORMS    += mainwindow.ui \
    rundialog.ui \
    gendialog.ui
