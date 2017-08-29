#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T10:56:53
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sudoku
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
        sudoku.cpp \
        dancing_link.cpp \
		sudoku_grid.cpp \
    sudoku_cell.cpp

HEADERS  += widget.h \
        utils.h \
        sudoku.h \
        dancing_link.h \
		config.h \
		sudoku_grid.h \
    sudoku_cell.h

FORMS    += widget.ui
