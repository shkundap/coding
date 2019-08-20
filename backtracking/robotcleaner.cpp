/*
============
Description
============
Given a robot cleaner in a room modeled as a grid.
Each cell in the grid can be empty or blocked.
The robot cleaner with 4 given APIs can move forward, turn left or turn right. Each turn it made is 90 degrees.
When it tries to move into a blocked cell, its bumper sensor detects the obstacle and it stays on the current cell.
Design an algorithm to clean the entire room using only the 4 given APIs shown below.

interface Robot {
  // returns true if next cell is open and robot moves into the cell.
  // returns false if next cell is obstacle and robot stays on the current cell.
  boolean move();

  // Robot will stay on the same cell after calling turnLeft/turnRight.
  // Each turn will be 90 degrees.
  void turnLeft();
  void turnRight();

  // Clean the current cell.
  void clean();
}
============
Example:
============
Input:
room = [
  [1,1,1,1,1,0,1,1],
  [1,1,1,1,1,0,1,1],
  [1,0,1,1,1,1,1,1],
  [0,0,0,1,0,0,0,0],
  [1,1,1,1,1,1,1,1]
],
row = 1,
col = 3

Explanation:
-------------
All grids in the room are marked by either 0 or 1.
0 means the cell is blocked, while 1 means the cell is accessible.
The robot initially starts at the position of row=1, col=3.
From the top left corner, its position is one row below and three columns right.
============
Notes:
============

The input is only given to initialize the room and the robot's position internally. You must solve this problem "blindfolded". In other words, you must control the robot using only the mentioned 4 APIs, without knowing the room layout and the initial robot's position.
The robot's initial position will always be in an accessible cell.
The initial direction of the robot will be facing up.
All accessible cells are connected, which means the all cells marked as 1 will be accessible by the robot.
Assume all four edges of the grid are all surrounded by wall.

*/

/**
 * // This is the robot's control interface.
 * // You should not implement it, or speculate about its implementation
 * class Robot {
 *   public:
 *     // Returns true if the cell in front is open and robot moves into the cell.
 *     // Returns false if the cell in front is blocked and robot stays in the current cell.
 *     bool move();
 *
 *     // Robot will stay in the same cell after calling turnLeft/turnRight.
 *     // Each turn will be 90 degrees.
 *     void turnLeft();
 *     void turnRight();
 *
 *     // Clean the current cell.
 *     void clean();
 * };
 */

 /*
 Approach:
 ---------
 - This approach considers that robot is at position (0,0) which is its start.
 - Since there is no API in robot class to determine if a cell has been already cleaned or not, we keep track of co-ordinates of each cleaned cell in a set called visited of integer pair which represents a point.
 - Every call to function cleans one cell, then to clean next cell in same direction we call the same function with the direction current direction. Once we have exhausted all possible cleaning values in that direction, we switch to next direction. Once we have cleaned a given cell from all 4 direction, we back track to previous cell. In the previous cell we proceed with cleaning in remaining direction. Once cleaning in this cell is also complete in all direction we move to the previous one. This step is repeated until we clean all the 4 direction of our starting cell.
 - Turning back to previous cell is achieved by turning left twice, this makes an 180 degree turn move to previous cell. then turn left twice again to get back to the same direction as previous cell.

Example to show how program cleans:
------------------------------------
Starting point : 2
Turning direction of robot: up -> right -> down -> left -> up (this is cricular). As robot can start in any direction and move cylically. Mathematically this can be represented as {(0,1), (1,0), (0,-1), (-1,0)}

 1 2 3 x
 5 4 x x

- Robot is at 2. It is facing up. No cells in up direction, we are starting moving in up direction.
- So it will turnright clean 3. nothing after 3. back tracks to 2.
- we will go in next direction in 2 which is down.( we are already done with up and right) clean 4.
- Nothing next 4. so we back track to 2 again.
- then we go in the last remaining direction that is left. clean 1.
- 1 doesnt have anything further in left direction, so moves up, then right then down cleans 5
- all 4 direction at 1 are done. So it moves back to 2.
- All the directions are cleaned at 2 which is the start point hence program terminates.
- One thing to note is that at a particular cell we need not start cleaning in upward direction only. It can start in any direction. That direction has to be continued to clean other cells in same line. Ultimately all direction has to be clean.

 */
class Solution {
public:

    void goBack(Robot& robot)
    {
        // turn 180 degree. which is like turning back. if we where '^' then now we are 'v'.
        robot.turnLeft();
        robot.turnLeft();

        // move to the previous cell
        robot.move();

        // turn 180 degree. which is like turning back to where we were heading. We move from being 'v' to '^'.
        robot.turnLeft();
        robot.turnLeft();
    }

    void cleanAndBacktrack(Robot& robot,
                           std::set<std::pair<int, int>> & visited,
                           int i, int j, int d, vector<pair<int,int>> &dir)
    {


        // clean current cell which you are in
        robot.clean();

        // Mark the cleaned cell as cleaned by adding it in  visited.
        visited.insert(make_pair(i, j));

        // This loop runs 4 time to cover 4 directions.
        for(int k = 0; k < 4 ; ++k)
        {
            // since we have to start from the direction which we came from previous cell. we do this modulo operation.
            // when k = 0, n_d will be same as d which is the direction in which we came from previous cell.
            int n_d = (d + k) % 4;

            // create the points of the next cell we will try to clean in the direction specified by d
            int i1 = i + dir[n_d].first;
            int j1 = j + dir[n_d].second;

            // if cell is a valid cell and has not already been cleaned. then call the function to clean
            if(visited.find(make_pair(i1, j1)) == visited.end() && robot.move())
            {
                // once you have cleaned all the 4 directions from the cell (i1, j1) this function returns
                cleanAndBacktrack(robot, visited, i1, j1, n_d, dir);

                // then we go back to previous cell to move to next direction in the previous cell.
                goBack(robot);

                //cout << "  backtrack : (" << i1 << ", " << j1 << ") -> (" << i << ", " << j << ")\n";
            }

            // This does the operation of moving to the next direction
            robot.turnLeft();
        }
    }

    void cleanRoom(Robot& robot) {
        // direction vector which represents all 4 sides from where we will clean a cell
        vector<pair<int,int>> dir = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};

        // to keep track of the cleaned cells.
        std::set<std::pair<int, int>> visited;

        // to start cleaning recursively from (0,0) in upward direction
        cleanAndBacktrack(robot, visited, 0, 0, 0, dir);
    }
  };
