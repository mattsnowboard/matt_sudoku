#include "../Puzzle.h"
#include "gtest/gtest.h"

#include <algorithm>

namespace {

class PuzzleTest : public ::testing::Test
{
protected:

    PuzzleTest()
    {
    }

    virtual ~PuzzleTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
    
    /**
     * Do some "random" modifications to a Puzzle
     */
    void Modify( Sudoku::Puzzle &p )
    {
        for ( size_t x = 1; x < 10; x++ )
        {
            for ( size_t y = 1; y < 10; y++ )
            {
                std::shared_ptr<Sudoku::Cell> c = p.GetCell( x, y );
                c->SetGuess( x );
                c->SetCorrect( y );
                c->Display( x == y );
                c->Mark( (x + y) % 9 + 1 );
            }
        }
    }
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
        Sudoku::Puzzle::Container::iterator it = row.begin();
        for ( size_t c = 0; c < row.size(); c++, ++it )
        {
            EXPECT_EQ( c + 1, (*it)->GetX() );
            EXPECT_EQ( r, (*it)->GetY() );
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
        Sudoku::Puzzle::Container::iterator it = col.begin();
        for ( size_t r = 0; r < col.size(); r++, ++it )
        {
            EXPECT_EQ( c, (*it)->GetX() );
            EXPECT_EQ( r + 1, (*it)->GetY() );
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
            Sudoku::Puzzle::Container::iterator it = block.begin();
            // now test the positions
            for ( size_t i = 0; i < 9; i++, ++it )
            {
                EXPECT_EQ( (x - 1) * 3 + 1 + (i % 3),
                           (*it)->GetX() )
                    << "x=" << x << ", y=" << y << ", i=" << i;
                EXPECT_EQ( (y - 1) * 3 + 1 + (i / 3),
                           (*it)->GetY() )
                    << "x=" << x << ", y=" << y << ", i=" << i;
            }
        }
    }
}

// Checks that getting blocks by Cell is equivalent to by coordinate
TEST_F( PuzzleTest, GetBlockByCellSameAsByCoord )
{
    Sudoku::Puzzle p;

    for ( size_t x = 1; x <= 3; x++ )
    {
        for ( size_t y = 1; y <= 3; y++ )
        {
            Sudoku::Puzzle::Container blockByCoord = p.GetBlock( x, y );
            Sudoku::Puzzle::Container::iterator it = blockByCoord.begin();
            // now test the positions
            for ( ; it != blockByCoord.end(); ++it )
            {
                Sudoku::Puzzle::Container blockByCell = p.GetBlock( *it );
                EXPECT_TRUE( std::equal( blockByCoord.begin(),
                                         blockByCoord.end(),
                                         blockByCell.begin() ) );
            }
        }
    }
}

// Get neighbors returns the union of the block, row, and column of a cell
TEST_F( PuzzleTest, NeighborSizeIsCorrect )
{
    Sudoku::Puzzle p;
    for ( size_t x = 1; x < 10; x++ )
    {
        for ( size_t y = 1; y < 10; y++ )
        {
            EXPECT_EQ( 21u, p.GetNeighbors( p.GetCell( x, y ) ).size() );
        }
    }
    
}

// Get Neighbors checks that Cell has a proper position
TEST_F( PuzzleTest, GetNeighborsThrowsIfBadPosition )
{
    Sudoku::Puzzle p;
    std::shared_ptr<Sudoku::Cell> c( new Sudoku::Cell );
    EXPECT_ANY_THROW( p.GetNeighbors( c ) );
}

// Get Neighbors checks that Cell has a proper position
TEST_F( PuzzleTest, GetNeighborsThrowsIfBadPositionXY )
{
    Sudoku::Puzzle p;
    EXPECT_ANY_THROW( p.GetNeighbors( 0, 1 ) );
    EXPECT_ANY_THROW( p.GetNeighbors( 1, 0 ) );
    EXPECT_ANY_THROW( p.GetNeighbors( 10, 1 ) );
    EXPECT_ANY_THROW( p.GetNeighbors( 1, 10 ) );
}

// Get Neighbors returns set union of all other sectors
TEST_F( PuzzleTest, GetNeighborsContainsRowColBlock )
{
    Sudoku::Puzzle p;
    for ( size_t x = 1; x < 10; x++ )
    {
        for ( size_t y = 1; y < 10; y++ )
        {
            std::shared_ptr<Sudoku::Cell> c = p.GetCell( x, y );
            Sudoku::Puzzle::Container N = p.GetNeighbors( c );
            Sudoku::Puzzle::Container row = p.GetRow( y );
            Sudoku::Puzzle::Container col = p.GetCol( x );
            Sudoku::Puzzle::Container block = p.GetBlock( (x - 1) / 3 + 1,
                                                          (y - 1) / 3 + 1 );
            EXPECT_TRUE( std::includes( N.begin(), N.end(),
                                        row.begin(), row.end(),
                                        Sudoku::CellSorter() ) );
            EXPECT_TRUE( std::includes( N.begin(), N.end(),
                                        col.begin(), col.end(),
                                        Sudoku::CellSorter() ) );
            EXPECT_TRUE( std::includes( N.begin(), N.end(),
                                        block.begin(), block.end(),
                                        Sudoku::CellSorter() ) );
        }
    }
}

// Get Neighbors returns same Cells for both types of parameters
TEST_F( PuzzleTest, GetNeighborsByCoordinatesIsSameAsByCell )
{
    Sudoku::Puzzle p;
    for ( size_t x = 1; x < 10; x++ )
    {
        for ( size_t y = 1; y < 10; y++ )
        {
            std::shared_ptr<Sudoku::Cell> c = p.GetCell( x, y );
            Sudoku::Puzzle::Container N1 = p.GetNeighbors( c );
            Sudoku::Puzzle::Container N2 = p.GetNeighbors( x, y );
            ASSERT_EQ( N1.size(), N2.size() );
            for ( Sudoku::Puzzle::Container::iterator it1 = N1.begin(),
                      it2 = N2.begin();
                  it1 != N1.end();
                  ++it1, ++it2 )
            {
                EXPECT_EQ( *it1, *it2 );
            }
        }
    }
}

// Check that get all returns all Cells
TEST_F( PuzzleTest, GetAllSizeIsCorrect )
{
    Sudoku::Puzzle p;
    Sudoku::Puzzle::Container all = p.GetAllCells();
    EXPECT_EQ( 81u, all.size() );
}

// Check that operator == is working on default Puzzles
TEST_F( PuzzleTest, EqualityComparisonForDefaultConstructor )
{
    Sudoku::Puzzle a, b;
    EXPECT_EQ( a, b );
}

// Check that operator == is working on modified Puzzles
TEST_F( PuzzleTest, EqualityComparisonForModifiedPuzzles )
{
    Sudoku::Puzzle a, b;
    Modify( a );
    Modify( b );
    EXPECT_EQ( a, b );
}

// Check that operator == fails if just one Cell changes
TEST_F( PuzzleTest, EqualityComparisonFailsIfOneCellChanged )
{
    Sudoku::Puzzle a, b;
    Modify( a );
    Modify( b );
    b.GetCell( 5, 5 )->SetGuess( 0 );
    EXPECT_FALSE( a == b );
}

// Check that copy constructor works
TEST_F( PuzzleTest, CopyConsturctorReturnsExactCopy )
{
    Sudoku::Puzzle original;
    Modify( original );
    Sudoku::Puzzle copy( original );
    EXPECT_EQ( original, copy );
}

// Check that assignment operator works
TEST_F( PuzzleTest, AssignmentOperatorReturnsExactCopy )
{
    Sudoku::Puzzle original, copy;
    Modify( original );
    EXPECT_FALSE( original == copy );
    copy = original;
    EXPECT_EQ( original, copy );
}

}  // namespace
