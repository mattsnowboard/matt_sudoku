#include "../AddHintMarksCommand.h"
#include "../Puzzle.h"
#include "../PuzzleMarker.h"
#include "gtest/gtest.h"

namespace {

class AddHintMarksCommandTest : public ::testing::Test
{
protected:
    AddHintMarksCommandTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
        _marker.reset( new Sudoku::PuzzleMarker );
    }

    virtual ~AddHintMarksCommandTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    std::shared_ptr<Sudoku::Puzzle> _puzzle;
    std::shared_ptr<Sudoku::IPuzzleMarker> _marker;
    std::shared_ptr<Sudoku::Command<Sudoku::Puzzle> > _command;
};

// Make sure we are checking for the proper Puzzle
TEST_F( AddHintMarksCommandTest, CannotExecuteIfWrongPuzzle )
{
    std::shared_ptr<Sudoku::Puzzle> fake( new Sudoku::Puzzle );
    _command = Sudoku::AddHintMarksCommand::CreateAddHintMarksCommand(
        _puzzle, _marker );
    EXPECT_ANY_THROW( _command->Execute( fake ) );
}

// Make sure we check correct param in unexecute
TEST_F( AddHintMarksCommandTest, CannotUnexecuteIfWrongPuzzle )
{
    std::shared_ptr<Sudoku::Puzzle> fake( new Sudoku::Puzzle );
    _command = Sudoku::AddHintMarksCommand::CreateAddHintMarksCommand(
        _puzzle, _marker );
    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_ANY_THROW( _command->Unexecute( fake ) );
}

// cannot be executed twice in a row
TEST_F( AddHintMarksCommandTest, CannotExecuteTwiceInARow )
{
    _command = Sudoku::AddHintMarksCommand::CreateAddHintMarksCommand(
        _puzzle, _marker );
    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_ANY_THROW( _command->Execute( _puzzle ) );
}

// cannot unexecute before execute
TEST_F( AddHintMarksCommandTest, CannotUnexecuteBeforeExecute )
{
    _command = Sudoku::AddHintMarksCommand::CreateAddHintMarksCommand(
        _puzzle, _marker );
    EXPECT_ANY_THROW( _command->Unexecute( _puzzle ) );
}

// cannot unexecute twice in a row
TEST_F( AddHintMarksCommandTest, CannotUnexecuteTwiceInARow )
{
    _command = Sudoku::AddHintMarksCommand::CreateAddHintMarksCommand(
        _puzzle, _marker );
    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_TRUE( _command->Unexecute( _puzzle ) );
    EXPECT_ANY_THROW( _command->Unexecute( _puzzle ) );
}

// Execute/Unexecute does not change Puzzle
TEST_F( AddHintMarksCommandTest, ExecuteUnexecuteDoesNotChangePuzzle )
{
    _puzzle->GetCell( 3, 3 )->Mark( 4 );
    _puzzle->GetCell( 4, 4 )->Mark( 9 );
    std::shared_ptr<Sudoku::Puzzle> copyPuzzle(
        new Sudoku::Puzzle( *_puzzle ) );
    _command = Sudoku::AddHintMarksCommand::CreateAddHintMarksCommand(
        _puzzle, _marker );
    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_TRUE( _command->Unexecute( _puzzle ) );
    EXPECT_EQ( *copyPuzzle, *_puzzle );
}

// check that marks get set
TEST_F( AddHintMarksCommandTest, SetsMarksOnExecute )
{
    // add some guess values to mark around
    _puzzle->GetCell( 3, 3 )->SetGuess( 4 );
    _puzzle->GetCell( 4, 4 )->SetGuess( 9 );
    _puzzle->GetCell( 1, 7 )->SetGuess( 3 );
    std::shared_ptr<Sudoku::Puzzle> copyPuzzle(
        new Sudoku::Puzzle( *_puzzle ) );
    _command = Sudoku::AddHintMarksCommand::CreateAddHintMarksCommand(
        _puzzle, _marker );
    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_EQ( *copyPuzzle, *_puzzle );    
}

// can execute, unexecute, execute
TEST_F( AddHintMarksCommandTest, CanExecuteUnexecuteExecute )
{
    _command = Sudoku::AddHintMarksCommand::CreateAddHintMarksCommand(
        _puzzle, _marker );
    EXPECT_TRUE( _command->Execute( _puzzle ) );
    EXPECT_TRUE( _command->Unexecute( _puzzle ) );
    EXPECT_TRUE( _command->Execute( _puzzle ) );
}

}  // namespace
