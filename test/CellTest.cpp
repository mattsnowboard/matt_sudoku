#include "../Cell.h"
#include "MockCellObserver.h"
#include "gtest/gtest.h"

namespace {

using ::testing::Ref;

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

    std::shared_ptr<Sudoku::MockCellObserver> observer;
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

// Cannot guess for correctly displayed
TEST_F( CellTest, CannotGuessIfCorrectDisplayed )
{
    Sudoku::Cell c;
    c.SetCorrect( 3 );
    c.Display( true );
    EXPECT_ANY_THROW( c.SetGuess( 2 ) );
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

// Is correct if correct answer displayed
TEST_F( CellTest, IsCorrectIfCorrectIsDisplayed )
{
    Sudoku::Cell c;
    c.SetCorrect( 5 );
    c.Display( true );
    EXPECT_TRUE( c.IsCorrect() );
}

// Not is correct if correct answer displayed but blank
TEST_F( CellTest, NotIsCorrectIfCorrectDisplayedBlank )
{
    Sudoku::Cell c;
    c.Display( true );
    EXPECT_FALSE( c.IsCorrect() );
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

// Tests the setting of Marks with a container
TEST_F( CellTest, CanSetClearSeveralMarks )
{
    Sudoku::Cell c;
    c.Mark( 4 );
    c.Mark( 5 );
    c.Mark( 6 );
    Sudoku::Cell::MarkContainer m;
    // have one in common and some removed and some new
    m.set( 4 );
    m.set( 9 );
    c.SetMarkContainer( m );
    EXPECT_FALSE( c.GetMarkContainer()[1] );
    EXPECT_FALSE( c.GetMarkContainer()[2] );
    EXPECT_FALSE( c.GetMarkContainer()[3] );
    EXPECT_TRUE( c.GetMarkContainer()[4] );
    EXPECT_FALSE( c.GetMarkContainer()[5] );
    EXPECT_FALSE( c.GetMarkContainer()[6] );
    EXPECT_FALSE( c.GetMarkContainer()[7] );
    EXPECT_FALSE( c.GetMarkContainer()[8] );
    EXPECT_TRUE( c.GetMarkContainer()[9] );
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

// test the observer code

TEST_F( CellTest, ThrowIfAddNullObserver )
{
    Sudoku::Cell c;
    EXPECT_ANY_THROW( c.AddObserver( observer ) );
}

TEST_F( CellTest, ThrowIfRemoveNullObserver )
{
    Sudoku::Cell c;
    EXPECT_ANY_THROW( c.RemoveObserver( observer ) );
}

TEST_F( CellTest, ThrowIfRemoveObserverTwice )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );
    c.RemoveObserver( observer );
    EXPECT_ANY_THROW( c.RemoveObserver( observer ) );
}

TEST_F( CellTest, ObserverUpdateOnSetGuess )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.SetGuess( 4 );
}

TEST_F( CellTest, ObserverUpdateOnEachGuess )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 2 );

    c.SetGuess( 4 );
    c.SetGuess( 5 );
}

TEST_F( CellTest, NoObserverUpdateIfUnchangedGuess )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.SetGuess( 4 );
    c.SetGuess( 4 );
}

TEST_F( CellTest, ObserverUpdateOnSetCorrect )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.SetCorrect( 4 );
}

TEST_F( CellTest, ObserverUpdateEachOnSetCorrect )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 2 );

    c.SetCorrect( 4 );
    c.SetCorrect( 9 );
}

TEST_F( CellTest, NoObserverUpdateOnSetCorrectUnchanged )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.SetCorrect( 4 );
    c.SetCorrect( 4 );
}

TEST_F( CellTest, ObserverUpdateOnMark )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.Mark( 4 );
}

TEST_F( CellTest, ObserverUpdateOnEachMark )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 3 );

    c.Mark( 3 );
    c.Mark( 2 );
    c.Mark( 7 );
}

TEST_F( CellTest, NoObserverUpdateIfUnchangedMark )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.Mark( 1 );
    c.Mark( 1 );
}

TEST_F( CellTest, ObserverUpdateOnUnmark )
{
    Sudoku::Cell c;
    c.Mark( 2 );
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.Unmark( 2 );
}

TEST_F( CellTest, NoObserverUpdateOnUnmarkUnchanged )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 0 );

    c.Unmark( 3 );
}

TEST_F( CellTest, SingleObserverUpdateOnMarkAll )
{
    Sudoku::Cell c;
    c.Mark( 3 );
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.MarkAll();
    c.MarkAll();
}

TEST_F( CellTest, NoObserverUpdateOnMarkAllIfMarked )
{
    Sudoku::Cell c;
    c.MarkAll();
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 0 );

    c.MarkAll();
}

TEST_F( CellTest, SingleObserverUpdateOnClearMarks )
{
    Sudoku::Cell c;
    c.Mark( 5 );
    c.Mark( 6 );
    c.Mark( 7 );
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.ClearMarks();
    c.ClearMarks();
}

TEST_F( CellTest, ObserverUpdateOnDisplayChange )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );

    c.Display( true );
}

TEST_F( CellTest, ObserverUpdateOnEachDisplayChange )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 2 );

    c.Display( true );
    c.Display( false );
}

TEST_F( CellTest, NoObserverUpdateOnDisplayUnchanged )
{
    Sudoku::Cell c;
    c.Display( true );
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 0 );

    c.Display( true );
}

// Make sure that we don't attach observers to copies
TEST_F( CellTest, NoObserverUpdateOnCopy )
{
    Sudoku::Cell c;
    observer.reset( new Sudoku::MockCellObserver );
    c.AddObserver( observer );

    Sudoku::Cell copy( c );

    EXPECT_CALL( *observer, Update( Ref( c ) ) )
        .Times( 1 );
    EXPECT_CALL( *observer, Update( Ref( copy ) ) )
        .Times( 0 );

    c.SetGuess( 3 );
    copy.SetGuess( 4 );
}


}  // namespace
