
#include "Puzzle.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "StdMapIteratorAdaptor.h"

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
        _grid.insert( std::make_pair(temp->GetPos(), temp ) );
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
    throw std::runtime_error( "NOT IMPLEMENTED" );
}

Puzzle::Container Puzzle::GetRow( size_t r )
{
    Cell::Validate( r, 1, 9 );
    Container row;
    std::copy_if( make_map_iterator_adaptor( _grid.begin() ), 
                  make_map_iterator_adaptor( _grid.end() ),
                  std::inserter( row, row.begin() ),
                  CellInRow( r ) );
    return row;
}

Puzzle::ConstContainer Puzzle::GetRow( size_t r ) const
{
    Cell::Validate( r, 1, 9 );
    ConstContainer row;
    std::copy_if( make_map_iterator_adaptor( _grid.begin() ), 
                  make_map_iterator_adaptor( _grid.end() ),
                  std::inserter( row, row.begin() ),
                  CellInRow( r ) );
    return row;
}

Puzzle::Container Puzzle::GetCol( size_t c )
{
    Cell::Validate( c, 1, 9 );
    Container col;
    std::copy_if( make_map_iterator_adaptor( _grid.begin() ), 
                  make_map_iterator_adaptor( _grid.end() ),
                  std::inserter( col, col.begin() ),
                  CellInCol( c ) );
    return col;
}

Puzzle::ConstContainer Puzzle::GetCol( size_t c ) const
{
    Cell::Validate( c, 1, 9 );
    ConstContainer col;
    std::copy_if( make_map_iterator_adaptor( _grid.begin() ), 
                  make_map_iterator_adaptor( _grid.end() ),
                  std::inserter( col, col.begin() ),
                  CellInCol( c ) );
    return col;
}

Puzzle::Container Puzzle::GetBlock( size_t x, size_t y )
{
    Cell::Validate( x, 1, 3 );
    Cell::Validate( y, 1, 3 );
    Container block;
    std::copy_if( make_map_iterator_adaptor( _grid.begin() ), 
                  make_map_iterator_adaptor( _grid.end() ),
                  std::inserter( block, block.begin() ),
                  CellInBlock( x, y ) );
    return block;
}

Puzzle::ConstContainer Puzzle::GetBlock( size_t x, size_t y ) const
{
    Cell::Validate( x, 1, 3 );
    Cell::Validate( y, 1, 3 );
    ConstContainer block;
    std::copy_if( make_map_iterator_adaptor( _grid.begin() ), 
                  make_map_iterator_adaptor( _grid.end() ),
                  std::inserter( block, block.begin() ),
                  CellInBlock( x, y ) );
    return block;
}

Puzzle::Container Puzzle::GetBlock( std::shared_ptr<Cell> c )
{
    return GetBlock( ( c->GetX() - 1 ) / 3 + 1,
                     ( c->GetY() - 1 ) / 3 + 1 );
}

Puzzle::ConstContainer Puzzle::GetBlock( std::shared_ptr<const Cell> c ) const
{
    return GetBlock( ( c->GetX() - 1 ) / 3 + 1,
                     ( c->GetY() - 1 ) / 3 + 1 );
}

std::shared_ptr<Cell> Puzzle::GetCell( size_t x, size_t y )
{
    Cell::Validate( x, 1, 9 );
    Cell::Validate( y, 1, 9 );
    Position p( x, y );
    CellMap::iterator found = _grid.find( p );
    if ( found == _grid.end() )
    {
        throw std::logic_error( "Could not find given cell in Puzzle" );
    }
    return ( found->second );
}

std::shared_ptr<const Cell> Puzzle::GetCell( size_t x, size_t y ) const
{
    Cell::Validate( x, 1, 9 );
    Cell::Validate( y, 1, 9 );
    Position p( x, y );
    CellMap::const_iterator found = _grid.find( p );
    if ( found == _grid.end() )
    {
        throw std::logic_error( "Could not find given cell in Puzzle" );
    }
    return ( found->second );
}

std::shared_ptr<Cell> Puzzle::GetCell( std::shared_ptr<const Cell> c )
{
    CellMap::iterator found = _grid.find( c->GetPos() );
    if ( found == _grid.end() )
    {
        throw std::logic_error( "Could not find given cell in Puzzle" );
    }
    return ( found->second );
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
    Container N;
    std::copy_if( make_map_iterator_adaptor( _grid.begin() ), 
                  make_map_iterator_adaptor( _grid.end() ),
                  std::inserter( N, N.begin() ),
                  CellIsNeighbor( x, y ) );
    // N.erase( c );
    return N;
}

Puzzle::ConstContainer Puzzle::GetNeighbors(
    std::shared_ptr<const Cell> c ) const
{
    size_t x = c->GetX();
    size_t y = c->GetY();
    return GetNeighbors( x, y );
}

Puzzle::ConstContainer Puzzle::GetNeighbors( size_t x, size_t y ) const
{
    Cell::Validate( x, 1, 9 );
    Cell::Validate( y, 1, 9 );
    // this should be large enough to hold all neighbors after removing overlap
    ConstContainer N;
    std::copy_if( make_map_iterator_adaptor( _grid.begin() ), 
                  make_map_iterator_adaptor( _grid.end() ),
                  std::inserter( N, N.begin() ),
                  CellIsNeighbor( x, y ) );
    // N.erase( c );
    return N;
}

Puzzle::ConstContainer Puzzle::GetAllCells() const
{
    ConstContainer c;
    std::copy( make_map_iterator_adaptor( _grid.begin() ), 
               make_map_iterator_adaptor( _grid.end() ),
               std::inserter( c, c.begin() ) );
    return c;
}

Puzzle::Container Puzzle::GetAllCells()
{
    Container c;
    std::copy( make_map_iterator_adaptor( _grid.begin() ),
               make_map_iterator_adaptor( _grid.end() ),
               std::inserter( c, c.begin() ) );
    return c;
}

bool Puzzle::operator==( const Puzzle &p ) const
{
    return ( p._grid.size() == _grid.size() &&
             std::equal( make_map_iterator_adaptor( _grid.begin() ),
                         make_map_iterator_adaptor( _grid.end() ),
                         make_map_iterator_adaptor( p._grid.begin() ),
                         CompareCell() ) );
}

std::ostream& operator<<( std::ostream &os, const Puzzle &p )
{
    return os << "A Puzzle Object (contains Cells)";
}

}
