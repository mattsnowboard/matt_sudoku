
#include "Cell.h"

namespace Sudoku
{

Cell::Cell()
    : _x( 0 ),
      _y( 0 ),
      _correctVal( 0 ),
      _guessedVal( 0 ),
      _displayCorrect( false )
{}

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
    _guessedVal = guess;
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

}
