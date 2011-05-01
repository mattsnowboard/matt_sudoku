#ifndef QT_SUDOKU_CELL_ITEM_DELEGATE_H
#define QT_SUDOKU_CELL_ITEM_DELEGATE_H

#include <QtGui>

namespace QtSudoku
{

class QtCellItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QtCellItemDelegate( QWidget *parent = NULL );

    void paint( QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index ) const;

    QSize sizeHint( const QStyleOptionViewItem &option,
                    const QModelIndex & index ) const;

    QWidget *createEditor( QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index ) const;
    void setEditorData( QWidget *editor,
                        const QModelIndex &index ) const;
    void setModelData( QWidget *editor,
                       QAbstractItemModel *model,
                       const QModelIndex &index) const;

    virtual ~QtCellItemDelegate();

private slots:
    void commitAndCloseEditor();
};

}

#endif
