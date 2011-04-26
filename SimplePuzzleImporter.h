#ifndef SUDOKU_SIMPLE_PUZZLE_IMPORTER_H
#define SUDOKU_SIMPLE_PUZZLE_IMPORTER_H

#include "IPuzzleImporter.h"

namespace Sudoku
{

/**
 * Imports Puzzles which do not record the solution
 * Format is ######### (times 9 lines) where numbers are correct values
 * which will be displayed, or 0 which is an unsolved Cell
 *
 * Will create Puzzle's with those values set as correct and displayed
 * and the rest completely blank
 * Any lines beyond the first 9 are ignored
 *
 * Does not handle any values other than 0-9 for the first 81 characters
 * (no characters beyond 81 are read)
 *
 * Whitespace is ignored
 *
 * Handles filenames without extensions
 */
class SimplePuzzleImporter : public IPuzzleImporter
{
public:
    SimplePuzzleImporter() {}

    /**
     * Given a stream, parse it and return a new Puzzle object
     * or NULL on error.
     * @param in Input stream with definition of Puzzle
     * @pre in follows the implementation format
     * @return The Puzzle that was created or NULL
     * @throw May throw on parsing error
     */
    virtual std::shared_ptr<Puzzle> Import( std::istream &in );

    /**
     * Some formats may be defined with file extensions
     * This will tell us if a given file name fits the pattern
     * @param name The entire file name (including extension)
     * @return true if the name fits a pattern for this importer type
     */
    virtual bool CanHandleExtension( const std::string &name );

    /**
     * 
     */
    virtual ~SimplePuzzleImporter() {}

private:
    SimplePuzzleImporter( const SimplePuzzleImporter & );
    SimplePuzzleImporter & operator=( const SimplePuzzleImporter & );
};

}

#endif
