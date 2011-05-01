#ifndef SUDOKU_PUZZLE_CONTROLLER_H
#define SUDOKU_PUZZLE_CONTROLLER_H

#include <memory>

namespace Sudoku
{

class ICommandExecutor;
class IPuzzleAccess;
class IPuzzleMarker;
class ISolver;

class PuzzleController
{
public:
    /**
     * Constructor, pass a few essential interfaces
     * @param exec Class that can execute commands
     * @param access Class that can return a pointer to a const Puzzle
     */
    PuzzleController( std::shared_ptr<ICommandExecutor> exec,
                      std::shared_ptr<IPuzzleAccess> access,
                      std::shared_ptr<IPuzzleMarker> marker,
                      std::shared_ptr<ISolver> solver );

    /**
     * Mark all Cells with their possible values to give hints to user
     * @post Cells are marked appropriately
     * @note undoable
     */
    void MarkHints();

    /**
     * Attempt to solve an unsolved puzzle
     * @pre Puzzle lacks solution
     * @post Solved puzzle is displayed
     */
    void Solve();

    ~PuzzleController() {}

private:
    PuzzleController( const PuzzleController & );
    PuzzleController& operator=( const PuzzleController & );

    /// Executes commands
    std::shared_ptr<ICommandExecutor> _executor;
    /// get the pointer to the Puzzle
    std::shared_ptr<IPuzzleAccess> _puzzleAccess;
    /// marker
    std::shared_ptr<IPuzzleMarker> _puzzleMarker;
    /// solver
    std::shared_ptr<ISolver> _solver;
};

}

#endif
