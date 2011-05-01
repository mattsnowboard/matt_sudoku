#ifndef QT_SUDOKU_CELL_ITEM_DELEGATE_H
#define QT_SUDOKU_CELL_ITEM_DELEGATE_H

#include <QtGui>

namespace QtSudoku
{

class QtCellItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT;
public:
    QtCellItemDelegate( QWidget *parent = NULL );

    void paint( QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index );

    QSize sizeHint( const QStyleOptionViewItem &option,
                    const QModelIndex & index ) const;

    virtual ~QtCellItemDelegate();
};

}

#endif
