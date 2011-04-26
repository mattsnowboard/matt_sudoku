#ifndef SUDOKU_GUESS_COMMAND_H
#define SUDOKU_GUESS_COMMAND_H

#include "Command.h"
#include <memory>

namespace Sudoku
{

class Cell;

/**
 * Guess Command to set the Guess value of a Cell
 */
class GuessCommand : public Command<Cell>
{
public:

    /**
     * Force user to create shared_ptr's
     * @param cell The Cell to issue a guess on
     * @param guess The value to guess
     */
    static std::shared_ptr<Command<Cell> > CreateGuessCommand(
        std::shared_ptr<const Cell> cell, int guess );

    virtual ~GuessCommand() {}

private:
    /**
     * Create the Command
     * @param cell The Cell to issue a guess on
     * @param guess The value to guess
     */
    GuessCommand( std::shared_ptr<const Cell> cell, int guess );

    /**
     * Set the guess value on a Cell, store the old value
     * @pre we can execute
     * @param non-const Cell to execute on
     * @return Success or Failure
     * @post we can unexecute
     */
    virtual bool execute( std::shared_ptr<Cell> c );

    /**
     * Set the guess value on a Cell to its previous value
     * @pre we can unexecute
     * @param non-const Cell to execute on
     * @return Success or Failure
     * @post we can execute
     */
    virtual bool unexecute( std::shared_ptr<Cell> c );

private:
    /// The value we want to guess
    int _newGuess;
    /// The previous value for undo
    int _oldGuess;
};

}

#endif
