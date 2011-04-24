#include "../SolverHelper.h"
#include "../PuzzleMarker.h"
#include "MockSolutionMethodFactory.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace {

using ::testing::Return;
using ::testing::_;
using ::testing::ContainerEq;
using ::testing::Property;

class SolverHelperTest : public ::testing::Test
{
protected:
    SolverHelperTest()
    {
        _factory.reset( new Sudoku::MockSolutionMethodFactory );
        _marker.reset( new Sudoku::PuzzleMarker );
        _helper.reset( new Sudoku::SolverHelper( _factory ) );
        _puzzle.reset( new Sudoku::Puzzle );

        ON_CALL( *_factory, CreateSingleCandidateMethod(_) )
            .WillByDefault( Return( _method ) );
        ON_CALL( *_factory, CreateExclusionMethod(_,_) )
            .WillByDefault( Return( _method ) );
        ON_CALL( *_factory, CreateBlockIntersectionMethod(_,_,_,_) )
            .WillByDefault( Return( _method ) );
        ON_CALL( *_factory, CreateCoveringSetMethod(_,_) )
            .WillByDefault( Return( _method ) );
    }

    virtual ~SolverHelperTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    std::shared_ptr<Sudoku::MockSolutionMethodFactory> _factory;
    std::shared_ptr<Sudoku::IPuzzleMarker> _marker;
    std::shared_ptr<Sudoku::SolverHelper> _helper;
    std::shared_ptr<Sudoku::Puzzle> _puzzle;
    std::shared_ptr<Sudoku::SolutionMethod> _method;
};

///////////// Single Candidate Method

TEST_F( SolverHelperTest, SingleCandidateReturnsNothingDefault )
{
    EXPECT_CALL( *_factory, CreateSingleCandidateMethod(_) )
        .Times( 0 );
    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllSingleCandidate( _puzzle );
    EXPECT_TRUE( m.empty() );
}

TEST_F( SolverHelperTest, SingleCandidateReturnsNothingIfAllMarked )
{
    EXPECT_CALL( *_factory, CreateSingleCandidateMethod(_) )
        .Times( 0 );
    _marker->UpdateMarks( _puzzle );
    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllSingleCandidate( _puzzle );
    EXPECT_TRUE( m.empty() );
}

TEST_F( SolverHelperTest, SingleCandidateReturnsOnlySingleCandidate )
{
    _marker->UpdateMarks( _puzzle );
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 3 );
    c->ClearMarks();
    c->Mark( 2 );

    EXPECT_CALL( *_factory, CreateSingleCandidateMethod( c ) )
        .Times( 1 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllSingleCandidate( _puzzle );
    EXPECT_EQ( 1u, m.size() );
}

TEST_F( SolverHelperTest, SingleCandidateReturnsManyMethods )
{
    _marker->UpdateMarks( _puzzle );
    size_t amount = 9;
    for ( size_t i = 1; i <= amount; i++ )
    {
        std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( i, 3 );
        c->ClearMarks();
        c->Mark( 2 );

        EXPECT_CALL( *_factory, CreateSingleCandidateMethod( c ) )
            .Times( 1 );
    }

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllSingleCandidate( _puzzle );
    EXPECT_EQ( amount, m.size() );
}

///////////// Exclusion Method

TEST_F( SolverHelperTest, ExclusionReturnsNothingDefault )
{
    EXPECT_CALL( *_factory, CreateExclusionMethod(_,_) )
        .Times( 0 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllExclusion( _puzzle );
    EXPECT_TRUE( m.empty() );
}

TEST_F( SolverHelperTest, ExclusionReturnsGuessedCell )
{
    _marker->UpdateMarks( _puzzle );
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 1 );
    c->SetGuess( 3 );
    EXPECT_CALL( *_factory, CreateExclusionMethod( _puzzle, c) )
        .Times( 1 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllExclusion( _puzzle );
    EXPECT_EQ( 1u, m.size() );
}

TEST_F( SolverHelperTest, ExclusionDoesNotReturnGuessedCellIfNeighborsUnmark )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 8, 1 );
    c->SetGuess( 3 );
    _marker->UpdateMarks( _puzzle );
    EXPECT_CALL( *_factory, CreateExclusionMethod(_,_) )
        .Times( 0 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllExclusion( _puzzle );
    EXPECT_TRUE( m.empty() );
}

///////////// Block Intersection Method

TEST_F( SolverHelperTest, BlockIntersectionReturnsNothingDefault )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 3, 8 );

    EXPECT_CALL( *_factory, CreateBlockIntersectionMethod(_,_,_,_) )
        .Times( 0 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllBlockIntersection( _puzzle, c );
    EXPECT_TRUE( m.empty() );
}

