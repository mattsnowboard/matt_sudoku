

#include "QtDirector.h"
#include "QtFactory.h"
#include <Log.h>

using namespace QtSudoku;

int main( int argc, char** argv )
{
    FILELog::ReportingLevel() = logERROR;
    QtFactory factory;
    std::shared_ptr<QtDirector> director = factory.CreateDirector( argc, argv );
    return director->Exec();
}
