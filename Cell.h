#ifndef SUDOKU_CELL_H
#define SUDOKU_CELL_H

#include <bitset>
#include <memory>
#include <set>
#include <stdexcept>
#include <vector>

namespace Sudoku
{

class  ICellObserver;

struct Position
{
    Position( size_t x_ = 0, size_t y_ = 0 ) : x( x_ ), y( y_ ) {}
    size_t x;
    size_t y;

    bool operator<( const Position &p ) const
    {
        return ( ( y < p.y ) || ( y == p.y && x < p.x ) );
    }
};

/**
 * Cell in a Sudoku board
 * Each cell can hold a value 1 through 9 (or 0 for blank)
 * Cells have a correct value, which may or may not be shown,
 * and a guessed value
 * Users may mark a cell with guesses
 */
class Cell
{
public:
    
    typedef std::bitset<10> MarkContainer;
    typedef std::set<int> MarkedValues;
    typedef std::vector<std::shared_ptr<ICellObserver> > ObserverContainer;

    /**
     * Default constructor just initializes everything to 0
     */
    Cell();

    /**
     * Allow copying a Cell
     * @param c Cell to copy
     * @post this Cell has same attributes as c
     */
    Cell( const Cell& c );

    /**
     * Copy all values from c to this Cell
     * @param c Cell to copy
     * @return this for method chaining
     */
    Cell& operator=( const Cell& c );

    /**
     * Set the position of the Cell on the board
     * @param x X index position (column number)
     * @param y Y index position (row number)
     * @pre x,y are each in range(1,9)
     */
    void SetPos( size_t x, size_t y );

    /**
     * Set the position of the Cell on the board
     * @param p x,y coordinate for column and row
     * @pre x,y are each in range(1,9)
     */
    void SetPos( Position p );

    /**
     * Get Cell's x position (column) on board
     * @return x
     */
    size_t GetX() const { return _pos.x; }

    /**
     * Get Cell's y position (row) on board
     * @return y
     */
    size_t GetY() const { return _pos.y; }

    /**
     * Get Cell's x and y position on board
     */
    Position GetPos() const { return _pos; }

    /**
     * Set the Correct value
     * @param correct The new correct value of this Cell
     * @pre correct is in {0...9}
     * @post _correctVal == correct
     * @post Notify observers if changed
     */
    void SetCorrect( int correct );

    /**
     * Set the Guessed value
     * @param guess The new guessed value of this Cell
     * @pre guess is in {0...9}
     * @post _guessedVal == guess
     * @post Notify observers if changed
     */
    void SetGuess( int guess );

    /**
     * Set the display flag
     * This determines if the correct answer is shown to the player
     * @param display Boolean value to show it or hide it
     * @post _displayCorrect is set to display
     * @post Notify observers if changed
     */
    void Display( bool display );

    /**
     * Mark
     * This marks a value as a users guess or a hint
     * @param mark Integer value to set the mark on
     * @pre mark is in {1...9}
     * @post _marks will set the value given here
     * @post Notify observers if changed
     */
    void Mark( int mark );

    /**
     * Unmark
     * This unmarks a value as a users guess or a hint
     * @param mark Integer value to clear the mark on
     * @pre mark is in {1...9}
     * @post _marks will clear the value given here
     * @post Notify observers if changed
     */
    void Unmark( int mark );

    /**
     * Clear marks
     * Clears all marked values
     * @post _marks has all values false
     * @post Notify observers if changed
     */
    void ClearMarks();

    /**
     * Mark all
     * Sets all marks for all values
     * @post _marks has all values true
     * @post Notify observers if changed
     */
    void MarkAll();

    /**
     * Check whether the correct value is already displayed
     * This is used by games to know what Cells users are allowed to guess
     * @return True if the correct value is not displayed
     */
    bool CanGuess() const { return !_displayCorrect; }

    /**
     * Get the value to display to the user
     * This will be the guess or the correct value if that is set to display
     * @return correct value if _displayCorrect == true else guessed value
     */
    int DisplayedValue() const;

    /**
     * Get the actual correct value even if it is hidden
     * This allows algorithms to look at the real answer even if it is hidden
     * Mostly used for validating puzzles
     * @return Actual correct value (if set)
     */
    int GetCorrectValue() const { return _correctVal; }

    /**
     * Check if the guessed value is correct
     * @note This will be false if _correctVal == 0
     * @return true if _correctVal == _guessedVal != 0 or correct is displayed
     */
    bool IsCorrect() const;

    /**
     * Get the Marks on this Cell
     * @return a Container where each mark is a bool at an index
     */
    const MarkContainer& GetMarkContainer() const { return _marks; }

    /**
     * Set or clear all Marks
     * @param m A MarkContainer defining the Marks you want
     * @post The Cell's marks are set to m
     * @post Notify observers if changed
     */
    void SetMarkContainer( const MarkContainer &m );

    /**
     * Get the values that are marked only
     * @note ignore mark on 0
     * @return vector of values that have marks
     */
    MarkedValues GetMarkedValues() const;

    /**
     * Add an observer to listen for changes to this Cell
     * @param o Observer to add
     * @post o gets updates
     */
    void AddObserver( std::shared_ptr<ICellObserver> o );

    /**
     * Remove an observer to stop listening for changes to this Cell
     * @param o Observer to remove
     * @pre o was observing Cell
     * @post o does not get updates
     */
    void RemoveObserver( std::shared_ptr<ICellObserver> o );

    /**
     * Notify all observers that something in this Cell changed
     * @pre Cell changed
     * @post All observers listening get notified
     */
    void NotifyObservers();

    /**
     * Check for ABSOLUTE equality
     * @param c The Cell to compare with
     * @return true if the Cells have the same data
     */
    bool operator==( const Cell& c ) const;

    /**
     * No cleanup needed
     */
    ~Cell() {}

    /**
     * Validate that input is in the range of cells (inclusive)
     * @param i, value to check
     * @param low, lowest value allowed
     * @param high, highest value allowed
     * @throw if I is outside range [low,high]
     */
    static void Validate( int i, int low = 1, int high = 9 );

private:

    /**
     * Used for debugging to print all of the data
     */
    friend std::ostream& operator<<( std::ostream &os, const Cell &c );

    /// x,y location on the Sudoku board {1...9}
    Position _pos;
    /// The correct value for this Cell which a user should try to guess
    int _correctVal;
    /// The value that a user has guessed
    int _guessedVal;
    /// Whether the correct value is displayed initially (no guessing needed)
    bool _displayCorrect;
    /// Allows user to mark a cell with possible values it can take
    MarkContainer _marks;

    ObserverContainer _observers;
};

// Attempt to inline this for performance
inline void Cell::Validate( int i, int low, int high )
{
    if ( i > high || i < low )
    {
        throw std::range_error( "Out of range value for Cell!" );
    }
}

}

#endif
