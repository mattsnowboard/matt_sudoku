#include "../BlockIntersectionMethod.h"
#include "gtest/gtest.h"

namespace {

class BlockIntersectionMethodTest : public ::testing::Test
{
protected:
    BlockIntersectionMethodTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
    }

    virtual ~BlockIntersectionMethodTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    Sudoku::Puzzle::Container GetRowMinusBlock(
        std::shared_ptr<Sudoku::Puzzle> puzzle,
        std::shared_ptr<Sudoku::Cell> cell )
    {
        Sudoku::Puzzle::Container row = puzzle->GetRow( cell->GetY() );
        Sudoku::Puzzle::Container block = puzzle->GetBlock( cell );
        Sudoku::Puzzle::Container rowDiff;
        std::set_difference( row.begin(), row.end(),
                             block.begin(), block.end(),
                             std::inserter( rowDiff, rowDiff.begin() ) );
        return rowDiff;
    }

    Sudoku::Puzzle::Container GetColMinusBlock(
        std::shared_ptr<Sudoku::Puzzle> puzzle,
        std::shared_ptr<Sudoku::Cell> cell )
    {
        Sudoku::Puzzle::Container col = puzzle->GetCol( cell->GetX() );
        Sudoku::Puzzle::Container block = puzzle->GetBlock( cell );
        Sudoku::Puzzle::Container colDiff;
        std::set_difference( col.begin(), col.end(),
                             block.begin(), block.end(),
                             std::inserter( colDiff, colDiff.begin() ) );
        return colDiff;
    }

    Sudoku::Puzzle::Container GetBlockMinusRow(
        std::shared_ptr<Sudoku::Puzzle> puzzle,
        std::shared_ptr<Sudoku::Cell> cell )
    {
        Sudoku::Puzzle::Container row = puzzle->GetRow( cell->GetY() );
        Sudoku::Puzzle::Container block = puzzle->GetBlock( cell );
        Sudoku::Puzzle::Container blockDiff;
        std::set_difference( block.begin(), block.end(),
                             row.begin(), row.end(),
                             std::inserter( blockDiff, blockDiff.begin() ) );
        return blockDiff;
    }

    Sudoku::Puzzle::Container GetBlockMinusCol(
        std::shared_ptr<Sudoku::Puzzle> puzzle,
        std::shared_ptr<Sudoku::Cell> cell )
    {
        Sudoku::Puzzle::Container col = puzzle->GetCol( cell->GetX() );
        Sudoku::Puzzle::Container block = puzzle->GetBlock( cell );
        Sudoku::Puzzle::Container blockDiff;
        std::set_difference( block.begin(), block.end(),
                             col.begin(), col.end(),
                             std::inserter( blockDiff, blockDiff.begin() ) );
        return blockDiff;
    }

    std::shared_ptr<Sudoku::Puzzle> _puzzle;
};

// Tests that the method only works if the Cell has the given mark
TEST_F( BlockIntersectionMethodTest, ForwardInvalidIfCellNotMarked )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    Sudoku::BlockIntersectionMethod bim( c, 2, rowDiff, blockDiff );
    EXPECT_FALSE( bim.VerifyForwardConditions() );
}

// Tests that the method won't work if the primary group doesn't have mark
TEST_F( BlockIntersectionMethodTest, ForwardInvalidIfPrimaryNotMarked )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 5;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_FALSE( bim.VerifyForwardConditions() );
}

// Tests that the method won't work if the secondary group has the mark
TEST_F( BlockIntersectionMethodTest, ForwardInvalidIfSecondaryHasMark )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 5;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    (*(rowDiff.begin()))->Mark( mark );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    (*(blockDiff.begin()))->Mark( mark );
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_FALSE( bim.VerifyForwardConditions() );
}

// Tests that the verify forward works
TEST_F( BlockIntersectionMethodTest, ForwardValidWorksRowBlock )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 5;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    (*(rowDiff.begin()))->Mark( mark );
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_TRUE( bim.VerifyForwardConditions() );
}

