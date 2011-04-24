
#include "MarkCommand.h"
#include "Cell.h"

namespace Sudoku
{

std::shared_ptr<Command> MarkCommand::CreateMarkCommand(
    std::shared_ptr<Cell> cell, int mark )
{
    std::shared_ptr<Command> c( new MarkCommand( cell, mark ) );
    return c;
}

MarkCommand::MarkCommand( std::shared_ptr<Cell> cell, int mark )
    : _cell( cell ), _mark( mark )
{}

bool MarkCommand::execute()
{
    if ( _cell->CanGuess() )
    {
        _wasMarked = _cell->GetMarkContainer()[_mark];
        if ( !_wasMarked )
        {
            _cell->Mark( _mark );
            return true;
        }
    }
    return false;
}

bool MarkCommand::unexecute()
{
    if ( _cell->CanGuess() )
    {
        if ( !_wasMarked )
        {
            _cell->Unmark( _mark );
            return true;
        }
    }
    return false;
}

}
