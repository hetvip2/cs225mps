/**
 * @file puzzle.cpp
 * Implementation of puzzle class.
 */
#include "puzzle.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>
 /**
* Default constructor for the puzzle state. This should initialize the
* puzzle to the solved state.
*/
PuzzleState::PuzzleState() {
    puzz[15] = 0;

    for (auto i = 0; i < 15; i++) {
        puzz[i] = i + 1;
    }
     
}
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
PuzzleState::PuzzleState(const std::array<char, 16> state) {
    //keeps track of valid visited states
    //initially all initialized to false
    std::array<bool, 16> visited = {false};
    
    //check if within bounds
    for (auto t : state) {
        if (t < 0) {
            throw std::invalid_argument("invalid state - below");
        }
        if (t >= 16) {
            throw std::invalid_argument("invalid state - too high");
        }
        
        visited[t] = true;
    }
    //set updated
    puzz = state;
}

   /**
    * Convert the puzzle state to an array.
    * @return an array representing the state of the puzzle in the same format
    * as described in the constructor.
    */
std::array<char, 16> PuzzleState::asArray() const {
    return puzz;
}
   
    /**
    * Overloaded operator== for the puzzle state. Puzzles are equal when the
    * value of each tile is the same.
    * @param rhs The puzzle state to compare to
    */

bool PuzzleState::operator==(const PuzzleState& rhs) const {
    return puzz == rhs.puzz;
}

    /**
    * Overloaded operator!= for the puzzle state.
    * @param rhs The puzzle state to compare to
    */
bool PuzzleState::operator!=(const PuzzleState& rhs) const {
    return !(*this == rhs);
}

    /**
    * Overloaded operator< for the puzzle state. The PuzzleState with the first
    * tile value less than the corresponding tile in the other PuzzleState is
    * considered less, you can assume both puzzle states are valid.
    * @param rhs The puzzle state to compare to
    */
bool PuzzleState::operator<(const PuzzleState& rhs) const {
    return puzz < rhs.puzz;
}

    /**
    * Get the neighbor specified by the direction. If the direction refers to an
    * invalid neighbor, return PuzzleState representing all zeros.
    * @param direction The direction to move a tile (e.x. UP means the empty
    * space should move down).
    */
PuzzleState PuzzleState::getNeighbor(Direction direction) const {
    
    //calculate the empty spot
    auto start = puzz.begin();
    auto space = std::find(start, puzz.end(), 0);
    
    int empty = space - start;

    //{UP, DOWN, LEFT, RIGHT}
    const int shifts[] = {4, -4, 1, -1};
    
    int idx = empty + shifts[static_cast<int>(direction)];

    auto left_invalid = (empty % 4 == 0 && direction == Direction::RIGHT);
    auto right_invalid = (empty % 4 == 3 && direction == Direction::LEFT);
    
    //Get the neighbor specified by the direction. If the direction refers to an invalid neighbor, return PuzzleState representing all zeros.
    if (idx < 0 || idx >= 16 || left_invalid || right_invalid) {
        return PuzzleState(std::array<char, 16>{0});
    }

    // swap empty and valid index
    std::array<char, 16> next = puzz;

    std::swap(next[empty], next[idx]);

    return PuzzleState(next);
}


    /**
    * Gets all possible PuzzleStates that result from a single move.
    * @return All possible next PuzzleStates in any order
    */
std::vector<PuzzleState> PuzzleState::getNeighbors() const {
    //vector to store all puzzstates
    std::vector<PuzzleState> puzzstates;
    
    auto directions = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    auto empty = PuzzleState(std::array<char, 16>{0});

    //check all directions
    for (auto dir : directions) {
        auto neighbor = getNeighbor(dir);
        //check valid
        if (neighbor != empty) {
            puzzstates.push_back(neighbor);
        }
    }
    return puzzstates;
}

    /**
    * Calculates the "manhattan distance" between the current state and the goal
    * state. This is the sum of the manhattan distances of each tile's current
    * location to its goal location.
    * @param desiredState The state to calculate the distance to
    * @return The manhattan distance between the current and goal states
    */
