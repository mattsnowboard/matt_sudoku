#include "../UnmarkCommand.h"
#include "../Cell.h"
#include "gtest/gtest.h"

namespace {

class UnmarkCommandTest : public ::testing::Test
{
protected:
    UnmarkCommandTest()
    {
        _cell.reset( new Sudoku::Cell );
    }

    virtual ~UnmarkCommandTest()
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
TEST_F( UnmarkCommandTest, CannotExecuteIfWrongCell )
{
    std::shared_ptr<Sudoku::Cell> fake( new Sudoku::Cell );
    _cell->Mark( 4 );
    fake->Mark( 4 );
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 4 );
    EXPECT_ANY_THROW( _command->Execute( fake ) );
}

// Make sure we check correct param in unexecute
TEST_F( UnmarkCommandTest, CannotUnexecuteIfWrongCell )
{
    std::shared_ptr<Sudoku::Cell> fake( new Sudoku::Cell );
    _cell->Mark( 4 );
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 4 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_ANY_THROW( _command->Unexecute( fake ) );
}

// Unmark cannot be executed if Cell has displayed correct value
TEST_F( UnmarkCommandTest, CannotExecuteIfCorrectDisplayed )
{
    _cell->SetCorrect( 8 );
    _cell->Display( true );
    _cell->Mark( 4 );
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 4 );
    EXPECT_FALSE( _command->Execute( _cell ) );
    EXPECT_TRUE( _cell->GetMarkContainer()[4] );
}

// Unmark cannot be executed if Cell lacks mark
TEST_F( UnmarkCommandTest, CannotExecuteIfUnmarked )
{
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 4 );
    EXPECT_FALSE( _command->Execute( _cell ) );
    EXPECT_FALSE( _cell->GetMarkContainer()[4] );
}

// Unmark cannot be unexecuted if Cell lacks mark
TEST_F( UnmarkCommandTest, CannotUnexecuteIfUnmarked )
{
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 4 );
    EXPECT_FALSE( _command->Execute( _cell ) );
    EXPECT_ANY_THROW( _command->Unexecute( _cell ) );
    EXPECT_FALSE( _cell->GetMarkContainer()[4] );
}

// Unmark cannot be executed twice in a row
TEST_F( UnmarkCommandTest, CannotExecuteTwiceInARow )
{
    _cell->Mark( 2 );
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 2 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_ANY_THROW( _command->Execute( _cell ) );
}

// Execute/Unexecute does not change Cell
TEST_F( UnmarkCommandTest, ExecuteUnexecuteDoesNotChangeCell )
{
    _cell->Mark( 3 );
    _cell->Mark( 4 );
    std::shared_ptr<Sudoku::Cell> copyCell( new Sudoku::Cell( *_cell ) );
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_TRUE( _command->Unexecute( _cell ) );
    EXPECT_EQ( *copyCell, *_cell );
}

// Execute clears mark
TEST_F( UnmarkCommandTest, ExecuteSetUnmark )
{
    _cell->Mark( 1 );
    _cell->Mark( 2 );
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 1 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_FALSE( _cell->GetMarkContainer()[1] );
    EXPECT_TRUE( _cell->GetMarkContainer()[2] );
}

// Unmark cannot be unexecuted before execute
TEST_F( UnmarkCommandTest, CannotUnexecuteBeforeExecute )
{
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 9 );
    EXPECT_ANY_THROW( _command->Unexecute( _cell ) );
}

// Unmark cannot be unexecuted twice in a row
TEST_F( UnmarkCommandTest, CannotUnexecuteTwiceInARow )
{
    _cell->Mark( 3 );
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_TRUE( _command->Unexecute( _cell ) );
    EXPECT_ANY_THROW( _command->Unexecute( _cell ) );
}

// Unmark can Execute then unexecute, then execute
TEST_F( UnmarkCommandTest, CanExecuteUnexecuteExecute )
{
    _cell->Mark( 3 );
    _command = Sudoku::UnmarkCommand::CreateUnmarkCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_TRUE( _command->Unexecute( _cell ) );
    EXPECT_TRUE( _command->Execute( _cell ) );
    EXPECT_FALSE( _cell->GetMarkContainer()[3] );
}

}  // namespace
