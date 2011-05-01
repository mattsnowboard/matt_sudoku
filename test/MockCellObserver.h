#ifndef SUDOKU_MOCK_CELL_OBSERVER_H
#define SUDOKU_MOCK_CELL_OBSERVER_H

#include "gmock/gmock.h"

#include "../ICellObserver.h"

namespace Sudoku
{

/**
 * Class to observer Cell updates
 */
class MockCellObserver : public ICellObserver
{
public:

    MOCK_METHOD1( Update, void ( const Cell &c ) );

};

}

#endif
