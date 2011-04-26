#ifndef SUDOKU_GAME_MANAGER_H
#define SUDOKU_GAME_MANAGER_H

#include <memory>
#include <stack>
#include <vector>
#include "ICommandDispatcher.h"
#include "ICommandExecutor.h"
#include "IPuzzleAccess.h"
#include "IFileImporter.h"

namespace Sudoku
{

class ICommandDispatcher;
class IPuzzleImporter;
class Puzzle;
class Cell;

class GameManager : public ICommandExecutor,
                    public IPuzzleAccess,
                    public ICommandDispatcher,
                    public IFileImporter
{
public:

    GameManager();

    /**
     * Import a Puzzle from a file
     * This will use modified Chain of Responsibility pattern
     * to figure out which format the file is in
     * @param filename The file we will load and try to read
     * @return success or failure
     * @post If we succeed, Puzzle is set, else it is still NULL
     */
    bool ImportFromFile( const std::string &filename );

    /**
     * Execute a command
     * @param command Command to execute
     * @return success or failure
     * @post if success, command is pushed on undo stack, redo stack is cleared
     */
    virtual bool Execute( std::shared_ptr<CommandBase> command );

    /**
     * Check if there is anything to undo
     * @return false if no commands in undo stack
     */
    virtual bool CanUndo() const { return !_undo.empty(); }

    /**
     * Attempt to undo the last command
     * @return success or failure (such as no undo commands remain)
     * @post If not empty, last command executed is unexecuted and pushed on
     *       to redo stack
     */
    virtual bool Undo();

    /**
     * Check if there is anything to redo
     * @return false if no commands in redo stack
     */
    virtual bool CanRedo() const { return !_redo.empty(); }

    /**
     * Attempt to redo the last command we undid
     * @return success or failure (such as no redo commands remain)
     * @post If not empty, last command unexecuted is executed and pushed on
     *       to undo stack
     */
    virtual bool Redo();

    virtual void HandleCommand( Command<Cell> &c );

    virtual void HandleCommand( Command<Puzzle> &p );

    virtual void HandleCommand( CommandBase &b );

    /**
     * Get a const pointer to the Puzzle
     * @return const pointer to internal Puzzle
     */
    std::shared_ptr<const Puzzle> GetPuzzle() const { return _puzzle; }

    virtual ~GameManager() {}

private:
    /// Keep track of the last command executed
    bool _lastExecuted;
    /// Keep track of what action to do when double dispatching
    bool _isExecute;
    /// This is the entire Sudoku board
    std::shared_ptr<Puzzle> _puzzle;
    /// Commands we can still undo
    std::stack<std::shared_ptr<CommandBase> > _undo;
    /// Commands we can still redo
    std::stack<std::shared_ptr<CommandBase> > _redo;

    /// These are used to import from a file
    typedef std::vector<std::shared_ptr<IPuzzleImporter> >  ImporterContainer;
    ImporterContainer _importers;
};

}

#endif
