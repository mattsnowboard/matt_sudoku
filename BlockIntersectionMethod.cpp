
#include "BlockIntersectionMethod.h"
#include <algorithm>
#include <functional>

namespace Sudoku
{

BlockIntersectionMethod::BlockIntersectionMethod( std::shared_ptr<Cell> c,
                                                  unsigned mark,
                                                  Puzzle::Container primary,
                                                  Puzzle::Container secondary )
    : _cell( c ), _mark( mark ), _primary( primary ), _secondary( secondary )
{
    verifyGeometry();
}

void BlockIntersectionMethod::ExecuteForward()
{
    //std::for_each( _primary.begin(), _primary.end(),
    //               std::bind2nd( std::mem_fun_ref( &Cell::Unmark ), _mark ) );
    for ( Puzzle::Container::iterator it = _primary.begin();
          it != _primary.end();
          ++it )
    {
        if ( (*it)->CanGuess() )
        {
            (*it)->Unmark( _mark );
        }
    }
}

bool BlockIntersectionMethod::VerifyForwardConditions()
{
    if ( !_cell->GetMarkContainer()[_mark] )
    {
        return false;
    }
    // secondary does not have mark
    for ( Puzzle::Container::iterator it = _secondary.begin();
          it != _secondary.end();
          ++it )
    {
        if ( (*it)->CanGuess() && (*it)->GetMarkContainer()[_mark] )
        {
            return false;
        }
    }
    // primary has mark somewhere
    for ( Puzzle::Container::iterator it = _primary.begin();
          it != _primary.end();
          ++it )
    {
        if ( (*it)->CanGuess() && (*it)->GetMarkContainer()[_mark] )
        {
            return true;
        }
    }
    return false;
}

void BlockIntersectionMethod::ExecuteReverse()

{
    for ( Puzzle::Container::iterator it = _primary.begin();
          it != _primary.end();
          ++it )
    {
        if ( (*it)->CanGuess() )
        {
            (*it)->Mark( _mark );
        }
    }
}

bool BlockIntersectionMethod::VerifyReverseConditions()
{
    if ( ! _cell->GetMarkContainer()[_mark] )
    {
        return false;
    }
    // secondary does not have mark
    for ( Puzzle::Container::iterator it = _secondary.begin();
          it != _secondary.end();
          ++it )
    {
        if ( (*it)->CanGuess() && (*it)->GetMarkContainer()[_mark] )
        {
            return false;
        }
    }
    // primary does not have mark, but has some marks
    bool has_some_mark = false;
    for ( Puzzle::Container::iterator it = _primary.begin();
          it != _primary.end();
          ++it )
    {
        if ( (*it)->CanGuess() )
        {
            if ( (*it)->GetMarkContainer()[_mark] )
            {
                return false;
            }
            if ( !(*it)->GetMarkedValues().empty() ) 
            {
                has_some_mark = true;
            }
        }
    }
    return has_some_mark;
}

void BlockIntersectionMethod::verifyGeometry()
{
    if ( _primary.size() != 6 && _secondary.size() != 6 )
    {
        throw std::logic_error(
            "Block Intersection Method requires sets of size 6." );
    }
    // check for overlap
    Puzzle::Container temp;
    std::set_intersection( _primary.begin(), _primary.end(),
                           _secondary.begin(), _secondary.end(),
                           std::inserter( temp, temp.begin() ),
                           Sudoku::CellSorter() );
    if ( !temp.empty() )
    {
        throw std::logic_error(
            "Block Intersection Method cannot work with overlapping sets." );
    }
    // finally, _primary and _secondary must be neighbors of _cell
    // @todo can't be tested unless we have the puzzle
}

}
