#include "aco_lib.h"

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <cmath>
#include <utility>

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

std::string generateCellId(int cellPosition) {
    std::string id;
    int it1 = (int)std::floor(cellPosition / 26);
    int it2 = (int)std::floor(cellPosition / (26 * 26));
    int remainder = cellPosition % 26;

    int base[2] = {it2, it1};
    
    for(auto c : base) {
        if(c != 0) {
            id += (char)(c + 64);
        }
    }

    id += (char)(remainder + 65);

    return id;
}

ACOLIB::Ant::Ant(ACOLIB::Cell *c, int y_position, int x_position) {
    currentCell = c;
    path_junctions.push_back(c->id);
    y = y_position;
    x = x_position;
}

std::string ACOLIB::Ant::getCurrentCell() {
    return currentCell->id;
}

void formatArray(double a[4], int index) {
    for(int i = 0; i < 4; i++) {
        if(i == index) {
            a[i] = 1.0;
        } else {
            a[i] = 0;
        }
    } 
}

void ACOLIB::Ant::move(ACOLIB::MazeGraph &mg, std::vector<std::vector<float>> &ph) {
    double moveProbabilities[4] = {0.0};
    int finalDirection = -1;
    int numOfTurns = 0;

    // check north
    if(currentCell->connections[0]) {
        moveProbabilities[0] = ph[y-1][x];

        ACOLIB::Cell *c = mg.getCell(y-1, x);

        if(c->isFinalNode) {
            finalDirection = 0;
        }

        ++numOfTurns;
    }

    // check east
    if(currentCell->connections[1]) {
        moveProbabilities[1] = ph[y][x+1];

        ACOLIB::Cell *c = mg.getCell(y, x+1);

        if(c->isFinalNode) {
            finalDirection = 1;
        }

        ++numOfTurns;
    }

    // check south
    if(currentCell->connections[2]) {
        moveProbabilities[2] = ph[y+1][x];

        ACOLIB::Cell *c = mg.getCell(y+1, x);

        if(c->isFinalNode) {
            finalDirection = 2;
        }

        ++numOfTurns;
    }

    // check west
    if(currentCell->connections[3]) {
        moveProbabilities[3] = ph[y][x-1];

        ACOLIB::Cell *c = mg.getCell(y, x-1);

        if(c->isFinalNode) {
            finalDirection = 3;
        }

        ++numOfTurns;
    }

    // check if it sees the final node
    if(finalDirection > -1) {
        switch (finalDirection)
        {
        case 0:
            formatArray(moveProbabilities, 0);
            break;
        case 1:
            formatArray(moveProbabilities, 1);
            break;
        case 2:
            formatArray(moveProbabilities, 2);
            break;
        case 3:
            formatArray(moveProbabilities, 3);
            break;
        default:
            break;
        }
    }

    // make sure that you reduce visibility of last visited node
    if(path_indexes.size() > 1) {
        size_t len = path_indexes.size();
        std::pair lastNode = path_indexes[len - 2]; 
        if(numOfTurns > 1) {
            // previously went north
            if(y - lastNode.first == 1) {
                moveProbabilities[0] = 0.;
            }

            // previously went east
            if(x - lastNode.second == -1) {
                moveProbabilities[1] = 0.;
            }

            // previously went south
            if(y - lastNode.first == -1) {
                moveProbabilities[2] = 0.;
            }

            // previously went west
            if(x - lastNode.second == 1) {
                moveProbabilities[3] = 0.;
            }
        }
    }

    double total = moveProbabilities[0] + moveProbabilities[1] + moveProbabilities[2] + moveProbabilities[3];
    double r = dis(gen);
    double threshold = 0.0;
    int direction = -1;

    for(int i = 0; i < 4; i++) {
        if(moveProbabilities[i] > 0) {
            threshold += moveProbabilities[i] / total;
            if(r <= threshold) {
                direction = i;
                break;
            }
        }
    }

    if(direction == 0) {
        ACOLIB::Cell *c = mg.getCell(y-1, x);
        path_junctions.push_back(c->id);
        path_indexes.push_back(std::make_pair(y-1, x));
        currentCell = c;
        --y;
    } else if(direction == 1) {
        ACOLIB::Cell *c = mg.getCell(y, x+1);
        path_junctions.push_back(c->id);
        path_indexes.push_back(std::make_pair(y, x+1));
        currentCell = c;
        ++x;
    } else if(direction == 2) {
        ACOLIB::Cell *c = mg.getCell(y+1, x);
        path_junctions.push_back(c->id);
        path_indexes.push_back(std::make_pair(y+1, x));
        currentCell = c;
        ++y;
    } else if(direction == 3) {
        ACOLIB::Cell *c = mg.getCell(y, x-1);
        path_junctions.push_back(c->id);
        path_indexes.push_back(std::make_pair(y, x-1));
        currentCell = c;
        --x;
    }

    return;
}

