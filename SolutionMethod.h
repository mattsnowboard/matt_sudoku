#ifndef SUDOKU_SOLUTION_METHOD_H
#define SUDOKU_SOLUTION_METHOD_H

namespace Sudoku
{

/**
 * Abstract class to define generic methods used to solve a puzzle
 * We will define specific methods in a way that they are all reversible
 * This allows one to generate solvable Sudoku puzzles in addition to solving
 */
class SolutionMethod
{
public:
    /**
     * Execute the method on the Puzzle
     * @pre Should verify the conditions first
     * @post Will modify the Puzzle as a step toward solving it
     */
    virtual void ExecuteForward() = 0;

    /**
     * Verify the assumed conditions for this method in the forward case
     * @return true if the given data can execute the method
     */
    virtual bool VerifyForwardConditions() = 0;

    /**
     * Execute the reverse method on the Puzzle
     * @pre Should verify the conditions first
     * @post Will modify the Puzzle as a step toward unsolving it
     */
    virtual void ExecuteReverse() = 0;

    /**
     * Verify the assumed conditions for this method in the reverse case
     * @return true if the given data can execute the method in reverse
     */
    virtual bool VerifyReverseConditions() = 0;

    /**
     * Allow deleting pointers to this base class
     */
    virtual ~SolutionMethod() {}

};

}

#endif
