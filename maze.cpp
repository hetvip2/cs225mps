/* Your code here! */
#include "maze.h"
#include "cs225/PNG.h"
#include <queue>
#include <algorithm>

//empty maze means w=0, h=0
  SquareMaze::SquareMaze(): width(0), height(0) {}

  /**
   * Makes a new SquareMaze of the given height and width.
   *
   * If this object already represents a maze it will clear all the
   * existing data before doing so. You will start with a square grid (like
   * graph paper) with the specified height and width. You will select
   * random walls to delete without creating a cycle, until there are no
   * more walls that could be deleted without creating a cycle. Do not
   * delete walls on the perimeter of the grid.
   *
   * Hints: You only need to store 2 bits per square: the "down" and
   * "right" walls. The finished maze is always a tree of corridors.)
   *
   * @param width The width of the SquareMaze (number of cells)
   * @param height The height of the SquareMaze (number of cells)
   */
  void SquareMaze::makeMaze(int width, int height){
    
    this->width = width;
    this->height = height;

    auto cells = width * height;

    rightWalls.resize(cells, true);
    downWalls.resize(cells, true);

    DisjointSets s;
    
    s.addelements(cells);

    std::vector<std::pair<int, int>> connects;

    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        //right neighbor
        if (col < width - 1) {
            int curr = row * width + col;
            int right = row * width + col + 1;
            connects.push_back({curr, right});
        }
        
        //bottom neighbor
        if (row < height - 1) {
            int curr = row * width + col;
            int bot = (row + 1) * width + col;
            connects.push_back({curr, bot});
        }
    }
}
    //generate random maze
    std::random_shuffle(connects.begin(), connects.end());

    for (const auto& c : connects) {
        int curr = c.first;
        int next = c.second;

        if (s.find(curr) != s.find(next)) {
            
            s.setunion(curr, next);
            
            int x1 = curr % width;
            int y1 = curr / width;
            int x2 = next % width;
            int y2 = next / width;

            if (x1 == x2) {
                downWalls[std::min(y1, y2) * width + x1] = false;
            } else {
                rightWalls[y1 * width + std::min(x1, x2)] = false;
            }
        }
    }
  }

  /**
   * This uses your representation of the maze to determine whether it is
   * possible to travel in the given direction from the square at
   * coordinates (x,y).
   *
   * For example, after makeMaze(2,2), the possible input coordinates will
   * be (0,0), (0,1), (1,0), and (1,1).
   *
   * You can not step off of the maze or through a wall.
   *
   * This function will be very helpful in solving the maze. It will also
   * be used by the grading program to verify that your maze is a tree that
   * occupies the whole grid, and to verify your maze solution. So make
   * sure that this function works!
   *
   * @param x The x coordinate of the current cell
   * @param y The y coordinate of the current cell
   * @param dir The desired direction to move from the current cell
   * @return whether you can travel in the specified direction
   */
  bool SquareMaze::canTravel(int x, int y, Direction dir) const{
    bool canMove = false;

    if (dir == Direction::RIGHT) {
        if (x < width - 1 && !rightWalls[y * width + x]) {
            canMove = true;
        }
    } 
    else if (dir == Direction::DOWN) {
        if (y < height - 1 && !downWalls[y * width + x]) {
            canMove = true;
        }
    } 
    else if (dir == Direction::LEFT) {
        if (x > 0 && !rightWalls[y * width + (x - 1)]) {
            canMove = true;
        }
    } 
    else if (dir == Direction::UP) {
        if (y > 0 && !downWalls[(y - 1) * width + x]) {
            canMove = true;
        }
    }

    return canMove;
  }

  /**
   * Sets whether or not the specified wall exists.
   *
   * This function should be fast (constant time). You can assume that in
   * grading we will not make your maze a non-tree and then call one of the
   * other member functions. setWall should not prevent cycles from
   * occurring, but should simply set a wall to be present or not present.
   * Our tests will call setWall to copy a specific maze into your
   * implementation.
   * 
   * NOTE: You will only need to support setting the bottom or right walls 
   * of every squares in the grid
   *
   * @param x The x coordinate of the current cell
   * @param y The y coordinate of the current cell
   * @param dir The desired direction to set the wall. This will always be RIGHT or DOWN
   * @param exists true if setting the wall to exist, false otherwise
   */
  void SquareMaze::setWall(int x, int y, Direction dir, bool exists){
    if (dir == Direction::RIGHT) {
        rightWalls[y * width + x] = exists;
    }

    else if (dir == Direction::DOWN) {
        downWalls[y * width + x] = exists;
    }
}

  /**
   * Solves this SquareMaze.
   *
   * For each square on the bottom row (maximum y coordinate), there is a
   * distance from the starting point (startX, 0) (i.e. any position on the
   * top row), which is defined as the length (measured as a number of steps)
   * of the only path through the maze from the starting point to that square.
   *
   * Select the square in the bottom row with the largest distance from the
   * starting point as the destination of the maze. `solveMaze()` returns
   * the winning path from the origin to the destination as a vector of
   * directions.
   *
   * If multiple paths of maximum length exist, use the one with the
   * destination cell that has the smallest x value.
   *
   * HINT: this function should run in time linear in the number of cells
   * in the maze.
   *
   * @return a vector of directions taken to solve the maze
   */
  std::vector<Direction> SquareMaze::solveMaze(int startX){
    if (startX < 0 || startX >= width) {
        throw std::out_of_range("Invalid");
    }

    std::vector<Direction> finalvec;
    std::vector<int> distance(width * height, -1);
    std::vector<Direction> cameFrom(width * height);
    std::queue<int> q;

    //top row, y = 0
    int start = startX;
    distance[start] = 0;
    q.push(start);

    // bfs
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        
        int x = curr % width;
        int y = curr / width;

        std::vector<std::pair<Direction, std::pair<int, int>>> directions = {
            {Direction::RIGHT, {1, 0}},
            {Direction::DOWN, {0, 1}},
            {Direction::LEFT, {-1, 0}},
            {Direction::UP, {0, -1}}
        };

        for (const auto& d : directions) {
            Direction dir = d.first;
            int _x = d.second.first;
            int _y = d.second.second;

            if (canTravel(x, y, dir)) {
                
                int X = x + _x;
                int Y = y + _y;
                int forw = Y * width + X;

                if (distance[forw] == -1) {
                    
                    distance[forw] = distance[curr] + 1;
                    cameFrom[forw] = dir;
                    q.push(forw);
                }
            }
        }
    }

    // bottom row max dist
    int max = -1;
    int temp = -1;
    for (int x = 0; x < width; x++) {
        int i = (height - 1) * width + x;
        if (distance[i] > max) {
            max = distance[i];
            temp = i;
        }
    }

    // backtracking
    int curr = temp;
    while (curr != start) {
        Direction dir = cameFrom[curr];
        finalvec.push_back(dir);
        
        // pos
        int x = curr % width;
        int y = curr / width;
        
        if (dir == Direction::RIGHT) {
            curr = y * width + (x - 1);
        } else if (dir == Direction::DOWN) {
            curr = (y - 1) * width + x;
        } else if (dir == Direction::LEFT) {
            curr = y * width + (x + 1);
        } else if (dir == Direction::UP) {
            curr = (y + 1) * width + x;
        }
    }

    std::reverse(finalvec.begin(), finalvec.end());

    return finalvec;
  }

  /**
   * Draws the maze without the solution.
   *
   * First, create a new PNG. Set the dimensions of the PNG to
   * (width*10+1,height*10+1). where height and width were the arguments to
   * makeMaze. Blacken the entire topmost row and leftmost column of
   * pixels, except for the entrance. Instead make a gap above the specified 
   * start point defined as the inclusive region from ((start*10)+1, 0) to ((start+1)*10-1, 0).
   * [The gap is the pixels larger than start*10 and smaller than (start+1)*10 ]
   * For each square in the maze, call its maze coordinates (x,y). If the right wall exists,
   * then blacken the pixels with coordinates ((x+1)*10,y*10+k) for k from
   * 0 to 10. If the bottom wall exists, then blacken the pixels with
   * coordinates (x*10+k, (y+1)*10) for k from 0 to 10.
   *
   * The resulting PNG will look like the sample image, except there will
   * be no exit from the maze and the red line will be missing.
   *
   * @return a PNG of the unsolved SquareMaze
   */
