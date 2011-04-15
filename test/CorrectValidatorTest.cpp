#include "../CorrectValidator.h"
#include "../Puzzle.h"
#include "gtest/gtest.h"

namespace {

class CorrectValidatorTest : public ::testing::Test
{
protected:
    CorrectValidatorTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
    }

    virtual ~CorrectValidatorTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    // make the puzzle correct with simple iteration
    // source: http://en.wikipedia.org/wiki/Algorithmics_of_sudoku#Solving_a_blank_Sudoku_grid
    void MakeCorrect()
    {
        for ( size_t i = 0; i < 9; i++ )
        {
            for ( size_t j = 0; j < 9; j++ )
            {
                std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( i+1, j+1 );
                c->SetCorrect( (i * 3 + i / 3 + j) % 9 + 1 );
            }
        }
    }

    std::shared_ptr<Sudoku::Puzzle> _puzzle;
    Sudoku::CorrectValidator cv;
};

// Test that validation returns false on a default puzzle
TEST_F( CorrectValidatorTest , ReturnsFalseForDefaultPuzzle )
{
    EXPECT_FALSE( cv.IsValid( _puzzle ) );
}

// Test that validation returns true on a good puzzle
TEST_F( CorrectValidatorTest , ReturnsTrueOnKnownGoodPuzzle )
{
    MakeCorrect();
    EXPECT_TRUE( cv.IsValid( _puzzle ) );
}

// Test that validation returns false if we change the good puzzle
TEST_F( CorrectValidatorTest , ReturnsFalseIfOneCellChanged )
{
    MakeCorrect();
    // increment one value
    _puzzle->GetCell( 5, 8 )->SetCorrect(
        (_puzzle->GetCell( 5, 8 )->GetCorrectValue() + 1) % 9 + 1 );
    EXPECT_FALSE( cv.IsValid( _puzzle ) );
}

}  // namespace
