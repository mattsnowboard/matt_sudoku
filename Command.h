#ifndef SUDOKU_COMMAND_H
#define SUDOKU_COMMAND_H

#include <stdexcept>

namespace Sudoku
{

/**
 * Base class for user-runnable commands
 * All commands derive from this and define execute/unexecute
 *
 * @note we force the convention that commands cannot be executed more than once
 * without unexecuting first and cannot be unexecuted unless they have been
 * executed
 */
class Command
{
public:

    /**
     * Execute
     * This should /b NOT be overridden by base
     * Checks/sets the state of the Command and calls protected virtual
     * @pre Not in Executed state
     * @throw if in execute state and we try to execute again
     * @return success or failure of command execution
     * @post _isExecuted == true if success
     */
    /* final */ bool Execute()
    {
        if ( _isExecuted )
        {
            // executing twice could be bad
            throw std::runtime_error( "Attempt to execute command twice!" );
        }
        // execute, record result and return result
        return ( _isExecuted = execute() );
    }

    /**
     * Unexecute (undo)
     * This should /b NOT be overridden by base
     * Checks/sets the state of the Command and calls protected virtual
     * @pre In Executed state
     * @throw if not in execute state and we try to unexecute again
     * @return success or failure of command uneecution
     * @post _isExecuted == false if success
     */
    /* final */ bool Unexecute()
    {
        if ( !_isExecuted )
        {
            // executing twice could be bad
            throw std::runtime_error( "Attempt to unexecute command twice!" );
        }
        // unexecute, record state, return if unexecute succeeded
        return !( _isExecuted = !unexecute() );
    }

    /**
     * This class is virtual, need derived destructors to get called
     */
    virtual ~Command() {}

protected:
    /**
     * Just set the state of the command as unexecuted
     */
    Command() : _isExecuted( false ) {}

    /**
     * The execute action is defined by derived classes
     * @return Success or Failure
     */
    virtual bool execute() = 0;

    /**
     * The unexecute action is defined by derived classes
     * @return Success or Failure
     */
    virtual bool unexecute() = 0;

private:
    /// Keep track of the state of the command
    bool _isExecuted;
};

}

#endif
