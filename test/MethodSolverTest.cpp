#include "../MethodSolver.h"
#include "../Puzzle.h"
#include "../SolverHelper.h"
#include "../PuzzleMarker.h"
#include "../SimpleValidator.h"
#include "../SolutionMethodFactory.h"
#include "gtest/gtest.h"

namespace {

class MethodSolverTest : public ::testing::Test
{
protected:
    MethodSolverTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
        _factory.reset( new Sudoku::SolutionMethodFactory );
        _helper.reset( new Sudoku::SolverHelper( _factory ) );
        _marker.reset( new Sudoku::PuzzleMarker );
        _validator = Sudoku::SimpleValidator::CreateGuessValidator();
        
        _solver.reset( new Sudoku::MethodSolver( _helper, _marker, _validator ) );
    }

    virtual ~MethodSolverTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    /**
     * SOURCE:
     * http://www.websudoku.com/?level=2&set_id=450688450
     */
    void MakeMediumPuzzle()
    {
        _puzzle->GetCell( 2, 1 )->SetCorrect( 2 );
        _puzzle->GetCell( 6, 1 )->SetCorrect( 8 );
        _puzzle->GetCell( 1, 2 )->SetCorrect( 4 );
        _puzzle->GetCell( 3, 2 )->SetCorrect( 7 );
        _puzzle->GetCell( 4, 2 )->SetCorrect( 3 );
        _puzzle->GetCell( 7, 2 )->SetCorrect( 2 );
        _puzzle->GetCell( 9, 2 )->SetCorrect( 5 );
        _puzzle->GetCell( 4, 3 )->SetCorrect( 2 );
        _puzzle->GetCell( 5, 3 )->SetCorrect( 5 );
        _puzzle->GetCell( 6, 3 )->SetCorrect( 9 );
        _puzzle->GetCell( 4, 3 )->SetCorrect( 2 );
        _puzzle->GetCell( 1, 4 )->SetCorrect( 2 );
        _puzzle->GetCell( 2, 4 )->SetCorrect( 1 );
        _puzzle->GetCell( 8, 4 )->SetCorrect( 5 );
        _puzzle->GetCell( 9, 4 )->SetCorrect( 4 );
        _puzzle->GetCell( 3, 5 )->SetCorrect( 3 );
        _puzzle->GetCell( 7, 5 )->SetCorrect( 7 );
        _puzzle->GetCell( 1, 6 )->SetCorrect( 8 );
        _puzzle->GetCell( 2, 6 )->SetCorrect( 7 );
        _puzzle->GetCell( 8, 6 )->SetCorrect( 3 );
        _puzzle->GetCell( 9, 6 )->SetCorrect( 2 );
        _puzzle->GetCell( 4, 7 )->SetCorrect( 8 );
        _puzzle->GetCell( 5, 7 )->SetCorrect( 6 );
        _puzzle->GetCell( 6, 7 )->SetCorrect( 4 );
        _puzzle->GetCell( 1, 8 )->SetCorrect( 6 );
        _puzzle->GetCell( 3, 8 )->SetCorrect( 4 );
        _puzzle->GetCell( 6, 8 )->SetCorrect( 1 );
        _puzzle->GetCell( 7, 8 )->SetCorrect( 8 );
        _puzzle->GetCell( 9, 8 )->SetCorrect( 3 );
        _puzzle->GetCell( 4, 9 )->SetCorrect( 5 );
        _puzzle->GetCell( 8, 9 )->SetCorrect( 4 );

        Sudoku::Puzzle::Container all = _puzzle->GetAllCells();
        for ( Sudoku::Puzzle::Container::iterator it = all.begin();
              it != all.end();
              ++it )
        {
            if ( (*it)->GetCorrectValue() != 0 )
            {
                (*it)->Display( true );
            }
        }
    }
    
    std::shared_ptr<Sudoku::Puzzle> _puzzle;
    std::shared_ptr<Sudoku::MethodSolver> _solver;
    std::shared_ptr<Sudoku::SolverHelper> _helper;
    std::shared_ptr<Sudoku::IPuzzleMarker> _marker;
    std::shared_ptr<Sudoku::IValidator> _validator;
    std::shared_ptr<Sudoku::SolutionMethodFactory> _factory;
};

// Test a given medium puzzle, can we solve it?
TEST_F( MethodSolverTest, CanSolveMediumPuzzle )
{
    MakeMediumPuzzle();
    EXPECT_NO_THROW( _solver->Solve( _puzzle ) );
    EXPECT_TRUE( _validator->IsValid( _puzzle ) );
}

}  // namespace
