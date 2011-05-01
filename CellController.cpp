#include "CellController.h"
#include "ICommandExecutor.h"
#include "IPuzzleAccess.h"
#include "Puzzle.h"
#include "Cell.h"
#include "GuessCommand.h"
#include "MarkCommand.h"
#include "UnmarkCommand.h"

namespace Sudoku
{

CellController::CellController( std::shared_ptr<ICommandExecutor> exec,
                                std::shared_ptr<IPuzzleAccess> access )
    : _executor( exec ),
      _puzzleAccess( access )
{
    if ( !_executor )
    {
        throw std::runtime_error(
            "Cannot create CellController with NULL CommandExecutor." );
    }
    if ( !_puzzleAccess )
    {
        throw std::runtime_error(
            "Cannot create CellController with NULL PuzzleAccess." );
    }
}

void CellController::MakeGuess( size_t x, size_t y, int guess )
{
    std::shared_ptr<const Cell> cell =
        _puzzleAccess->GetPuzzle()->GetCell( x, y );
    std::shared_ptr<CommandBase> command = 
        GuessCommand::CreateGuessCommand( cell, guess );
    _executor->Execute( command );
}

void CellController::Mark( size_t x, size_t y, int mark )
{
    std::shared_ptr<const Cell> cell =
        _puzzleAccess->GetPuzzle()->GetCell( x, y );
    std::shared_ptr<CommandBase> command = 
        MarkCommand::CreateMarkCommand( cell, mark );
    _executor->Execute( command );
}

void CellController::Unmark( size_t x, size_t y, int mark )
{
    std::shared_ptr<const Cell> cell =
        _puzzleAccess->GetPuzzle()->GetCell( x, y );
    std::shared_ptr<CommandBase> command = 
        UnmarkCommand::CreateUnmarkCommand( cell, mark );
    _executor->Execute( command );
}

}
