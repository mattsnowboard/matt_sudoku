#ifndef SUDOKU_MARK_COMMAND_H
#define SUDOKU_MARK_COMMAND_H

#include "Command.h"
#include <memory>

namespace Sudoku
{

class Cell;

/**
 * Mark Command to add a mark to a Cell
 */
class MarkCommand : public Command
{
public:

    /**
     * Force user to create shared_ptr's
     * @param cell The Cell to mark
     * @param mark The value to mark
     */
    static std::shared_ptr<Command> CreateMarkCommand(
        std::shared_ptr<Cell> cell, int mark );

    virtual ~MarkCommand() {}

private:
    /**
     * Create the Command
     * @param cell The Cell to mark
     * @param guess The value to mark
     */
    MarkCommand( std::shared_ptr<Cell> cell, int mark );

    /**
     * Mark the given value on the Cell, store the old value
     * @pre we can execute
     * @return Success or Failure
     * @post we can unexecute
     */
    virtual bool execute();

    /**
     * Unmark the given value on the Cell if we had placed the mark
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
