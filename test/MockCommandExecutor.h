#ifndef SUDOKU_MOCK_COMMAND_EXECUTOR_H
#define SUDOKU_MOCK_COMMAND_EXECUTOR_H

#include "gmock/gmock.h"

#include "../ICommandExecutor.h"

namespace Sudoku
{

class MockCommandExecutor : public ICommandExecutor
{
public:
    MOCK_METHOD1( Execute, bool ( std::shared_ptr<CommandBase> command ) );

    MOCK_CONST_METHOD0( CanUndo, bool () );

    MOCK_METHOD0( Undo, bool () );

    MOCK_CONST_METHOD0( CanRedo, bool () );

    MOCK_METHOD0( Redo, bool () );

    MOCK_METHOD1( AddObserver, void ( std::shared_ptr<ICommandObserver> o ) );

    MOCK_METHOD1( RemoveObserver, void ( std::shared_ptr<ICommandObserver> o ) );

};

}

#endif
