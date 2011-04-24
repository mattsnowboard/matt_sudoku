
#include "GuessCommand.h"
#include "Cell.h"

namespace Sudoku
{

std::shared_ptr<Command> GuessCommand::CreateGuessCommand(
    std::shared_ptr<Cell> cell, int guess )
{
    std::shared_ptr<Command> c( new GuessCommand( cell, guess ) );
    return c;
}

GuessCommand::GuessCommand( std::shared_ptr<Cell> cell, int guess )
    : _cell( cell ), _newGuess( guess )
{}

bool GuessCommand::execute()
{
    bool success = _cell->CanGuess();
    if ( success )
    {
        _oldGuess = _cell->DisplayedValue();
        _cell->SetGuess( _newGuess );
    }
    return success;
}

bool GuessCommand::unexecute()
{
    bool success = _cell->CanGuess();
    if ( success )
    {
        _cell->SetGuess( _oldGuess );
    }
    return success;
}

}