int PuzzleState::manhattanDistance(const PuzzleState desiredState) const {
    int sum = 0;
    for (auto i = 0; i <= 15; i++) {
        auto target = puzz[i];
        //validity check
        if (target != 0) {

            auto start = desiredState.puzz.begin();
            auto idx = std::find(start, desiredState.puzz.end(), target) - start;
            
            //use index to find goal
            int goal_row = idx / 4;
            int goal_col = idx % 4;
            
            //find curr
            auto curr_row = i / 4;
            auto curr_col = i % 4;
            
            //calc diff
            auto rowdiff = std::abs(curr_row - goal_row);
            auto coldiff = std::abs(curr_col - goal_col);
            
            auto distance = rowdiff + coldiff;
            
            //add to sum
            sum += distance;
        }
    }
    return sum;
}

// set and map hash function
namespace std {
    template <>
    struct hash<PuzzleState> {
        size_t operator()(const PuzzleState& state) const {
            size_t hash = 0;
            auto arr = state.asArray();
            for (size_t i = 0; i < arr.size(); i++) {
                hash = (hash * 31) + arr[i];
            }
            return hash;
        }
    };
}

//
std::vector<PuzzleState> pathfix (
    const std::unordered_map<PuzzleState, PuzzleState>& map, 
    PuzzleState curr
) {
    std::vector<PuzzleState> vecPath;
    
    vecPath.push_back(curr);
    
    while (map.find(curr) != map.end()) {
        curr = map.at(curr);
        vecPath.push_back(curr);
    }

    auto start = vecPath.begin();
    auto end = vecPath.end();
    
    std::reverse(start, end);
    
    return vecPath;
}

std::vector<PuzzleState> solveBFS(
    const PuzzleState& startState,
    const PuzzleState& desiredState,
    size_t* iterations
) {
    if (iterations){
        *iterations = 0;
    } 

    std::unordered_set<PuzzleState> visitedset;
    std::unordered_map<PuzzleState, PuzzleState> before;
    std::queue<PuzzleState> stateq;


    visitedset.insert(startState);

    stateq.push(startState);


    while (!stateq.empty()) {
        
        if (iterations){
            (*iterations)++;
        } 

        PuzzleState curr = stateq.front();
        
        stateq.pop();

        // see if its the goal
        if (curr == desiredState) {
            return pathfix(before, curr);
        }

        // check each neighbor
        for (const auto& neighbor : curr.getNeighbors()) {
            if (visitedset.find(neighbor) != visitedset.end()) {
                continue;
            } 

            stateq.push(neighbor);

            // if its added to stateq add it to visitedset
            visitedset.insert(neighbor);
            before[neighbor] = curr;
        }
    }

    // if no solution
    return {};
}

std::vector<PuzzleState> solveAstar(
    const PuzzleState& startState,
    const PuzzleState& desiredState,
    size_t* iterations
) {
    if (iterations) {
        *iterations = 0;
    } 

    // comparator for priority queue
    auto compare = [](const std::pair<int, PuzzleState>& one,
                      const std::pair<int, PuzzleState>& two) {
        return one.first > two.first;
    };

    std::priority_queue<
        std::pair<int, PuzzleState>,
        std::vector<std::pair<int, PuzzleState>>,
        decltype(compare)
    > stateq(compare);

    std::unordered_map<PuzzleState, int> mapscore;

    std::unordered_map<PuzzleState, PuzzleState> before;

    std::unordered_set<PuzzleState> visitedset;

    // first start state
    mapscore[startState] = 0;
    
    int f = startState.manhattanDistance(desiredState);
   
    stateq.push({f, startState});

    while (!stateq.empty()) {
        if (iterations) {
            (*iterations)++;
        } 

        PuzzleState curr = stateq.top().second;
        
        stateq.pop();

        // skip visitedset nodes
        if (visitedset.find(curr) != visitedset.end()){
            continue;
        } 
        // check if goal
        if (curr == desiredState) {
            return pathfix(before, curr);
        }

        visitedset.insert(curr);

        // iterate through each neighbor
        for (const auto& neighbor : curr.getNeighbors()) {
            if (visitedset.find(neighbor) != visitedset.end()){
                continue;
            } 

            // temporary mapscore
            int temp = mapscore[curr] + 1;

            if (mapscore.find(neighbor) == mapscore.end() || temp < mapscore[neighbor]) {
                before[neighbor] = curr;
                mapscore[neighbor] = temp;


                int total = temp + neighbor.manhattanDistance(desiredState);

                stateq.push({total, neighbor});
            }
        }
    }

    // if no sol, empty
    return {};
}
