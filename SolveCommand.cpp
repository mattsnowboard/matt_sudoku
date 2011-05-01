#include "SolveCommand.h"
#include "ISolver.h"
#include "Puzzle.h"

namespace Sudoku
{

std::shared_ptr<Command<Puzzle> > SolveCommand::Create(
    std::shared_ptr<const Puzzle> puzzle,
    std::shared_ptr<ISolver> solver )
{
    std::shared_ptr<Command<Puzzle> > c(
        new SolveCommand( puzzle, solver ) );
    return c;
}

SolveCommand::SolveCommand(
    std::shared_ptr<const Puzzle> puzzle,
    std::shared_ptr<ISolver> solver )
    : Command<Puzzle>( puzzle ), _solver( solver )
{}

bool SolveCommand::execute( std::shared_ptr<Puzzle> p )
{
    if ( !_solver )
    {
        throw std::runtime_error(
            "Cannot execute hint command without solver." );
    }
    if ( _copyPuzzle )
    {
        throw std::runtime_error(
            "The old puzzle state was not cleared properly on last execute." );
    }
    // store old state
    _copyPuzzle.reset( new Puzzle( *p ) );
    _solver->Solve( p );
    return true;
}

bool SolveCommand::unexecute( std::shared_ptr<Puzzle> p )
{
    if ( !_copyPuzzle )
    {
        throw std::runtime_error(
            "The old puzzle was not stored properly on last execute." );
    }
    Puzzle::Container all = p->GetAllCells();
    Puzzle::Container copy = _copyPuzzle->GetAllCells();
    for ( Puzzle::Container::iterator it = all.begin(), it2 = copy.begin();
          it != all.end();
          ++it, ++it2 )
    {
        *(*it) = *(*it2);
    }
    _copyPuzzle.reset();
    return true;
}

}
