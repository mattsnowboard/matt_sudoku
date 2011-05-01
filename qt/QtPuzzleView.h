#ifndef QT_SUDOKU_PUZZLE_VIEW_H
#define QT_SUDOKU_PUZZLE_VIEW_H

#include <QtGui>

namespace QtSudoku
{

class QtCellItemDelegate;

class QtPuzzleView : public QTableView
{
    Q_OBJECT;
public:
    QtPuzzleView( QWidget *parent = NULL );

    virtual void setModel( QAbstractItemModel *model );

protected:
    void resizeRows( int rowCount );
    void resizeCols( int colCount );

protected slots:
    void columnCountChanged( int oldCount, int newCount );
    void rowCountChanged( int oldCount, int newCount );
};

}

#endif
