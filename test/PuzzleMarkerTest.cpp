#include "../PuzzleMarker.h"
#include "../Puzzle.h"
#include "gtest/gtest.h"

namespace {

class PuzzleMarkerTest : public ::testing::Test
{
protected:
    PuzzleMarkerTest()
    {
        _puzzle.reset( new Sudoku::Puzzle );
    }

    virtual ~PuzzleMarkerTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

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
};

// Given a blank Puzzle, mark everything
TEST_F( PuzzleMarkerTest, MarksAllOnBlank )
{
    Sudoku::PuzzleMarker marker;
    marker.UpdateMarks( _puzzle );
    // verify all marks
    Sudoku::Puzzle::Container all = _puzzle->GetAllCells();
    for ( Sudoku::Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        std::shared_ptr<Sudoku::Cell> c = *it;
        EXPECT_TRUE( c->GetMarkContainer()[1] );
        EXPECT_TRUE( c->GetMarkContainer()[2] );
        EXPECT_TRUE( c->GetMarkContainer()[3] );
        EXPECT_TRUE( c->GetMarkContainer()[4] );
        EXPECT_TRUE( c->GetMarkContainer()[5] );
        EXPECT_TRUE( c->GetMarkContainer()[6] );
        EXPECT_TRUE( c->GetMarkContainer()[7] );
        EXPECT_TRUE( c->GetMarkContainer()[8] );
        EXPECT_TRUE( c->GetMarkContainer()[9] );
    }
}

// Given a solved Puzzle, mark nothing
TEST_F( PuzzleMarkerTest, MarksNothingIfSolved )
{
    MakeCorrect();
    Sudoku::PuzzleMarker marker;
    marker.UpdateMarks( _puzzle );
    // verify all marks
    Sudoku::Puzzle::Container all = _puzzle->GetAllCells();
    for ( Sudoku::Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        std::shared_ptr<Sudoku::Cell> c = *it;
        EXPECT_TRUE( c->GetMarkContainer()[1] );
        EXPECT_TRUE( c->GetMarkContainer()[2] );
        EXPECT_TRUE( c->GetMarkContainer()[3] );
        EXPECT_TRUE( c->GetMarkContainer()[4] );
        EXPECT_TRUE( c->GetMarkContainer()[5] );
        EXPECT_TRUE( c->GetMarkContainer()[6] );
        EXPECT_TRUE( c->GetMarkContainer()[7] );
        EXPECT_TRUE( c->GetMarkContainer()[8] );
        EXPECT_TRUE( c->GetMarkContainer()[9] );
    }
}

// If we specify guesses in Neighbors, that cell gets marks
TEST_F( PuzzleMarkerTest, SetsProperMarksForControlledNeighbors )
{
    // take a cell's neighbors and add guesses
    // check that cell gets proper marks
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 7, 2 );
    Sudoku::Puzzle::Container N = _puzzle->GetNeighbors( c );
    N.erase( c );
    // mark every 3rd with a 3, every 4th with a 9, every 7th with a 2
    int i = 0;
    for ( Sudoku::Puzzle::Container::iterator it = N.begin();
          it != N.end();
          ++it, ++i )
    {
        if ( i % 3 == 0 )
        {
            (*it)->SetGuess( 3 );
        }
        else if ( i % 4 == 0 )
        {
            (*it)->SetGuess( 9 );
        }
        else if ( i % 7 == 0 )
        {
            (*it)->SetGuess( 2 );
        }
    }

    Sudoku::PuzzleMarker marker;
    marker.UpdateMarks( _puzzle );

    EXPECT_TRUE( c->GetMarkContainer()[1] );
    EXPECT_FALSE( c->GetMarkContainer()[2] );
    EXPECT_FALSE( c->GetMarkContainer()[3] );
    EXPECT_TRUE( c->GetMarkContainer()[4] );
    EXPECT_TRUE( c->GetMarkContainer()[5] );
    EXPECT_TRUE( c->GetMarkContainer()[6] );
    EXPECT_TRUE( c->GetMarkContainer()[7] );
    EXPECT_TRUE( c->GetMarkContainer()[8] );
    EXPECT_FALSE( c->GetMarkContainer()[9] );
}

