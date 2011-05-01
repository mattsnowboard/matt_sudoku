
TEMPLATE = app
TARGET = qtsudoku
DEPENDPATH += .
INCLUDEPATH += . ..
CONFIG += qt warn_on debug
QMAKE_CXXFLAGS += -std=c++0x
LIBS += -L/home/matt/Documents/cse335/honors_option -lSudokuLib

# Input
HEADERS += QtPuzzleModel.h QtPuzzleView.h QtGameApplication.h QtDirector.h \
    QtMainWindow.h QtFactory.h QtCellItemDelegate.h QtCellEditor.h
SOURCES += main.cpp QtPuzzleModel.cpp QtPuzzleView.cpp QtGameApplication.cpp \
    QtDirector.cpp QtMainWindow.cpp QtFactory.cpp QtCellItemDelegate.cpp \
    QtCellEditor.cpp
