#include "../SolvedPuzzleImporter.h"
#include "../Puzzle.h"
#include "../Cell.h"
#include "gtest/gtest.h"

namespace {

class SolvedPuzzleImporterTest : public ::testing::Test
{
protected:

    SolvedPuzzleImporterTest()
    {
        _importer.reset( new Sudoku::SolvedPuzzleImporter );
    }

    virtual ~SolvedPuzzleImporterTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
    
    // check that _puzzle is as defined in string
    bool CheckPuzzleWithString( const std::string &s )
    {
        char c;
        size_t str_index = 0;
        for ( size_t y = 1; y <= 9; y++ )
        {
            for ( size_t x = 1; x <= 9; x++ )
            {
                c = s[str_index++];
                int val = c - '0';
                c = s[str_index++];
                bool show = c == '+';
                std::shared_ptr<Sudoku::Cell> cell = _puzzle->GetCell( x, y );
                if ( cell->GetCorrectValue() != val ||
                     ( show ==  cell->CanGuess() ) )
                {
                    return false;
                }
            }
        }
        return true;
    }

    std::shared_ptr<Sudoku::Puzzle> _puzzle;
    std::shared_ptr<Sudoku::IPuzzleImporter> _importer;
};

// Given Example1 file, check that it gets imported
TEST_F( SolvedPuzzleImporterTest, CanImportExample1 )
{
    std::string example1( "\
1+2+3-4+5+6-7+8-9-\
4-5+6-7-8-9+1-2-3+\
7+8-9-1-2+3-4+5-6+\
2-3+4-5+6-7-8-9+1-\
5-6-7-8+9+1-2-3+4-\
8-9-1+2-3+4-5+6-7-\
3+4+5+6-7+8+9-1-2+\
6-7+8+9-1+2-3-4+5+\
9+1+2-3-4+5+6+7-8-" );

    std::istringstream instream( example1 );
    _puzzle = _importer->Import( instream );

    ASSERT_TRUE( _puzzle );
    EXPECT_TRUE( CheckPuzzleWithString( example1 ) );
}

TEST_F( SolvedPuzzleImporterTest, ThrowsOnNonNumericInput )
{
    std::string example1( "\
1+2+3-4+5+6-7+8-9-\
4-5+6-7-p8-9+1-2-3+\
7+8-9-1-2+3-4+5-6+\
2-3+4-5+6-7-8-9+1-\
5-6-7-8+9+1-2-3+4-\
8-9-1+2-3+4-5+6-7-\
3+4+5+6-7+8+9-1-2+\
6-7+8+9-1+2-3-4+5+\
9+1+2-3-4+5+6+7-8-" );

    std::istringstream instream( example1 );
    EXPECT_ANY_THROW( _puzzle = _importer->Import( instream ) );
}

TEST_F( SolvedPuzzleImporterTest, IgnoresExtraCharacters )
{
    std::string example1( "\
1+2+3-4+5+6-7+8-9-\
4-5+6-7-8-9+1-2-3+\
7+8-9-1-2+3-4+5-6+\
2-3+4-5+6-7-8-9+1-\
5-6-7-8+9+1-2-3+4-\
8-9-1+2-3+4-5+6-7-\
3+4+5+6-7+8+9-1-2+\
6-7+8+9-1+2-3-4+5+\
9+1+2-3-4+5+6+7-8-\
0160004809999\n\
abcdefg\n\n()#*#)$@)*&(*&!!!(@)#*)(!@_!@()@#*&" );


    std::istringstream instream( example1 );
    EXPECT_NO_THROW( _puzzle = _importer->Import( instream ) );
    ASSERT_TRUE( _puzzle );
    EXPECT_TRUE( CheckPuzzleWithString( example1 ) );
}

// Check that input works and ignores white space
TEST_F( SolvedPuzzleImporterTest, IgnoresWhitespace )
{
    std::string example1( "\
1+2+3-4+5+6-7+8-9-\
4-5+6-7-8-9+1-2-3+\
7+8-9-1-2+3-4+5-6+\
2-3+4-5+6-7-8-9+1-\
5-6-7-8+9+1-2-3+4-\
8-9-1+2-3+4-5+6-7-\
3+4+5+6-7+8+9-1-2+\
6-7+8+9-1+2-3-4+5+\
9+1+2-3-4+5+6+7-8-" );


    std::string example1ws( "\
   1  +2+   3-4+     5 +6-7+8-9-\
4 - 5 + 6 - 7 -  8 - 9  + 1 - 2 - 3 + \n\
7+8-9-1-2+3-4+5-6+\n\
2-3+4-5+\t6-7-8-9+1-\
5-6-7-8+9+\t1-2-3+4-\
8-9-1+2-3+4-5+6-7-\
3+4+5+6-7+8+9-1-2+\
6-7+8+9-1+2-3-4+5+\
9+1+2-3-4+5+6+7-8-" );

    std::istringstream instream( example1ws );
    _puzzle = _importer->Import( instream );

    ASSERT_TRUE( _puzzle );
    EXPECT_TRUE( CheckPuzzleWithString( example1 ) );
}

// Throw if not enough text while parsing for number
TEST_F( SolvedPuzzleImporterTest, ThrowsIfNotEnoughInputNumber )
{
    std::string example1( "\
1+2+3-4+5+6-7+8-9-\
4-5+6-7-8-9+1-2-3+\
7+8-9-1-2+3-4+5-6+\
2-3+4-5+6-7-8-9+1-\
5-6-7-8+9+1-2-3+4-\
8-9-1+2-3+4-5+6-7-\
3+4+5+6-7+8+9-1-2+\
6-7+8+9-1+2-3-4+5+\
9+1+2-3-" );

    std::istringstream instream( example1 );

    EXPECT_ANY_THROW( _puzzle = _importer->Import( instream ) );
}

// Throw if not enough text while parsing for + or minus
TEST_F( SolvedPuzzleImporterTest, ThrowsIfNotEnoughInputModifier )
{
    std::string example1( "\
1+2+3-4+5+6-7+8-9-\
4-5+6-7-8-9+1-2-3+\
7+8-9-1-2+3-4+5-6+\
2-3+4-5+6-7-8-9+1-\
5-6-7-8+9+1-2-3+4-\
8-9-1+2-3+4-5+6-7-\
3+4+5+6-7+8+9-1-2+\
6-7+8+9-1+2-3-4+5+\
9+1+2-3" );

    std::istringstream instream( example1 );

    EXPECT_ANY_THROW( _puzzle = _importer->Import( instream ) );
}

// Throw if not enough text while parsing for number whitespace check
TEST_F( SolvedPuzzleImporterTest, ThrowsIfNotEnoughInputNumberWS )
{
    std::string example1( "\
1+2+3-4+5+6-7+8-9-\
4-5+6-7-8-9+1-2-3+\
7+8-9-1-2+3-4+5-6+\
2-3+4-5+6-7-8-9+1-\
5-6-7-8+9+1-2-3+4-\
8-9-1+2-3+4-5+6-7-\
3+4+5+6-7+8+9-1-2+\
6-7+8+9-1+2-3-4+5+\
9+1+2-3-   " );

    std::istringstream instream( example1 );

    EXPECT_ANY_THROW( _puzzle = _importer->Import( instream ) );
}

// Throw if not enough text while parsing for + or minus whitespace check
TEST_F( SolvedPuzzleImporterTest, ThrowsIfNotEnoughInputModifierWS )
{
    std::string example1( "\
1+2+3-4+5+6-7+8-9-\
4-5+6-7-8-9+1-2-3+\
7+8-9-1-2+3-4+5-6+\
2-3+4-5+6-7-8-9+1-\
5-6-7-8+9+1-2-3+4-\
8-9-1+2-3+4-5+6-7-\
3+4+5+6-7+8+9-1-2+\
6-7+8+9-1+2-3-4+5+\
9+1+2-3     " );

    std::istringstream instream( example1 );

    EXPECT_ANY_THROW( _puzzle = _importer->Import( instream ) );
}

}  // namespace
