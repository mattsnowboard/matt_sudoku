#ifndef SUDOKU_SINGLE_CANDIDATE_METHOD_H
#define SUDOKU_SINGLE_CANDIDATE_METHOD_H

#include <memory>
#include "SolutionMethod.h"

namespace Sudoku
{

class Cell;

class SingleCandidateMethod : public SolutionMethod
{
public:
    /**
     * Create the method for a specific Cell
     */
    SingleCandidateMethod( std::shared_ptr<Cell> c ) : _cell( c ) {}

    /**
     * This will remove the single remaining mark and set it as the guess
     * @pre Cell given has only one Mark (which is accurate to the Puzzle)
     * @post Cell has no marks and has a guess set
     */
    virtual void ExecuteForward();

    /**
     * Verify that the cell has only one mark
     * @return true if the precondition holds
     */
    virtual bool VerifyForwardConditions();

    /**
     * Remove the guess from a Cell (set it to 0) and set it as a Mark
     * @pre Cell has a guess and no marks
     * @post Cell has a mark and no guess
     */
    virtual void ExecuteReverse();

    /**
     * Verify that the cell has a guess and no marks
     * @return true if the precondition holds
     */
    virtual bool VerifyReverseConditions();

    /**
     * Do nothing
     */
    virtual ~SingleCandidateMethod() {}

private:
    SingleCandidateMethod( const SingleCandidateMethod & );
    SingleCandidateMethod & operator=( const SingleCandidateMethod & );

    std::shared_ptr<Cell> _cell;
};

}

#endif
