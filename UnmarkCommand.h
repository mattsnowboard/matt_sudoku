#ifndef SUDOKU_UNMARK_COMMAND_H
#define SUDOKU_UNMARK_COMMAND_H

#include "Command.h"
#include <memory>

namespace Sudoku
{

class Cell;

/**
 * Unmark Command to remove a mark from a Cell
 */
class UnmarkCommand : public Command
{
public:

    /**
     * Force user to create shared_ptr's
     * @param cell The Cell to unmark
     * @param mark The value to unmark
     */
    static std::shared_ptr<Command> CreateUnmarkCommand(
        std::shared_ptr<Cell> cell, int mark );

    virtual ~UnmarkCommand() {}

private:
    /**
     * Create the Command
     * @param cell The Cell to unmark
     * @param mark The value to unmark
     */
    UnmarkCommand( std::shared_ptr<Cell> cell, int mark );

    /**
     * Unmark the given value on the Cell, store the old value
     * @pre we can execute
     * @return Success or Failure
     * @post we can unexecute
     */
    virtual bool execute();

    /**
     * Mark the given value on the Cell if we had removed the mark
     * @pre we can unexecute
     * @return Success or Failure
     * @post we can execute
     */
    virtual bool unexecute();

private:
    /// Cell we are acting upon
    std::shared_ptr<Cell> _cell;
    /// The value we want to mark
    int _mark;
    /// The previous state of the mark
    bool _wasMarked;
};

}

#endif
