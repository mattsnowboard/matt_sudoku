
#include "QtGameApplication.h"

#include <QErrorMessage>

namespace QtSudoku
{

QtGameApplication::QtGameApplication( int &argc, char **argv )
    : QApplication( argc, argv )
{}

bool QtGameApplication::notify( QObject * receiver, QEvent * event )
{
    try
    {
        return QApplication::notify( receiver, event );
    } catch( std::exception& e )
    {
        QErrorMessage message( QApplication::focusWidget() );
        message.showMessage( e.what() );
        message.exec();
    }
    catch( ... )
    {
        QErrorMessage message( QApplication::focusWidget() );
        message.showMessage( "Unknown Exception" );
        message.exec();
    }
    return false;
}    

}
