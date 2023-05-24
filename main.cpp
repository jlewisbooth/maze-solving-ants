#include <cmath>
#include <iostream>
#include <string>

#include "maze_builder.h"
#include "aco_lib.h"

const int height = 10;
const int width = 10;

int main(int argc, char* argv[])
{
    MazeBuilder::Maze maze(height, width, 0, 0);

     std::cout << "STARTING PROCESS \n\n\n\n\n\n" << std::endl;

    maze.generate();
    maze.showMaze();

    ACOLIB::AntSolver solver(20, 20, maze.grid);

    solver.calculate();

    return 0;
}