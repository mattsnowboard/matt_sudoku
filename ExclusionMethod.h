#ifndef SUDOKU_EXCLUSION_METHOD_H
#define SUDOKU_EXCLUSION_METHOD_H

#include <memory>
#include "SolutionMethod.h"
#include "Puzzle.h"

namespace Sudoku
{

class Cell;

class ExclusionMethod : public SolutionMethod
{
public:
    /**
     * Create the method for a specific Cell
     */
    ExclusionMethod( std::shared_ptr<Puzzle> p, std::shared_ptr<Cell> c );

    /**
     * This will remove the marks from all neighbors of a guessed cell
     * @pre Cell given has a guess
     * @post Neighbors of Cell have the Cell's value removed from marks
     */
    virtual void ExecuteForward();

    /**
     * Verify that the cell has a guess
     * @return true if the precondition holds
     */
    virtual bool VerifyForwardConditions();

    /**
     * This will mark the guessed value of the Cell on each neighbor with no
     * guessed value
     * @pre Cell has a guess
     * @post Neighbors of Cell have the Cell's guessed value marked if
     *       they did not have a guessed value
     */
    virtual void ExecuteReverse();

    /**
     * Verify that the cell has a guess and the neighbors don't have it marked
     * @return true if the precondition holds
     */
    virtual bool VerifyReverseConditions();

    /**
     * Do nothing
     */
    virtual ~ExclusionMethod() {}

private:
    ExclusionMethod( const ExclusionMethod & );
    ExclusionMethod & operator=( const ExclusionMethod & );

    Puzzle::Container _neighbors;
    std::shared_ptr<Cell> _cell;
    int _guessVal;
};

}

#endif
