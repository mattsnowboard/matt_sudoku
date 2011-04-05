
#include "ExclusionMethod.h"
#include "Cell.h"
#include "Puzzle.h"
#include <algorithm>
#include <functional>

namespace Sudoku
{

namespace
{

class CanGuessCheck : public std::unary_function<const std::shared_ptr<Cell>,
                                                 bool>
{
public:
    bool operator()( const std::shared_ptr<Cell> &c ) const
    {
        return ( c->CanGuess() && c->DisplayedValue() == 0 );
    }
};

}

ExclusionMethod::ExclusionMethod( std::shared_ptr<Puzzle> p,
                                  std::shared_ptr<Cell> c )
    : _neighbors( p->GetNeighbors( c ) ),
      _cell( c ),
      _guessVal( c->DisplayedValue() )
{
    Puzzle::Container temp = p->GetNeighbors( c );
    // remove Cell from neighbors
    temp.erase( _cell );
    // remove solved and already guessed cells
    std::remove_copy_if( temp.begin(), temp.end(),
                         std::inserter( _neighbors, _neighbors.begin() ),
                         std::not1( CanGuessCheck() ) );
}

void ExclusionMethod::ExecuteForward()
{
    for ( Puzzle::Container::iterator it = _neighbors.begin();
          it != _neighbors.end();
          ++it )
    {
        (*it)->Unmark( _guessVal );
    }
}

bool ExclusionMethod::VerifyForwardConditions()
{
    return ( _cell->CanGuess() && _guessVal != 0 );
}

void ExclusionMethod::ExecuteReverse()
{
    for ( Puzzle::Container::iterator it = _neighbors.begin();
          it != _neighbors.end();
          ++it )
    {
        (*it)->Mark( _guessVal );
    }
}

bool ExclusionMethod::VerifyReverseConditions()
{
    return ( _cell->CanGuess() && _guessVal != 0 );
}

}