// If we change guesses in neighbors after marking, update still works
TEST_F( PuzzleMarkerTest, SetsProperMarksAfterNeighborsChange )
{
    // take a cell's neighbors and add guesses
    // check that cell gets proper marks
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 7, 2 );
    Sudoku::Puzzle::Container N = _puzzle->GetNeighbors( c );
    N.erase( c );
    // mark every 3rd with a 3, every 4th with a 9, every 7th with a 2
    int i = 0;
    for ( Sudoku::Puzzle::Container::iterator it = N.begin();
          it != N.end();
          ++it, ++i )
    {
        if ( i % 3 == 0 )
        {
            (*it)->SetGuess( 3 );
        }
        else if ( i % 4 == 0 )
        {
            (*it)->SetGuess( 9 );
        }
        else if ( i % 7 == 0 )
        {
            (*it)->SetGuess( 2 );
        }
    }

    Sudoku::PuzzleMarker marker;
    marker.UpdateMarks( _puzzle );

    // change guesses
    for ( Sudoku::Puzzle::Container::iterator it = N.begin();
          it != N.end();
          ++it )
    {
        if ( (*it)->DisplayedValue() == 3 )
        {
            (*it)->SetGuess( 1 );
        }
    }

    marker.UpdateMarks( _puzzle );

    EXPECT_FALSE( c->GetMarkContainer()[1] );
    EXPECT_FALSE( c->GetMarkContainer()[2] );
    EXPECT_TRUE( c->GetMarkContainer()[3] );
    EXPECT_TRUE( c->GetMarkContainer()[4] );
    EXPECT_TRUE( c->GetMarkContainer()[5] );
    EXPECT_TRUE( c->GetMarkContainer()[6] );
    EXPECT_TRUE( c->GetMarkContainer()[7] );
    EXPECT_TRUE( c->GetMarkContainer()[8] );
    EXPECT_FALSE( c->GetMarkContainer()[9] );
}

// check that clearing marks works
TEST_F( PuzzleMarkerTest, CanClearAllMarks )
{
    // mark randomly
    Sudoku::Puzzle::Container all = _puzzle->GetAllCells();
    // mark every 3rd with a 3, every 4th with a 9, every 7th with a 2
    int i = 0;
    for ( Sudoku::Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it, ++i )
    {
        if ( i % 3 == 0 )
        {
            (*it)->SetGuess( 3 );
        }
        else if ( i % 4 == 0 )
        {
            (*it)->SetGuess( 9 );
        }
        else if ( i % 7 == 0 )
        {
            (*it)->SetGuess( 2 );
        }
        else if ( i % 19 == 0 )
        {
            (*it)->SetGuess( 1 );
        }
    }

    Sudoku::PuzzleMarker marker;
    marker.UpdateMarks( _puzzle );
    marker.ClearPuzzle( _puzzle );

    for ( Sudoku::Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        std::shared_ptr<Sudoku::Cell> c = *it;
        EXPECT_FALSE( c->GetMarkContainer()[1] );
        EXPECT_FALSE( c->GetMarkContainer()[2] );
        EXPECT_FALSE( c->GetMarkContainer()[3] );
        EXPECT_FALSE( c->GetMarkContainer()[4] );
        EXPECT_FALSE( c->GetMarkContainer()[5] );
        EXPECT_FALSE( c->GetMarkContainer()[6] );
        EXPECT_FALSE( c->GetMarkContainer()[7] );
        EXPECT_FALSE( c->GetMarkContainer()[8] );
        EXPECT_FALSE( c->GetMarkContainer()[9] );
    }

}


}  // namespace
