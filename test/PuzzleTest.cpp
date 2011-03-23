#include "../Puzzle.h"
#include "gtest/gtest.h"

namespace {

// The fixture for testing class Foo.
class PuzzleTest : public ::testing::Test
{
protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    PuzzleTest()
    {
        // You can do set-up work for each test here.
    }

    virtual ~PuzzleTest()
    {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp()
    {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown()
    {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Foo.
};

// Get Cell returns correctly positioned cells
TEST_F( PuzzleTest, GetCellPositionsCorrect )
{
    Sudoku::Puzzle p;
    for ( size_t x = 1; x < 10; x++ )
    {
        for ( size_t y = 1; y < 10; y++ )
        {
            EXPECT_EQ( x, p.GetCell( x, y )->GetX() );
            EXPECT_EQ( y, p.GetCell( x, y )->GetY() );
        }
    }
}

// Get Cell throws when out of range
TEST_F( PuzzleTest, GetCellThrowsOutOfRange )
{
    Sudoku::Puzzle p;
    EXPECT_ANY_THROW( p.GetCell( 0, 0 ) );
    EXPECT_ANY_THROW( p.GetCell( 0, 1 ) );
    EXPECT_ANY_THROW( p.GetCell( 1, 0 ) );
    EXPECT_ANY_THROW( p.GetCell( 10, 10 ) );
    EXPECT_ANY_THROW( p.GetCell( 10, 1 ) );
    EXPECT_ANY_THROW( p.GetCell( 1, 10 ) );
}

// Get Cell does not throw when in range
TEST_F( PuzzleTest, GetCellNoThrowInRange )
{
    Sudoku::Puzzle p;
    EXPECT_NO_THROW( p.GetCell( 1, 1 ) );
    EXPECT_NO_THROW( p.GetCell( 9, 9 ) );
}

// Checks that the rows returned are size 9
TEST_F( PuzzleTest, RowSizeIsNine )
{
    Sudoku::Puzzle p;
    for ( size_t r = 1; r < 10; r++ )
    {
        Sudoku::Puzzle::Container row = p.GetRow( r );
        EXPECT_EQ( 9u, row.size() );
    }
}

// Don't try to access out of range rows
TEST_F( PuzzleTest, ThrowWhenRowOutOfRange )
{
    Sudoku::Puzzle p;
    EXPECT_ANY_THROW( p.GetRow( 0 ) );
    EXPECT_ANY_THROW( p.GetRow( 10 ) );
}

// Checks the positions of Cells returned by GetRow
TEST_F( PuzzleTest, GetRowReturnsCorrectCells )
{
    Sudoku::Puzzle p;
    for ( size_t r = 1; r < 10; r++ )
    {
        Sudoku::Puzzle::Container row = p.GetRow( r );
        for ( size_t c = 0; c < row.size(); c++ )
        {
            EXPECT_EQ( c + 1, row[c]->GetX() );
            EXPECT_EQ( r, row[c]->GetY() );
        }
    }
}

// Checks that the columns returned are size 9
TEST_F( PuzzleTest, ColSizeIsNine )
{
    Sudoku::Puzzle p;
    for ( size_t c = 1; c < 10; c++ )
    {
        Sudoku::Puzzle::Container col = p.GetCol( c );
        EXPECT_EQ( 9u, col.size() );
    }
}

// Don't try to access out of range cols
TEST_F( PuzzleTest, ThrowWhenColOutOfRange )
{
    Sudoku::Puzzle p;
    EXPECT_ANY_THROW( p.GetCol( 0 ) );
    EXPECT_ANY_THROW( p.GetCol( 10 ) );
}

// Checks the positions of Cells returned by GetCol
TEST_F( PuzzleTest, GetColReturnsCorrectCells )
{
    Sudoku::Puzzle p;
    for ( size_t c = 1; c < 10; c++ )
    {
        Sudoku::Puzzle::Container col = p.GetCol( c );
        for ( size_t r = 0; r < col.size(); r++ )
        {
            EXPECT_EQ( c, col[r]->GetX() );
            EXPECT_EQ( r + 1, col[r]->GetY() );
        }
    }
}

// Checks that the blocks returned are size 9
TEST_F( PuzzleTest, BlockSizeIsNine )
{
    Sudoku::Puzzle p;
    for ( size_t x = 1; x <= 3; x++ )
    {
        for ( size_t y = 1; y <= 3; y++ )
        {
            Sudoku::Puzzle::Container block = p.GetBlock( x, y );
            EXPECT_EQ( 9u, block.size() );
        }
    }
}

// Don't try to access out of range blocks
TEST_F( PuzzleTest, ThrowWhenBlockOutOfRange )
{
    Sudoku::Puzzle p;
    EXPECT_ANY_THROW( p.GetBlock( 0, 1 ) );
    EXPECT_ANY_THROW( p.GetBlock( 1, 0 ) );
    EXPECT_ANY_THROW( p.GetBlock( 0, 0 ) );
    EXPECT_ANY_THROW( p.GetBlock( 4, 1 ) );
    EXPECT_ANY_THROW( p.GetBlock( 1, 4 ) );
    EXPECT_ANY_THROW( p.GetBlock( 4, 4 ) );
}

// Checks the positions of Cells returned by GetBlock
TEST_F( PuzzleTest, GetBlockReturnsCorrectCells )
{
    Sudoku::Puzzle p;

    for ( size_t x = 1; x <= 3; x++ )
    {
        for ( size_t y = 1; y <= 3; y++ )
        {
            Sudoku::Puzzle::Container block = p.GetBlock( x, y );
            // now test the positions
            for ( size_t i = 0; i < 9; i++ )
            {
                EXPECT_EQ( (x - 1) * 3 + 1 + (i % 3),
                           block[i]->GetX() )
                    << "x=" << x << ", y=" << y << ", i=" << i;
                EXPECT_EQ( (y - 1) * 3 + 1 + (i / 3),
                           block[i]->GetY() )
                    << "x=" << x << ", y=" << y << ", i=" << i;
            }
        }
    }
}


}  // namespace
