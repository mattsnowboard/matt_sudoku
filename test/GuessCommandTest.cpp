#include "../GuessCommand.h"
#include "../Cell.h"
#include "gtest/gtest.h"

namespace {

class GuessCommandTest : public ::testing::Test
{
protected:

    GuessCommandTest()
    {
        _cell.reset( new Sudoku::Cell );
    }

    virtual ~GuessCommandTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    std::shared_ptr<Sudoku::Cell> _cell;
    std::shared_ptr<Sudoku::Command<Sudoku::Cell> > _command;
};

// Make sure we check correct param in execute
TEST_F( GuessCommandTest, CannotExecuteIfWrongCell )
{
    std::shared_ptr<Sudoku::Cell> fake( new Sudoku::Cell );
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 4 );
    EXPECT_ANY_THROW( _command->Execute( fake ) );
}

// Make sure we check correct param in unexecute
TEST_F( GuessCommandTest, CannotUnexecuteIfWrongCell )
{
    std::shared_ptr<Sudoku::Cell> fake( new Sudoku::Cell );
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 4 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_ANY_THROW( _command->Unexecute( fake ) );
}

// Guess cannot be executed if Cell has displayed correct value
TEST_F( GuessCommandTest, CannotExecuteIfCorrectDisplayed )
{
    _cell->SetCorrect( 8 );
    _cell->Display( true );
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 4 );
    EXPECT_FALSE( _command->Execute( _cell ) );
    // check guessed value of Cell
    _cell->Display( false );
    EXPECT_NE( 4, _cell->DisplayedValue() );
}

// Guess cannot be executed twice in a row
TEST_F( GuessCommandTest, CannotExecuteTwiceInARow )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_ANY_THROW( _command->Execute( _cell ) );
}

// Execute/Unexecute does not change Cell
TEST_F( GuessCommandTest, ExecuteUnexecuteDoesNotChangeCell )
{
    _cell->SetGuess( 1 );
    std::shared_ptr<Sudoku::Cell> copyCell( new Sudoku::Cell( *_cell ) );
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_TRUE( _command->Unexecute( _cell ) );
    EXPECT_EQ( *copyCell, *_cell );
}

// Execute sets display value
TEST_F( GuessCommandTest, ExecuteSetGuessValue )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 7 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_EQ( 7, _cell->DisplayedValue() );
}

// Guess cannot be unexecuted before execute
TEST_F( GuessCommandTest, CannotUnexecuteBeforeExecute )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 9 );
    EXPECT_ANY_THROW( _command->Unexecute( _cell ) );
}

// Guess cannot be unexecuted twice in a row
TEST_F( GuessCommandTest, CannotUnexecuteTwiceInARow )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_TRUE( _command->Unexecute( _cell ) );
    EXPECT_ANY_THROW( _command->Unexecute( _cell ) );
}

// Guess can Execute then unexecute, then execute
TEST_F( GuessCommandTest, CanExecuteUnexecuteExecute )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_TRUE( _command->Unexecute( _cell ) );
    EXPECT_TRUE( _command->Execute( _cell ) );
}


}  // namespace
