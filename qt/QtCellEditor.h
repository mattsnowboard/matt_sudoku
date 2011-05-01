#ifndef QT_SUDOKU_CELL_EDITOR_H
#define QT_SUDOKU_CELL_EDITOR_H

#include <Cell.h>

#include <QtGui>

namespace QtSudoku
{

class QtCellEditor : public QWidget
{
    Q_OBJECT;
public:
    QtCellEditor( const QStyleOptionViewItem &option,
                  QWidget *parent = NULL );

    void setMarks( QBitArray m )
    {
        _marks = m;
    }

    QBitArray getMarks() const
    {
        return _marks;
    }

signals:
    void editingFinished();
    
protected:
    void paintEvent( QPaintEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );

private:
    QBitArray _marks;
    QStyleOptionViewItem _option;
};

}

#endif
