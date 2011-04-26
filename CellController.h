#ifndef SUDOKU_CELL_CONTROLLER_H
#define SUDOKU_CELL_CONTROLLER_H

#include <memory>

namespace Sudoku
{

class ICommandExecutor;
class IPuzzleAccess;

class CellController
{
public:
    /**
     * Constructor, pass a few essential interfaces
     * @param exec Class that can execute commands
     * @param access Class that can return a pointer to a const Puzzle
     */
    CellController( std::shared_ptr<ICommandExecutor> exec,
                    std::shared_ptr<IPuzzleAccess> access );

    /**
     * Make a guess for a Cell at a position by executing a command
     * @param x X position of Cell (column)
     * @param y Y position of Cell (row)
     * @param guess The value to guess
     * @post Attempts to execute the command
     */
    void MakeGuess( size_t x, size_t y, int guess );

    /**
     * Make a mark for a Cell at a position by executing a command
     * @param x X position of Cell (column)
     * @param y Y position of Cell (row)
     * @param mark The value to mark
     * @post Attempts to execute the command
     */
    void Mark( size_t x, size_t y, int mark );

    /**
     * Remove a mark for a Cell at a position by executing a command
     * @param x X position of Cell (column)
     * @param y Y position of Cell (row)
     * @param mark The value to unmark
     * @post Attempts to execute the command
     */
    void Unmark( size_t x, size_t y, int mark );

    ~CellController() {}

private:
    CellController( const CellController & );
    CellController& operator=( const CellController & );

    /// Executes commands
    std::shared_ptr<ICommandExecutor> _executor;
    /// get the pointer to the Puzzle
    std::shared_ptr<IPuzzleAccess> _puzzleAccess;
};

}

#endif
