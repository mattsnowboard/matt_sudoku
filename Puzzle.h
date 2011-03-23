#ifndef SUDOKU_PUZZLE_H
#define SUDOKU_PUZZLE_H

#include <memory>
#include <valarray>
#include "Cell.h"

namespace Sudoku
{

/**
 * This is how to sort Cells in a Puzzle
 * Used in set operations on groups of cells (STL set operations need a
 * compare function clearly)
 */
class CellSorter
{
public:
    /**
     * Compare Cells to sort in left-right, top-bottom order
     * @note We use const references of shared_ptr for performace
     * @param a First Cell to compare
     * @param b Second Cell to compare
     * @return if a < b as defined above
     */
    bool operator()( const std::shared_ptr<Cell> &a,
                     const std::shared_ptr<Cell> &b )
    {
        return ( ( a->GetY() < b->GetY() ) ||
                 ( a->GetY() == b->GetY() &&
                   a->GetX() < b->GetX() )
            );
    }
};

class Puzzle
{
public:

    /// This is the container type we use for holding Cells internally
    typedef std::valarray<std::shared_ptr<Cell> > Container;

    /**
     * Create a blank board with all positions of Cells set
     * @post We have a 9x9 board of empty Cells with correct positions
     */
    Puzzle();

    /**
     * Randomize the solution of all Cells
     * Keep Guesses blank and Display false
     * This is a blank board which we can use to generate a game
     */
    void Randomize();

    Container GetRow( size_t r );

    Container GetCol( size_t c );
    
    Container GetBlock( int x, int y );
    
    std::shared_ptr<Cell> GetCell( size_t x, size_t y );

    Container GetNeighbors( const Cell &c );

private:

//    Cell _grid[9][9];
    static const size_t ARRAY_SIZE = 81;
    static const size_t SECTOR_SIZE = 9;
    Container _grid;

};

}

#endif
