#ifndef SUDOKU_GAME_MANAGER_H
#define SUDOKU_GAME_MANAGER_H

#include <memory>
#include <stack>
#include <vector>
#include "ICommandDispatcher.h"
#include "ICommandExecutor.h"
#include "IPuzzleAccess.h"
#include "IFileImporter.h"
#include "ICommandObserver.h"

namespace Sudoku
{

class CellController;
class GameController;
class PuzzleController;
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

    static std::shared_ptr<GameManager> Create();

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
     * Reset the puzzle pointer and clear what exists
     * @post Puzzle is blank
     */
    virtual void NewPuzzle();

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

    /**
     * Add an observer to listen for changes to the command state
     * @param o Observer to add
     * @post o gets updates
     */
    virtual void AddObserver( std::shared_ptr<ICommandObserver> o );

    /**
     * Remove an observer to stop listening for changes to the command state
     * @param o Observer to remove
     * @pre o was observing Cell
     * @post o does not get updates
     */
    virtual void RemoveObserver( std::shared_ptr<ICommandObserver> o );

    /**
     * Double dispatch on Cell Commands
     * Gets pointer to non-const Cell and executes
     * @param c Command to execute
     */
    virtual void HandleCommand( Command<Cell> &c );

    /**
     * Double dispatch on Puzzle Commands
     * Gets pointer to non-const Puzzle and executes
     * @param c Command to execute
     */
    virtual void HandleCommand( Command<Puzzle> &p );

    /**
     * Double Dispatch default
     * This is an unknown type of command
     * @throw Always
     */
    virtual void HandleCommand( CommandBase &b );

    /**
     * Get a const pointer to the Puzzle
     * @return const pointer to internal Puzzle
     */
    virtual std::shared_ptr<const Puzzle> GetPuzzle() const;

    /**
     * Lets a listener attach to all Cells in the Puzzle
     * This can't be done with the const Puzzle that is accessible
     * @param o Observer to add to each Cell in current Puzzle
     * @post o gets notified by Cells on update
     */
    virtual void ListenToAllCells( std::shared_ptr<ICellObserver> o );

    /**
     * Accessor for the Game Controller
     * @return Game Controller pointer
     */
    std::shared_ptr<GameController> GetGameController()
    {
        return _gameController;
    }

    /**
     * Accessor for the Puzzle Controller
     * @return Puzzle Controller pointer
     */
    std::shared_ptr<PuzzleController> GetPuzzleController()
    {
        return _puzzleController;
    }

    /**
     * Accessor for the Cell Controller
     * @return Cell Controller pointer
     */
    std::shared_ptr<CellController> GetCellController()
    {
        return _cellController;
    }

    virtual ~GameManager() {}

protected:
    /**
     * Protected to ensure we only use shared_ptr
     */
    GameManager();

    /**
     * Notify all observers that undo is enabled/disabled
     * @param yes Enbale or disable
     * @post observers get update
     */
    void notifyCanUndo( bool yes );

    /**
     * Notify all observers that redo is enabled/disabled
     * @param yes Enbale or disable
     * @post observers get update
     */
    void notifyCanRedo( bool yes );

    /**
     * Add the Game Controller
     * Need to initialize this before so we can pass self to constructor
     * @param gameController GameController
     */
    void addGameController( std::shared_ptr<GameController> gameController );

    /**
     * Add the Puzzle Controller
     * Need to initialize this before so we can pass self to constructor
     * @param puzzleController PuzzleController
     */
    void addPuzzleController(
        std::shared_ptr<PuzzleController> puzzleController );

    /**
     * Add the Cell Controller
     * Need to initialize this before so we can pass self to constructor
     * @param cellController CellController
     */
    void addCellController( std::shared_ptr<CellController> cellController );

private:
    GameManager( const GameManager & );
    GameManager & operator=( const GameManager & );

    /**
     * Clear undo stack and notify
     */
    void clearUndo();

    /**
     * Clear redo stack and notify
     */
    void clearRedo();

    /**
     * Whenever we have a new Puzzle, we need to reattach those that care
     */
    void attachAllCellObservers();

    /**
     * Actually attaching a single observer to all cells
     * @pre _puzzle != NULL
     */
    void attachCellObserver( std::shared_ptr<ICellObserver> o );

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

    /// Who cares about the command state
    typedef std::vector<std::shared_ptr<ICommandObserver> > ObserverContainer;
    ObserverContainer _observers;
    
    /// Deferred Cell listeners
    typedef std::vector<std::shared_ptr<ICellObserver> > CellObserverContainer;
    CellObserverContainer _cellObservers;

    /// Controllers for other modules to use
    std::shared_ptr<GameController> _gameController;
    std::shared_ptr<PuzzleController> _puzzleController;
    std::shared_ptr<CellController> _cellController;

    /// These are used to import from a file
    typedef std::vector<std::shared_ptr<IPuzzleImporter> >  ImporterContainer;
    ImporterContainer _importers;
};

}

#endif
