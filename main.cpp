#include <cmath>
#include <iostream>
#include <string>

#include "maze_builder.h"
#include "aco_lib.h"

int height = 10;
int width = 10;

int main(int argc, char* argv[])
{   
    // printf("\033[2J");

    std::cout << "Please input the height of the maze: ";
    std::cin >> height;

    std::cout << height << std::endl;

    std::cout << "Please input the width of the maze: ";
    std::cin >> width;

    MazeBuilder::Maze maze(height, width, 0, 0);

    // generation of maze
    maze.generate();
    maze.showMaze();

    ACOLIB::AntSolver solver(20, 20, maze.grid);

    solver.calculate();

    return 0;
}