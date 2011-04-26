#ifndef SUDOKU_PUZZLE_MARKER_H
#define SUDOKU_PUZZLE_MARKER_H

#include "IPuzzleMarker.h"

namespace Sudoku
{

/**
 * Implementation of interface
 */
class PuzzleMarker : public IPuzzleMarker
{
public:
    PuzzleMarker() {}

    /**
     * Start fresh with no marks
     * @param puzzle The Puzzle to mark
     */
    virtual void ClearPuzzle( std::shared_ptr<Puzzle> puzzle );

    /**
     * Update the marks on a Puzzle based on the displayed values
     * @param puzzle The Puzzle to mark
     */
    virtual void UpdateMarks( std::shared_ptr<Puzzle> puzzle );
private:
    PuzzleMarker( const PuzzleMarker & );
    PuzzleMarker & operator=( const PuzzleMarker & );
};

}

#endif
