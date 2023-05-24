#include <iostream>
#include <vector>
#include <random>
#include <string>

namespace ACOLIB {
    struct Cell {
        std::string id;
        bool isFinalNode;
        // N, E, S, W
        bool connections[4];
    };

    class MazeGraph {
        public:
            // generate cells from maze matrix in construction
            MazeGraph(std::vector<std::vector<short>>);
            
            // gets cell at position
            Cell* getCell(int /* y */, int /* x */);

            // graph of cells
            std::vector<std::vector<Cell>> graph;
    };

    class Ant {
        public:
            Ant(Cell*, int /* Y position */, int /* X position */);

            void move(MazeGraph&, std::vector<std::vector<float>>&);
            void clearPaths(Cell*);
            std::string getCurrentCell();
            void setCurrentCell(Cell*);

            std::vector<std::string> path_junctions;
            std::vector<std::pair<int, int>> path_indexes;

        private:
            Cell *currentCell;
            int x;
            int y;
    };

    class AntSolver {
        public:
            AntSolver(int /* iterations */, int /* number of ants */, std::vector<std::vector<short>> /* 2D graph of maze */);

            // start pheromone matrix as default
            void initialisePheromones();

            // update pheromone matrix with ant paths
            void updatePheromones();

            // get global pheromone at position
            float getPheromoneAtPosition(int /* y */, int /* x */);

            // set global pheromone at position
            void setPheromoneAtPosition(int /* y */, int /* x */, float /* pheromone value */);

            // start iterations of ants
            void calculate();

            // get finished cell
            std::string getFinishCell();

            // pheromone matrix
            std::vector<std::vector<float>> pheromones;

        private:
            std::vector<Ant> ants;
            int iterations;
            int numOfAnts;
            MazeGraph graph;
            int height;
            int width;
            float initial_pheromone = 0.1;
    };
}