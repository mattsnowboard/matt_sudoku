#ifndef SUDOKU_ICELL_OBSERVER_H
#define SUDOKU_ICELL_OBSERVER_H

namespace Sudoku
{

class Cell;

/**
 * Class to observer Cell updates
 */
class ICellObserver
{
public:
    /**
     * When a Cell changes, it notifies its observers and passes
     * itself so they know which Cell changed
     * @param c The Cell that changed
     */
    virtual void Update( const Cell &c ) = 0;

    virtual ~ICellObserver() {}
};

}

#endif
