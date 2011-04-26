
#include "MarkCommand.h"
#include "Cell.h"

namespace Sudoku
{

std::shared_ptr<Command<Cell> > MarkCommand::CreateMarkCommand(
    std::shared_ptr<const Cell> cell, int mark )
{
    std::shared_ptr<Command<Cell> > c( new MarkCommand( cell, mark ) );
    return c;
}

MarkCommand::MarkCommand( std::shared_ptr<const Cell> cell, int mark )
    : Command<Cell>( cell ), _mark( mark )
{}

bool MarkCommand::execute( std::shared_ptr<Cell> c )
{
    if ( c->CanGuess() )
    {
        _wasMarked = c->GetMarkContainer()[_mark];
        if ( !_wasMarked )
        {
            c->Mark( _mark );
            return true;
        }
    }
    return false;
}

bool MarkCommand::unexecute( std::shared_ptr<Cell> c )
{
    if ( c->CanGuess() )
    {
        if ( !_wasMarked )
        {
            c->Unmark( _mark );
            return true;
        }
    }
    return false;
}

}
