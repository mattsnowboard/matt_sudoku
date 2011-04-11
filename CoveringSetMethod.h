#ifndef SUDOKU_COVERING_SET_METHOD_H
#define SUDOKU_COVERING_SET_METHOD_H

#include <memory>
#include "SolutionMethod.h"
#include "Puzzle.h"

namespace Sudoku
{

class Cell;

class CoveringSetMethod : public SolutionMethod
{
public:
    /**
     * Create the method for a specific subset of a sector
     */
    CoveringSetMethod( Puzzle::Container sector, Puzzle::Container subset );

    /**
     * This will remove the marks contained in subset from sector\subset
     * @pre VerifyForwardConditions
     * @post the set sector\subset will not have any marks from subset
     */
    virtual void ExecuteForward();

    /**
     * Verify that the subset size is equal to the number of marks it contains
     * Verify that subset is a subset of sector
     * @return true if the precondition holds
     */
    virtual bool VerifyForwardConditions();

    /**
     * This will mark the marks from the subset in all sector\subset cells
     * which do not already have a guess set.
     * @pre VerifyReverseConditions
     * @post All marks found in subset will be marked in sector unless
     *       a Cell already has a guess or a solution
     */
    virtual void ExecuteReverse();

    /**
     * Verify that the subset size is equal to the number of marks it contains
     * Verify that sector\subset does not have any of the marks found in subset
     * Verify that subset is a subset of sector
     * @return true if the precondition holds
     */
    virtual bool VerifyReverseConditions();

    /**
     * Do nothing
     */
    virtual ~CoveringSetMethod() {}

private:
    Puzzle::Container _sector;
    Puzzle::Container _subset;
};

}

#endif
