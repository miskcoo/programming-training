#-------------------------------------------------
#
# Project created by QtCreator 2017-09-03T23:55:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = draughts-client
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    chessboard.cpp \
    chesspiece.cpp \
	draughts.cpp

HEADERS  += mainwindow.h \
    chessboard.h \
    config.h \
    chesspiece.h \
	draughts.h \
	utils.h

FORMS    += mainwindow.ui
