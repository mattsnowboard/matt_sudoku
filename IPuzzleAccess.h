#ifndef SUDOKU_IPUZZLE_ACCESS_H
#define SUDOKU_IPUZZLE_ACCESS_H

#include <memory>

namespace Sudoku
{

class Puzzle;

class IPuzzleAccess
{
public:
    /**
     * Get a const pointer to a puzzle
     * @return Puzzle
     */
    virtual std::shared_ptr<const Puzzle> GetPuzzle() const = 0;

    virtual ~IPuzzleAccess() {}
};

}

#endif
