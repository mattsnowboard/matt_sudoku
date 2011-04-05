
#include "Puzzle.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace Sudoku
{

// utilities to compare for stl algorithm stuff
namespace
{

class CellInRow : public std::unary_function<const std::shared_ptr<Cell>,
                                             bool>
{
public:
    CellInRow( size_t row ) : _row( row ) {}

    // note use of const ref to shared_ptr's
    bool operator() ( const std::shared_ptr<Cell> &a ) const
    {
        return ( a->GetY() == _row );
    }
private:
    size_t _row;
};

class CellInCol : public std::unary_function<const std::shared_ptr<Cell>,
                                             bool>
{
public:
    CellInCol( size_t col ) : _col( col ) {}

    // note use of const ref to shared_ptr's
    bool operator() ( const std::shared_ptr<Cell> &a ) const
    {
        return ( a->GetX() == _col );
    }
private:
    size_t _col;
};

class CellInBlock : public std::unary_function<const std::shared_ptr<Cell>,
                                               bool>
{
public:
    CellInBlock( size_t x, size_t y ) : _x( x ), _y( y )  {}

    // note use of const ref to shared_ptr's
    bool operator() ( const std::shared_ptr<Cell> &a ) const
    {
        return ( ( a->GetX() - 1 ) / 3 + 1 == _x &&
                 ( a->GetY() - 1 ) / 3 + 1 == _y );
    }
private:
    size_t _x, _y;
};

class CellIsNeighbor : public CellInRow, CellInCol, CellInBlock
{
public:
    CellIsNeighbor( size_t x, size_t y )
        : CellInRow( y ),
          CellInCol( x ),
          CellInBlock( ( x - 1 ) / 3 + 1, ( y - 1 ) / 3 + 1 ) {}

    bool operator() ( const std::shared_ptr<Cell> &a ) const
    {
        return ( CellInRow::operator()( a ) ||
                 CellInCol::operator()( a ) ||
                 CellInBlock::operator()( a ) );
    }
};

class CompareCell : public std::binary_function<const std::shared_ptr<Cell>,
                                               const std::shared_ptr<Cell>,
                                               bool>
{
public:
    bool operator() ( const std::shared_ptr<Cell> &a,
                      const std::shared_ptr<Cell> &b ) const
    {
        return ( *a == *b );
    }
};

}

Puzzle::Puzzle()
{
    // set up positions
    for ( size_t i = 0; i < ARRAY_SIZE; i++ )
    {
        std::shared_ptr<Cell> temp( new Cell );
        temp->SetPos( i % SECTOR_SIZE + 1,
                      i / SECTOR_SIZE + 1 );
        _grid.insert( temp );
    }
}

Puzzle::Puzzle( const Puzzle &p )
{
    *this = p;
}

Puzzle& Puzzle::operator=( const Puzzle &p )
{
    if ( this != &p )
    {
        _grid.clear();
        std::copy( p._grid.begin(),
                   p._grid.end(),
                   std::inserter( _grid, _grid.begin() ) );
    }
    return *this;
}

void Puzzle::Randomize()
{

}

Puzzle::Container Puzzle::GetRow( size_t r )
{
    Cell::Validate( r, 1, 9 );
    Container row;
    std::copy_if( _grid.begin(), _grid.end(),
                  std::inserter( row, row.begin() ),
                  CellInRow( r ) );
    return row;
}

Puzzle::Container Puzzle::GetCol( size_t c )
{
    Cell::Validate( c, 1, 9 );
    Container col;
    std::copy_if( _grid.begin(), _grid.end(),
                  std::inserter( col, col.begin() ),
                  CellInCol( c ) );
    return col;
}

Puzzle::Container Puzzle::GetBlock( int x, int y )
{
    Cell::Validate( x, 1, 3 );
    Cell::Validate( y, 1, 3 );
    Container block;
    std::copy_if( _grid.begin(), _grid.end(),
                  std::inserter( block, block.begin() ),
                  CellInBlock( x, y ) );
    return block;
}

std::shared_ptr<Cell> Puzzle::GetCell( size_t x, size_t y )
{
    Cell::Validate( x, 1, 9 );
    Cell::Validate( y, 1, 9 );
    std::shared_ptr<Cell> target( new Cell );
    target->SetPos( x, y );
    Container::iterator found = _grid.find( target );
    if ( found == _grid.end() )
    {
        throw std::logic_error( "Could not find given cell in Puzzle" );
    }
    return (*found);
}

Puzzle::Container Puzzle::GetNeighbors( std::shared_ptr<Cell> c )
{
    size_t x = c->GetX();
    size_t y = c->GetY();
    return GetNeighbors( x, y );
}

Puzzle::Container Puzzle::GetNeighbors( size_t x, size_t y )
{
    Cell::Validate( x, 1, 9 );
    Cell::Validate( y, 1, 9 );
    // this should be large enough to hold all neighbors after removing overlap
    Puzzle::Container N;
    std::copy_if( _grid.begin(), _grid.end(),
                  std::inserter( N, N.begin() ),
                  CellIsNeighbor( x, y ) );
    // N.erase( c );
    return N;
}

Puzzle::Container Puzzle::GetAllCells()
{
    return _grid;
}

bool Puzzle::operator==( const Puzzle &p ) const
{
    return ( p._grid.size() == _grid.size() &&
             std::equal( _grid.begin(),
                         _grid.end(),
                         p._grid.begin(),
                         CompareCell() ) );
}

std::ostream& operator<<( std::ostream &os, const Puzzle &p )
{
    return os << "A Puzzle Object (contains Cells)";
}

}
