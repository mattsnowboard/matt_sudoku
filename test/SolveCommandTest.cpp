#include "../SolveCommand.h"
#include "../Puzzle.h"
#include "MockSolver.h"
#include "gtest/gtest.h"

namespace {

using ::testing::_;
using ::testing::Ref;
using ::testing::Invoke;

class SolveCommandTest : public ::testing::Test
{
protected:

    SolveCommandTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
        _solver.reset( new Sudoku::MockSolver );
        
        ON_CALL( *_solver, Solve(_) )
            .WillByDefault( Invoke( this, &SolveCommandTest::changePuzzle ) );
    }

    virtual ~SolveCommandTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    // just some nonsense changes to see if we can roll them back
    void changePuzzle( std::shared_ptr<Sudoku::Puzzle> p )
    {
        Sudoku::Puzzle::Container all = p->GetAllCells();
        for ( Sudoku::Puzzle::Container::iterator it = all.begin();
              it != all.end();
              ++it )
        {
            (*it)->SetGuess( 1 );
        }
    }

    std::shared_ptr<Sudoku::Puzzle> _puzzle;
    std::shared_ptr<Sudoku::MockSolver> _solver;
    std::shared_ptr<Sudoku::Command<Sudoku::Puzzle> > _command;
};

// Make sure we are checking for the proper Puzzle
TEST_F( SolveCommandTest, CannotExecuteIfWrongPuzzle )
{
    std::shared_ptr<Sudoku::Puzzle> fake( new Sudoku::Puzzle );
    _command = Sudoku::SolveCommand::Create( _puzzle, _solver );
    EXPECT_ANY_THROW( _command->Execute( fake ) );
}

// Make sure we check correct param in unexecute
TEST_F( SolveCommandTest, CannotUnexecuteIfWrongPuzzle )
{
    std::shared_ptr<Sudoku::Puzzle> fake( new Sudoku::Puzzle );
    _command = Sudoku::SolveCommand::Create( _puzzle, _solver );

    EXPECT_CALL( *_solver, Solve( _puzzle ) )
        .Times( 1 );

    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_ANY_THROW( _command->Unexecute( fake ) );
}

// cannot be executed twice in a row
TEST_F( SolveCommandTest, CannotExecuteTwiceInARow )
{
    _command = Sudoku::SolveCommand::Create( _puzzle, _solver );

    EXPECT_CALL( *_solver, Solve( _puzzle ) )
        .Times( 1 );

    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_ANY_THROW( _command->Execute( _puzzle ) );
}

// cannot unexecute before execute
TEST_F( SolveCommandTest, CannotUnexecuteBeforeExecute )
{
    _command = Sudoku::SolveCommand::Create( _puzzle, _solver );

    EXPECT_CALL( *_solver, Solve( _puzzle ) )
        .Times( 0 );

    EXPECT_ANY_THROW( _command->Unexecute( _puzzle ) );
}

// cannot unexecute twice in a row
TEST_F( SolveCommandTest, CannotUnexecuteTwiceInARow )
{
    _command = Sudoku::SolveCommand::Create( _puzzle, _solver );

    EXPECT_CALL( *_solver, Solve( _puzzle ) )
        .Times( 1 );

    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_TRUE( _command->Unexecute( _puzzle ) );
    EXPECT_ANY_THROW( _command->Unexecute( _puzzle ) );
}

// Execute/Unexecute does not change Puzzle
TEST_F( SolveCommandTest, ExecuteUnexecuteDoesNotChangePuzzle )
{
    _puzzle->GetCell( 3, 3 )->SetCorrect( 4 );
    _puzzle->GetCell( 4, 4 )->SetCorrect( 9 );
    std::shared_ptr<Sudoku::Puzzle> copy( new Sudoku::Puzzle( *_puzzle ) );
    _command = Sudoku::SolveCommand::Create( _puzzle, _solver );

    EXPECT_CALL( *_solver, Solve( _puzzle ) )
        .Times( 1 );

    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_TRUE( _command->Unexecute( _puzzle ) );
    EXPECT_EQ( *copy, *_puzzle );
}

}  // namespace
