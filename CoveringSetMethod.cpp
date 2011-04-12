
#include "CoveringSetMethod.h"

namespace Sudoku
{

CoveringSetMethod::CoveringSetMethod( Puzzle::Container sector,
                                      Puzzle::Container subset )
    : _sector( sector ), _subset( subset )
{
    std::set_difference( _sector.begin(), _sector.end(),
                         _subset.begin(), _subset.end(),
                         std::inserter( _diff, _diff.begin() ),
                         _sector.value_comp() );
    for ( Puzzle::Container::iterator it = _subset.begin();
          it != _subset.end();
          ++it )
    {
        Cell::MarkedValues m = (*it)->GetMarkedValues();
        if ( _subMarks.empty() )
        {
            _subMarks = m;
        }
        // union these
        std::set_union( _subMarks.begin(), _subMarks.end(),
                        m.begin(), m.end(),
                        std::inserter( _subMarks, _subMarks.begin() ),
                        _subMarks.value_comp() );
    }
}

void CoveringSetMethod::ExecuteForward()
{
    for ( Puzzle::Container::iterator it = _diff.begin();
          it != _diff.end();
          ++it )
    {
        if ( (*it)->CanGuess() )
        {
            for ( Cell::MarkedValues::iterator mit = _subMarks.begin();
                  mit != _subMarks.end();
                  ++mit )
            {
                (*it)->Unmark( *mit );
            }
        }
    }
}

bool CoveringSetMethod::VerifyForwardConditions()
{
    if ( _subset.empty() || _subMarks.size() != _subset.size() )
    {
        return false;
    }
    // check for the subset definition
    if ( !std::includes( _sector.begin(), _sector.end(),
                         _subset.begin(), _subset.end(),
                         _sector.value_comp() ) )
    {
        return false;
    }
    // find one mark in the diff
    for ( Puzzle::Container::iterator it = _diff.begin();
          it != _diff.end();
          ++it )
    {
        if ( (*it)->CanGuess() )
        {
            Cell::MarkedValues curr = (*it)->GetMarkedValues();
            Cell::MarkedValues::iterator found = std::find_first_of(
                curr.begin(), curr.end(),
                _subMarks.begin(), _subMarks.end() );
            if ( found != curr.end() )
            {
                return true;
            }
        }
    }
    return false;
}

void CoveringSetMethod::ExecuteReverse()
{
    for ( Puzzle::Container::iterator it = _diff.begin();
          it != _diff.end();
          ++it )
    {
        for ( Cell::MarkedValues::iterator mit = _subMarks.begin();
              mit != _subMarks.end();
              ++mit )
        {
            if ( (*it)->CanGuess() )
            {
                (*it)->Mark( *mit );
            }
        }
    }
}

bool CoveringSetMethod::VerifyReverseConditions()
{
    if ( _subset.empty() || _subMarks.size() != _subset.size() )
    {
        return false;
    }
    // check for the subset definition
    if ( !std::includes( _sector.begin(), _sector.end(),
                         _subset.begin(), _subset.end(),
                         _sector.value_comp()  ) )
    {
        return false;
    }
    // diff must have no marks from the _subMarks
    for ( Puzzle::Container::iterator it = _diff.begin();
          it != _diff.end();
          ++it )
    {
        if ( (*it)->CanGuess() )
        {
            Cell::MarkedValues curr = (*it)->GetMarkedValues();
            Cell::MarkedValues::iterator found = std::find_first_of(
                curr.begin(), curr.end(),
                _subMarks.begin(), _subMarks.end() );
            if ( found != curr.end() )
            {
                return false;
            }
        }
    }
    return true;
}

}
