
#include "UnmarkCommand.h"
#include "Cell.h"

namespace Sudoku
{

std::shared_ptr<Command<Cell> > UnmarkCommand::CreateUnmarkCommand(
    std::shared_ptr<const Cell> cell, int mark )
{
    std::shared_ptr<Command<Cell> > c( new UnmarkCommand( cell, mark ) );
    return c;
}

UnmarkCommand::UnmarkCommand( std::shared_ptr<const Cell> cell, int mark )
    : Command<Cell>( cell ), _mark( mark )
{}

bool UnmarkCommand::execute( std::shared_ptr<Cell> c )
{
    if ( c->CanGuess() )
    {
        _wasMarked = c->GetMarkContainer()[_mark];
        if ( _wasMarked )
        {
            c->Unmark( _mark );
            return true;
        }
    }
    return false;
}

bool UnmarkCommand::unexecute( std::shared_ptr<Cell> c )
{
    if ( c->CanGuess() )
    {
        if ( _wasMarked )
        {
            c->Mark( _mark );
            return true;
        }
    }
    return false;
}

}
