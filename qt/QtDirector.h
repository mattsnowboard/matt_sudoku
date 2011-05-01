#ifndef QT_SUDOKU_DIRECTOR_H
#define QT_SUDOKU_DIRECTOR_H

#include <QtGui>
#include <memory>

#include <ICommandObserver.h>

namespace Sudoku
{
class IPuzzleAccess;
class GameController;
}

namespace QtSudoku
{

class QtFactory;
class QtGameApplication;
class QtPuzzleModel;
class QtMainWindow;
class QtCellItemDelegate;

class QtDirector : public QObject,
                   public Sudoku::ICommandObserver
{
    Q_OBJECT;
public:
    friend class QtFactory;

    int Exec();

    ~QtDirector() {}

    /**
     * Notification that Undo is possible
     * @param yes whether or not it is possible
     */
    virtual void CanUndo( bool yes );

    /**
     * Notifaction that Redo is possible
     * @param yes whether or not it is possible
     */
    virtual void CanRedo( bool yes );

signals:
    void canUndo( bool yes );
    void canRedo( bool yes );

public slots:
    bool LoadPuzzle();
    void NewPuzzle();
    void Undo();
    void Redo();

protected:

    QtDirector( int &argc, char **arg,
                std::shared_ptr<Sudoku::IPuzzleAccess> access,
                std::shared_ptr<Sudoku::GameController> gameController,
                std::shared_ptr<QtPuzzleModel> puzzleModel );

private:
    QtGameApplication *_app;

    QTableView *_tableView;

    QtMainWindow *_window;
    QLayout *_layout;

    std::shared_ptr<QtPuzzleModel> _model;
    std::shared_ptr<QtCellItemDelegate> _delegate;

    std::shared_ptr<Sudoku::IPuzzleAccess> _puzzleAccess;
    std::shared_ptr<Sudoku::GameController> _gameController;
};

}

#endif
