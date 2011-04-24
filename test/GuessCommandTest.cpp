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
    std::shared_ptr<Sudoku::Command> _command;
};

// Guess cannot be executed if Cell has displayed correct value
TEST_F( GuessCommandTest, CannotExecuteIfCorrectDisplayed )
{
    _cell->SetCorrect( 8 );
    _cell->Display( true );
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 4 );
    EXPECT_FALSE( _command->Execute() );
    // check guessed value of Cell
    _cell->Display( false );
    EXPECT_NE( 4, _cell->DisplayedValue() );
}

// Guess cannot be executed twice in a row
TEST_F( GuessCommandTest, CannotExecuteTwiceInARow )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_ANY_THROW( _command->Execute() );
}

// Execute/Unexecute does not change Cell
TEST_F( GuessCommandTest, ExecuteUnexecuteDoesNotChangeCell )
{
    _cell->SetGuess( 1 );
    std::shared_ptr<Sudoku::Cell> copyCell( new Sudoku::Cell( *_cell ) );
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_TRUE( _command->Unexecute() );
    EXPECT_EQ( *copyCell, *_cell );
}

// Execute sets display value
TEST_F( GuessCommandTest, ExecuteSetGuessValue )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 7 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_EQ( 7, _cell->DisplayedValue() );
}

// Guess cannot be unexecuted before execute
TEST_F( GuessCommandTest, CannotUnexecuteBeforeExecute )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 9 );
    EXPECT_ANY_THROW( _command->Unexecute() );
}

// Guess cannot be unexecuted twice in a row
TEST_F( GuessCommandTest, CannotUnexecuteTwiceInARow )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_TRUE( _command->Unexecute() );
    EXPECT_ANY_THROW( _command->Unexecute() );
}

// Guess can Execute then unexecute, then execute
TEST_F( GuessCommandTest, CanExecuteUnexecuteExecute )
{
    _command = Sudoku::GuessCommand::CreateGuessCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_TRUE( _command->Unexecute() );
    EXPECT_TRUE( _command->Execute() );
}


}  // namespace
