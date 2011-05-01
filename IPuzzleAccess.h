#ifndef SUDOKU_IPUZZLE_ACCESS_H
#define SUDOKU_IPUZZLE_ACCESS_H

#include <memory>

namespace Sudoku
{

class Puzzle;
class ICellObserver;

class IPuzzleAccess
{
public:
    /**
     * Get a const pointer to a puzzle
     * @return Puzzle
     */
    virtual std::shared_ptr<const Puzzle> GetPuzzle() const = 0;

    /**
     * We hold on to the real Puzzle, this lets Observers attach to it
     * @param o Observer to add to each Cell in current Puzzle
     * @post o gets notified by Cells on update
     */
    virtual void ListenToAllCells( std::shared_ptr<ICellObserver> o ) = 0;

    virtual ~IPuzzleAccess() {}
};

}

#endif
