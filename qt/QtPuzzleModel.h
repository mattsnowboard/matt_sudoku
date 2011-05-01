#ifndef QT_SUDOKU_PUZZLE_MODEL_H
#define QT_SUDOKU_PUZZLE_MODEL_H

#include <QtGui>
#include <memory>
#include "ICellObserver.h"
#include "Cell.h"

namespace Sudoku
{
class CellController;
class IPuzzleAccess;
class GameController;
class PuzzleController;
}

namespace QtSudoku
{

class QtPuzzleModel : public QAbstractTableModel, public Sudoku::ICellObserver
{
    Q_OBJECT;
public:
//    typedef std::map<Sudoku::Position, int > CellModelMap;

    QtPuzzleModel( QObject *parent,
                   std::shared_ptr<Sudoku::IPuzzleAccess> access,
                   std::shared_ptr<Sudoku::GameController> gameController,
                   std::shared_ptr<Sudoku::PuzzleController> puzzleController,
                   std::shared_ptr<Sudoku::CellController> cellController );
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    // read data
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    // edit data
    bool setData( const QModelIndex & index,
                  const QVariant & value,
                  int role = Qt::EditRole );
    Qt::ItemFlags flags( const QModelIndex & index ) const;

    // disable some stuff
    QVariant headerData( int section,
                         Qt::Orientation orientation,
                         int role ) const;

    void Update( const Sudoku::Cell &c );

public slots:
    void NewPuzzle();
    bool LoadPuzzle();
    void MarkPuzzleHints();
    void Solve();
    void Undo();
    void Redo();

signals:
    void ValueChanged( int x, int y, int value );
    void hasPuzzle( bool yes );

private:
    std::shared_ptr<Sudoku::IPuzzleAccess> _access;
    std::shared_ptr<Sudoku::GameController> _gameController;
    std::shared_ptr<Sudoku::PuzzleController> _puzzleController;
    std::shared_ptr<Sudoku::CellController> _cellController;
};

}

#endif
