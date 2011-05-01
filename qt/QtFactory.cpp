#include "QtFactory.h"
#include "QtDirector.h"
#include "QtPuzzleModel.h"

#include <GameManager.h>

namespace QtSudoku
{

std::shared_ptr<QtDirector> QtFactory::CreateDirector( int &argc, char **argv )
{
    _manager = Sudoku::GameManager::Create();
    std::shared_ptr<QtPuzzleModel> puzzleModel(
        new QtPuzzleModel( NULL,
                           _manager,
                           _manager->GetGameController(),
                           _manager->GetPuzzleController(),
                           _manager->GetCellController() ) );
    _manager->ListenToAllCells( puzzleModel );
    std::shared_ptr<QtDirector> director(
        new QtDirector( argc, argv,
                        _manager,
                        _manager->GetGameController(),
                        puzzleModel ) );
    _manager->AddObserver( director );
    return director;
}

}
