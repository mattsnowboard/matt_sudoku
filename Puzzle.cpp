
#include "Puzzle.h"
#include <stdexcept>

namespace Sudoku
{

Puzzle::Puzzle() : _grid( ARRAY_SIZE )
{
    // set up positions
    for ( size_t i = 0; i < ARRAY_SIZE; i++ )
    {
        _grid[i].reset( new Cell );
        _grid[i]->SetPos( i % SECTOR_SIZE + 1,
                          i / SECTOR_SIZE + 1 );
    }
}

void Puzzle::Randomize()
{

}

Puzzle::Container Puzzle::GetRow( size_t r )
{
    Cell::Validate( r, 1, 9 );
    return _grid[ std::slice( (r - 1) * SECTOR_SIZE,
                              SECTOR_SIZE,
                              1 ) ];
}

Puzzle::Container Puzzle::GetCol( size_t c )
{
    Cell::Validate( c, 1, 9 );
    return _grid[ std::slice( (c - 1),
                              SECTOR_SIZE,
                              SECTOR_SIZE ) ];
}

Puzzle::Container Puzzle::GetBlock( int x, int y )
{
    Cell::Validate( x, 1, 3 );
    Cell::Validate( y, 1, 3 );

    size_t start = (y - 1) * SECTOR_SIZE * 3 + (x - 1) * 3;
    size_t lengths[] = { 3, 3 };
    size_t strides[] = { 9, 1 };

    return _grid[ std::gslice( start,
                               std::valarray<size_t>( lengths, 2 ),
                               std::valarray<size_t>( strides, 2 ) ) ];
}

std::shared_ptr<Cell> Puzzle::GetCell( size_t x, size_t y )
{
    Cell::Validate( x, 1, 9 );
    Cell::Validate( y, 1, 9 );
    return _grid[ ( y - 1 ) * SECTOR_SIZE + ( x - 1 ) ];
}

// @todo Rethink container
Puzzle::Container Puzzle::GetNeighbors( const Cell &c )
{
    // this should be large enough to hold all neighbors after removing overlap
    Puzzle::Container N( SECTOR_SIZE * 3 - 6 );
    Puzzle::Container row = GetRow( c.GetY() );
    Puzzle::Container col = GetCol( c.GetX() );
    Puzzle::Container block = GetBlock( ( ( c.GetX() - 1 ) / 3 ) + 1,
                                        ( ( c.GetY() - 1 ) / 3 ) + 1 );
    std::set_union( &row[0], &row[row.size()],
                    &col[0], &col[col.size()],
                    &N[0],
                    CellSorter() );
    size_t int_size = row.size() + col.size();
    std::set_union( &N[0], &N[int_size],
                    &block[0], &block[block.size()],
                    &N[int_size],
                    CellSorter() );
    return N;
}

}
