#ifndef SUDOKU_IPUZZLE_IMPORTER_H
#define SUDOKU_IPUZZLE_IMPORTER_H

#include <memory>
#include <iostream>

namespace Sudoku
{

class Puzzle;

/**
 * Interface to import a Puzzle from a stream (e.g., a File)
 * Can handle various formats in different implementations
 */
class IPuzzleImporter
{
public:
    /**
     * Given a stream, parse it and return a new Puzzle object
     * or NULL on error.
     * @param in Input stream with definition of Puzzle
     * @pre in follows the implementation format
     * @return The Puzzle that was created or NULL
     * @throw May throw on parsing error
     */
    virtual std::shared_ptr<Puzzle> Import( std::istream &in ) = 0;

    /**
     * Some formats may be defined with file extensions
     * This will tell us if a given file name fits the pattern
     * @param name The entire file name (including extension)
     * @return true if the name fits a pattern for this importer type
     */
    virtual bool CanHandleExtension( const std::string &name ) = 0;

    /**
     * 
     */
    virtual ~IPuzzleImporter() {}
};

}

#endif
