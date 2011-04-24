#ifndef SUDOKU_ISOLVER_H
#define SUDOKU_ISOLVER_H

#include <memory>

namespace Sudoku
{

class Puzzle;

/**
 * Interface which can take a Puzzle with few values displayed and solve it
 * Can define multiple implementations
 */
class ISolver
{
public:
    /**
     * Solve a Puzzle
     * @pre Puzzle has minimal Correct values set (is solvable)
     * @post Puzzle has all Guesses set
     * @throw May throw if the Puzzle is unsolvable (Solver gets stuck)
     * @param p Puzzle to solve
     */
    virtual void Solve( std::shared_ptr<Puzzle> p ) = 0;

    /**
     * Change all of the guesses the Correct Values of Cells
     * This means that now a user can Solve the Puzzle as well
     * @pre Puzzle has Guesses (which are correct)
     * @post Guesses are now CorrectValues (hidden), Guesses are blank
     */
    virtual void CommitGuesses( std::shared_ptr<Puzzle> p ) = 0;

    virtual ~ISolver() {}
};

}

#endif
