#ifndef SUDOKU_METHOD_SOLVER_H
#define SUDOKU_METHOD_SOLVER_H

#include "ISolver.h"
#include <string>

namespace Sudoku
{

class IPuzzleMarker;
class IValidator;
class SolverHelper;

/**
 * This implementation uses Methods which have inverse actions (allowing 
 * the generation of solvable puzzles).
 */
class MethodSolver : public ISolver
{
public:
    MethodSolver( std::shared_ptr<SolverHelper> helper,
                  std::shared_ptr<IPuzzleMarker> marker,
                  std::shared_ptr<IValidator> validator )
        : _helper( helper ), _marker( marker ), _validator( validator ) {}

    /**
     * Solve a Puzzle
     * @pre Puzzle has minimal Correct values set (is solvable)
     * @post Puzzle has all CorrectValue's set
     * @throw May throw if the Puzzle is unsolvable (Solver gets stuck)
     * @param p Puzzle to solve
     */
    virtual void Solve( std::shared_ptr<Puzzle> p );

    /**
     * Change all of the guesses the Correct Values of Cells
     * This means that now a user can Solve the Puzzle as well
     * @pre Puzzle has Guesses (which are correct)
     * @post Guesses are now CorrectValues (hidden), Guesses are blank
     */
    virtual void CommitGuesses( std::shared_ptr<Puzzle> p );

    /**
     * Solving is non-graphical, we will create a Log
     * Accessor
     */
    std::string GetLog() const;

    virtual ~MethodSolver() {}

    /// Puzzle most likely cannot be solved if there are less than 17 solved
    /// cells to begin with
    static const unsigned MIN_CORRECT_VALUES = 17;
private:
    MethodSolver( const MethodSolver & );
    MethodSolver & operator=( const MethodSolver & );

    /// Helper to get possible Methods
    std::shared_ptr<SolverHelper> _helper;
    /// Sets marks appropriately
    std::shared_ptr<IPuzzleMarker> _marker;
    /// Checks if Solver got it right
    std::shared_ptr<IValidator> _validator;
};

}

#endif
