
#include "GuessCommand.h"
#include "Cell.h"

namespace Sudoku
{

std::shared_ptr<Command<Cell> > GuessCommand::CreateGuessCommand(
    std::shared_ptr<const Cell> cell, int guess )
{
    std::shared_ptr<Command<Cell> > c( new GuessCommand( cell, guess ) );
    return c;
}

GuessCommand::GuessCommand( std::shared_ptr<const Cell> cell, int guess )
    : Command<Cell>( cell ), _newGuess( guess )
{}

bool GuessCommand::execute( std::shared_ptr<Cell> c )
{
    bool success = c->CanGuess();
    if ( success )
    {
        _oldGuess = c->DisplayedValue();
        c->SetGuess( _newGuess );
    }
    return success;
}

bool GuessCommand::unexecute( std::shared_ptr<Cell> c )
{
    bool success = c->CanGuess();
    if ( success )
    {
        c->SetGuess( _oldGuess );
    }
    return success;
}

}
