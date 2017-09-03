#-------------------------------------------------
#
# Project created by QtCreator 2017-09-03T23:55:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = draughts
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        draughts.cpp \
    chessboard.cpp \
    chesspiece.cpp

HEADERS  += mainwindow.h \
		draughts.h \
		utils.h \
    chessboard.h \
	config.h \
    chesspiece.h

FORMS    += mainwindow.ui