// Tests that verify works for columsn and blocks
TEST_F( BlockIntersectionMethodTest, ForwardValidWorksBlockCol )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 5;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container colDiff = GetColMinusBlock( _puzzle, c );
    (*(blockDiff.begin()))->Mark( mark );
    Sudoku::BlockIntersectionMethod bim( c, mark, blockDiff, colDiff );
    EXPECT_TRUE( bim.VerifyForwardConditions() );
}

// Test that forward execute will unmark primary group, for block-col primary
TEST_F( BlockIntersectionMethodTest, ForwardUnmarksPrimaryGroupBlockCol )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 8;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusCol( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container colDiff = GetColMinusBlock( _puzzle, c );
    for ( Sudoku::Puzzle::Container::iterator it = blockDiff.begin();
          it != blockDiff.end();
          ++it )
    {
        (*it)->Mark( mark );
    }
    Sudoku::BlockIntersectionMethod bim( c, mark, blockDiff, colDiff );
    EXPECT_TRUE( bim.VerifyForwardConditions() );
    bim.ExecuteForward();
    for ( Sudoku::Puzzle::Container::iterator it = blockDiff.begin();
          it != blockDiff.end();
          ++it )
    {
        EXPECT_FALSE( (*it)->GetMarkContainer()[mark] );
    }
}

// Test that forward execute will unmark primary group, for row-block primary
TEST_F( BlockIntersectionMethodTest, ForwardUnmarksPrimaryGroupRowBlock )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 1 );
    unsigned mark = 8;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    for ( Sudoku::Puzzle::Container::iterator it = rowDiff.begin();
          it != rowDiff.end();
          ++it )
    {
        (*it)->Mark( mark );
    }
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_TRUE( bim.VerifyForwardConditions() );
    bim.ExecuteForward();
    for ( Sudoku::Puzzle::Container::iterator it = rowDiff.begin();
          it != rowDiff.end();
          ++it )
    {
        EXPECT_FALSE( (*it)->GetMarkContainer()[mark] );
    }
}

// Test that forward execute will not affect correctly displayed values
TEST_F( BlockIntersectionMethodTest, ForwardDoesNotUnmarkCorrect )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 9, 3 );
    unsigned mark = 2;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    for ( Sudoku::Puzzle::Container::iterator it = rowDiff.begin();
          it != rowDiff.end();
          ++it )
    {
        (*it)->Mark( mark );
    }
    // this one shouldn't get unmarked
    (*rowDiff.begin())->Display( true );
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_TRUE( bim.VerifyForwardConditions() );
    bim.ExecuteForward();
    for ( Sudoku::Puzzle::Container::iterator it = rowDiff.begin();
          it != rowDiff.end();
          ++it )
    {
        if ( (*it)->CanGuess() )
        {
            EXPECT_FALSE( (*it)->GetMarkContainer()[mark] );
        }
        else
        {
            EXPECT_TRUE( (*it)->GetMarkContainer()[mark] );
        }
    }
}

////////////////////// Reverse

// Tests that the method only works if the Cell has the given mark
TEST_F( BlockIntersectionMethodTest, ReverseInvalidIfCellNotMarked )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    Sudoku::BlockIntersectionMethod bim( c, 2, rowDiff, blockDiff );
    EXPECT_FALSE( bim.VerifyReverseConditions() );
}

// Tests that the method won't work if the primary group doesn't have any mark
TEST_F( BlockIntersectionMethodTest, ReverseInvalidIfPrimaryNotMarked )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 5;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_FALSE( bim.VerifyReverseConditions() );
}

// Tests that the method won't work if the primary group has the mark
TEST_F( BlockIntersectionMethodTest, ReverseInvalidIfPrimaryHasMark )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 4, 3 );
    unsigned mark = 5;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    (*(rowDiff.begin()))->Mark( mark );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_FALSE( bim.VerifyReverseConditions() );
}

// Tests that the method won't work if the secondary group has the mark
TEST_F( BlockIntersectionMethodTest, ReverseInvalidIfSecondaryHasMark )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 5;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    (*(rowDiff.begin()))->Mark( mark + 1 );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    (*(blockDiff.begin()))->Mark( mark );
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_FALSE( bim.VerifyReverseConditions() );
}

