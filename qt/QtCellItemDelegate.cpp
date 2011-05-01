#include "QtCellItemDelegate.h"

#include <QDebug>

#include "QtCellEditor.h"

namespace QtSudoku
{

QtCellItemDelegate::QtCellItemDelegate( QWidget *parent ) :
    QStyledItemDelegate( parent )
{
}

void QtCellItemDelegate::paint( QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index ) const
{
    painter->save();
    QStyledItemDelegate::paint( painter, option, index );
    
    // add thicker lines around groups of 9
    painter->setPen( QPen( QBrush( QPalette::Highlight ), 2 ) );

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
    painter->restore();
}

QSize QtCellItemDelegate::sizeHint( const QStyleOptionViewItem &option,
                                     const QModelIndex & index ) const 
{
    return QSize( 60, 60 );
}

QWidget* QtCellItemDelegate::createEditor( QWidget *parent,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index ) const
{
    if ( qVariantCanConvert<QBitArray>( index.data( Qt::UserRole ) ) )
    {
        QtCellEditor *editor = new QtCellEditor( option, parent );
        connect( editor, SIGNAL( editingFinished() ),
                 this, SLOT( commitAndCloseEditor() ) );
        return editor;
    }
    return QStyledItemDelegate::createEditor( parent, option, index );
}

void QtCellItemDelegate::setEditorData( QWidget *editor,
                                        const QModelIndex &index ) const
{
    if ( qVariantCanConvert<QBitArray>( index.data( Qt::UserRole ) ) )
    {
        QBitArray marks = qVariantValue<QBitArray>( index.data( Qt::UserRole) );
        QtCellEditor *cellEditor = qobject_cast<QtCellEditor *>( editor );
        cellEditor->setMarks( marks );
    }
    else
    {
        QStyledItemDelegate::setEditorData( editor, index );
    }
}

void QtCellItemDelegate::setModelData( QWidget *editor,
                                       QAbstractItemModel *model,
                                       const QModelIndex &index ) const
{
    if ( qVariantCanConvert<QBitArray>( index.data( Qt::UserRole ) ) )
    {
        QtCellEditor *cellEditor = qobject_cast<QtCellEditor *>( editor );
        model->setData( index, qVariantFromValue( cellEditor->getMarks() ) );
    }
    else
    {
        QStyledItemDelegate::setModelData( editor, model, index );
    }
}

void QtCellItemDelegate::commitAndCloseEditor()
{
    QtCellEditor *editor = qobject_cast<QtCellEditor *>(sender());
    emit commitData( editor );
    emit closeEditor( editor );
}

QtCellItemDelegate::~QtCellItemDelegate()
{
}

}
