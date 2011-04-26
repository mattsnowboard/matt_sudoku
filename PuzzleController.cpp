
#include "PuzzleController.h"
#include "IPuzzleMarker.h"

namespace Sudoku
{

PuzzleController::PuzzleController( std::shared_ptr<ICommandExecutor> exec,
                                    std::shared_ptr<IPuzzleAccess> access,
                                    std::shared_ptr<IPuzzleMarker> marker )
    : _executor( exec ), _puzzleAccess( access ), _puzzleMarker( marker )
{}

void PuzzleController::MarkHints()
{
    std::shared_ptr<const Puzzle> puzzle = _puzzleAccess->GetPuzzle();
    std::shared_ptr<CommandBase> command = 
        AddHintMarksCommand::CreateAddHintMarksCommand( puzzle, _puzzleMarker );
    _executor->Execute( command );
}

}
