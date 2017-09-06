#-------------------------------------------------
#
# Project created by QtCreator 2017-09-03T23:55:51
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = draughts
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    chessboard.cpp \
    chesspiece.cpp \
	draughts.cpp \
    login_dialog.cpp \
    listen_dialog.cpp

HEADERS  += mainwindow.h \
    chessboard.h \
    config.h \
    chesspiece.h \
	draughts.h \
	utils.h \
    login_dialog.h \
    listen_dialog.h

FORMS    += mainwindow.ui \
    login_dialog.ui \
    listen_dialog.ui

RESOURCES += \
    res.qrc
