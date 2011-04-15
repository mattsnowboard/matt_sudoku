
#include "Cell.h"
#include <iostream>
#include <stdexcept>

namespace Sudoku
{

Cell::Cell()
    : _x( 0 ),
      _y( 0 ),
      _correctVal( 0 ),
      _guessedVal( 0 ),
      _displayCorrect( false )
{}

Cell::Cell( const Cell &c )
{
    *this = c;
}

Cell& Cell::operator=( const Cell &c )
{
    if ( this != &c )
    {
        _x = c._x;
        _y = c._y;
        _correctVal = c._correctVal;
        _guessedVal = c._guessedVal;
        _displayCorrect = c._displayCorrect;
        _marks = c._marks;
    }
    return *this;
}

void Cell::SetPos( size_t x, size_t y )
{
    Validate( x, 1, 9 );
    Validate( y, 1, 9 );
    _x = x;
    _y = y;
}

void Cell::SetCorrect( int correct )
{
    Validate( correct, 0, 9 );
    _correctVal = correct;
}

void Cell::SetGuess( int guess )
{
    Validate( guess, 0, 9 );
    if ( CanGuess() )
    {
        _guessedVal = guess;
    }
    else
    {
        throw std::logic_error( "Cannot guess on a correct displayed Cell" );
    }
}

int Cell::DisplayedValue() const
{
    if ( _displayCorrect )
    {
        return _correctVal;
    }
    else
    {
        return _guessedVal;
    }
}

bool Cell::IsCorrect() const
{
    return _correctVal == _guessedVal && _correctVal != 0;
}

void Cell::SetMarkContainer( const Cell::MarkContainer &m )
{
    _marks &= m;
    _marks |= m;
}

Cell::MarkedValues Cell::GetMarkedValues() const
{
    MarkedValues M;
    if ( _marks.any() )
    {
        // skip number 0
        for ( size_t i = 1; i < _marks.size(); i++ )
        {
            if ( _marks[i] )
            {
                M.insert( i );
            }
        }
    }
    return M;
}

void Cell::Accept( Visitor& v )
{

}

bool Cell::operator==( const Cell& c ) const
{
    return ( c._x == _x &&
             c._y == _y &&
             c._correctVal == _correctVal &&
             c._guessedVal == _guessedVal &&
             c._displayCorrect == _displayCorrect &&
             c._marks == _marks );
}

std::ostream& operator<<( std::ostream &os, const Cell &c )
{
    return os << "Cell at (" << c._x << ", " << c._y << ")"
              << ", Correct=" << c._correctVal
              << ", Guess=" << c._guessedVal
              << ", displayed? " << c._displayCorrect;
}

}