// Tests that the verify forward works
TEST_F( BlockIntersectionMethodTest, ReverseValidWorksRowBlock )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 3;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    (*(rowDiff.begin()))->Mark( mark + 1 );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_TRUE( bim.VerifyReverseConditions() );
}

// Tests that verify works for columsn and blocks
TEST_F( BlockIntersectionMethodTest, ReverseValidWorksBlockCol )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 5;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container colDiff = GetColMinusBlock( _puzzle, c );
    (*(blockDiff.begin()))->Mark( mark + 1 );
    Sudoku::BlockIntersectionMethod bim( c, mark, blockDiff, colDiff );
    EXPECT_TRUE( bim.VerifyReverseConditions() );
}

// Test that reverse execute will mark primary group, for block-col primary
TEST_F( BlockIntersectionMethodTest, ReverseMarksPrimaryGroupBlockCol )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 7;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusCol( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container colDiff = GetColMinusBlock( _puzzle, c );
    for ( Sudoku::Puzzle::Container::iterator it = blockDiff.begin();
          it != blockDiff.end();
          ++it )
    {
        // mark something
        (*it)->Mark( mark + 1 );
    }
    Sudoku::BlockIntersectionMethod bim( c, mark, blockDiff, colDiff );
    EXPECT_TRUE( bim.VerifyReverseConditions() );
    bim.ExecuteReverse();
    for ( Sudoku::Puzzle::Container::iterator it = blockDiff.begin();
          it != blockDiff.end();
          ++it )
    {
        EXPECT_TRUE( (*it)->GetMarkContainer()[mark] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[mark + 1] );
    }
}

// Test that reverse execute will mark primary group, for row-block primary
TEST_F( BlockIntersectionMethodTest, ReverseMarksPrimaryGroupRowBlock )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    unsigned mark = 6;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    for ( Sudoku::Puzzle::Container::iterator it = rowDiff.begin();
          it != rowDiff.end();
          ++it )
    {
        (*it)->Mark( mark + 1 );
    }
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_TRUE( bim.VerifyReverseConditions() );
    bim.ExecuteReverse();
    for ( Sudoku::Puzzle::Container::iterator it = rowDiff.begin();
          it != rowDiff.end();
          ++it )
    {
        EXPECT_TRUE( (*it)->GetMarkContainer()[mark] );
        EXPECT_TRUE( (*it)->GetMarkContainer()[mark + 1] );
    }
}

// Test that reverse execute will not affect correctly displayed values
TEST_F( BlockIntersectionMethodTest, ReverseDoesNotMarkCorrect )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 9, 3 );
    unsigned mark = 2;
    c->Mark( mark );
    // primary
    Sudoku::Puzzle::Container rowDiff = GetRowMinusBlock( _puzzle, c );
    // secondary
    Sudoku::Puzzle::Container blockDiff = GetBlockMinusRow( _puzzle, c );
    for ( Sudoku::Puzzle::Container::iterator it = rowDiff.begin();
          it != rowDiff.end();
          ++it )
    {
        (*it)->Mark( mark + 1 );
    }
    // this one shouldn't get unmarked
    (*rowDiff.begin())->Display( true );
    Sudoku::BlockIntersectionMethod bim( c, mark, rowDiff, blockDiff );
    EXPECT_TRUE( bim.VerifyReverseConditions() );
    bim.ExecuteReverse();
    for ( Sudoku::Puzzle::Container::iterator it = rowDiff.begin();
          it != rowDiff.end();
          ++it )
    {
        if ( (*it)->CanGuess() )
        {
            EXPECT_TRUE( (*it)->GetMarkContainer()[mark] );
        }
        else
        {
            EXPECT_FALSE( (*it)->GetMarkContainer()[mark] );
        }
        EXPECT_TRUE( (*it)->GetMarkContainer()[mark + 1] );
    }
}

}  // namespace
