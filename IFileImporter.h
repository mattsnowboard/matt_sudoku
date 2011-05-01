#ifndef SUDOKU_IFILE_IMPORTER_H
#define SUDOKU_IFILE_IMPORTER_H

#include <string>

namespace Sudoku
{

class IFileImporter
{
public:
    /**
     * Load a puzzle from a file and report success or failure
     * @param filename The file to load
     * @return Success or failure
     */
    virtual bool ImportFromFile( const std::string &filename ) = 0;

    /**
     * Reset the puzzle pointer and clear what exists
     */
    virtual void NewPuzzle() = 0;

    virtual ~IFileImporter() {}
};

}

#endif
