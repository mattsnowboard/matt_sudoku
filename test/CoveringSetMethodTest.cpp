#include "../CoveringSetMethod.h"
#include "gtest/gtest.h"

namespace {

class CoveringSetMethodTest : public ::testing::Test
{
protected:

    CoveringSetMethodTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
    }

    virtual ~CoveringSetMethodTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    std::shared_ptr<Sudoku::Puzzle> _puzzle;
};

// Forward invalid if subset empty
TEST_F( CoveringSetMethodTest, ForwardInvalidEmptySubset )
{
    Sudoku::Puzzle::Container row = _puzzle->GetRow( 2 );
    Sudoku::Puzzle::Container sub;
    Sudoku::CoveringSetMethod csm( row, sub );
    EXPECT_FALSE( csm.VerifyForwardConditions() );
}

// Forward invalid if subset is not part of sector
TEST_F( CoveringSetMethodTest, ForwardInvalidIfSubsetNotPartOfSector )
{
    Sudoku::Puzzle::Container row = _puzzle->GetRow( 2 );
    Sudoku::Puzzle::Container sub = _puzzle->GetCol( 3 );
    // make subset smaller just because
    sub.erase( sub.begin() );
    Sudoku::CoveringSetMethod csm( row, sub );
    EXPECT_FALSE( csm.VerifyForwardConditions() );
}

// Forward invalid if subset is smaller than its number of marks
TEST_F( CoveringSetMethodTest, ForwardInvalidIfSubsetTooManyMarks )
{
    Sudoku::Puzzle::Container row = _puzzle->GetCol( 5 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = row.begin();
    // add a mark to row so it has one to remove
    (*it)->Mark( 2 );
    // grab two items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        // mark three things for a set of 2
        (*it)->Mark( 2 );
        (*it)->Mark( 4 );
        (*it)->Mark( 5 );
    }
    Sudoku::CoveringSetMethod csm( row, sub );
    EXPECT_FALSE( csm.VerifyForwardConditions() );
}

// Forward invalid if subset is larger than its number of marks
TEST_F( CoveringSetMethodTest, ForwardInvalidIfSubsetTooFewMarks )
{
    Sudoku::Puzzle::Container block = _puzzle->GetBlock( 1, 1 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = block.begin();
    // add a mark to block so it has one to remove
    (*it)->Mark( 2 );
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        // mark 2 things for a set of 3
        (*it)->Mark( 2 );
        (*it)->Mark( 5 );
    }
    Sudoku::CoveringSetMethod csm( block, sub );
    EXPECT_FALSE( csm.VerifyForwardConditions() );
}

// Forward invalid if sector\subset does not have the marks
TEST_F( CoveringSetMethodTest, ForwardInvalidIfOutsideSubsetLacksMarks )
{
    Sudoku::Puzzle::Container block = _puzzle->GetBlock( 2, 1 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = block.begin();
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        (*it)->Mark( 2 );
        (*it)->Mark( 5 );
        (*it)->Mark( 7 );
    }
    Sudoku::CoveringSetMethod csm( block, sub );
    EXPECT_FALSE( csm.VerifyForwardConditions() );
}

// Forward valid works
TEST_F( CoveringSetMethodTest, ForwardValidWorks )
{
    Sudoku::Puzzle::Container block = _puzzle->GetBlock( 2, 1 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = block.begin();
    // add a mark to block so it has one to remove
    (*it)->Mark( 2 );
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        (*it)->Mark( 2 );
        (*it)->Mark( 5 );
        (*it)->Mark( 7 );
    }
    Sudoku::CoveringSetMethod csm( block, sub );
    EXPECT_TRUE( csm.VerifyForwardConditions() );
}

// Execute forward removes marks found in subset from sector\subset
TEST_F( CoveringSetMethodTest, ExecuteForwardRemovesMarks )
{
    Sudoku::Puzzle::Container col = _puzzle->GetCol( 2 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = col.begin();
    // lets add this mark to everything
    for ( ; it != col.end(); ++it )
    {
        (*it)->Mark( 4 );
        // this mark is common only to the non subset
        (*it)->Mark( 1 );
        (*it)->Mark( 2 );
    }
    it = col.begin();
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        (*it)->Mark( 4 );
        (*it)->Mark( 5 );
        (*it)->Mark( 7 );
        (*it)->Unmark( 1 );
        (*it)->Unmark( 2 );
    }
    Sudoku::CoveringSetMethod csm( col, sub );
    EXPECT_TRUE( csm.VerifyForwardConditions() );
    csm.ExecuteForward();
    Sudoku::Puzzle::Container diff;
    std::set_difference( col.begin(), col.end(),
                         sub.begin(), sub.end(),
                         std::inserter( diff, diff.begin() ),
                         col.value_comp() );
    for ( it = diff.begin(); it != diff.end(); ++it )
    {
        EXPECT_FALSE( (*it)->GetMarkContainer()[4] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[5] );
        EXPECT_FALSE( (*it)->GetMarkContainer()[7] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[1] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[2] );
    }
}

