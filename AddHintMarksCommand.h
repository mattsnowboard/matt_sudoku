#ifndef SUDOKU_ADD_HINT_MARKS_COMMAND_H
#define SUDOKU_ADD_HINT_MARKS_COMMAND_H

#include "Command.h"
#include <memory>
#include <vector>
#include "Cell.h"

namespace Sudoku
{

class Puzzle;
class IPuzzleMarker;

/**
 * A command to add hints to a Puzzle
 */
class AddHintMarksCommand : public Command<Puzzle>
{
public:
    /**
     * Force user to create shared_ptr's
     * @param puzzle The Puzzle to add hint marks to
     */
    static std::shared_ptr<Command<Puzzle> > CreateAddHintMarksCommand(
        std::shared_ptr<const Puzzle> puzzle,
        std::shared_ptr<IPuzzleMarker> marker );

    virtual ~AddHintMarksCommand() {}

private:
    /**
     * Create the Command
     * @param puzzle The Puzzle to add hint marks to
     */
    AddHintMarksCommand( std::shared_ptr<const Puzzle> Puzzle,
                         std::shared_ptr<IPuzzleMarker> marker );

    /**
     * Use the Marker to get the hints
     * @pre we can execute
     * @param non-const Puzzle to execute on
     * @return Success or Failure
     * @post we can unexecute
     */
    virtual bool execute( std::shared_ptr<Puzzle> p );

    /**
     * Undo the hint marks (restore every Cell's previous mark
     * @pre we can unexecute
     * @param non-const Cell to execute on
     * @return Success or Failure
     * @post we can execute
     */
    virtual bool unexecute( std::shared_ptr<Puzzle> p );

private:
    /// Marker can add hints
    std::shared_ptr<IPuzzleMarker> _marker;
    /// Need to store old values (Memento)
    std::vector<Cell::MarkContainer> _oldMarks;
};

}

#endif
