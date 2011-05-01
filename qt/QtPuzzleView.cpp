#include "QtPuzzleView.h"
#include "QtCellItemDelegate.h"

namespace QtSudoku
{

QtPuzzleView::QtPuzzleView( QWidget *parent )
    : QTableView( parent )
{
    // change the view how we want it
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setSelectionMode( SingleSelection );
}

void QtPuzzleView::setModel( QAbstractItemModel *model )
{
    QTableView::setModel( model );
    resizeRows( model->rowCount() );
    resizeCols( model->columnCount() );
}

void QtPuzzleView::resizeRows( int rowCount )
{
    for ( int i = 0; i < rowCount; i++)
    {
        setRowHeight( i, 60 );
    }
}

void QtPuzzleView::resizeCols( int colCount )
{
    for ( int i = 0; i < colCount; i++)
    {
        setColumnWidth( i, 60 );
    }
}

void QtPuzzleView::columnCountChanged( int oldCount, int newCount )
{
    resizeCols( newCount );
}

void QtPuzzleView::rowCountChanged( int oldCount, int newCount )
{
    resizeRows( newCount );
}

}
