#ifndef SUDOKU_ICOMMAND_DISPATCHER_H
#define SUDOKU_ICOMMAND_DISPATCHER_H

namespace Sudoku
{

class Cell;
class Puzzle;
class CommandBase;
template <class T>
class Command;

class ICommandDispatcher
{
public:
    virtual void HandleCommand( Command<Cell> &c ) = 0;

    virtual void HandleCommand( Command<Puzzle> &p ) = 0;

    virtual void HandleCommand( CommandBase &b ) = 0;

    virtual ~ICommandDispatcher() {}
};

}

#endif