// Execute forward does not affect the cells with correct values
TEST_F( CoveringSetMethodTest, ExecuteForwardDoesNotTouchCorrect )
{
    Sudoku::Puzzle::Container col = _puzzle->GetCol( 2 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = col.begin();
    // lets add this mark to everything
    for ( ; it != col.end(); ++it )
    {
        (*it)->Mark( 4 );
        // this mark is common only to the non subset
        (*it)->Mark( 1 );
        (*it)->Mark( 2 );
    }
    // make the first item correct
    it = col.begin();
    (*it)->Display( true );
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        (*it)->Mark( 4 );
        (*it)->Mark( 5 );
        (*it)->Mark( 7 );
        (*it)->Unmark( 1 );
        (*it)->Unmark( 2 );
    }
    Sudoku::CoveringSetMethod csm( col, sub );
    EXPECT_TRUE( csm.VerifyForwardConditions() );
    csm.ExecuteForward();
    Sudoku::Puzzle::Container diff;
    std::set_difference( col.begin(), col.end(),
                         sub.begin(), sub.end(),
                         std::inserter( diff, diff.begin() ),
                         col.value_comp() );
    for ( it = diff.begin(); it != diff.end(); ++it )
    {
        if ( (*it)->CanGuess() )
        {
            EXPECT_FALSE( (*it)->GetMarkContainer()[4] );
            EXPECT_FALSE( (*it)->GetMarkContainer()[5] );
            EXPECT_FALSE( (*it)->GetMarkContainer()[7] );
        }
        else
        {
            EXPECT_TRUE( (*it)->GetMarkContainer()[4] );
        }
        EXPECT_TRUE( (*it)->GetMarkContainer()[1] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[2] );
    }
}

////////////////// REVERSE

// Reverse invalid if subset empty
TEST_F( CoveringSetMethodTest, ReverseInvalidEmptySubset )
{
    Sudoku::Puzzle::Container row = _puzzle->GetRow( 2 );
    Sudoku::Puzzle::Container sub;
    Sudoku::CoveringSetMethod csm( row, sub );
    EXPECT_FALSE( csm.VerifyReverseConditions() );
}

// Reverse invalid if subset is not part of sector
TEST_F( CoveringSetMethodTest, ReverseInvalidIfSubsetNotPartOfSector )
{
    Sudoku::Puzzle::Container row = _puzzle->GetRow( 4 );
    Sudoku::Puzzle::Container sub = _puzzle->GetCol( 8 );
    // make subset smaller just because
    sub.erase( sub.begin() );
    Sudoku::CoveringSetMethod csm( row, sub );
    EXPECT_FALSE( csm.VerifyReverseConditions() );
}

// Reverse invalid if subset is smaller than its number of marks
TEST_F( CoveringSetMethodTest, ReverseInvalidIfSubsetTooManyMarks )
{
    Sudoku::Puzzle::Container row = _puzzle->GetCol( 1 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = row.begin();
    // add a mark to row so it has one to remove
    (*it)->Mark( 4 );
    // grab two items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        // mark three things for a set of 2
        (*it)->Mark( 1 );
        (*it)->Mark( 4 );
        (*it)->Mark( 5 );
    }
    Sudoku::CoveringSetMethod csm( row, sub );
    EXPECT_FALSE( csm.VerifyReverseConditions() );
}

// Reverse invalid if subset is larger than its number of marks
TEST_F( CoveringSetMethodTest, ReverseInvalidIfSubsetTooFewMarks )
{
    Sudoku::Puzzle::Container block = _puzzle->GetBlock( 3, 3 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = block.begin();
    // add a mark to block so it has one to remove
    (*it)->Mark( 2 );
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        // mark 2 things for a set of 3
        (*it)->Mark( 2 );
        (*it)->Mark( 5 );
    }
    Sudoku::CoveringSetMethod csm( block, sub );
    EXPECT_FALSE( csm.VerifyReverseConditions() );
}

