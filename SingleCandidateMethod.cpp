#include "SingleCandidateMethod.h"
#include "Cell.h"

namespace Sudoku
{

void SingleCandidateMethod::ExecuteForward()
{
    // @todo Here we may want to store old value for undo
    Cell::MarkedValues m = _cell->GetMarkedValues();
    if ( m.empty() )
    {
        throw std::logic_error(
            "Cannot execute Single Candidate Method without marks" );
    }
    // set the guess to the single marked value
    _cell->SetGuess( *m.begin() );
    // remove the marks
    _cell->ClearMarks();
}

bool SingleCandidateMethod::VerifyForwardConditions()
{
    // check that there is one mark, no correct value displayed, and no guess
    return ( _cell->GetMarkedValues().size() == 1 &&
             _cell->CanGuess() && _cell->DisplayedValue() == 0 );
}

void SingleCandidateMethod::ExecuteReverse()
{
    // mark the old guess
    _cell->Mark( _cell->DisplayedValue() );
    // clear the guess
    _cell->SetGuess( 0 );
}

bool SingleCandidateMethod::VerifyReverseConditions()
{
    // check that there are no marks and a guess is set
    return ( _cell->GetMarkedValues().empty() &&
             _cell->CanGuess() && _cell->DisplayedValue() != 0 );
}

}
