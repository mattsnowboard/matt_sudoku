
#include "Puzzle.h"
#include "Cell.h"

#include <QApplication>
#include <QTextEdit>

int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    QTextEdit textEdit;

    textEdit.show();

    return app.exec();
}
