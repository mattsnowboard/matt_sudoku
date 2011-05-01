
#include "Cell.h"
#include "ICellObserver.h"
#include <iostream>
#include <algorithm>

namespace Sudoku
{

Cell::Cell()
    : _pos( 0, 0 ),
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
        _pos = c._pos;
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
    _pos.x = x;
    _pos.y = y;
}

void Cell::SetPos( Position p )
{
    Validate( p.x, 1, 9 );
    Validate( p.y, 1, 9 );
    _pos = p;
}

void Cell::SetCorrect( int correct )
{
    Validate( correct, 0, 9 );
    if ( _correctVal != correct )
    {
        NotifyObservers();
    }
    _correctVal = correct;
}

void Cell::SetGuess( int guess )
{
    Validate( guess, 0, 9 );
    if ( CanGuess() )
    {
        if ( _guessedVal != guess )
        {
            NotifyObservers();
        }
        _guessedVal = guess;
    }
    else
    {
        throw std::logic_error( "Cannot guess on a correct displayed Cell" );
    }
}

void Cell::Display( bool display )
{
    if ( _displayCorrect != display )
    {
        NotifyObservers();
    }
    _displayCorrect = display;
}

void Cell::Mark( int mark )
{
    if ( !_marks.test( mark ) )
    {
        NotifyObservers();
    }
    _marks.set( mark );
}

void Cell::Unmark( int mark )
{
    if ( _marks.test( mark ) )
    {
        NotifyObservers();
    }
    _marks.reset( mark );
}

void Cell::ClearMarks()
{
    if ( _marks.any() )
    {
        NotifyObservers();
    }
    _marks.reset();
}

void Cell::MarkAll()
{
    if ( _marks.count() < _marks.size() )
    {
        NotifyObservers();
    }
    _marks.set();
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
    return ( _correctVal != 0 ) &&
        ( _correctVal == _guessedVal || _displayCorrect );
}

void Cell::SetMarkContainer( const Cell::MarkContainer &m )
{
    if ( _marks != m )
    {
        NotifyObservers();
    }
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

void Cell::AddObserver( std::shared_ptr<ICellObserver> o )
{
    if ( !o )
    {
        throw std::runtime_error( "Cannot add NULL observer." );
    }
    _observers.push_back( o );
}

void Cell::RemoveObserver( std::shared_ptr<ICellObserver> o )
{
    if ( !o )
    {
        throw std::runtime_error( "Cannot remove NULL observer." );
    }
    ObserverContainer::iterator found = std::find( _observers.begin(),
                                                   _observers.end(),
                                                   o );
    if ( found == _observers.end() )
    {
        throw std::runtime_error(
            "Attempt to remove observer not listening to Cell." );
    }
    _observers.erase( found );
}

void Cell::NotifyObservers()
{
    for ( ObserverContainer::iterator it = _observers.begin();
          it != _observers.end();
          ++it )
    {
        (*it)->Update( *this );
    }
}

bool Cell::operator==( const Cell& c ) const
{
    return ( c._pos.x == _pos.x &&
             c._pos.y == _pos.y &&
             c._correctVal == _correctVal &&
             c._guessedVal == _guessedVal &&
             c._displayCorrect == _displayCorrect &&
             c._marks == _marks );
}

std::ostream& operator<<( std::ostream &os, const Cell &c )
{
    return os << "Cell at (" << c._pos.x << ", " << c._pos.y << ")"
              << ", Correct=" << c._correctVal
              << ", Guess=" << c._guessedVal
              << ", displayed? " << c._displayCorrect
              << ", marks = " << c._marks.to_string();
}

}
