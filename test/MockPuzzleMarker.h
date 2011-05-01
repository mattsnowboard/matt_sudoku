#ifndef SUDOKU_MOCK_PUZZLE_MARKER_H
#define SUDOKU_MOCK_PUZZLE_MARKER_H

#include "gmock/gmock.h"

#include "IPuzzleMarker.h"

namespace Sudoku
{

class MockPuzzleMarker : public IPuzzleMarker
{
public:
    MOCK_METHOD1( ClearPuzzle, void ( std::shared_ptr<Puzzle> puzzle ) );
    MOCK_METHOD1( UpdateMarks, void ( std::shared_ptr<Puzzle> puzzle ) );
};

}

#endif