cs225::PNG*::SquareMaze::drawMaze(int start) const{
    int width_ = width * 10 + 1;
    int height_ = height * 10 + 1;

    cs225::PNG* pngreturn = new cs225::PNG(width_, height_);

    //top row and left col with black
    for (int i = 0; i < width_; i++) {
        pngreturn->getPixel(i, 0) = cs225::HSLAPixel(0, 0, 0);
    }
    for (int j = 0; j < height_; j++) {
        pngreturn->getPixel(0, j) = cs225::HSLAPixel(0, 0, 0);
    }

    // use start+1 to create gap
    //0,0,1 is white
    for (int x = start * 10 + 1; x < (start + 1) * 10; x++) {
        pngreturn->getPixel(x, 0) = cs225::HSLAPixel(0, 0, 1);
    }

    // walls for every cell
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
          // right
          auto cell = y * width + x;
          if (rightWalls[cell]) {
              for (int i = 0; i < 11; i++) {
                  pngreturn->getPixel((x + 1) * 10, y * 10 + i) = cs225::HSLAPixel(0, 0, 0);
              }
          }
          //bottom
          if (downWalls[cell]) {
              for (int i = 0; i < 11; i++) {
                  pngreturn->getPixel(x * 10 + i, (y + 1) * 10) = cs225::HSLAPixel(0, 0, 0);
              }
          }
        }
    }

    return pngreturn;
}

  /**
   * This function calls drawMaze, then solveMaze; it modifies the PNG
   * from drawMaze to show the solution vector and the exit.
   *
   * Start at pixel (5,5). Each direction in the solution vector
   * corresponds to a trail of 11 red pixels in the given direction. If the
   * first step is downward, color pixels (5,5) through (5,15) red. (Red is
   * 0,1,0.5,1 in HSLA). Then if the second step is right, color pixels (5,15)
   * through (15,15) red. Then if the third step is up, color pixels
   * (15,15) through (15,5) red. Continue in this manner until you get to
   * the end of the solution vector, so that your output looks analogous
   * the above picture.
   *
   * Make the exit by undoing the bottom wall of the destination square:
   * call the destination maze coordinates (x,y), and whiten the pixels
   * with coordinates (x*10+k, (y+1)*10) for k from 1 to 9.
   *
   * @return a PNG of the solved SquareMaze
   */
