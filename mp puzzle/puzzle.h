/**
 * @file puzzle.h
 * Implementation of puzzle class.
 */
#pragma once
#include <vector>
#include <array>
#include <iostream>

class PuzzleState {
public:
    enum class Direction {
        UP,   // Refers to moving a tile up (i.e. the empty space goes down.)
        DOWN, // Refers to moving a tile down (i.e. the empty space goes up.)
        LEFT, // Refers to moving a tile left (i.e. the empty space goes right.)
        RIGHT // Refers to moving a tile right (i.e. the empty space goes left.)
    };

    /**
    * Default constructor for the puzzle state. This should initialize the
    * puzzle to the solved state.
    */
    PuzzleState();

    /**
    * Custom constructor for the puzzle state. Invalid inputs should initialize
    * the puzzle representing all zeros. A char is used to represent the value
    * of each tile, where 0 represents the empty space, it is also typically how
    * a byte is represented in C/C++. Almost always, a char will represent a octet
    * (8 bits), which allow for 256 possible values, in this case, since each
    * tile can only have a value from 0-15 (4-bits), using a char is sufficient.
    *
    * @param state The starting state of the puzzle. The first entry in the
    * array is the top left tile, then the remaining entries are in row-major
    * order. The following is a depiction of a puzzle where the value of each
    * tile represents its index in the array:
    * 0  1  2  3
    * 4  5  6  7
    * 8  9  10 11
    * 12 13 14 15
    */
    PuzzleState(const std::array<char, 16> state);

    /**
    * Convert the puzzle state to an array.
    * @return an array representing the state of the puzzle in the same format
    * as described in the constructor.
    */
    std::array<char, 16> asArray() const;

    /**
    * Overloaded operator== for the puzzle state. Puzzles are equal when the
    * value of each tile is the same.
    * @param rhs The puzzle state to compare to
    */
    bool operator==(const PuzzleState &rhs) const;

    /**
    * Overloaded operator!= for the puzzle state.
    * @param rhs The puzzle state to compare to
    */
    bool operator!=(const PuzzleState &rhs) const;

    /**
    * Overloaded operator< for the puzzle state. The PuzzleState with the first
    * tile value less than the corresponding tile in the other PuzzleState is
    * considered less, you can assume both puzzle states are valid.
    * @param rhs The puzzle state to compare to
    */
    bool operator<(const PuzzleState &rhs) const;

    /**
    * Get the neighbor specified by the direction. If the direction refers to an
    * invalid neighbor, return PuzzleState representing all zeros.
    * @param direction The direction to move a tile (e.x. UP means the empty
    * space should move down).
    */
    PuzzleState getNeighbor(Direction direction) const;

    /**
    * Gets all possible PuzzleStates that result from a single move.
    * @return All possible next PuzzleStates in any order
    */
    std::vector<PuzzleState> getNeighbors() const;

    /**
    * Calculates the "manhattan distance" between the current state and the goal
    * state. This is the sum of the manhattan distances of each tile's current
    * location to its goal location.
    * @param desiredState The state to calculate the distance to
    * @return The manhattan distance between the current and goal states
    */
    int manhattanDistance(const PuzzleState desiredState = PuzzleState()) const;


private:
    std::array<char, 16> puzz;
};

/**
* Solves the puzzle using BFS.
* @param startState The starting state of the puzzle
* @param desiredState The final goal state of the puzzle after solving
* @param iterations The number of iterations it took to solve the puzzle. An
* iteration is defined as the number of times a state is popped from the data
* structure (NOTE: this should include the start and desired states, but not any
* states that are immediately discarded, if applicable). We will use the value
* stored at this pointer to evaluate efficiency. Ignore if NULL.
* @return The path to the solution. The first element of the vector is the start
* state, and the last element is the desired state. Empty if no solution exists.
*/
std::vector<PuzzleState> solveBFS(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations = NULL);

/**
* Solves the puzzle using A* with manhattan distance as a heuristic.
* @param startState The starting state of the puzzle
* @param desiredState The final goal state of the puzzle after solving
* @param iterations The number of iterations it took to solve the puzzle. An
* iteration is defined as the number of times a state is popped from the data
* structure (NOTE: this should include the start and desired states, but not any
* states that are immediately discarded, if applicable). We will use the value
* stored at this pointer to evaluate efficiency. Ignore if NULL.
* @return The path to the solution. The first element of the vector is the start
* state, and the last element is the desired state. Empty if no solution exists.
*/
std::vector<PuzzleState> solveAstar(const PuzzleState& startState, const PuzzleState &desiredState, size_t *iterations = NULL);

/**
 * Overloaded operator<< for the puzzle state, you can use this to print the puzzle.
 */
inline std::ostream &operator<<(std::ostream &os, const PuzzleState &puzzle) {
    std::array<char, 16> arr = puzzle.asArray();
    for (size_t i = 0; i < 16; ++i) {
        if (arr[i] < 10)
            os << " ";
        os << (int)arr[i] << " ";
        if ((i + 1) % 4 == 0) {
            os << std::endl;
        }
    }
    return os;
}

