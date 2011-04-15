
#include "PuzzleMarker.h"
#include "Puzzle.h"

namespace Sudoku
{

void PuzzleMarker::ClearPuzzle( std::shared_ptr<Puzzle> puzzle )
{
    Puzzle::Container all = puzzle->GetAllCells();
    for ( Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        (*it)->ClearMarks();
    }
}

void PuzzleMarker::UpdateMarks( std::shared_ptr<Puzzle> puzzle )
{
    Puzzle::Container all = puzzle->GetAllCells();
    for ( Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        // get neighbors
        Puzzle::Container N = puzzle->GetNeighbors( *it );
        // all values are possible
        Cell::MarkContainer possible;
        possible.set();
        // iterator over neighbors and find what is possible
        for ( Puzzle::Container::iterator neighbors = N.begin();
              neighbors != N.end();
              ++neighbors )
        {
            possible.reset( (*neighbors)->DisplayedValue() );
        }
        // update Cell marks
        (*it)->SetMarkContainer( possible );
    }
}

}