cs225::PNG*::SquareMaze::drawMazeWithSolution(int start) {

    if (start < 0 || start >= width) {
        throw std::out_of_range("Invalid");
    }

    cs225::PNG* pngreturn = drawMaze(start);
    
    std::vector<Direction> path = solveMaze(start);
    
    // right + 5 pix right
    int x = start * 10 + 5;

    //5 pix from top
    int y = 5;

    for (Direction dir : path) {
        int xdiff = 0;
        int ydiff = 0;
        
        if (dir == Direction::RIGHT) {
            xdiff = 1;
        } else if (dir == Direction::DOWN) {
            ydiff = 1;
        } else if (dir == Direction::LEFT) {
            xdiff = -1;
        } else if (dir == Direction::UP) {
            ydiff = -1;
        }
        
        // 11 pix in dir
        for (int i = 0; i < 11; i++) {
            if (x >= 0 && x < width * 10 + 1 && y >= 0 && y < height * 10 + 1) {
                pngreturn->getPixel(x, y) = cs225::HSLAPixel(0, 1, 0.5, 1); // Red
            }
            x += xdiff;
            y += ydiff;
        }
        
        //step back
        x -= xdiff;
        y -= ydiff;
    }
    
    int x_ = x / 10;
    int y_ = y / 10;
 
    if (y_ < height && x_ < width) {
        for (int j = 1; j < 10; j++) {
            if (x_ * 10 + j < width * 10 + 1) {
                pngreturn->getPixel(x_ * 10 + j, (y_ + 1) * 10) = cs225::HSLAPixel(0, 0, 1);
            }
        }
    }
    
    return pngreturn;
}




