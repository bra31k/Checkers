QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Checkers
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Cell.cpp

HEADERS  += MainWindow.hpp \
    Cell.hpp

FORMS    += MainWindow.ui
