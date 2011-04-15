#ifndef SUDOKU_CORRECT_VALIDATOR_H
#define SUDOKU_CORRECT_VALIDATOR_H

#include "IValidator.h"
#include <memory>

namespace Sudoku
{

class Cell;

/**
 * Abstract Functor
 * Gets the value desired from a Cell
 */
class CellChecker
{
public:
    virtual int operator()( const std::shared_ptr<Cell> &c ) const = 0;
};

/**
 * Gets the correct value of a Cell for checking
 */
class CellCorrectChecker : public CellChecker
{
public:
    virtual int operator()( const std::shared_ptr<Cell> &c ) const;
};

/**
 * Gets the guessed (displayed) value of a Cell
 */
class CellGuessChecker : public CellChecker
{
public:
    virtual int operator()( const std::shared_ptr<Cell> &c ) const;
};

/**
 * Validates Puzzles by looping over all 27 sectors
 */
class SimpleValidator : public IValidator
{
public:
    /**
     * For checking the correct values of a Puzzle that is generated
     * @return Validator
     */
    static std::shared_ptr<SimpleValidator> CreateCorrectValidator();

    /**
     * For checking the guessed values when someone is solving a puzzle
     * @return Validator
     */
    static std::shared_ptr<SimpleValidator> CreateGuessValidator();

    /**
     * Check if a given Puzzle is valid
     * Looks at the correct values
     */
    virtual bool IsValid( std::shared_ptr<Puzzle> p );

private:
    SimpleValidator( std::shared_ptr<CellChecker> check )
        : _checker( check ) {}

    /// Functor to check a Cell
    std::shared_ptr<CellChecker> _checker;
};

}

#endif
