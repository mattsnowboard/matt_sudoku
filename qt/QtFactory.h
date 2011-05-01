#ifndef QT_SUDOKU_FACTORY_H
#define QT_SUDOKU_FACTORY_H

#include <memory>

namespace Sudoku
{
class GameManager;
}

namespace QtSudoku
{

class QtDirector;

class QtFactory
{
public:
    QtFactory() {}

    std::shared_ptr<QtDirector> CreateDirector( int &argc, char **argv );

private:
    std::shared_ptr<Sudoku::GameManager> _manager;
};

}

#endif
