#include "../PuzzleController.h"
#include "../Puzzle.h"
#include "MockCommandExecutor.h"
#include "MockPuzzleAccess.h"
#include "MockPuzzleMarker.h"
#include "MockSolver.h"
#include "gtest/gtest.h"

namespace {

using ::testing::Return;
using ::testing::_;

class PuzzleControllerTest : public ::testing::Test
{
protected:
    PuzzleControllerTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
        _puzzleAccess.reset( new Sudoku::MockPuzzleAccess );
        _commandExec.reset( new Sudoku::MockCommandExecutor );
        _marker.reset( new Sudoku::MockPuzzleMarker );
        _solver.reset( new Sudoku::MockSolver );

        ON_CALL( *_puzzleAccess, GetPuzzle() )
            .WillByDefault( Return( _puzzle ) );
    }

    virtual ~PuzzleControllerTest()
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
    std::shared_ptr<Sudoku::MockPuzzleMarker> _marker;
    std::shared_ptr<Sudoku::MockSolver> _solver;
    std::shared_ptr<Sudoku::Puzzle> _puzzle;
};

// make sure we throw on null pointers
TEST_F( PuzzleControllerTest, ThrowIfNullPuzzleAccess )
{
    _puzzleAccess.reset();
    EXPECT_ANY_THROW(
        Sudoku::PuzzleController controller( _commandExec,
                                             _puzzleAccess,
                                             _marker,
                                             _solver ) );
}

// make sure we throw on null pointers
TEST_F( PuzzleControllerTest, ThrowIfNullCommandExecutor )
{
    _commandExec.reset();
    EXPECT_ANY_THROW(
        Sudoku::PuzzleController controller( _commandExec,
                                             _puzzleAccess,
                                             _marker,
                                             _solver ) );
}

// make sure we throw on null pointers
TEST_F( PuzzleControllerTest, ThrowIfNullMarker )
{
    _marker.reset();
    EXPECT_ANY_THROW(
        Sudoku::PuzzleController controller( _commandExec,
                                             _puzzleAccess,
                                             _marker,
                                             _solver ) );
}

// make sure we throw on null pointers
TEST_F( PuzzleControllerTest, ThrowIfNullSolver )
{
    _solver.reset();
    EXPECT_ANY_THROW(
        Sudoku::PuzzleController controller( _commandExec,
                                             _puzzleAccess,
                                             _marker,
                                             _solver ) );
}

// check that guessing works
TEST_F( PuzzleControllerTest, MarkHintsExecutesCommand )
{
    Sudoku::PuzzleController controller( _commandExec,
                                         _puzzleAccess,
                                         _marker,
                                         _solver );
    
    EXPECT_CALL( *_puzzleAccess, GetPuzzle() )
        .Times( 1 );
    EXPECT_CALL( *_commandExec, Execute(_) )
        .Times( 1 );

    controller.MarkHints();
}

// check that solver works
TEST_F( PuzzleControllerTest, SolveExecutesCommand )
{
    Sudoku::PuzzleController controller( _commandExec,
                                         _puzzleAccess,
                                             _marker,
                                             _solver );
    
    EXPECT_CALL( *_puzzleAccess, GetPuzzle() )
        .Times( 1 );
    EXPECT_CALL( *_commandExec, Execute(_) )
        .Times( 1 );

    controller.Solve();
}

}  // namespace
