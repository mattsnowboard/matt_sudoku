#ifndef SUDOKU_MOCK_SOLUTION_METHOD_FACTORY_H
#define SUDOKU_MOCK_SOLUTION_METHOD_FACTORY_H

#include "gmock/gmock.h"

#include "../SolutionMethodFactory.h"

namespace Sudoku
{

class MockSolutionMethodFactory : public SolutionMethodFactory
{
public:
    MOCK_METHOD1( CreateSingleCandidateMethod, 
                  std::shared_ptr<SolutionMethod> ( std::shared_ptr<Cell> c ) );

    MOCK_METHOD2( CreateExclusionMethod,
                  std::shared_ptr<SolutionMethod> (
                      std::shared_ptr<Puzzle> p,
                      std::shared_ptr<Cell> c ) );

    MOCK_METHOD4( CreateBlockIntersectionMethod,
                  std::shared_ptr<SolutionMethod> (
                      std::shared_ptr<Cell> c,
                      unsigned mark,
                      Puzzle::Container primary,
                      Puzzle::Container secondary ) );

    MOCK_METHOD2( CreateCoveringSetMethod,
                  std::shared_ptr<SolutionMethod> (
                      Puzzle::Container sector,
                      Puzzle::Container subset ) );
};

}

#endif