// Reverse invalid if sector\subset has marks from subset
TEST_F( CoveringSetMethodTest, ReverseInvalidIfOutsideSubsetHasMarks )
{
    Sudoku::Puzzle::Container block = _puzzle->GetBlock( 2, 1 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = block.begin();
    // mark all even outside subset
    for ( ; it != block.end(); ++it )
    {
        (*it)->Mark( 2 );
        (*it)->Mark( 5 );
        (*it)->Mark( 7 );
    }
    it = block.begin();
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );

    Sudoku::CoveringSetMethod csm( block, sub );
    EXPECT_FALSE( csm.VerifyReverseConditions() );
}

// Reverse valid works
TEST_F( CoveringSetMethodTest, ReverseValidWorks )
{
    Sudoku::Puzzle::Container block = _puzzle->GetBlock( 2, 1 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = block.begin();
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        (*it)->Mark( 2 );
        (*it)->Mark( 5 );
        (*it)->Mark( 7 );
    }
    Sudoku::CoveringSetMethod csm( block, sub );
    EXPECT_TRUE( csm.VerifyReverseConditions() );
}

// Execute reverse adds marks found in subset to sector\subset
TEST_F( CoveringSetMethodTest, ExecuteReverseAddsMarks )
{
    Sudoku::Puzzle::Container col = _puzzle->GetCol( 2 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = col.begin();
    // lets add this mark to everything
    for ( ; it != col.end(); ++it )
    {
        // this mark is common only to the non subset
        (*it)->Mark( 1 );
        (*it)->Mark( 2 );
    }
    it = col.begin();
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        (*it)->Mark( 4 );
        (*it)->Mark( 5 );
        (*it)->Mark( 7 );
        (*it)->Unmark( 1 );
        (*it)->Unmark( 2 );
    }
    Sudoku::CoveringSetMethod csm( col, sub );
    EXPECT_TRUE( csm.VerifyReverseConditions() );
    csm.ExecuteReverse();
    Sudoku::Puzzle::Container diff;
    std::set_difference( col.begin(), col.end(),
                         sub.begin(), sub.end(),
                         std::inserter( diff, diff.begin() ),
                         col.value_comp() );
    for ( it = diff.begin(); it != diff.end(); ++it )
    {
        EXPECT_TRUE( (*it)->GetMarkContainer()[4] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[5] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[7] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[1] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[2] );
    }
}

// Execute reverse does not affect the cells with correct values
TEST_F( CoveringSetMethodTest, ExecuteReverseDoesNotTouchCorrect )
{
    Sudoku::Puzzle::Container col = _puzzle->GetCol( 2 );
    Sudoku::Puzzle::Container sub;
    Sudoku::Puzzle::Container::iterator it = col.begin();
    for ( ; it != col.end(); ++it )
    {
        // this mark is common only to the non subset
        (*it)->Mark( 1 );
        (*it)->Mark( 2 );
    }
    // make the first item correct
    it = col.begin();
    (*it)->Display( true );
    // grab three items
    it++;
    sub.insert( *it );
    it++; it++;
    sub.insert( *it );
    it++;
    sub.insert( *it );
    for ( it = sub.begin(); it != sub.end(); ++it )
    {
        (*it)->Mark( 4 );
        (*it)->Mark( 5 );
        (*it)->Mark( 7 );
        (*it)->Unmark( 1 );
        (*it)->Unmark( 2 );
    }
    Sudoku::CoveringSetMethod csm( col, sub );
    EXPECT_TRUE( csm.VerifyReverseConditions() );
    csm.ExecuteReverse();
    Sudoku::Puzzle::Container diff;
    std::set_difference( col.begin(), col.end(),
                         sub.begin(), sub.end(),
                         std::inserter( diff, diff.begin() ),
                         col.value_comp() );
    for ( it = diff.begin(); it != diff.end(); ++it )
    {
        if ( (*it)->CanGuess() )
        {
            EXPECT_TRUE( (*it)->GetMarkContainer()[4] );
            EXPECT_TRUE( (*it)->GetMarkContainer()[5] );
            EXPECT_TRUE( (*it)->GetMarkContainer()[7] );
        }
        else
        {
            EXPECT_FALSE( (*it)->GetMarkContainer()[4] );
            EXPECT_FALSE( (*it)->GetMarkContainer()[5] );
            EXPECT_FALSE( (*it)->GetMarkContainer()[7] );
        }
        EXPECT_TRUE( (*it)->GetMarkContainer()[1] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[2] );
    }
}


}  // namespace
