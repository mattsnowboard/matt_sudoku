
#include "PuzzleController.h"
#include "ICommandExecutor.h"
#include "IPuzzleAccess.h"
#include "AddHintMarksCommand.h"
#include "SolveCommand.h"

namespace Sudoku
{

PuzzleController::PuzzleController( std::shared_ptr<ICommandExecutor> exec,
                                    std::shared_ptr<IPuzzleAccess> access,
                                    std::shared_ptr<IPuzzleMarker> marker,
                                    std::shared_ptr<ISolver> solver )
    : _executor( exec ),
      _puzzleAccess( access ),
      _puzzleMarker( marker ),
      _solver( solver )
{
    if ( !_executor )
    {
        throw std::runtime_error(
            "Cannot create PuzzleController with NULL CommandExecutor." );
    }
    if ( !_puzzleAccess )
    {
        throw std::runtime_error(
            "Cannot create PuzzleController with NULL PuzzleAccess." );
    }
    if ( !_puzzleMarker )
    {
        throw std::runtime_error(
            "Cannot create PuzzleController with NULL PuzzleMarker." );
    }
    if ( !_solver )
    {
        throw std::runtime_error(
            "Cannot create PuzzleController with NULL Solver.");
    }
}

void PuzzleController::MarkHints()
{
    std::shared_ptr<const Puzzle> puzzle = _puzzleAccess->GetPuzzle();
    std::shared_ptr<CommandBase> command = 
        AddHintMarksCommand::CreateAddHintMarksCommand( puzzle, _puzzleMarker );
    _executor->Execute( command );
}

void PuzzleController::Solve()
{
    std::shared_ptr<const Puzzle> puzzle = _puzzleAccess->GetPuzzle();
    std::shared_ptr<CommandBase> command =
        SolveCommand::Create( puzzle, _solver );
    _executor->Execute( command );
}

}
