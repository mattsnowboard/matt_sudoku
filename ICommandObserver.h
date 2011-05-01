#ifndef SUDOKU_ICOMMAND_OBSERVER_H
#define SUDOKU_ICOMMAND_OBSERVER_H

namespace Sudoku
{

class ICommandObserver
{
public:
    /**
     * Notification that Undo is possible
     * @param yes whether or not it is possible
     */
    virtual void CanUndo( bool yes ) = 0;

    /**
     * Notifaction that Redo is possible
     * @param yes whether or not it is possible
     */
    virtual void CanRedo( bool yes ) = 0;

    virtual ~ICommandObserver() {}
};

}

#endif
