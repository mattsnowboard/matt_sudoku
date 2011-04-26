#ifndef SUDOKU_SOLVED_PUZZLE_IMPORTER_H
#define SUDOKU_SOLVED_PUZZLE_IMPORTER_H

#include "IPuzzleImporter.h"

namespace Sudoku
{

/**
 * Imports Puzzles which include the full solution and which Cells to display
 * Format is #{+|-} where # is 1 through 9 and + and - mean show and hide
 *
 * Will create Puzzle's with those values set as correct and set certain
 * ones to display and others to be hidden
 * Any lines beyond the first 9 are ignored
 *
 * Does not handle any values other than 0-9,+.- for the first 162 characters
 * (no characters beyond 162 are read)
 *
 * Whitespace is ignored
 *
 */
class SolvedPuzzleImporter : public IPuzzleImporter
{
public:
    SolvedPuzzleImporter();

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
    virtual ~SolvedPuzzleImporter() {}
private:
    SolvedPuzzleImporter( const SolvedPuzzleImporter & );
    SolvedPuzzleImporter & operator=( const SolvedPuzzleImporter & );

    char _show;
    char _hide;
};

}

#endif
