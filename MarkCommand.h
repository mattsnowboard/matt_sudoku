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
class MarkCommand : public Command<Cell>
{
public:

    /**
     * Force user to create shared_ptr's
     * @param cell The Cell to mark
     * @param mark The value to mark
     */
    static std::shared_ptr<Command<Cell> > CreateMarkCommand(
        std::shared_ptr<const Cell> cell, int mark );

    virtual ~MarkCommand() {}

private:
    /**
     * Create the Command
     * @param cell The Cell to mark
     * @param guess The value to mark
     */
    MarkCommand( std::shared_ptr<const Cell> cell, int mark );

    /**
     * Mark the given value on the Cell, store the old value
     * @pre we can execute
     * @param non-const Cell to execute on
     * @return Success or Failure
     * @post we can unexecute
     */
    virtual bool execute( std::shared_ptr<Cell> c );

    /**
     * Unmark the given value on the Cell if we had placed the mark
     * @pre we can unexecute
     * @param non-const Cell to execute on
     * @return Success or Failure
     * @post we can execute
     */
    virtual bool unexecute( std::shared_ptr<Cell> c );

private:
    /// The value we want to mark
    int _mark;
    /// The previous state of the mark
    bool _wasMarked;
};

}

#endif
