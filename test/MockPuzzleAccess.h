#ifndef SUDOKU_MOCK_PUZZLE_ACCESS_H
#define SUDOKU_MOCK_PUZZLE_ACCESS_H

#include "gmock/gmock.h"

#include "../IPuzzleAccess.h"

namespace Sudoku
{

class MockPuzzleAccess : public IPuzzleAccess
{
public:
    MOCK_CONST_METHOD0( GetPuzzle, std::shared_ptr<const Puzzle> () );

    MOCK_METHOD1( ListenToAllCells, void ( std::shared_ptr<ICellObserver> o ) );
};

}

#endif
