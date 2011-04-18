#include "../PlayerValidator.h"
#include "../Puzzle.h"
#include "gtest/gtest.h"

namespace {

class PlayerValidatorTest : public ::testing::Test
{
protected:

    PlayerValidatorTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
    }

    virtual ~PlayerValidatorTest()
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

    void DisplaySome()
    {
        for ( size_t i = 0; i < 9; i++ )
        {
            for ( size_t j = 0; j < 9; j++ )
            {
                if ( i % 3 == 0 && j % 2 == 1 )
                {
                    std::shared_ptr<Sudoku::Cell> c =
                        _puzzle->GetCell( i+1, j+1 );
                    c->Display( true );
                }
            }
        }
    }

    void GuessSomeCorrect()
    {
        for ( size_t i = 0; i < 9; i++ )
        {
            for ( size_t j = 0; j < 9; j++ )
            {
                if ( i % 3 == 1 && j % 2 == 0 )
                {
                    std::shared_ptr<Sudoku::Cell> c =
                        _puzzle->GetCell( i+1, j+1 );
                    c->SetGuess( c->GetCorrectValue() );
                }
            }
        }        
    }

    void GuessIncorrect( size_t x, size_t y )
    {
        std::shared_ptr<Sudoku::Cell>  c = _puzzle->GetCell( x, y );
        c->SetGuess( (c->GetCorrectValue() + 1) % 9 + 1 );
    }

    std::shared_ptr<Sudoku::Puzzle> _puzzle;
    std::shared_ptr<Sudoku::PlayerValidator> v;
};

// tests that default blank is valid
TEST_F( PlayerValidatorTest, ValidByDefaultBlank )
{
    v.reset( new Sudoku::PlayerValidator );
    EXPECT_TRUE( v->IsValid( _puzzle ) );
}

// tests that given a correct puzzle with no guesses, some displayed is valid
TEST_F( PlayerValidatorTest, ValidIfCorrectWithSomeAnswers )
{
    v.reset( new Sudoku::PlayerValidator );
    MakeCorrect();
    DisplaySome();
    EXPECT_TRUE( v->IsValid( _puzzle ) );
}

// tests that given a correct puzzle with some correct guesses is valid
TEST_F( PlayerValidatorTest, ValidIfCorrectWithSomeGuesses )
{
    v.reset( new Sudoku::PlayerValidator );
    MakeCorrect();
    DisplaySome();
    GuessSomeCorrect();
    EXPECT_TRUE( v->IsValid( _puzzle ) );
}

// tests that given a correct puzzle with an incorrect guess is invalid
TEST_F( PlayerValidatorTest, InvalidIfCorrectWithIncorrectGuesses )
{
    v.reset( new Sudoku::PlayerValidator );
    MakeCorrect();
    DisplaySome();
    GuessSomeCorrect();
    GuessIncorrect( 1, 1 );
    EXPECT_FALSE( v->IsValid( _puzzle ) );
}

}  // namespace
