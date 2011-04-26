#ifndef SUDOKU_GAME_CONTROLLER_H
#define SUDOKU_GAME_CONTROLLER_H

#include <memory>
#include <string>

namespace Sudoku
{

class ICommandExecutor;
class IFileImporter;

class GameController
{
public:
    GameController( std::shared_ptr<ICommandExecutor> exec,
                    std::shared_ptr<IFileImporter> importer );

    void Undo();

    void Redo();

    void ImportFromFile( const std::string &filename );

    ~GameController() {}
private:
    /// to do undo/redo
    std::shared_ptr<ICommandExecutor> _executor;
    /// to import puzzles from file
    std::shared_ptr<IFileImporter> _importer;
};

}

#endif
