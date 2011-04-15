#include "CorrectValidator.h"
#include "Puzzle.h"

namespace Sudoku
{

bool CorrectValidator::IsValid( std::shared_ptr<Puzzle> p )
{
    // check all Sectors for 9 unique values
    
    // start with the rows
    for ( size_t r = 1; r <= 9; r++ )
    {
        std::set<int> values;
        Puzzle::Container row = p->GetCol( r );
        for ( Puzzle::Container::iterator it = row.begin();
              it != row.end();
              ++it )
        {
            values.insert( (*it)->GetCorrectValue() );
        }
        if ( values.size() != row.size() )
        {
            return false;
        }
    }

    // then columns
    for ( size_t c = 1; c <= 9; c++ )
    {
        std::set<int> values;
        Puzzle::Container col = p->GetCol( c );
        for ( Puzzle::Container::iterator it = col.begin();
              it != col.end();
              ++it )
        {
            values.insert( (*it)->GetCorrectValue() );
        }
        if ( values.size() != col.size() )
        {
            return false;
        }
    }

    // then blocks
    for ( size_t x = 1; x <= 3; x++ )
    {
        for ( size_t y = 1; y <= 3; y++ )
        {
            std::set<int> values;
            Puzzle::Container block = p->GetBlock( x, y );
            for ( Puzzle::Container::iterator it = block.begin();
                  it != block.end();
                  ++it )
            {
                values.insert( (*it)->GetCorrectValue() );
            }
            if ( values.size() != block.size() )
            {
                return false;
            }
        }

    }

    return true;
}

}
