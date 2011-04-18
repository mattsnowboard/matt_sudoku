#ifndef SUDOKU_SOLUTION_METHOD_FACTORY_H
#define SUDOKU_SOLUTION_METHOD_FACTORY_H

#include <memory>

#include "SingleCandidateMethod.h"
#include "ExclusionMethod.h"
#include "BlockIntersectionMethod.h"
#include "CoveringSetMethod.h"

namespace Sudoku
{

/**
 * This creates the actual Solution Method objects
 * It is mostly useful to make the PuzzleHelper testable
 */
class SolutionMethodFactory
{
public:
    virtual std::shared_ptr<SolutionMethod> CreateSingleCandidateMethod(
        std::shared_ptr<Cell> c )
    {
        std::shared_ptr<SolutionMethod> s(
            new SingleCandidateMethod( c ) );
        return s;
    }

    virtual std::shared_ptr<SolutionMethod> CreateExclusionMethod(
        std::shared_ptr<Puzzle> p,
        std::shared_ptr<Cell> c )
    {
        std::shared_ptr<SolutionMethod> s(
            new ExclusionMethod( p, c ) );
        return s;
    }

    virtual std::shared_ptr<SolutionMethod> CreateBlockIntersectionMethod(
        std::shared_ptr<Cell> c,
        unsigned mark,
        Puzzle::Container primary,
        Puzzle::Container secondary )
    {
        std::shared_ptr<SolutionMethod> s(
            new BlockIntersectionMethod( c, mark, primary, secondary ) );
        return s;
    }

    virtual std::shared_ptr<SolutionMethod> CreateCoveringSetMethod(
        Puzzle::Container sector,
        Puzzle::Container subset )
    {
        std::shared_ptr<SolutionMethod> s(
            new CoveringSetMethod( sector, subset ) );
        return s;
    }
};

}

#endif
