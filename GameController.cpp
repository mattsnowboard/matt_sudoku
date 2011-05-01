#include "GameController.h"
#include "ICommandExecutor.h"
#include "IFileImporter.h"

namespace Sudoku
{

GameController::GameController( std::shared_ptr<ICommandExecutor> exec,
                                std::shared_ptr<IFileImporter> importer )
    : _executor( exec ), _importer( importer )
{}

void GameController::Undo()
{
    if ( _executor->CanUndo() )
    {
        _executor->Undo();
    }
}

void GameController::Redo()
{
    if ( _executor->CanRedo() )
    {
        _executor->Redo();
    }
}

bool GameController::ImportFromFile( const std::string &filename )
{
    return _importer->ImportFromFile( filename );
}

void GameController::NewPuzzle()
{
    _importer->NewPuzzle();
}

}
