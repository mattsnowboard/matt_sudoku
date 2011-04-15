#ifndef SUDOKU_IPUZZLE_MARKER_H
#define SUDOKU_IPUZZLE_MARKER_H

#include <memory>

namespace Sudoku
{

class Puzzle;

/**
 * Interface class which can add/remove marks to a puzzle to aid solving
 * Used by automated solver and hints for user
 */
class IPuzzleMarker
{
public:
    /**
     * Start fresh with no marks
     * @param puzzle The Puzzle to mark
     */
    virtual void ClearPuzzle( std::shared_ptr<Puzzle> puzzle ) = 0;

    /**
     * Update the marks on a Puzzle based on the displayed values
     * @param puzzle The Puzzle to mark
     */
    virtual void UpdateMarks( std::shared_ptr<Puzzle> puzzle ) = 0;
};

}

#endif
