
#include "QtPuzzleModel.h"

#include <CellController.h>
#include <GameController.h>
#include <PuzzleController.h>
#include <IPuzzleAccess.h>
#include <Puzzle.h>

#include <stdexcept>

namespace QtSudoku
{

QtPuzzleModel::QtPuzzleModel(
    QObject * parent, 
    std::shared_ptr<Sudoku::IPuzzleAccess> access,
    std::shared_ptr<Sudoku::GameController> gameController,
    std::shared_ptr<Sudoku::PuzzleController> puzzleController,
    std::shared_ptr<Sudoku::CellController> cellController )
    : QAbstractTableModel( parent ),
      _access( access ),
      _gameController( gameController ),
      _puzzleController( puzzleController ),
      _cellController( cellController )
{
    if ( !_access )
    {
        throw std::runtime_error(
            "Cannot instantiate a QtPuzzleModel without a PuzzleAccess" );
    }
    if ( !_gameController )
    {
        throw std::runtime_error(
            "Cannot instantiate a QtPuzzleModel without a GameController" );
    }
    if ( !_puzzleController )
    {
        throw std::runtime_error(
            "Cannot instantiate a QtPuzzleModel without a PuzzleController" );
    }
    if ( !_cellController )
    {
        throw std::runtime_error(
            "Cannot instantiate a QtPuzzleModel without a CellController" );
    }
    emit hasPuzzle( _access->GetPuzzle() );
}

int QtPuzzleModel::rowCount( const QModelIndex & parent ) const
{
    return 9;
}

int QtPuzzleModel::columnCount( const QModelIndex & parent ) const
{
    return 9;
}

QVariant QtPuzzleModel::data( const QModelIndex &index,
                              int role ) const
{
    // don't access null pointer
    if ( _access->GetPuzzle() && index.isValid() )
    {
        std::shared_ptr<const Sudoku::Cell> c =
            _access->GetPuzzle()->GetCell( index.column() + 1,
                                           index.row() + 1 );
        if ( role == Qt::DisplayRole )
        {
            // display the value or nothing for "0"
            if ( c->DisplayedValue() != 0 )
            {
                return QString( "%1" )
                    .arg( c->DisplayedValue() );
            }
            else
            {
                // if display value is 0, we can draw the marks
                QString markstr;
                Sudoku::Cell::MarkContainer marks = c->GetMarkContainer();
                if ( marks.any() )
                {
                    for ( size_t i = 1; i < marks.size(); i++ )
                    {
                        QString next;
                        if ( marks[i] )
                        {
                            next = QString( "%1 " ).arg( i );
                        }
                        else
                        {
                            next = QString( "  " );
                        }
                        markstr += next;
                        if ( i == 3 || i == 6 )
                        {
                            markstr += "\n";
                        }
                    }
                    return markstr;
                }
            }
        }

        if ( role == Qt::FontRole )
        {
            QFont font;
            if ( c->DisplayedValue() != 0 )
            {
                font.setPointSize( 24 );
                if ( !c->CanGuess() )
                {
                    font.setBold( true );
                }
            }
            else
            {
                Sudoku::Cell::MarkContainer marks = c->GetMarkContainer();
                if ( marks.any() )
                {
                    font.setFamily( "courier" );
                    font.setPointSize( 10 );
                }
            }
            return font;
        }

        if ( role == Qt::TextAlignmentRole )
        {
            if ( c->DisplayedValue() == 0 )
            {
                Sudoku::Cell::MarkContainer marks = c->GetMarkContainer();
                if ( marks.any() )
                {
                    return Qt::AlignLeft + Qt::AlignTop;
                }
            }
            return Qt::AlignCenter;
        }

        if ( role == Qt::ForegroundRole )
        {

        }

        if ( role == Qt::BackgroundRole )
        {

        }

        if ( role == Qt::UserRole )
        {
            // if ( c->DisplayedValue() == 0 )
            // {
            //     QBitArray a( 10 );
            //     Sudoku::Cell::MarkContainer marks = c->GetMarkContainer();
            //     for ( size_t i = 1; i < marks.size(); i++ )
            //     {
            //         if ( marks[i] )
            //         {
            //             a.setBit( i );
            //         }
            //     }
            //     return a;
            // }
        }
    }
    return QVariant();
}

bool QtPuzzleModel::setData( const QModelIndex & index,
                             const QVariant & value,
                             int role )
{
    if (role == Qt::EditRole)
    {
        int guess = value.toInt();

        // issue command
        _cellController->MakeGuess( index.column() + 1,
                                    index.row() + 1,
                                    guess );

        //emit editCompleted( result );
    }
    return true;
}

Qt::ItemFlags QtPuzzleModel::flags( const QModelIndex & index ) const
{
    if ( _access->GetPuzzle() )
    {
        if ( index.isValid() )
        {
            std::shared_ptr<const Sudoku::Cell> c =
                _access->GetPuzzle()->GetCell( index.column() + 1,
                                           index.row() + 1 );
            if ( c->CanGuess() )
            {
                return Qt::ItemIsSelectable |
                    Qt::ItemIsEditable |
                    Qt::ItemIsEnabled;
            }
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }

    }
    return 0;
}

QVariant QtPuzzleModel::headerData( int, Qt::Orientation, int ) const
{
    return QVariant();
}

void QtPuzzleModel::NewPuzzle()
{
    _gameController->NewPuzzle();
    emit hasPuzzle( _access->GetPuzzle() );
}

bool QtPuzzleModel::LoadPuzzle()
{
    QString fileName =
        QFileDialog::getOpenFileName( QApplication::focusWidget() );
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
            emit hasPuzzle( _access->GetPuzzle() );
            return false;
        }
        emit hasPuzzle( _access->GetPuzzle() );
        return true;
    }
    emit hasPuzzle( _access->GetPuzzle() );    
    return false;
}

void QtPuzzleModel::MarkPuzzleHints()
{
    _puzzleController->MarkHints();
}

void QtPuzzleModel::Solve()
{
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    _puzzleController->Solve();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void QtPuzzleModel::Undo()
{
    _gameController->Undo();
}

void QtPuzzleModel::Redo()
{
    _gameController->Redo();
}

void QtPuzzleModel::Update( const Sudoku::Cell &c )
{
    emit dataChanged( createIndex( c.GetY() - 1, c.GetX() - 1 ),
                      createIndex( c.GetY() - 1, c.GetX() - 1 ) );
}

}
