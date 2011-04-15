#ifndef SUDOKU_CORRECT_VALIDATOR_H
#define SUDOKU_CORRECT_VALIDATOR_H

#include "IValidator.h"

namespace Sudoku
{

class CorrectValidator : public IValidator
{
public:
    CorrectValidator() {}

    /**
     * Check if a given Puzzle is valid
     * Looks at the correct values
     */
    virtual bool IsValid( std::shared_ptr<Puzzle> p );
};

}

#endif
