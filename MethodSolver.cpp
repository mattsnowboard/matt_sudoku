
#include "MethodSolver.h"
#include "Puzzle.h"
#include "SolverHelper.h"
#include "IPuzzleMarker.h"
#include "IValidator.h"
#include "SolutionMethod.h"

#define FILELOG_MAX_LEVEL logDEBUG4
#include "Log.h"

namespace Sudoku
{

namespace
{

bool ExecuteAllMethods( SolverHelper::MethodContainer methods )
{
    bool anyExecuted = false;
    for ( SolverHelper::MethodContainer::iterator it = methods.begin();
          it != methods.end();
          ++it )
    {
        FILE_LOG(logINFO) << "Attempting Method.";
        if ( (*it)->VerifyForwardConditions() )
        {
            (*it)->ExecuteForward();
            FILE_LOG(logINFO) << "Executed Method.";
            anyExecuted = true;
        }
    }
    return anyExecuted;
}

bool ExecuteRandomMethod( SolverHelper::MethodContainer methods )
{
    if ( !methods.empty() )
    {
        // scramble methods and get first one
        std::random_shuffle( methods.begin(), methods.end() );
        FILE_LOG(logINFO) << "Attempting Method.";
        if ( methods.front()->VerifyForwardConditions() )
        {
            methods.front()->ExecuteForward();
            FILE_LOG(logINFO) << "Executed Method.";
            return true;
        }
    }
    return false;
}

}

void MethodSolver::Solve( std::shared_ptr<Puzzle> p )
{
    if ( !_marker || !_helper || !_validator )
    {
        throw std::runtime_error(
            "Need PuzzleMarker, SolverHelper, and Validator" );
    }
    
    // ensure we have minimum solved Cells
    unsigned solvedCount = 0;
    Puzzle::Container all = p->GetAllCells();
    FILE_LOG(logINFO) << "Checking if we have minimum cells to solve.";
    for ( Puzzle::Container::iterator it = all.begin();
          it != all.end() && solvedCount < MIN_CORRECT_VALUES;
          ++it )
    {
        if ( !(*it)->CanGuess() && (*it)->DisplayedValue() != 0 )
        {
            ++solvedCount;
            FILE_LOG(logDEBUG) << "Found solved Cell: " << *it;
        }
    }
    if ( solvedCount < MIN_CORRECT_VALUES )
    {
        FILE_LOG(logERROR) << "Too few clues to solve Puzzle (" << solvedCount
                           << ")";
        throw std::runtime_error( "Too few clues to solve Puzzle." );
    }

    _marker->UpdateMarks( p );
    FILE_LOG(logINFO) << "Added appropriate marks";


    // use the helper to get the increasingly difficult Methods
    // grab one and execute
    // repeat for all at each difficulty
    // repeat entire process until puzzle solved or no more Methods
    bool gotAnyMethods = false;
    bool valid = false;
    do
    {
        gotAnyMethods = false;

        // Single Candidate, we can safely execute all
        FILE_LOG(logINFO) << "Single Candidate Methods";
        SolverHelper::MethodContainer methods =
            _helper->GetAllSingleCandidate( p );
        if ( !methods.empty() )
        {
            bool success = ExecuteAllMethods( methods );
            gotAnyMethods = gotAnyMethods || success;
        }
        
        // Exclusion Method
        FILE_LOG(logINFO) << "Exclusion Methods";
        methods.clear();
        methods = _helper->GetAllExclusion( p );
        if ( !methods.empty() )
        {
            bool success = ExecuteAllMethods( methods );
            gotAnyMethods = gotAnyMethods || success;
        }

        // Block Intersection
        methods.clear();
        // go over all Cells with marks
        for ( Puzzle::Container::iterator itC = all.begin();
              itC != all.end();
              ++itC )
        {
            if ( (*itC)->CanGuess() && (*itC)->GetMarkContainer().any() )
            {
                FILE_LOG(logINFO) << "Checking for Block Intersection Methods"
                                  << " on Cell: " << *itC;
                methods = _helper->GetAllBlockIntersection( p, *itC );
                
                if ( !methods.empty() )
                {
                    bool success = ExecuteAllMethods( methods );
                    gotAnyMethods = gotAnyMethods || success;
                }
            }
        }

        // Covering Set
        methods.clear();
        // go over all sectors
        for ( size_t row = 1; row <= 9; row++ )
        {
            FILE_LOG(logINFO) << "Checking row " << row
                              << " for Covering Set Methods.";
            Puzzle::Container row_ = p->GetRow( row );
            methods = _helper->GetAllCoveringSet( p, row_ );
            if ( !methods.empty() )
            {
                    bool success = ExecuteRandomMethod( methods );
                    gotAnyMethods = gotAnyMethods || success;
            }
        }
        for ( size_t col = 1; col <= 9; col++ )
        {
            FILE_LOG(logINFO) << "Checking col " << col
                              << " for Covering Set Methods.";
            Puzzle::Container col_ = p->GetCol( col );
            methods = _helper->GetAllCoveringSet( p, col_ );
            if ( !methods.empty() )
            {
                    bool success = ExecuteRandomMethod( methods );
                    gotAnyMethods = gotAnyMethods || success;
            }
        }
        for ( size_t x = 1; x <= 3; x++ )
        {
            for ( size_t y = 1; y <= 3; y++ )
            {
                FILE_LOG(logINFO) << "Checking block " << x << ", " << y
                                  << " for Covering Set Methods.";
                Puzzle::Container block_ = p->GetBlock( x, y );
                methods = _helper->GetAllCoveringSet( p, block_ );
                if ( !methods.empty() )
                {
                    bool success = ExecuteRandomMethod( methods );
                    gotAnyMethods = gotAnyMethods || success;
                }
            }
        }

    } while ( !(valid = _validator->IsValid( p )) && gotAnyMethods );

    if ( !valid )
    {
        FILE_LOG(logERROR) << "Could not solve Puzzle.";
        throw std::runtime_error( "Could not solve Puzzle" );
    }
}

void MethodSolver::CommitGuesses( std::shared_ptr<Puzzle> p )
{
    Puzzle::Container all = p->GetAllCells();
    FILE_LOG(logINFO) << "Committing Guesses to correct answers.";
    for ( Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        if ( (*it)->CanGuess() && (*it)->DisplayedValue() != 0 )
        {
            FILE_LOG(logDEBUG) << "Committing Cell: " << (*it);
            (*it)->SetCorrect( (*it)->DisplayedValue() );
            (*it)->SetGuess( 0 );
            (*it)->ClearMarks();
            FILE_LOG(logDEBUG) << "Done Committing Cell: " << (*it);
        }
    }
}


}
