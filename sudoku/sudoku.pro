#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T10:56:53
#
#-------------------------------------------------

QT	   += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sudoku
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp \
		sudoku.cpp \
		dancing_link.cpp \
		sudoku_grid.cpp \
		sudoku_cell.cpp \
		timer.cpp \
		action_queue.cpp \ 
		digit_button.cpp \
		utils.cpp \
		tool_button.cpp

HEADERS  += mainwindow.h \
		utils.h \
		sudoku.h \
		dancing_link.h \
		sudoku_grid.h \
		sudoku_cell.h \
		timer.h \
		action_queue.h \
		config.h \
		digit_button.h \ 
		tool_button.h


FORMS	+= mainwindow.ui

RESOURCES += \
    res.qrc
