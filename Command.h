#ifndef SUDOKU_COMMAND_H
#define SUDOKU_COMMAND_H

#include <stdexcept>
#include <memory>

#include "ICommandDispatcher.h"

namespace Sudoku
{

class CommandBase
{
public:
    virtual void Accept( ICommandDispatcher &d ) = 0;

    virtual ~CommandBase() {}
};

/**
 * Base class for user-runnable commands
 * All commands derive from this and define execute/unexecute
 * Commands all work on single shared_ptr target
 *
 * @note we force the convention that commands cannot be executed more than once
 * without unexecuting first and cannot be unexecuted unless they have been
 * executed
 */
template <class T>
class Command : public CommandBase
{
public:

    /**
     * Execute
     * This should /b NOT be overridden by base
     * Checks/sets the state of the Command and calls protected virtual
     * @pre Not in Executed state
     * @throw if in execute state and we try to execute again
     * @param non-const pointer to execute on
     * @return success or failure of command execution
     * @post _isExecuted == true if success
     */
    /* final */ bool Execute( std::shared_ptr<T> t )
    {
        if ( _isExecuted )
        {
            // executing twice could be bad
            throw std::runtime_error( "Attempt to execute command twice!" );
        }
        if ( !t || t != _target )
        {
            throw std::invalid_argument( "Executing on the wrong target." );
        }
        // execute, record result and return result
        return ( _isExecuted = execute( t ) );
    }

    /**
     * Unexecute (undo)
     * This should /b NOT be overridden by base
     * Checks/sets the state of the Command and calls protected virtual
     * @pre In Executed state
     * @throw if not in execute state and we try to unexecute again
     * @param non-const pointer to execute on
     * @return success or failure of command uneecution
     * @post _isExecuted == false if success
     */
    /* final */ bool Unexecute( std::shared_ptr<T> t )
    {
        if ( !_isExecuted )
        {
            // executing twice could be bad
            throw std::runtime_error( "Attempt to unexecute command twice!" );
        }
        if ( !t || t != _target )
        {
            throw std::invalid_argument( "Unexecuting on the wrong taget." );
        }
        // unexecute, record state, return if unexecute succeeded
        return !( _isExecuted = !unexecute( t ) );
    }

    std::shared_ptr<const T> GetTarget() const { return _target; }

    virtual void Accept( ICommandDispatcher &d )
    {
        d.HandleCommand( *this );
    }

    /**
     * This class is virtual, need derived destructors to get called
     */
    virtual ~Command() {}

protected:
    /**
     * Just set the state of the command as unexecuted
     */
    Command( std::shared_ptr<const T> target)
        : _target( target ),
          _isExecuted( false ) {}

    /**
     * The execute action is defined by derived classes
     * @return Success or Failure
     */
    virtual bool execute( std::shared_ptr<T> t ) = 0;

    /**
     * The unexecute action is defined by derived classes
     * @return Success or Failure
     */
    virtual bool unexecute( std::shared_ptr<T> t ) = 0;

    /// The target to act on (const)
    std::shared_ptr<const T> _target;

private:
    /// Keep track of the state of the command
    bool _isExecuted;
};

}

#endif
