
#include <QtGui>

#include "QtDirector.h"

#include "QtCellItemDelegate.h"
#include "QtGameApplication.h"
#include "QtMainWindow.h"
#include "QtPuzzleModel.h"
#include "QtPuzzleView.h"

#include <GameManager.h>
#include <GameController.h>

#include <QDebug>

#include <stdexcept>

namespace QtSudoku
{

QtDirector::QtDirector( int &argc, char **argv,
                        std::shared_ptr<Sudoku::IPuzzleAccess> access,
                        std::shared_ptr<Sudoku::GameController> controller,
                        std::shared_ptr<QtPuzzleModel> puzzleModel )
    : _app( new QtGameApplication( argc, argv ) ),
      _tableView( new QtPuzzleView ),
      _window( new QtMainWindow( _tableView ) ),
      _model( puzzleModel ),
      _delegate( new QtCellItemDelegate( _tableView ) ),
      _puzzleAccess( access ),
      _gameController( controller )
{
    if ( !_model )
    {
        throw std::runtime_error(
            "Cannot instantiate with NULL QtPuzzleModel" );
    }
    if ( !_puzzleAccess )
    {
        throw std::runtime_error(
            "Cannot instantiate with NULL PuzzleAccess" );
    }
    if ( !_gameController )
    {
        throw std::runtime_error(
            "Cannot instantiate with NULL GameController" );
    }

    _tableView->setModel( _model.get() );
    _tableView->setItemDelegate( _delegate.get() );

    qDebug() << "My delegate is: " << _delegate.get();
    qDebug() << "Table view is using: " << _tableView->itemDelegate();

    connect( _window->GetLoadAction(), SIGNAL( triggered() ),
             _model.get(), SLOT( LoadPuzzle() ) );
    connect( _window->GetNewAction(), SIGNAL( triggered() ),
             _model.get(), SLOT( NewPuzzle() ) );
    connect( _window->GetUndoAction(), SIGNAL( triggered() ),
             _model.get(), SLOT( Undo() ) );
    connect( _window->GetRedoAction(), SIGNAL( triggered() ),
             _model.get(), SLOT( Redo() ) );
    connect( _window->GetMarkHintsAction(), SIGNAL( triggered() ),
             _model.get(), SLOT( MarkPuzzleHints() ) );
    connect( _window->GetSolveAction(), SIGNAL( triggered() ),
             _model.get(), SLOT( Solve() ) );

    connect( _model.get(), SIGNAL( hasPuzzle(bool) ),
             _window->GetMarkHintsAction(), SLOT( setEnabled( bool ) ) );
    connect( _model.get(), SIGNAL( hasPuzzle(bool) ),
             _window->GetSolveAction(), SLOT( setEnabled( bool ) ) );

    connect( this, SIGNAL( canUndo( bool ) ),
             _window->GetUndoAction(), SLOT( setEnabled( bool ) ) );

    connect( this, SIGNAL( canRedo( bool ) ),
             _window->GetRedoAction(), SLOT( setEnabled( bool ) ) );

    _window->setWindowTitle(
        QApplication::translate("windowlayout", "Sudoku"));
    _window->show();
}

void QtDirector::CanUndo( bool yes )
{
    emit canUndo( yes );
}

void QtDirector::CanRedo( bool yes )
{
    emit canRedo( yes );
}

bool QtDirector::LoadPuzzle()
{
    QString fileName = QFileDialog::getOpenFileName( _window );
    if ( !fileName.isEmpty() )
    {
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        bool ret = _gameController->ImportFromFile( fileName.toStdString() );
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        if ( !ret )
        {
            QErrorMessage message( QApplication::focusWidget() );
            message.showMessage( "Failed to open file" );
            message.exec();
            return false;
        }
        _window->statusBar()->showMessage( tr("File Loaded"), 5000 );
        return true;
    }
    return false;
}

void QtDirector::NewPuzzle()
{
    _gameController->NewPuzzle();
}

void QtDirector::Undo()
{
    _gameController->Undo();
}

void QtDirector::Redo()
{
    _gameController->Redo();
}

int QtDirector::Exec()
{
    return _app->exec();
}

}
