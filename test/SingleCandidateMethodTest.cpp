
#include "../SingleCandidateMethod.h"
#include "../Cell.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class SingleCandidateMethod.
class SingleCandidateMethodTest : public ::testing::Test
{
protected:

    SingleCandidateMethodTest()
    {
    }

    virtual ~SingleCandidateMethodTest()
    {
    }

    virtual void SetUp()
    {
        _cell.reset( new Sudoku::Cell );
    }

    virtual void TearDown()
    {
    }

    std::shared_ptr<Sudoku::Cell> _cell;
};

// Operation is invalid if no marks are set
TEST_F( SingleCandidateMethodTest, ForwardInvalidForNoMarks )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    EXPECT_FALSE( scm.VerifyForwardConditions() );
}

// Operation is invalid if multiple marks are set
TEST_F( SingleCandidateMethodTest, ForwardInvalidForMultipleMarks )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->Mark( 1 );
    _cell->Mark( 2 );
    EXPECT_FALSE( scm.VerifyForwardConditions() );
}

// Operation is invalid if the correct value is displayed
TEST_F( SingleCandidateMethodTest, ForwardInvalidIfCorrectDisplayed )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->Display( true );
    EXPECT_FALSE( scm.VerifyForwardConditions() );
}

// Operation is invalid if there is already a guess
TEST_F( SingleCandidateMethodTest, ForwardInvalidIfGuessSet )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->SetGuess( 4 );
    EXPECT_FALSE( scm.VerifyForwardConditions() );
}

// Operation is valid works
TEST_F( SingleCandidateMethodTest, ForwardValidWorks )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->Mark( 6 );
    EXPECT_TRUE( scm.VerifyForwardConditions() );
}

// Forward operation removes mark and sets guess
TEST_F( SingleCandidateMethodTest, ForwardExecuteWorks )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->Mark( 8 );
    scm.ExecuteForward();
    EXPECT_EQ( 8, _cell->DisplayedValue() );
    EXPECT_TRUE( _cell->GetMarkedValues().empty() );
}

// Forward operation throws if there is no mark to set
TEST_F( SingleCandidateMethodTest, ForwardThrowsIfNoMarks )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    EXPECT_ANY_THROW( scm.ExecuteForward() );
}

// After forward operation, reverse operation is valid
TEST_F( SingleCandidateMethodTest, ForwardExecuteValidatesReverse )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->Mark( 1 );
    scm.ExecuteForward();
    EXPECT_TRUE( scm.VerifyReverseConditions() );
}

// Reverse Operation is invalid if no guess is set
TEST_F( SingleCandidateMethodTest, ReverseInvalidForNoGuess )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    EXPECT_FALSE( scm.VerifyReverseConditions() );
}

// Reverse Operation is invalid if marks are set
TEST_F( SingleCandidateMethodTest, ReverseInvalidForMarksSet )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->Mark( 2 );
    EXPECT_FALSE( scm.VerifyReverseConditions() );
}

// Reverse Operation is invalid if the correct value is displayed
TEST_F( SingleCandidateMethodTest, ReverseInvalidIfCorrectDisplayed )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->Display( true );
    EXPECT_FALSE( scm.VerifyReverseConditions() );
}

// Reverse Operation is valid works
TEST_F( SingleCandidateMethodTest, ReverseValidWorks )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->SetGuess( 2 );
    EXPECT_TRUE( scm.VerifyReverseConditions() );
}

// Reverse operation removes guess and sets mark
TEST_F( SingleCandidateMethodTest, ReverseExecuteWorks )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->SetGuess( 5 );
    scm.ExecuteReverse();
    EXPECT_EQ( 0, _cell->DisplayedValue() );
    EXPECT_EQ( 1u, _cell->GetMarkedValues().size() );
    EXPECT_TRUE( _cell->GetMarkContainer()[ 5 ] );
}

// After Reverse operation, forward operation is valid
TEST_F( SingleCandidateMethodTest, ReverseExecuteValidatesForward )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->SetGuess( 5 );
    scm.ExecuteReverse();
    EXPECT_TRUE( scm.VerifyForwardConditions() );
}

// Forward then reverse the operation returns to original
TEST_F( SingleCandidateMethodTest, ForwardThenReverseIsOriginal )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->Mark( 3 );
    scm.ExecuteForward();
    scm.ExecuteReverse();
    EXPECT_EQ( 0, _cell->DisplayedValue() );
    EXPECT_EQ( 1u, _cell->GetMarkedValues().size() );
    EXPECT_TRUE( _cell->GetMarkContainer()[ 3 ] );
}

// Reverse then forward the operation returns to original
TEST_F( SingleCandidateMethodTest, ReverseThenForwardIsOriginal )
{
    Sudoku::SingleCandidateMethod scm( _cell );
    _cell->SetGuess( 3 );
    scm.ExecuteReverse();
    scm.ExecuteForward();
    EXPECT_EQ( 3, _cell->DisplayedValue() );
    EXPECT_TRUE( _cell->GetMarkedValues().empty() );
}

}  // namespace
