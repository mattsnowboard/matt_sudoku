#ifndef SUDOKU_IFILE_IMPORTER_H
#define SUDOKU_IFILE_IMPORTER_H

#include <string>

namespace Sudoku
{

class IFileImporter
{
public:
    virtual bool ImportFromFile( const std::string &filename ) = 0;

    virtual ~IFileImporter() {}
};

}

#endif
