#include "../SimplePuzzleImporter.h"
#include "../Puzzle.h"
#include "../Cell.h"
#include "gtest/gtest.h"

namespace {

class SimplePuzzleImporterTest : public ::testing::Test
{
protected:

    SimplePuzzleImporterTest()
    {
        _importer.reset( new Sudoku::SimplePuzzleImporter );
    }

    virtual ~SimplePuzzleImporterTest()
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
                // ignore new lines
                while ( (c = s[str_index++]) == '\n' ) {}
                int val = c - '0';
                std::shared_ptr<Sudoku::Cell> cell = _puzzle->GetCell( x, y );
                if ( cell->GetCorrectValue() != val || 
                     ( val != 0 && cell->CanGuess() ) ||
                     ( val == 0 && !cell->CanGuess() ) )
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
TEST_F( SimplePuzzleImporterTest, CanImportExample1 )
{
    std::string example1( "\
069000130\n\
050008009\n\
720000640\n\
000730500\n\
900000001\n\
004065000\n\
097000053\n\
500200010\n\
016000480\n" );

    std::istringstream instream( example1 );
    _puzzle = _importer->Import( instream );

    ASSERT_TRUE( _puzzle );
    EXPECT_TRUE( CheckPuzzleWithString( example1 ) );
}

TEST_F( SimplePuzzleImporterTest, ThrowsOnNonNumericInput )
{
    std::string example1( "\
06900k130\n\
050008009\n\
720000640\n\
000730500\n\
900000001\n\
004065000\n\
097000053\n\
500200010\n\
016000480\n" );

    std::istringstream instream( example1 );
    EXPECT_ANY_THROW( _puzzle = _importer->Import( instream ) );
}

TEST_F( SimplePuzzleImporterTest, IgnoresExtraCharacters )
{
    std::string example1( "\
069000130\n\
050008009\n\
720000640\n\
000730500\n\
900000001\n\
004065000\n\
097000053\n\
500200010\n\
0160004809999\n\
abcdefg\n\n()#*#)$@)*&(*&!!!(@)#*)(!@_!@()@#*&" );

    std::istringstream instream( example1 );
    EXPECT_NO_THROW( _puzzle = _importer->Import( instream ) );
    ASSERT_TRUE( _puzzle );
    EXPECT_TRUE( CheckPuzzleWithString( example1 ) );
}

// Check that input works and ignores white space
TEST_F( SimplePuzzleImporterTest, IgnoresWhitespace )
{
    std::string example1( "\
069000130\n\
050008009\n\
720000640\n\
000730500\n\
900000001\n\
004065000\n\
097000053\n\
500200010\n\
016000480\n" );

    std::string example1ws( "\
0 6 9 0 0 0 1 3 0\n\
050008009\n\
7      20000640\n\
0\t\t\t\t\t00730500\n\
900000001\n\
004065000\n\n\n\n\n\
097000053\n\
500200010\n\
016000480\n" );

    std::istringstream instream( example1ws );
    _puzzle = _importer->Import( instream );

    ASSERT_TRUE( _puzzle );
    EXPECT_TRUE( CheckPuzzleWithString( example1 ) );
}

// Throw if not enough text
TEST_F( SimplePuzzleImporterTest, ThrowsIfNotEnoughInput )
{
    std::string example1( "\
069000130\n\
050008009\n\
720000640\n\
000730500\n\
900000001\n\
004065000\n\
097000053\n\
500200010\n\
0160004\n" );
    std::istringstream instream( example1 );
    EXPECT_ANY_THROW( _puzzle = _importer->Import( instream ) );
}

// Throw if not enough text in whitespace check
TEST_F( SimplePuzzleImporterTest, ThrowsIfNotEnoughInputWS )
{
    std::string example1( "\
069000130\n\
050008009\n\
720000640\n\
000730500\n\
900000001\n\
004065000\n\
097000053\n\
500200010\n\
016000           \n" );
    std::istringstream instream( example1 );
    EXPECT_ANY_THROW( _puzzle = _importer->Import( instream ) );
}

}  // namespace
