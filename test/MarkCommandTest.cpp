#include "../MarkCommand.h"
#include "../Cell.h"
#include "gtest/gtest.h"

namespace {

class MarkCommandTest : public ::testing::Test
{
protected:
    MarkCommandTest()
    {
        _cell.reset( new Sudoku::Cell );
    }

    virtual ~MarkCommandTest()
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

// 
TEST_F( MarkCommandTest,  )
{
}

// Mark cannot be executed if Cell has displayed correct value
TEST_F( MarkCommandTest, CannotExecuteIfCorrectDisplayed )
{
    _cell->SetCorrect( 8 );
    _cell->Display( true );
    _command = Sudoku::MarkCommand::CreateMarkCommand( _cell, 4 );
    EXPECT_FALSE( _command->Execute() );
    EXPECT_FALSE( _cell->GetMarkContainer()[4] );
}

// Mark cannot be executed if Cell has mark
TEST_F( MarkCommandTest, CannotExecuteIfAlreadyMarked )
{
    _cell->Mark( 4 );
    _command = Sudoku::MarkCommand::CreateMarkCommand( _cell, 4 );
    EXPECT_FALSE( _command->Execute() );
    EXPECT_TRUE( _cell->GetMarkContainer()[4] );
}

// Mark cannot be unexecuted if Cell has mark
TEST_F( MarkCommandTest, CannotUnexecuteIfAlreadyMarked )
{
    _cell->Mark( 4 );
    _command = Sudoku::MarkCommand::CreateMarkCommand( _cell, 4 );
    EXPECT_FALSE( _command->Execute() );
    EXPECT_ANY_THROW( _command->Unexecute() );
    EXPECT_TRUE( _cell->GetMarkContainer()[4] );
}

// Mark cannot be executed twice in a row
TEST_F( MarkCommandTest, CannotExecuteTwiceInARow )
{
    _command = Sudoku::MarkCommand::CreateMarkCommand( _cell, 2 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_ANY_THROW( _command->Execute() );
}

// Execute/Unexecute does not change Cell
TEST_F( MarkCommandTest, ExecuteUnexecuteDoesNotChangeCell )
{
    std::shared_ptr<Sudoku::Cell> copyCell( new Sudoku::Cell( *_cell ) );
    _command = Sudoku::MarkCommand::CreateMarkCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_TRUE( _command->Unexecute() );
    EXPECT_EQ( *copyCell, *_cell );
}

// Execute sets mark
TEST_F( MarkCommandTest, ExecuteSetMark )
{
    _command = Sudoku::MarkCommand::CreateMarkCommand( _cell, 1 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_TRUE( _cell->GetMarkContainer()[1] );
}

// Mark cannot be unexecuted before execute
TEST_F( MarkCommandTest, CannotUnexecuteBeforeExecute )
{
    _command = Sudoku::MarkCommand::CreateMarkCommand( _cell, 9 );
    EXPECT_ANY_THROW( _command->Unexecute() );
}

// Mark cannot be unexecuted twice in a row
TEST_F( MarkCommandTest, CannotUnexecuteTwiceInARow )
{
    _command = Sudoku::MarkCommand::CreateMarkCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_TRUE( _command->Unexecute() );
    EXPECT_ANY_THROW( _command->Unexecute() );
}

// Mark can Execute then unexecute, then execute
TEST_F( MarkCommandTest, CanExecuteUnexecuteExecute )
{
    _command = Sudoku::MarkCommand::CreateMarkCommand( _cell, 3 );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_TRUE( _command->Unexecute() );
    EXPECT_TRUE( _command->Execute() );
    EXPECT_TRUE( _cell->GetMarkContainer()[3] );
}

}  // namespace
