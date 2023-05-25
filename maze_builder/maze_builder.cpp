#include "maze_builder.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <tuple>

// Based on Kruskals Algorithm for Maze generation - https://en.wikipedia.org/wiki/Kruskal%27s_algorithm

MazeBuilder::Tree::Tree(short x, short y) {

    tileY = y;
    tileX = x;
    parent = this;
    root = this;
};

MazeBuilder::Tree* MazeBuilder::Tree::getRoot() {
    if(parent != root) {
        return parent->getRoot();
    }

    return root;
};

MazeBuilder::Tree* MazeBuilder::Tree::getReference() {
    return root;
};

void MazeBuilder::Tree::setParent(Tree* t) {
    parent = t;
};

bool MazeBuilder::Tree::connected(Tree* t) {
    return parent->getRoot() == t->getRoot();
};

void MazeBuilder::Tree::connect(Tree* t) {
    MazeBuilder::Tree* r = t->getRoot();
    r->setParent(root);
};

std::vector<short> MazeBuilder::Tree::getTile() {
    std::vector<short> tiles = {tileX, tileY};
    return tiles;
};

MazeBuilder::Maze::Maze(short h, short w, int seed, double delay) {
    height = h;
    width = w;

    // define edges of maze
    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };

    grid.resize(height, std::vector<short>(width, 0));
    std::vector<short> edge;

    for(short i = 0; i < height; i++) {
        std::vector<Tree*> init = {};

        for(short j = 0; j < width; j++) {
            if(i > 0) {
                edges.push_back({j, i, N});
            }
            
            if(j > 0) {
                edges.push_back({j, i, W});
            }

            // define set
            init.push_back(new Tree(j, i));
        }

        sets.push_back(init);
    }

    std::shuffle(std::begin(edges), std::end(edges), rng);
}

void MazeBuilder::Maze::generate() {
    while(!edges.empty()) {
        // [0] X, [1] Y, [2] direction
        auto edge = edges.back();
        short nx = edge[0] + DX[edge[2]], ny = edge[1] + DY[edge[2]];

        Tree* set1 = sets[edge[1]][edge[0]];
        Tree* set2 = sets[ny][nx];

        if(!set1->connected(set2)) {
            
            set1->connect(set2);
            grid[edge[1]][edge[0]] |= edge[2];
            grid[ny][nx] |= OPPOSITE[edge[2]];
        }

        edges.pop_back();
    }

    // construct cells of maze

}

void MazeBuilder::Maze::showMaze() {
    // clear console to use ANSI escape commands
    printf("\033[2J");

    for(int i = 1; i < height + 1; i++) {
        for(int j = 1; j < width + 1; j++) {
            short cell = grid[i-1][j-1];
            
            printf("\033[%d;%dH%d", i + 1, j*3 + width*2 + 3, cell);

            // N
            if(!(cell & 1)) {
                printf("\033[%d;%dH_", i, j*2 + 2);
            } 

            // S
            if(!(cell & 2)) {
                printf("\033[%d;%dH_", i + 1, j*2 + 2);
            }

            // E
            if(!(cell & 4)) {
                printf("\033[%d;%dH|", i + 1, j*2 + 3);
            }

            // W
            if(!(cell & 8)) {
                printf("\033[%d;%dH|", i + 1, j*2 + 1);
            }
        }
        
         std::cout << std::endl;
    }

    printf("\033[%d;0H ^^ Generated Graph with dimensions: %dx%d ^^", height + 3, width);

    std::cout << std::endl;
}

std::map<short, short> MazeBuilder::Maze::DX = {
    { 4, 1 },
    { 8, -1 },
    { 1, 0 },
    { 2, 0 }
};

std::map<short, short> MazeBuilder::Maze::DY = {
    { 4, 0 },
    { 8, 0 },
    { 1, -1 },
    { 2, 1 }
};

std::map<short, short> MazeBuilder::Maze::OPPOSITE = {
    { 4, 8 },
    { 8, 4 },
    { 1, 2 },
    { 2, 1 }
};