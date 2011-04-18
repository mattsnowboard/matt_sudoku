#ifndef SUDOKU_SOLVER_HELPER_H
#define SUDOKU_SOLVER_HELPER_H

#include <memory>
#include <vector>

#include "Puzzle.h"

namespace Sudoku
{

// Various solving methods we will find
class SolutionMethod;
class BlockIntersectionMethod;
class CoveringSetMethod;
class ExclusionMethod;
class SingleCandidateMethod;

class SolutionMethodFactory;

/**
 * Class to help a Solver
 * Works by finding all possible moves
 */
class SolverHelper
{
public:
    /// Container of SolutionMethod pointers
    typedef std::vector<std::shared_ptr<SolutionMethod> > MethodContainer;

    SolverHelper( std::shared_ptr<SolutionMethodFactory> factory )
        : _factory( factory ) {}

    /**
     * Gets all Cells with only one Mark
     * @param p Puzzle to look at
     * @return Collection of SingleCandidateMethod ojbects which can be executed
     */
    MethodContainer GetAllSingleCandidate( std::shared_ptr<Puzzle> p );

    // For ExclusionMethod, we do that for when adding a guess

    /**
     * Get all the ways to do a block intersection for a given Cell
     * This will attempt all combinations of Row/Column/Block
     * As well as looking at all marks on the given Cell c
     * @param p Puzzle to look at
     * @param c Cell to look at
     * @return Collection of BlockIntersectionMethod objects around c which
     *         can be executed
     */
    MethodContainer GetAllBlockIntersection( std::shared_ptr<Puzzle> p,
                                             std::shared_ptr<Cell> c );

    /**
     * Get all the covering sets for a given sector of a puzzle
     * This will look at a sector and find all groups of 2 to 4 cells
     * Which have that same number of marks in common
     * @param p Puzzle to look at
     * @param sector Sector to look in
     * @return Collection of CoveringSetMethod objects which can be executed
     *         in that sector
     */
    MethodContainer GetAllCoveringSet( std::shared_ptr<Puzzle> p,
                                       const Puzzle::Container &sector );

private:
    std::shared_ptr<SolutionMethodFactory> _factory;
};

}

#endif
