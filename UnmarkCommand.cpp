
#include "UnmarkCommand.h"
#include "Cell.h"

namespace Sudoku
{

std::shared_ptr<Command> UnmarkCommand::CreateUnmarkCommand(
    std::shared_ptr<Cell> cell, int mark )
{
    std::shared_ptr<Command> c( new UnmarkCommand( cell, mark ) );
    return c;
}

UnmarkCommand::UnmarkCommand( std::shared_ptr<Cell> cell, int mark )
    : _cell( cell ), _mark( mark )
{}

bool UnmarkCommand::execute()
{
    if ( _cell->CanGuess() )
    {
        _wasMarked = _cell->GetMarkContainer()[_mark];
        if ( _wasMarked )
        {
            _cell->Unmark( _mark );
            return true;
        }
    }
    return false;
}

bool UnmarkCommand::unexecute()
{
    if ( _cell->CanGuess() )
    {
        if ( _wasMarked )
        {
            _cell->Mark( _mark );
            return true;
        }
    }
    return false;
}

}