TEST_F( SolverHelperTest, BlockIntersectionReturnsNothingIfOnlyCellMarked )
{
    std::shared_ptr<Sudoku::Cell> c = _puzzle->GetCell( 1, 3 );
    c->Mark( 4 );
    c->Mark( 5 );
    c->Mark( 6 );

    EXPECT_CALL( *_factory, CreateBlockIntersectionMethod(_,_,_,_) )
        .Times( 0 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllBlockIntersection( _puzzle, c );
    EXPECT_TRUE( m.empty() );
}

// @todo Test this more

///////////// Covering Set Method

TEST_F( SolverHelperTest, CoveringSetReturnsNothingDefault )
{
    Sudoku::Puzzle::Container sector = _puzzle->GetRow( 3 );

    EXPECT_CALL( *_factory, CreateCoveringSetMethod(_,_) )
        .Times( 0 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllCoveringSet( _puzzle, sector );
    EXPECT_TRUE( m.empty() );
}

TEST_F( SolverHelperTest, CoveringSetReturnsNothingIfAllMarked )
{
    Sudoku::Puzzle::Container sector = _puzzle->GetRow( 3 );
    _marker->UpdateMarks( _puzzle );

    EXPECT_CALL( *_factory, CreateCoveringSetMethod(_,_) )
        .Times( 0 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllCoveringSet( _puzzle, sector );
    EXPECT_TRUE( m.empty() );
}

TEST_F( SolverHelperTest, CoveringSetIgnoresPairIfNoMarksToRemove )
{
    Sudoku::Puzzle::Container sector = _puzzle->GetBlock( 1, 2 );
    Sudoku::Puzzle::Container::iterator it = sector.begin();
    (*it)->Mark( 1 );
    (*it)->Mark( 2 );
    ++it;
    (*it)->Mark( 1 );
    (*it)->Mark( 2 );

    EXPECT_CALL( *_factory, CreateCoveringSetMethod(_,_) )
        .Times( 0 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllCoveringSet( _puzzle, sector );
    EXPECT_TRUE( m.empty() );
}

TEST_F( SolverHelperTest, CoveringSetReturnsPairs )
{
    Sudoku::Puzzle::Container sector = _puzzle->GetBlock( 1, 2 );
    Sudoku::Puzzle::Container::iterator it = sector.begin();
    (*it)->Mark( 1 );
    (*it)->Mark( 2 );
    ++it;
    (*it)->Mark( 1 );
    (*it)->Mark( 2 );
    ++it;
    (*it)->Mark( 1 );

    EXPECT_CALL( *_factory, CreateCoveringSetMethod(_,_) )
        .Times( 0 );
    EXPECT_CALL( *_factory, CreateCoveringSetMethod(
                     ContainerEq(sector),
                     Property( &Sudoku::Puzzle::Container::size, 2 ) ) )
        .Times( 3 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllCoveringSet( _puzzle, sector );
    EXPECT_EQ( 3u, m.size() );
}

TEST_F( SolverHelperTest, CoveringSetReturnsMultiplePairs )
{
    Sudoku::Puzzle::Container sector = _puzzle->GetCol( 6 );
    Sudoku::Puzzle::Container::iterator it = sector.begin();
    (*it)->Mark( 1 );
    (*it)->Mark( 2 );
    ++it;
    ++it;
    ++it;
    (*it)->Mark( 1 );
    ++it;
    (*it)->Mark( 2 );

    EXPECT_CALL( *_factory, CreateCoveringSetMethod(_,_) )
        .Times( 0 );
    EXPECT_CALL( *_factory, CreateCoveringSetMethod(
                     ContainerEq(sector),
                     Property( &Sudoku::Puzzle::Container::size, 2 ) ) )
        .Times( 3 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllCoveringSet( _puzzle, sector );
    EXPECT_EQ( 3u, m.size() );
}

TEST_F( SolverHelperTest, CoveringSetJoinsGroupsOfThree )
{
    Sudoku::Puzzle::Container sector = _puzzle->GetCol( 6 );
    Sudoku::Puzzle::Container::iterator it = sector.begin();
    (*it)->Mark( 1 );
    (*it)->Mark( 2 );
    ++it;
    (*it)->Mark( 1 );
    ++it;
    (*it)->Mark( 2 );
    (*it)->Mark( 3 );
    // this one is so they have something to remove
    ++it;
    ++it;
    (*it)->Mark( 1 );
    (*it)->Mark( 7 );
    (*it)->Mark( 8 );
    (*it)->Mark( 9 );

    EXPECT_CALL( *_factory, CreateCoveringSetMethod(_,_) )
        .Times( 0 );
    EXPECT_CALL( *_factory, CreateCoveringSetMethod(
                     ContainerEq(sector),
                     Property( &Sudoku::Puzzle::Container::size, 3 ) ) )
        .Times( 1 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllCoveringSet( _puzzle, sector );
    EXPECT_EQ( 1u, m.size() );
}

TEST_F( SolverHelperTest, CoveringSetJoinsGroupsOfFour )
{
    Sudoku::Puzzle::Container sector = _puzzle->GetCol( 6 );
    Sudoku::Puzzle::Container::iterator it = sector.begin();
    (*it)->Mark( 1 );
    (*it)->Mark( 2 );
    ++it;
    (*it)->Mark( 1 );
    ++it;
    (*it)->Mark( 2 );
    (*it)->Mark( 3 );
    ++it;
    (*it)->Mark( 3 );
    (*it)->Mark( 7 );
    // this is the one they can unmark
    ++it;
    ++it;
    (*it)->Mark( 2 );
    (*it)->Mark( 4 );
    (*it)->Mark( 5 );
    (*it)->Mark( 8 );
    (*it)->Mark( 9 );

    EXPECT_CALL( *_factory, CreateCoveringSetMethod(_,_) )
        .Times( 0 );
    EXPECT_CALL( *_factory, CreateCoveringSetMethod(
                     ContainerEq(sector),
                     Property( &Sudoku::Puzzle::Container::size, 4 ) ) )
        .Times( 1 );

    Sudoku::SolverHelper::MethodContainer m =
        _helper->GetAllCoveringSet( _puzzle, sector );
    EXPECT_EQ( 1u, m.size() );
}


}  // namespace
