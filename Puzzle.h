#ifndef SUDOKU_PUZZLE_H
#define SUDOKU_PUZZLE_H

#include <memory>
#include <valarray>
#include <map>
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
     * @param a First Cell to compare
     * @param b Second Cell to compare
     * @return if a < b as defined above
     */
    bool operator()( std::shared_ptr<const Cell> a,
                     std::shared_ptr<const Cell> b ) const
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
    typedef std::set<std::shared_ptr<Cell>, CellSorter> Container;
    typedef std::set<std::shared_ptr<const Cell>, CellSorter> ConstContainer;
    typedef std::map<Position, std::shared_ptr<Cell> > CellMap;

    /**
     * Create a blank board with all positions of Cells set
     * @post We have a 9x9 board of empty Cells with correct positions
     */
    Puzzle();

    /**
     * Copy constructor
     * This Puzzle will have copies of the Cells from the other
     * @param p Puzzle to copy
     * @post This Puzzle is an exact copy of p
     */
    Puzzle( const Puzzle &p );

    /**
     * Copy another Puzzle, replacing the existing
     * @param p Puzzle to copy
     * @return this for method chaining
     */
    Puzzle& operator=( const Puzzle &p );

    /**
     * Randomize the solution of all Cells
     * Keep Guesses blank and Display false
     * This is a blank board which we can use to generate a game
     */
    void Randomize();

    /**
     * Get a row by index
     * @param r Row number
     * @pre r is in range [1,9]
     * @return vector of Cell's in the given row
     */
    Container GetRow( size_t r );

    /**
     * Get a column by index
     * @param c Column number
     * @pre c is in range [1,9]
     * @return vector of Cell's in the given column
     */
    Container GetCol( size_t c );
    
    /**
     * Get a 3 by 3 block by coordinate
     * @param x X coordinate
     * @param y Y coordinate
     * @pre x, y are each in range [1,3]
     * @return vector of Cell's in the given block
     */
    Container GetBlock( size_t x, size_t y );

    /**
     * Figures out which block to get for a given Cell
     * @overload Container GetBlock( size_t x, size_t y )
     */
    Container GetBlock( std::shared_ptr<Cell> c );
    
    /**
     * Get a cell by position
     * @param x X coordinate of Cell (starting from 1)
     * @param y Y coordinate of Cell (starting from 1)
     * @pre x and y are both in range [1,9]
     * @return a pointer (can modify) to the Cell at the given position
     */
    std::shared_ptr<Cell> GetCell( size_t x, size_t y );

    /**
     * Get all of the neighbors of a given Cell
     * This is equivalent to forming the union of a Cell's row, column, and 
     * block
     * @param c The Cell positioned in the grid
     * @return a vector of Cell's neighboring c (same row, column, or block)
     */
    Container GetNeighbors( std::shared_ptr<Cell> c );

    /**
     * Get all of the neighbors of a given Cell by position
     * @param x X coordinate of Cell
     * @param y Y coordinate of Cell
     * @overload Container GetNeighbors( std::shared_ptr<Cell> c )
     */
    Container GetNeighbors( size_t x, size_t y );

    /**
     * Get all of the Cells in the grid
     * @return all Cells in a container
     */
    Container GetAllCells();

    /**
     * Get all of the Cells in the grid
     * @return all Cells in a container
     */
    ConstContainer GetAllCells() const;

    /**
     * Check if two puzzles contain the exact same grid
     * @param p Other puzzle to compare
     * @return true if every Cell in each grid is the same
     */
    bool operator==( const Puzzle &p ) const;

private:

    /**
     * Used for Google Test only, doesn't print anything useful
     */
    friend std::ostream& operator<<( std::ostream &os, const Puzzle &p );

//    Cell _grid[9][9];
    static const size_t ARRAY_SIZE = 81;
    static const size_t SECTOR_SIZE = 9;
    CellMap _grid;
    //Container _grid;

};

}

#endif
