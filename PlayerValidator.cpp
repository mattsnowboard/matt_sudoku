#include "PlayerValidator.h"

#include "Puzzle.h"

namespace Sudoku
{

bool PlayerValidator::IsValid( std::shared_ptr<Puzzle> p )
{
    Puzzle::Container all = p->GetAllCells();
    for ( Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        if ( (*it)->DisplayedValue() != 0 && !(*it)->IsCorrect() )
        {
            return false;
        }
    }

    return true;
}

}
