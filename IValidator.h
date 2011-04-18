#ifndef SUDOKU_IVALIDATOR_H
#define SUDOKU_IVALIDATOR_H

#include <memory>

namespace Sudoku
{

class Puzzle;

/**
 * Validator Interface
 *
 * This can take a Puzzle and determine if it is valid
 */
class IValidator
{
public:
    /**
     * Check if a given Puzzle is valid
     * The implementation can define what this means
     * Also, it can examine either the "correct" values of Cells or "guesses"
     * @param p The Puzzle to validate
     * @return True if valid puzzle
     */
    virtual bool IsValid( std::shared_ptr<Puzzle> p ) = 0;

    virtual ~IValidator() {}
};

}

#endif
