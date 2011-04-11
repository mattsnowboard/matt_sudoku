#ifndef SUDOKU_BLOCK_INTERSECTION_METHOD_H
#define SUDOKU_BLOCK_INTERSECTION_METHOD_H

#include <memory>
#include "SolutionMethod.h"
#include "Puzzle.h"

namespace Sudoku
{

class Cell;

class BlockIntersectionMethod : public SolutionMethod
{
public:
    /**
     * Create the method for a specific Cell
     */
    BlockIntersectionMethod( std::shared_ptr<Cell> c,
                             unsigned mark,
                             Puzzle::Container primary,
                             Puzzle::Container secondary );

    /**
     * This will remove the mark from all cells in the primary group
     * @pre VerifyForwardConditions
     * @post Primary and secondary groups will not have the mark
     */
    virtual void ExecuteForward();

    /**
     * Verify that the given cell has the given mark set
     * verify that the given mark is in the primary group
     * Verify that the given mark is not contained in the secondary group
     * @return true if the precondition holds
     */
    virtual bool VerifyForwardConditions();

    /**
     * This will mark the given value for all cells in the primary group
     * @pre VerifyReverseConditions
     * @post Primary group will have the given value marked, secondary will not
     *       This does not affect primary group cells with empty marks
     */
    virtual void ExecuteReverse();

    /**
     * Verify that the given cell has the given mark set
     * verify that the given mark is not in the primary group
     * verify that there is a cell in the primary group with marks
     * Verify that the given mark is not contained in the secondary group
     * @return true if the precondition holds
     */
    virtual bool VerifyReverseConditions();

    /**
     * Do nothing
     */
    virtual ~BlockIntersectionMethod() {}

private:
    /**
     * We can separately ensure that the given sets fit some rules
     * This is independent to whether the Method applies, it is more fundamental
     * @throw exception if the subsets given don't make sense
     */
    void verifyGeometry();

private:
    /// Cell we are looking at
    std::shared_ptr<Cell> _cell;
    /// The mark we are using
    unsigned _mark;
    /// The (sector - 3 common Cells) which contains the mark
    Puzzle::Container _primary;
    /// The (sector - 3 common Cells) which does not contain the mark
    Puzzle::Container _secondary;
};

}

#endif
