
#include "../ExclusionMethod.h"
#include "../Puzzle.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class ExclusionMethod
class ExclusionMethodTest : public ::testing::Test
{
protected:

    ExclusionMethodTest()
    {
    }

    virtual ~ExclusionMethodTest()
    {
    }

    virtual void SetUp()
    {
        _puzzle.reset( new Sudoku::Puzzle );
        _markedPuzzle.reset( new Sudoku::Puzzle );
        // mark all cells
        for ( size_t x = 1; x < 10; x++ )
        {
            for ( size_t y = 1; y < 10; y++ )
            {
                std::shared_ptr<Sudoku::Cell> c = _markedPuzzle->GetCell( x, y );
                c->MarkAll();
            }
        }
    }

    virtual void TearDown()
    {
    }

    std::shared_ptr<Sudoku::Puzzle> _puzzle;
    std::shared_ptr<Sudoku::Puzzle> _markedPuzzle;
};

// Not valid if cell has no guess
TEST_F( ExclusionMethodTest, ForwardInvalidForNoGuess )
{
    Sudoku::ExclusionMethod em( _puzzle, _puzzle->GetCell( 1, 2 ) );
    EXPECT_FALSE( em.VerifyForwardConditions() );
}

// Not valid if cell displays correct value
TEST_F( ExclusionMethodTest, ForwardInvalidForCorrectDisplayed )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 1, 2 );
    // doesn't matter but,
    c->SetGuess( 4 );
    c->SetCorrect( 5 );
    c->Display( true );
    Sudoku::ExclusionMethod em( _puzzle, c );
    EXPECT_FALSE( em.VerifyForwardConditions() );
}

// show that we can get verify to pass
TEST_F( ExclusionMethodTest, ForwardIsValidWorks )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 1, 2 );
    c->SetGuess( 4 );
    Sudoku::ExclusionMethod em( _puzzle, c );
    EXPECT_TRUE( em.VerifyForwardConditions() );
}

// show that exclusion will remove marks from neighbors only
TEST_F( ExclusionMethodTest, ForwardOperationRemovesNeighborMarks )
{
    std::shared_ptr<Sudoku::Cell> c = _markedPuzzle->GetCell( 1, 2 );
    c->SetGuess( 4 );
    c->ClearMarks();
    Sudoku::Puzzle::Container N = _markedPuzzle->GetNeighbors( c );
    N.erase( c );
    // verify that marks are set
    for ( Sudoku::Puzzle::Container::iterator it = N.begin();
          it != N.end();
          ++it )
    {
        EXPECT_TRUE( (*it)->GetMarkContainer()[1] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[2] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[3] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[4] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[5] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[6] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[7] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[8] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[9] );
    }
    Sudoku::ExclusionMethod em( _markedPuzzle, c );
    em.ExecuteForward();
    for ( Sudoku::Puzzle::Container::iterator it = N.begin();
          it != N.end();
          ++it )
    {
        EXPECT_TRUE( (*it)->GetMarkContainer()[1] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[2] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[3] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[4] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[5] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[6] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[7] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[8] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[9] );
    }
}

// show that exclusion will not touch non neighbors
TEST_F( ExclusionMethodTest, ForwardOperationDoesNotAffectNonNeighbors )
{
    std::shared_ptr<Sudoku::Cell> c = _markedPuzzle->GetCell( 4, 2 );
    c->SetGuess( 7 );
    c->ClearMarks();
    Sudoku::Puzzle::Container All = _markedPuzzle->GetAllCells();
    Sudoku::Puzzle::Container N = _markedPuzzle->GetNeighbors( c );
    Sudoku::Puzzle::Container NotNeighbor;
    std::set_difference( All.begin(), All.end(),
                         N.begin(), N.end(),
                         std::inserter( NotNeighbor, NotNeighbor.begin() ) );
    // verify that marks are set
    for ( Sudoku::Puzzle::Container::iterator it = NotNeighbor.begin();
          it != NotNeighbor.end();
          ++it )
    {
        EXPECT_TRUE( (*it)->GetMarkContainer()[1] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[2] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[3] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[4] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[5] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[6] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[7] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[8] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[9] );
    }
    Sudoku::ExclusionMethod em( _markedPuzzle, c );
    em.ExecuteForward();
    for ( Sudoku::Puzzle::Container::iterator it = NotNeighbor.begin();
          it != NotNeighbor.end();
          ++it )
    {
        EXPECT_TRUE( (*it)->GetMarkContainer()[1] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[2] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[3] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[4] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[5] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[6] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[7] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[8] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[9] );
    }
}

// show that exclusion works even if there are no marks to change
TEST_F( ExclusionMethodTest, ForwardOperationDoesNothingIfNoMarks )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 1, 2 );
    c->SetGuess( 4 );
    Sudoku::Puzzle::Container all = _puzzle->GetAllCells();
    // verify that marks are unset
    for ( Sudoku::Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        EXPECT_FALSE( (*it)->GetMarkContainer()[1] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[2] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[3] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[4] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[5] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[6] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[7] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[8] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[9] );
    }
    Sudoku::ExclusionMethod em( _puzzle, c );
    em.ExecuteForward();
    for ( Sudoku::Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        EXPECT_FALSE( (*it)->GetMarkContainer()[1] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[2] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[3] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[4] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[5] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[6] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[7] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[8] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[9] );
    }
}

// show that a Neighbor with Display(true) AND the mark set will not change
TEST_F( ExclusionMethodTest, DISABLED_ForwardOperationWillNotUnmarkCorrectNeighbors )
{
}

// Not valid if cell has no guess
TEST_F( ExclusionMethodTest, ReverseInvalidForNoGuess )
{
    Sudoku::ExclusionMethod em( _puzzle, _puzzle->GetCell( 1, 2 ) );
    EXPECT_FALSE( em.VerifyReverseConditions() );
}

// Not valid if cell displays correct value
TEST_F( ExclusionMethodTest, ReverseInvalidForCorrectDisplayed )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 1, 2 );
    // doesn't matter but,
    c->SetGuess( 4 );
    c->SetCorrect( 5 );
    c->Display( true );
    Sudoku::ExclusionMethod em( _puzzle, c );
    EXPECT_FALSE( em.VerifyReverseConditions() );
}

// show that we can get verify to pass
TEST_F( ExclusionMethodTest, ReverseIsValidWorks )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 1, 2 );
    c->SetGuess( 4 );
    Sudoku::ExclusionMethod em( _puzzle, c );
    EXPECT_TRUE( em.VerifyReverseConditions() );
}

// show that reverse exclusion will add marks to neighbors only
TEST_F( ExclusionMethodTest, DISABLED_ReverseOperationAddsNeighborMarks )
{
}

// show that exclusion will not touch non neighbors
TEST_F( ExclusionMethodTest, DISABLED_ReverseOperationDoesNotAffectNonNeighbors )
{
}

// show that exclusion works even if there are existing marks for the value
TEST_F( ExclusionMethodTest, DISABLED_ReverseOperationDoesNothingIfAlreadyMarked )
{
}

// show that a Neighbor with Display(true) AND the mark clear will not change
TEST_F( ExclusionMethodTest, DISABLED_ReverseOperationWillNotMarkCorrectNeighbors )
{
}

}  // namespace
