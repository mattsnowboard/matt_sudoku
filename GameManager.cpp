#include "GameManager.h"

#include "Cell.h"
#include "CellController.h"
#include "Command.h"
#include "GameController.h"
#include "MethodSolver.h"
#include "Puzzle.h"
#include "PuzzleController.h"
#include "PuzzleMarker.h"
#include "SimplePuzzleImporter.h"
#include "SimpleValidator.h"
#include "SolutionMethodFactory.h"
#include "SolvedPuzzleImporter.h"
#include "SolverHelper.h"

#include <fstream>
#include <stdexcept>

namespace Sudoku
{

std::shared_ptr<GameManager> GameManager::Create()
{
    std::shared_ptr<GameManager> gm( new GameManager );

    std::shared_ptr<GameController> gc( new GameController( gm, gm ) );

    std::shared_ptr<IPuzzleMarker> marker( new PuzzleMarker );
    std::shared_ptr<SolutionMethodFactory> factory(
        new SolutionMethodFactory );
    std::shared_ptr<SolverHelper> helper( new SolverHelper( factory ) );
    std::shared_ptr<IValidator> validator =
        Sudoku::SimpleValidator::CreateGuessValidator();

    std::shared_ptr<ISolver> solver(
        new MethodSolver( helper, marker, validator ) );

    std::shared_ptr<PuzzleController> pc(
        new PuzzleController( gm, gm, marker, solver ) );

    std::shared_ptr<CellController> cc( new CellController( gm, gm ) );

    gm->addGameController( gc );
    gm->addPuzzleController( pc );
    gm->addCellController( cc );

    return gm;
}

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
    for ( ImporterContainer::iterator it = _importers.begin();
          it != _importers.end();
          ++it )
    {
        if ( (*it)->CanHandleExtension( filename ) )
        {
            try
            {
                std::ifstream filestream( filename );
                if ( !filestream )
                {
                    // failed
                    return false;
                }
                std::shared_ptr<Puzzle> p = (*it)->Import( filestream );
                if ( p )
                {
                    _puzzle.reset();
                    _puzzle = p;
                    clearUndo();
                    clearRedo();
                    attachAllCellObservers();
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

void GameManager::NewPuzzle()
{
    _puzzle.reset( new Puzzle );
    clearUndo();
    clearRedo();
    attachAllCellObservers();
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
        clearRedo();
        notifyCanUndo( true );
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
        notifyCanRedo( true );
        notifyCanUndo( !_undo.empty() );
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
        notifyCanRedo( !_redo.empty() );
        notifyCanUndo( true );
    }
    return _lastExecuted;
}

void GameManager::AddObserver( std::shared_ptr<ICommandObserver> o )
{
    if ( !o )
    {
        throw std::runtime_error( "Cannot add NULL observer." );
    }
    _observers.push_back( o );
}

void GameManager::RemoveObserver( std::shared_ptr<ICommandObserver> o )
{
    if ( !o )
    {
        throw std::runtime_error( "Cannot remove NULL observer." );
    }
    ObserverContainer::iterator found = std::find( _observers.begin(),
                                                   _observers.end(),
                                                   o );
    if ( found == _observers.end() )
    {
        throw std::runtime_error(
            "Attempt to remove observer not listening to Cell." );
    }
    _observers.erase( found );
}

void GameManager::HandleCommand( Command<Cell> &command )
{
    std::shared_ptr<Cell> c = _puzzle->GetCell( command.GetTarget() );
    if ( _isExecute )
    {
        _lastExecuted = command.Execute( c );
    }
    else
    {
        _lastExecuted = command.Unexecute( c );
    }
}

void GameManager::HandleCommand( Command<Puzzle> &command )
{
    if ( _isExecute )
    {
        _lastExecuted = command.Execute( _puzzle );
    }
    else
    {
        _lastExecuted = command.Unexecute( _puzzle );
    }
}

void GameManager::HandleCommand( CommandBase &command )
{
    throw std::runtime_error( "Improper Command Type Found." );
}

std::shared_ptr<const Puzzle> GameManager::GetPuzzle() const
{
//    if ( !_puzzle )
//    {
//        throw std::runtime_error( "Attempt to access NULL Puzzle!" );
//    }
    return _puzzle;
}

void GameManager::ListenToAllCells( std::shared_ptr<ICellObserver> o )
{
    _cellObservers.push_back( o );
    // do we already have a puzzle?
    if ( _puzzle )
    {
        attachCellObserver( o );
    }
}

void GameManager::addGameController(
    std::shared_ptr<GameController> gameController )
{
    if ( !gameController )
    {
        throw std::runtime_error( "Attempt to pass NULL GameController" );
    }
    _gameController = gameController;
}

void GameManager::addPuzzleController(
        std::shared_ptr<PuzzleController> puzzleController )
{
    if ( !puzzleController )
    {
        throw std::runtime_error( "Attempt to pass NULL PuzzleController" );
    }
    _puzzleController = puzzleController;
}

void GameManager::addCellController(
    std::shared_ptr<CellController> cellController )
{
    if ( !cellController )
    {
        throw std::runtime_error( "Attempt to pass NULL CellController" );
    }
    _cellController = cellController;
}

void GameManager::notifyCanUndo( bool yes )
{
    for ( ObserverContainer::iterator it = _observers.begin();
          it != _observers.end();
          ++it )
    {
        (*it)->CanUndo( yes );
    }
}

void GameManager::notifyCanRedo( bool yes )
{
    for ( ObserverContainer::iterator it = _observers.begin();
          it != _observers.end();
          ++it )
    {
        (*it)->CanRedo( yes );
    }
}

void GameManager::clearUndo()
{
    while ( !_undo.empty() ) { _undo.pop(); }
    notifyCanUndo( false );
}

void GameManager::clearRedo()
{
    while ( !_redo.empty() ) { _redo.pop(); }
    notifyCanRedo( false );
}

void GameManager::attachAllCellObservers()
{
    for ( CellObserverContainer::iterator it = _cellObservers.begin();
          it != _cellObservers.end();
          ++it )
    {
        attachCellObserver( *it );
    }
}

void GameManager::attachCellObserver( std::shared_ptr<ICellObserver> o )
{
    Puzzle::Container all = _puzzle->GetAllCells();
    for ( Puzzle::Container::iterator it = all.begin();
          it != all.end();
          ++it )
    {
        (*it)->AddObserver( o );
    }
}

}
