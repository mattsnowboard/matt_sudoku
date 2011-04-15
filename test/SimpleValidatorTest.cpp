#include "../SimpleValidator.h"
#include "../Puzzle.h"
#include "gtest/gtest.h"

namespace {

class SimpleValidatorTest : public ::testing::Test
{
protected:
    SimpleValidatorTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
    }

    virtual ~SimpleValidatorTest()
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

    // make the puzzle correct with simple iteration
    // source: http://en.wikipedia.org/wiki/Algorithmics_of_sudoku#Solving_a_blank_Sudoku_grid
    void MakeCorrectGuess()
    {
        for ( size_t i = 0; i < 9; i++ )
        {
            for ( size_t j = 0; j < 9; j++ )
            {
                std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( i+1, j+1 );
                // every few will have correct set
                if ( i % 4 == 0 && j % 2 == 0 )
                {
                    c->SetCorrect( (i * 3 + i / 3 + j) % 9 + 1 );
                    c->Display( true );
                }
                else
                {
                    c->SetGuess( (i * 3 + i / 3 + j) % 9 + 1 );
                }
            }
        }
    }

    std::shared_ptr<Sudoku::Puzzle> _puzzle;
    std::shared_ptr<Sudoku::SimpleValidator> _validator;
};

/////////////////// Correct

// Test that validation returns false on a default puzzle
TEST_F( SimpleValidatorTest , CorrectValidatorReturnsFalseForDefaultPuzzle )
{
    _validator = Sudoku::SimpleValidator::CreateCorrectValidator();
    EXPECT_FALSE( _validator->IsValid( _puzzle ) );
}

// Test that validation returns true on a good puzzle
TEST_F( SimpleValidatorTest , CorrectValidatorReturnsTrueOnKnownGoodPuzzle )
{
    _validator = Sudoku::SimpleValidator::CreateCorrectValidator();
    MakeCorrect();
    EXPECT_TRUE( _validator->IsValid( _puzzle ) );
}

// Test that validation returns false if we change the good puzzle
TEST_F( SimpleValidatorTest , CorrectValidatorReturnsFalseIfOneCellChanged )
{
    _validator = Sudoku::SimpleValidator::CreateCorrectValidator();
    MakeCorrect();
    // increment one value
    _puzzle->GetCell( 5, 8 )->SetCorrect(
        (_puzzle->GetCell( 5, 8 )->GetCorrectValue() + 1) % 9 + 1 );
    EXPECT_FALSE( _validator->IsValid( _puzzle ) );
}

//////////////// Guessed

// Test that validation returns false on a default puzzle
TEST_F( SimpleValidatorTest , GuessValidatorReturnsFalseForDefaultPuzzle )
{
    _validator = Sudoku::SimpleValidator::CreateGuessValidator();
    EXPECT_FALSE( _validator->IsValid( _puzzle ) );
}

// Test that validation returns true on a good puzzle
TEST_F( SimpleValidatorTest , GuessValidatorReturnsTrueOnKnownGoodPuzzle )
{
    _validator = Sudoku::SimpleValidator::CreateGuessValidator();
    MakeCorrectGuess();
    EXPECT_TRUE( _validator->IsValid( _puzzle ) );
}

// Test that validation returns false if we change the good puzzle
TEST_F( SimpleValidatorTest , GuessValidatorReturnsFalseIfOneCellChanged )
{
    _validator = Sudoku::SimpleValidator::CreateGuessValidator();
    MakeCorrectGuess();
    // increment one value
    _puzzle->GetCell( 5, 8 )->SetGuess(
        (_puzzle->GetCell( 5, 8 )->DisplayedValue() + 1) % 9 + 1 );
    EXPECT_FALSE( _validator->IsValid( _puzzle ) );
}

}  // namespace
