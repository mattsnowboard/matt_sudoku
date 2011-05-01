#include "QtCellEditor.h"

#include <QDebug>

namespace QtSudoku
{

QtCellEditor::QtCellEditor( const QStyleOptionViewItem &option,
                            QWidget *parent )
    : QWidget( parent ),
      _option( option )
{
    setMouseTracking( true );
    setAutoFillBackground( true );
}

void QtCellEditor::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );
    qDebug() << "paint editor: " << _marks.size();
    // draw all numbers in a grid shape
    int x = _option.rect.left();
    int y = _option.rect.top();
    int w = _option.rect.width();
    int h = _option.rect.height();

    painter.setBrush( QBrush( Qt::white ) );
    painter.drawRect( x, y, w, h );

    for ( int i = 1; i < _marks.size(); ++i )
    {
        if ( _marks.testBit( i ) )
        {
            painter.setPen( QPen( QBrush( Qt::black ), 1 ) );
        }
        else
        {
            painter.setPen( QPen( QBrush( Qt::lightGray ), 1 ) );
        }
        QString num;
        num = QString( "%1" ).arg( i );
        painter.drawText( x + ((i - 1) % 3) * w / 3,
                          y + ((i - 1) / 3) * h / 3,
                          num );
    }
}

void QtCellEditor::mouseMoveEvent( QMouseEvent *event )
{
    qDebug() << "mouse move editor";
}

void QtCellEditor::mousePressEvent( QMouseEvent *event )
{
    qDebug() << "mouse press editor";
    emit editingFinished();
}


}
