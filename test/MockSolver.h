#ifndef SUDOKU_MOCK_SOLVER_H
#define SUDOKU_MOCK_SOLVER_H

#include "gmock/gmock.h"

#include "../ISolver.h"

namespace Sudoku
{

class MockSolver : public ISolver
{
public:

    MOCK_METHOD1( Solve, void ( std::shared_ptr<Puzzle> p ) );

    MOCK_METHOD1( CommitGuesses,  void ( std::shared_ptr<Puzzle> p ) );
};

}

#endif
