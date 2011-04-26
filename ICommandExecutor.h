#ifndef SUDOKU_ICOMMAND_EXECUTOR_H
#define SUDOKU_ICOMMAND_EXECUTOR_H

#include <memory>

namespace Sudoku
{

class CommandBase;

/**
 * Class which can execute commands and hold undo/redo stack
 * Commands cannot be executed with const pointers, this guy translates
 * the const pointer to a non-const so he must have a way to get non-const
 */
class ICommandExecutor
{
public:

    /**
     * Execute a command
     * @param command Command to execute
     * @return success or failure
     * @post if success, command is pushed on undo stack, redo stack is cleared
     */
    virtual bool Execute( std::shared_ptr<CommandBase> command ) = 0;

    /**
     * Check if there is anything to undo
     * @return false if no commands in undo stack
     */
    virtual bool CanUndo() const = 0;

    /**
     * Attempt to undo the last command
     * @return success or failure (such as no undo commands remain)
     * @post If not empty, last command executed is unexecuted and pushed on
     *       to redo stack
     */
    virtual bool Undo() = 0;

    /**
     * Check if there is anything to redo
     * @return false if no commands in redo stack
     */
    virtual bool CanRedo() const = 0;

    /**
     * Attempt to redo the last command we undid
     * @return success or failure (such as no redo commands remain)
     * @post If not empty, last command unexecuted is executed and pushed on
     *       to undo stack
     */
    virtual bool Redo() = 0;

    virtual ~ICommandExecutor() {}
};

}

#endif
