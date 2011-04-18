#ifndef SUDOKU_PLAYER_VALIDATOR_H
#define SUDOKU_PLAYER_VALIDATOR_H

#include "IValidator.h"

namespace Sudoku
{

/**
 * This is to validate Cells using the check if guess==correct or no guess
 * It is to be used when a player is solving a puzzle with a known solution
 */
class PlayerValidator : public IValidator
{
public:
    PlayerValidator() {}

    /**
     * Check if a given Puzzle is valid
     * Checks Cell::IsCorrect
     * @param p The Puzzle to validate
     * @return True if valid puzzle (based on what is displayed only
     */
    virtual bool IsValid( std::shared_ptr<Puzzle> p );
};

}

#endif
