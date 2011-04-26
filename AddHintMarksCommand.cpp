#include "AddHintMarksCommand.h"
#include "IPuzzleMarker.h"
#include "Puzzle.h"

namespace Sudoku
{

std::shared_ptr<Command<Puzzle> > AddHintMarksCommand::CreateAddHintMarksCommand(
    std::shared_ptr<const Puzzle> puzzle,
    std::shared_ptr<IPuzzleMarker> marker )
{
    std::shared_ptr<Command<Puzzle> > c(
        new AddHintMarksCommand( puzzle, marker ) );
    return c;
}

AddHintMarksCommand::AddHintMarksCommand(
    std::shared_ptr<const Puzzle> puzzle,
    std::shared_ptr<IPuzzleMarker> marker )
    : Command<Puzzle>( puzzle ), _marker( marker )
{}

bool AddHintMarksCommand::execute( std::shared_ptr<Puzzle> p )
{
    if ( !_marker )
    {
        throw std::runtime_error(
            "Cannot execute hint command without marker." );
    }
    if ( !_oldMarks.empty() )
    {
        throw std::runtime_error(
            "The old marks were not cleared properly on last execute." );
    }
    // store all old values
    Puzzle::Container all = p->GetAllCells();
    for ( Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        _oldMarks.push_back( (*it)->GetMarkContainer() );
    }
    _marker->UpdateMarks( p );
    return true;
}

bool AddHintMarksCommand::unexecute( std::shared_ptr<Puzzle> p )
{
    Puzzle::Container all = p->GetAllCells();
    if ( _oldMarks.empty() || _oldMarks.size() != all.size() )
    {
        throw std::runtime_error(
            "The old marks were not stored properly on last execute." );
    }
    std::vector<Cell::MarkContainer>::iterator mit = _oldMarks.begin();
    for ( Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it, ++mit )
    {
        (*it)->SetMarkContainer( *mit );
    }
    _oldMarks.clear();
    return true;
}

}