void ACOLIB::Ant::clearPaths(ACOLIB::Cell *c) {
    path_junctions.clear();
    path_indexes.clear();
    x = 0;
    y = 0;

    path_junctions.push_back(c->id);
}

void ACOLIB::Ant::setCurrentCell(Cell *c) {
    currentCell = c;
}

ACOLIB::Cell* ACOLIB::MazeGraph::getCell(int y, int x) {
    return &graph[y][x];
}

ACOLIB::MazeGraph::MazeGraph(std::vector<std::vector<short>> mazeMatrix) {
    int height = mazeMatrix.size();
    for(short i = 0; i < height; i++) {
        int width = mazeMatrix[i].size();
        std::vector<ACOLIB::Cell> row;
        for(short j = 0; j < width; j++) {
            ACOLIB::Cell cell = {
                .id = generateCellId(i*width + j),
                .isFinalNode = (i == height - 1 && j == width - 1) ? true : false,
                .connections = {(bool)(mazeMatrix[i][j] & 1), (bool)(mazeMatrix[i][j] & 4), (bool)(mazeMatrix[i][j] & 2), (bool)(mazeMatrix[i][j] & 8)}
            };

            row.push_back(cell);
        }

        graph.push_back(row);
    }
}

ACOLIB::AntSolver::AntSolver(int i, int n, std::vector<std::vector<short>> mazeMatrix): graph(mazeMatrix) {
    iterations = i;
    numOfAnts = n;

    height = mazeMatrix.size();
    width = mazeMatrix[0].size();

    // initialise pheromones to start at initial value
    this->initialisePheromones();

    for(int i = 0; i < numOfAnts; i++) {
        // initialise ant at start node of graph
        ACOLIB::Cell *c = graph.getCell(0, 0);
        ACOLIB::Ant a(c, 0, 0);
        ants.push_back(a);
    }
}

void ACOLIB::AntSolver::calculate() {
    // get the first cell
    ACOLIB::Cell *c = graph.getCell(0, 0);

    // run all iterations to it's full conclusion
    for(int i = 0; i < iterations; i++) {
        std::cout << "ITERATION " << i << std::endl;
        for(int a = 0; a < numOfAnts; a++) {
            std::string currentCell = ants[a].getCurrentCell();
            std::string finishCell = this->getFinishCell();

            while(currentCell != finishCell) {
                ants[a].move(graph, pheromones);
                currentCell = ants[a].getCurrentCell();
            }

            for(auto step: ants[a].path_junctions) {
                std::cout << step << "->";
            };

            std::cout << "\n" << std::endl;
        }

        this->updatePheromones();

        for(auto &ant: ants) {
            ant.clearPaths(c);
            ant.setCurrentCell(c);
        }
    }
}

void ACOLIB::AntSolver::updatePheromones() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            pheromones[i][j] *= 0.5;
        }
    }

    ACOLIB::Ant bestAnt = ants[0];
    for(const Ant& ant: ants) {
        int size = ant.path_indexes.size();
        int currentBestSize = bestAnt.path_indexes.size();
        
        if(size < currentBestSize) {
            bestAnt = ant;
        }
    }

    for(const auto& step: bestAnt.path_indexes) {
        pheromones[step.first][step.second] += 2.0;
    }
}

std::string ACOLIB::AntSolver::getFinishCell() {
    ACOLIB::Cell *c = graph.getCell(height - 1, width - 1);
    return c->id;
}

void ACOLIB::AntSolver::initialisePheromones() {
    for(int i = 0; i < height; i++) {
        std::vector<float> row;
        for(int j = 0; j < width; j++) {
            row.push_back(1.0);
        }
        pheromones.push_back(row);
    }
}