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
    /**
     * Game Controller needs a few Interfaces
     * @param exec Command Executor to do undo/redo
     * @param importer File importer
     */
    GameController( std::shared_ptr<ICommandExecutor> exec,
                    std::shared_ptr<IFileImporter> importer );

    /**
     * Undo a command
     */
    void Undo();

    /**
     * Redo a command
     */
    void Redo();

    /**
     * Load a Puzzle from a file
     * @param filename The file to load
     * @post Puzzle pointer in GameManager will be set on success
     */
    bool ImportFromFile( const std::string &filename );

    /**
     * Create a new Puzzle (resets what was there)
     * @post Puzzle pointer in GameManager will be reset
     */
    void NewPuzzle();

    ~GameController() {}
private:
    GameController( const GameController & );
    GameController & operator=( const GameController & );

    /// to do undo/redo
    std::shared_ptr<ICommandExecutor> _executor;
    /// to import puzzles from file
    std::shared_ptr<IFileImporter> _importer;
};

}

#endif
