#ifndef QT_SUDOKU_GAME_APPLICATION_H
#define QT_SUDOKU_GAME_APPLICATION_H

#include <QApplication>

namespace QtSudoku
{

class QtGameApplication : public QApplication
{
public:
    QtGameApplication( int &argc, char **argv );

    virtual bool notify( QObject * receiver, QEvent * event );

    virtual ~QtGameApplication() {}
};

}

#endif
