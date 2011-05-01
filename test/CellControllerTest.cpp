#include "../CellController.h"
#include "../Puzzle.h"
#include "MockCommandExecutor.h"
#include "MockPuzzleAccess.h"
#include "gtest/gtest.h"

namespace {

using ::testing::Return;
using ::testing::_;

class CellControllerTest : public ::testing::Test
{
protected:
    CellControllerTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
        _puzzleAccess.reset( new Sudoku::MockPuzzleAccess );
        _commandExec.reset( new Sudoku::MockCommandExecutor );

        ON_CALL( *_puzzleAccess, GetPuzzle() )
            .WillByDefault( Return( _puzzle ) );
    }

    virtual ~CellControllerTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    std::shared_ptr<Sudoku::MockPuzzleAccess> _puzzleAccess;
    std::shared_ptr<Sudoku::MockCommandExecutor> _commandExec;
    std::shared_ptr<Sudoku::Puzzle> _puzzle;
};

// make sure we throw on null pointers
TEST_F( CellControllerTest, ThrowIfNullPuzzleAccess )
{
    _puzzleAccess.reset();
    EXPECT_ANY_THROW(
        Sudoku::CellController controller( _commandExec, _puzzleAccess ) );
}

// make sure we throw on null pointers
TEST_F( CellControllerTest, ThrowIfNullCommandExecutor )
{
    _commandExec.reset();
    EXPECT_ANY_THROW(
        Sudoku::CellController controller( _commandExec, _puzzleAccess ) );
}

// check that guessing works
TEST_F( CellControllerTest, MakeGuessExecutesCommand )
{
    Sudoku::CellController controller( _commandExec, _puzzleAccess );
    
    EXPECT_CALL( *_puzzleAccess, GetPuzzle() )
        .Times( 1 );
    EXPECT_CALL( *_commandExec, Execute(_) )
        .Times( 1 );

    controller.MakeGuess( 3, 3, 3 );
}

// check that marking works
TEST_F( CellControllerTest, MarkExecutesCommand )
{
    Sudoku::CellController controller( _commandExec, _puzzleAccess );
    
    EXPECT_CALL( *_puzzleAccess, GetPuzzle() )
        .Times( 1 );
    EXPECT_CALL( *_commandExec, Execute(_) )
        .Times( 1 );

    controller.Mark( 3, 3, 9 );
}

// check that unmarking works
TEST_F( CellControllerTest, UnmarkExecutesCommand )
{
    Sudoku::CellController controller( _commandExec, _puzzleAccess );
    
    EXPECT_CALL( *_puzzleAccess, GetPuzzle() )
        .Times( 1 );
    EXPECT_CALL( *_commandExec, Execute(_) )
        .Times( 1 );

    controller.Unmark( 2, 7, 9 );
}

}  // namespace
