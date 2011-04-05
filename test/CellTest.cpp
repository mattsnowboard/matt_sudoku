#include "../Cell.h"
#include "gtest/gtest.h"

namespace {

class CellTest : public ::testing::Test
{
protected:

    CellTest()
    {
    }

    virtual ~CellTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    
};

// Tests that the default cell (with 0 values) will not report correct
TEST_F( CellTest, NotCorrectByDefault )
{
    Sudoku::Cell c;
    EXPECT_FALSE( c.IsCorrect() );
}

// Cells should display just a zero by default
TEST_F( CellTest, DisplaysZeroByDefault )
{
    Sudoku::Cell c;
    EXPECT_EQ( 0, c.DisplayedValue() );
}

// Cell Marks should have 10 places, allows indexing on actual value (ignore 0)
// we will rely on that constant in other code
TEST_F( CellTest, MarksHasSize10 )
{
    Sudoku::Cell c;
    ASSERT_EQ( 10u, c.GetMarkContainer().size() );
}

// Cells should have no marks by default
TEST_F( CellTest, HasNoMarksByDefault )
{
    Sudoku::Cell c;
    EXPECT_TRUE( c.GetMarkContainer().none() );
    EXPECT_TRUE( c.GetMarkedValues().empty() );
}

// Exception if value out of range
TEST_F( CellTest, ThrowsOnOutOfRangeGuess )
{
    Sudoku::Cell c;
    EXPECT_ANY_THROW( c.SetGuess( 10 ) );
    EXPECT_ANY_THROW( c.SetGuess( -1 ) );
}

TEST_F( CellTest, ThrowsOnOutOfRangeCorrect )
{
    Sudoku::Cell c;
    EXPECT_ANY_THROW( c.SetCorrect( 10 ) );
    EXPECT_ANY_THROW( c.SetCorrect( -1 ) );
}

// No Exception if value in range 0 to 9
TEST_F( CellTest, NoThrowInRangeGuess )
{
    Sudoku::Cell c;
    EXPECT_NO_THROW( c.SetGuess( 0 ) );
    EXPECT_NO_THROW( c.SetGuess( 9 ) );
}

TEST_F( CellTest, NoThrowInRangeCorrect )
{
    Sudoku::Cell c;
    EXPECT_NO_THROW( c.SetCorrect( 0 ) );
    EXPECT_NO_THROW( c.SetCorrect( 9 ) );
}

// Can display correct value if set to display
TEST_F( CellTest, DisplaysCorrectIfDisplayTrue )
{
    Sudoku::Cell c;
    c.SetCorrect( 3 );
    c.SetGuess( 2 );
    c.Display( true );
    EXPECT_EQ( 3, c.DisplayedValue() );
}

// Can display guessed value if set to not display
TEST_F( CellTest, DisplaysGuessIfDisplayFalse )
{
    Sudoku::Cell c;
    c.SetCorrect( 3 );
    c.SetGuess( 2 );
    c.Display( false );
    EXPECT_EQ( 2, c.DisplayedValue() );
}

// Guess gets changed
TEST_F( CellTest, CanChangeGuess )
{
    Sudoku::Cell c;
    c.SetCorrect( 3 );
    c.SetGuess( 2 );
    c.Display( false );
    EXPECT_EQ( 2, c.DisplayedValue() );
    c.SetGuess( 9 );
    EXPECT_EQ( 9, c.DisplayedValue() );
}

// Correct check works
TEST_F( CellTest, IsCorrectWorks )
{
    Sudoku::Cell c;
    c.SetCorrect( 3 );
    c.SetGuess( 2 );
    EXPECT_FALSE( c.IsCorrect() );
    c.SetGuess( 3 );
    EXPECT_TRUE( c.IsCorrect() );
    c.SetCorrect( 9 );
    EXPECT_FALSE( c.IsCorrect() );
    c.SetGuess( 9 );
    EXPECT_TRUE( c.IsCorrect() );
}

// Check if can guess works
TEST_F( CellTest, CorrectlyReportsCanGuess )
{
    Sudoku::Cell c;
    c.Display( true );
    EXPECT_FALSE( c.CanGuess() );
    c.Display( false );
    EXPECT_TRUE( c.CanGuess() );
}

// Can set Marks
TEST_F( CellTest, CanSetMarks )
{
    Sudoku::Cell c;
    c.Mark( 1 );
    c.Mark( 5 );
    c.Mark( 9 );

    EXPECT_TRUE( c.GetMarkContainer()[1] );
    EXPECT_FALSE( c.GetMarkContainer()[2] );
    EXPECT_FALSE( c.GetMarkContainer()[3] );
    EXPECT_FALSE( c.GetMarkContainer()[4] );
    EXPECT_TRUE( c.GetMarkContainer()[5] );
    EXPECT_FALSE( c.GetMarkContainer()[6] );
    EXPECT_FALSE( c.GetMarkContainer()[7] );
    EXPECT_FALSE( c.GetMarkContainer()[8] );
    EXPECT_TRUE( c.GetMarkContainer()[9] );

    // check other container
    Sudoku::Cell::MarkedValues marks = c.GetMarkedValues();
    EXPECT_TRUE( marks.find( 1 ) != marks.end() );
    EXPECT_TRUE( marks.find( 5 ) != marks.end() );
    EXPECT_TRUE( marks.find( 9 ) != marks.end() );
    EXPECT_EQ( 3u, marks.size() );
}

// Can toggle Marks
TEST_F( CellTest, CanToggleMarks )
{
    Sudoku::Cell c;
    c.Mark( 3 );

    EXPECT_TRUE( c.GetMarkContainer()[3] );
    Sudoku::Cell::MarkedValues marks = c.GetMarkedValues();
    EXPECT_TRUE( marks.find( 3 ) != marks.end() );
    EXPECT_EQ( 1u, marks.size() );

    c.Unmark( 3 );

    EXPECT_FALSE( c.GetMarkContainer()[3] );
    marks = c.GetMarkedValues();
    EXPECT_EQ( 0u, marks.size() );

    c.Mark( 3 );

    EXPECT_TRUE( c.GetMarkContainer()[3] );
    marks = c.GetMarkedValues();
    EXPECT_TRUE( marks.find( 3 ) != marks.end() );
    EXPECT_EQ( 1u, marks.size() );
}

// Multiple marks on same spot does not unmark
TEST_F( CellTest, CanMarkRepeatedly )
{
    Sudoku::Cell c;
    c.Mark( 3 );

    EXPECT_TRUE( c.GetMarkContainer()[3] );
    Sudoku::Cell::MarkedValues marks = c.GetMarkedValues();
    EXPECT_TRUE( marks.find( 3 ) != marks.end() );
    EXPECT_EQ( 1u, marks.size() );

    c.Mark( 3 );

    EXPECT_TRUE( c.GetMarkContainer()[3] );
    marks = c.GetMarkedValues();
    EXPECT_TRUE( marks.find( 3 ) != marks.end() );
    EXPECT_EQ( 1u, marks.size() );
}

// Can clear all marks
TEST_F( CellTest, CanClearAllMarks )
{
    Sudoku::Cell c;
    c.Mark( 4 );
    c.Mark( 5 );
    c.Mark( 6 );
    c.ClearMarks();
    EXPECT_TRUE( c.GetMarkContainer().none() );
    EXPECT_TRUE( c.GetMarkedValues().empty() );
}

// Can set all marks
TEST_F( CellTest, CanSetAllMarks )
{
    Sudoku::Cell c;
    c.Mark( 4 );
    c.Mark( 5 );
    c.Mark( 6 );
    c.MarkAll();
    EXPECT_TRUE( c.GetMarkContainer()[1] );
    EXPECT_TRUE( c.GetMarkContainer()[2] );
    EXPECT_TRUE( c.GetMarkContainer()[3] );
    EXPECT_TRUE( c.GetMarkContainer()[4] );
    EXPECT_TRUE( c.GetMarkContainer()[5] );
    EXPECT_TRUE( c.GetMarkContainer()[6] );
    EXPECT_TRUE( c.GetMarkContainer()[7] );
    EXPECT_TRUE( c.GetMarkContainer()[8] );
    EXPECT_TRUE( c.GetMarkContainer()[9] );
    EXPECT_EQ( 9u, c.GetMarkedValues().size() );
}

// Setting board position
TEST_F( CellTest, CanSetPos )
{
    Sudoku::Cell c;
    c.SetPos( 1, 2 );
    EXPECT_EQ( 1u, c.GetX() );
    EXPECT_EQ( 2u, c.GetY() );
}

// Exception if position out of range
TEST_F( CellTest, ThrowsOnOutOfRangePos )
{
    Sudoku::Cell c;
    EXPECT_ANY_THROW( c.SetPos( 1, -1 ) );
    EXPECT_ANY_THROW( c.SetPos( -1, -1 ) );
    EXPECT_ANY_THROW( c.SetPos( 0, 1 ) );
    EXPECT_ANY_THROW( c.SetPos( 10, 1 ) );
    EXPECT_ANY_THROW( c.SetPos( 1, 10 ) );
}

// No Exception if position in range
TEST_F( CellTest, NoThrowOnInRangePos )
{
    Sudoku::Cell c;
    EXPECT_NO_THROW( c.SetPos( 1, 9 ) );
    EXPECT_NO_THROW( c.SetPos( 9, 1 ) );
}

// Make sure operator== is working
TEST_F( CellTest, EqualityComparisonIsCorrect )
{
    Sudoku::Cell a, b;
    a.SetPos( 5, 6 );
    b.SetPos( 5, 6 );
    a.SetCorrect( 3 );
    b.SetCorrect( 3 );
    a.SetGuess( 3 );
    b.SetGuess( 3 );
    a.Display( true );
    b.Display( true );
    a.Mark( 3 );
    b.Mark( 3 );
    EXPECT_EQ( a, b );
}

// Make sure operator== can return false
TEST_F( CellTest, EqualityComparisonCanReturnFalse )
{
    Sudoku::Cell a, b;
    a.SetPos( 5, 6 );
    b.SetPos( 1, 1 );
    EXPECT_FALSE( a == b );
}

// Make sure copy constructor is working
TEST_F( CellTest, CopyConstructReturnsExactCopy )
{
    Sudoku::Cell original;
    original.SetPos( 3, 4 );
    original.SetCorrect( 5 );
    original.SetGuess( 2 );
    original.Display( true );
    original.Mark( 2 );
    original.Mark( 7 );
    Sudoku::Cell copy( original );
    EXPECT_EQ( original, copy );
}

// Make sure assignment operator is working
TEST_F( CellTest, AssignmentOperatorReturnsExactCopy )
{
    Sudoku::Cell original, copy;
    original.SetPos( 3, 4 );
    original.SetCorrect( 5 );
    original.SetGuess( 2 );
    original.Display( true );
    original.Mark( 2 );
    original.Mark( 7 );
    copy = original;
    EXPECT_EQ( original, copy );
}

}  // namespace
