#include "QtCellItemDelegate.h"

#include <QDebug>

namespace QtSudoku
{

QtCellItemDelegate::QtCellItemDelegate( QWidget *parent ) :
    QStyledItemDelegate( parent )
{
    qDebug() << "CONSTRUCT";
}

void QtCellItemDelegate::paint( QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index )
{
    painter->save();
    qDebug() << "Begin Paint";
    QStyledItemDelegate::paint( painter, option, index );
    
    qDebug() << "   Begin custom paint";
    // add thicker lines around groups of 9
    painter->setPen( QPen( QBrush( QColor( Qt::blue ) ), 5 ) );

    // top of a group
    if ( index.row() % 3 == 0 )
    {
        painter->drawLine( option.rect.topLeft(),
                           option.rect.topRight() );
    }

    // bottom of a group
    if ( index.row() % 3 == 2 )
    {
        painter->drawLine( option.rect.bottomLeft(),
                           option.rect.bottomRight() );
    }

    // left of a group
    if ( index.column() % 3 == 0 )
    {
        painter->drawLine( option.rect.topLeft(),
                           option.rect.bottomLeft() );
    }

    // right of a group
    if ( index.column() % 3 == 2 )
    {
        painter->drawLine( option.rect.topRight(),
                           option.rect.bottomRight() );
    }
    qDebug() << "End paint";
    painter->restore();
}

QSize QtCellItemDelegate::sizeHint( const QStyleOptionViewItem &option,
                                     const QModelIndex & index ) const 
{
    return QSize( 60, 60 );
}

QtCellItemDelegate::~QtCellItemDelegate()
{
    qDebug() << "Destroy delegate!";
}

}
