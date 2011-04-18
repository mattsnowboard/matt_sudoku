#include "SolverHelper.h"

#include "Puzzle.h"

#include "BlockIntersectionMethod.h"
#include "CoveringSetMethod.h"
#include "ExclusionMethod.h"
#include "SingleCandidateMethod.h"

#include "SolutionMethodFactory.h"

namespace Sudoku
{

// helpers
namespace
{

Cell::MarkContainer ContainerMarks( const Puzzle::Container &c )
{
    Cell::MarkContainer allMarks;
    for ( Puzzle::Container::iterator it = c.begin(); it != c.end(); ++it )
    {
        allMarks |= (*it)->GetMarkContainer();
    }
    return allMarks;
}

// functor to remove cells we don't need from CoveringSetMethod
class CellHasMarks : public std::unary_function<const std::shared_ptr<Cell>,
                                                bool>
{
public:
    // note use of const ref to shared_ptr's
    bool operator() ( const std::shared_ptr<Cell> &c ) const
    {
        return ( c->CanGuess() &&
                 c->DisplayedValue() == 0 &&
                 c->GetMarkContainer().any()  );
    }
};

}

SolverHelper::MethodContainer SolverHelper::GetAllSingleCandidate(
    std::shared_ptr<Puzzle> p )
{
    MethodContainer m;
    // iterate over cells
    Puzzle::Container all = p->GetAllCells();
    for ( Puzzle::Container::iterator it = all.begin(); it != all.end(); ++it )
    {
        if ( (*it)->CanGuess() && (*it)->DisplayedValue() == 0 &&
             (*it)->GetMarkedValues().size() == 1 )
        {
            std::shared_ptr<SolutionMethod> candidate(
                _factory->CreateSingleCandidateMethod( *it ) );
            m.push_back( candidate );
        }
    }

    return m;
}

SolverHelper::MethodContainer SolverHelper::GetAllBlockIntersection(
    std::shared_ptr<Puzzle> p,
    std::shared_ptr<Cell> c )
{
    MethodContainer m;
    // look at every mark on c
    Cell::MarkedValues marks = c->GetMarkedValues();

    // get all groupings of row\block, block\row, col\block, block\col
    Puzzle::Container row = p->GetRow( c->GetY() );
    Puzzle::Container col = p->GetCol( c->GetX() );
    Puzzle::Container block = p->GetBlock( c );

    Puzzle::Container rowMinusBlock;
    std::set_difference( row.begin(), row.end(),
                         block.begin(), block.end(),
                         std::inserter( rowMinusBlock, rowMinusBlock.begin() ),
                         row.value_comp() );
    Cell::MarkContainer rowMinusBlockMarks = ContainerMarks( rowMinusBlock );

    Puzzle::Container colMinusBlock;
    std::set_difference( col.begin(), col.end(),
                         block.begin(), block.end(),
                         std::inserter( colMinusBlock, colMinusBlock.begin() ),
                         col.value_comp() );
    Cell::MarkContainer colMinusBlockMarks = ContainerMarks( colMinusBlock );

    Puzzle::Container blockMinusRow;
    std::set_difference( block.begin(), block.end(),
                         row.begin(), row.end(),
                         std::inserter( blockMinusRow, blockMinusRow.begin() ),
                         block.value_comp() );
    Cell::MarkContainer blockMinusRowMarks = ContainerMarks( blockMinusRow );

    Puzzle::Container blockMinusCol;
    std::set_difference( block.begin(), block.end(),
                         col.begin(), col.end(),
                         std::inserter( blockMinusCol, blockMinusCol.begin() ),
                         block.value_comp() );
    Cell::MarkContainer blockMinusColMarks = ContainerMarks( blockMinusCol );

    for ( Cell::MarkedValues::iterator it = marks.begin();
          it != marks.end();
          ++it )
    {
        std::shared_ptr<SolutionMethod> method;
        if ( rowMinusBlockMarks[*it] && !blockMinusRowMarks[*it] )
        {
            method =  _factory->CreateBlockIntersectionMethod(
                c, *it, rowMinusBlock, blockMinusRow );
            m.push_back( method );
        }
        else if ( !rowMinusBlockMarks[*it] && blockMinusRowMarks[*it] )
        {
            method = _factory->CreateBlockIntersectionMethod(
                c, *it, blockMinusRow, rowMinusBlock );
            m.push_back( method );
        }

        if ( colMinusBlockMarks[*it] && !blockMinusColMarks[*it] )
        {
            method = _factory->CreateBlockIntersectionMethod(
                c, *it, colMinusBlock, blockMinusCol );
            m.push_back( method );
        }
        else if ( !colMinusBlockMarks[*it] && blockMinusColMarks[*it] )
        {
            method = _factory->CreateBlockIntersectionMethod(
                c, *it, blockMinusCol, colMinusBlock );
            m.push_back( method );
        }
    }

    return m;
}

SolverHelper::MethodContainer SolverHelper::GetAllCoveringSet(
    std::shared_ptr<Puzzle> p,
    const Puzzle::Container &sector )
{
    MethodContainer m;
    // cut sector down to cells with no guess and some marks
    std::vector<std::shared_ptr<Cell> > prunedSector;
    std::copy_if( sector.begin(), sector.end(),
                  std::back_inserter( prunedSector ),
                  CellHasMarks() );

    // keep a list of possible subsets and then reduce the list
    std::vector<Puzzle::Container> subsets;

    // look at n-tuples (n=2,3,4)
    if ( prunedSector.size() >= 2 )
    {
        std::vector<std::shared_ptr<Cell> >::iterator first;
        std::vector<std::shared_ptr<Cell> >::iterator second;
        std::vector<std::shared_ptr<Cell> >::iterator third;
        std::vector<std::shared_ptr<Cell> >::iterator fourth;
        for ( first = prunedSector.begin();
              first != prunedSector.end();
              ++first )
        {
            //std::cout << **first << std::endl;
            Puzzle::Container temp;
            temp.insert( *first );

            for ( second = first + 1;
                  second != prunedSector.end();
                  ++second )
            {
                //std::cout << "\t" << **second << std::endl;
                bool triple = false;
                temp.insert( *second );
                // now check all 3-tuples
                for ( third = second + 1;
                      third != prunedSector.end();
                      ++third )
                {
                    //std::cout << "\t\t" << **third<< std::endl;
                    bool fourTuple = false;
                    temp.insert( *third );
                    // now check all 4-tuples
                    for ( fourth = third + 1;
                          fourth != prunedSector.end();
                          ++fourth )
                    {
                        //std::cout << "\t\t\t" << **fourth << std::endl;
                        temp.insert( *fourth );
                        Cell::MarkContainer tempMarks = ContainerMarks( temp );
                        if ( tempMarks.count() == 4 )
                        {
                            // ok these four work
                            subsets.push_back( temp );
                            //std::shared_ptr<SolutionMethod> method(
                            //    _factory->CreateCoveringSetMethod(
                            //        sector, temp ) );
                            //m.push_back( method );
                            fourTuple = true;
                        }
                        // remove before going to next fourth
                        temp.erase( *fourth );
                    }
                    // ignore triple if it is a part of the larger group
                    if ( !fourTuple )
                    {
                        // check triples after checking deeper
                        Cell::MarkContainer tempMarks = ContainerMarks( temp );
                        if ( tempMarks.count() == 3 )
                        {
                            // make sure we haven't already seen this
                            bool unique = true;
                            for ( std::vector<Puzzle::Container>::iterator it =
                                      subsets.begin();
                                  it != subsets.end();
                                  ++it )
                            {
                                if ( std::includes( (*it).begin(), (*it).end(),
                                                    temp.begin(), temp.end(),
                                                    temp.value_comp() ) )
                                {
                                    unique = false;
                                    break;
                                }
                            }
                            if ( unique )
                            {
                                // ok these three work
                                subsets.push_back( temp );
                                //std::shared_ptr<SolutionMethod> method(
                                //    _factory->CreateCoveringSetMethod(
                                //        sector, temp ) );
                                //m.push_back( method );
                                triple = true;
                            }
                        }
                    }
                    else
                    {
                        triple = true;
                    }
                    // remove before going to next third
                    temp.erase( *third );
                }
                // ignore pairs that are subsets
                if ( !triple )
                {
                    // check pairs after checking deeper
                    Cell::MarkContainer tempMarks = ContainerMarks( temp );
                    if ( tempMarks.count() == 2 )
                    {
                        // make sure we haven't already seen this
                        bool unique = true;
                        for ( std::vector<Puzzle::Container>::iterator it =
                                  subsets.begin();
                              it != subsets.end();
                              ++it )
                        {
                            if ( std::includes( (*it).begin(), (*it).end(),
                                                temp.begin(), temp.end(),
                                                temp.value_comp() ) )
                            {
                                unique = false;
                                break;
                            }
                        }
                        if ( unique )
                        {
                            // ok these two work
                            subsets.push_back( temp );
                            //std::shared_ptr<SolutionMethod> method(
                            //    _factory->CreateCoveringSetMethod(
                            //        sector, temp ) );
                            //m.push_back( method );
                        }
                    }
                }
                // remove before going to next second
                temp.erase( *second );
            }
        }
    }

    for ( std::vector<Puzzle::Container>::iterator it = subsets.begin();
          it != subsets.end();
          ++it )
    {
        std::shared_ptr<SolutionMethod> method(
            _factory->CreateCoveringSetMethod(
                sector, *it ) );
        m.push_back( method );
    }


    return m;
}

}
