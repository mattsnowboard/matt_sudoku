#ifndef QT_SUDOKU_MAIN_WINDOW_H
#define QT_SUDOKU_MAIN_WINDOW_H

#include <QtGui>

namespace QtSudoku
{

class QtMainWindow : public QMainWindow
{
    Q_OBJECT;
public:
    QtMainWindow( QAbstractItemView *view,
                  QWidget * parent = NULL,
                  Qt::WindowFlags flags = 0 );

    ~QtMainWindow() {}

    // Allow other things to connect to the action SIGNALs
    QAction* GetNewAction();
    QAction* GetLoadAction();
    QAction* GetExitAction();
    QAction* GetUndoAction();
    QAction* GetRedoAction();
    QAction* GetMarkHintsAction();
    QAction* GetSolveAction();

public slots:
//    void EnableUndo( bool yes );
//    void EnableRedo( bool yes );

protected:
    void closeEvent( QCloseEvent *event );

private slots:
    void about();

private:
    void createActions();
    void createMenus();
    void createStatusBar();

    void readSettings();
    void writeSettings();
    bool askSaveSettings();

    QAbstractItemView *_view;

    QMenu *_fileMenu;
    QMenu *_editMenu;
    QMenu *_helpMenu;
    QAction *_newAction;
    QAction *_loadAction;
    QAction *_exitAction;
    QAction *_undoAction;
    QAction *_redoAction;
    QAction *_markHintsAction;
    QAction *_solveAction;
    QAction *_aboutAction;
    QAction *_aboutQtAction;
};

}

#endif
