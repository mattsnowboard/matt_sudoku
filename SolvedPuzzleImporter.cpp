#include "SolvedPuzzleImporter.h"
#include "Puzzle.h"

#include "Log.h"

namespace Sudoku
{

SolvedPuzzleImporter::SolvedPuzzleImporter() : _show( '+' ), _hide( '-' )
{}

std::shared_ptr<Puzzle> SolvedPuzzleImporter::Import( std::istream &in )
{
    std::shared_ptr<Puzzle> p( new Puzzle );

    FILE_LOG(logINFO) << "Begin parsing stream.";

    // parse all char's to ints for each Cell
    char curr;
    for ( size_t y = 1; y <= 9; y++ )
    {
        for ( size_t x = 1; x <= 9; x++ )
        {
            // skip over whitespace
            do
            {
                in >> curr;
                FILE_LOG(logDEBUG2) << "Read character: '" << curr << "'";
            } while ( in &&
                      ( curr == ' ' || curr == '\n' || curr == '\t' ) );
            if ( !in )
            {
                FILE_LOG(logWARNING)
                    << "Reached end of file while still filling puzzle";
                throw std::runtime_error( "End of file reached." );
            }
            FILE_LOG(logDEBUG1) << " Handling character: '" << curr << "'";
            // check that curr is an int
            if ( curr < '0' || curr > '9' )
            {
                FILE_LOG(logWARNING) << "The character was non-numeric.";
                throw std::domain_error( "Found non-numeric value" );
            }
            int val = curr - '0';
            FILE_LOG(logDEBUG2) << "Character converted to: " << val;

            // get show/hide
            // skip over whitespace
            do
            {
                in >> curr;
                FILE_LOG(logDEBUG2) << "Read character: '" << curr << "'";
            } while ( in &&
                      ( curr == ' ' || curr == '\n' || curr == '\t' ) );
            if ( !in )
            {
                FILE_LOG(logWARNING)
                    << "Reached end of file while still filling puzzle";
                throw std::runtime_error( "End of file reached." );
            }
            FILE_LOG(logDEBUG1) << " Handling character: '" << curr << "'";
            // check that curr is an int
            if ( curr != _show && curr != _hide )
            {
                FILE_LOG(logWARNING) << "The character was not show or hide.";
                throw std::domain_error( "Character was not show or hide" );
            }

            std::shared_ptr<Cell> cell = p->GetCell( x, y );
            cell->SetCorrect( val );
            cell->Display( curr == _show );
            FILE_LOG(logINFO) << "Set correct value for cell at: "
                              << x << ", " << y;
        }
    }


    return p;
}

bool SolvedPuzzleImporter::CanHandleExtension( const std::string &name )
{
    size_t dot = name.find_last_of( '.' );
    return ( name.substr( dot + 1 ) == "solved" );
}

}
