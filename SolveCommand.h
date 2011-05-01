#ifndef SUDOKU_SOLVE_COMMAND_H
#define SUDOKU_SOLVE_COMMAND_H

#include "Command.h"
#include <memory>
#include <vector>
#include "Cell.h"

namespace Sudoku
{

class Puzzle;
class ISolver;

/**
 * A command to solve a puzzle
 */
class SolveCommand : public Command<Puzzle>
{
public:
    /**
     * Force user to create shared_ptr's
     * @param puzzle The Puzzle to add hint marks to
     */
    static std::shared_ptr<Command<Puzzle> > Create(
        std::shared_ptr<const Puzzle> puzzle,
        std::shared_ptr<ISolver> solver );

    virtual ~SolveCommand() {}

protected:
    /**
     * Create the Command
     * @param puzzle The Puzzle to solve
     */
    SolveCommand( std::shared_ptr<const Puzzle> Puzzle,
                  std::shared_ptr<ISolver> solver );

    /**
     * Use the solver
     * @pre we can execute
     * @param non-const Puzzle to execute on
     * @return Success or Failure
     * @post we can unexecute
     */
    virtual bool execute( std::shared_ptr<Puzzle> p );

    /**
     * Undo the finding the solution (restore to previous state)
     * @pre we can unexecute
     * @param non-const Cell to execute on
     * @return Success or Failure
     * @post we can execute
     */
    virtual bool unexecute( std::shared_ptr<Puzzle> p );

private:
    SolveCommand( const SolveCommand & );
    SolveCommand & operator=( const SolveCommand & );

    /// Marker can add hints
    std::shared_ptr<ISolver> _solver;
    /// Need to store old values (Memento)
    std::shared_ptr<Puzzle> _copyPuzzle;
};

}

#endif
