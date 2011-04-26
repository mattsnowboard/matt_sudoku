#include "GameManager.h"
#include "Cell.h"
#include "Puzzle.h"
#include "Command.h"
#include "SimplePuzzleImporter.h"
#include "SolvedPuzzleImporter.h"
#include <fstream>
#include <stdexcept>

namespace Sudoku
{

GameManager::GameManager()
    : _lastExecuted( false ), _isExecute( false )
{
    std::shared_ptr<IPuzzleImporter> temp( new SolvedPuzzleImporter );
    _importers.push_back( temp );
    temp.reset( new SimplePuzzleImporter );
    _importers.push_back( temp );
}

bool GameManager::ImportFromFile( const std::string &filename )
{
    std::ifstream filestream( filename );
    if ( !filestream )
    {
        // failed
        return false;
    }
    for ( ImporterContainer::iterator it = _importers.begin();
          it != _importers.end();
          ++it )
    {
        if ( (*it)->CanHandleExtension( filename ) )
        {
            try
            {
                std::shared_ptr<Puzzle> p = (*it)->Import( filestream );
                if ( p )
                {
                    _puzzle.reset();
                    _puzzle = p;
                    return true;
                }
            }
            catch ( ... )
            {
                continue;
            }
        }
    }

    // could not load file in any importer we had
    return false;
}

bool GameManager::Execute( std::shared_ptr<CommandBase> command )
{
    if ( !_puzzle )
    {
        throw std::runtime_error( "Cannot execute Command without a Puzzle." );
    }
    _lastExecuted = false;
    _isExecute = true;
    command->Accept( *this );
    if ( _lastExecuted )
    {
        _undo.push( command );
        // clear redo
        while ( !_redo.empty() ) { _redo.pop(); }
    }
    return _lastExecuted;
}

bool GameManager::Undo()
{
    if ( !_puzzle )
    {
        throw std::runtime_error( "Cannot undo Command without a Puzzle." );
    }
    if ( !CanUndo() )
    {
        return false;
    }
    std::shared_ptr<CommandBase> command = _undo.top();
    _lastExecuted = false;
    _isExecute = false;
    command->Accept( *this );
    if ( _lastExecuted )
    {
        // move to proper stack
        _undo.pop();
        _redo.push( command );
    }
    return _lastExecuted;
}

bool GameManager::Redo()
{
    if ( !_puzzle )
    {
        throw std::runtime_error( "Cannot redo Command without a Puzzle." );
    }
    if ( !CanRedo() )
    {
        return false;
    }
    std::shared_ptr<CommandBase> command = _redo.top();
    _lastExecuted = false;
    _isExecute = true;
    command->Accept( *this );
    if ( _lastExecuted )
    {
        // move to proper stack
        _redo.pop();
        _undo.push( command );
    }
    return _lastExecuted;
}

void GameManager::HandleCommand( Command<Cell> &command )
{
    std::shared_ptr<Cell> c = _puzzle->GetCell( command.GetTarget() );
    _lastExecuted = command.Execute( c );
}

void GameManager::HandleCommand( Command<Puzzle> &command )
{

}

void GameManager::HandleCommand( CommandBase &command )
{
    throw std::runtime_error( "Improper Command Type Found." );
}

}
