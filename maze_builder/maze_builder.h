#include <iostream>
#include <vector>
#include <map>
#include <tuple>

namespace MazeBuilder {
    class Tree
    {
        public:
            Tree(short, short);

            // methods
            bool connected(Tree*);
            void connect(Tree*);
            MazeBuilder::Tree* getRoot();
            MazeBuilder::Tree* getReference();
            void setRoot(Tree*);
            void setParent(Tree*);
            std::vector<short> getTile();

            // variables
            Tree* parent;
            Tree* root;
            short tileX;
            short tileY;
    };

    class Maze
    {
        public:
            Maze(short, short, int, double);

            // methods
            void generate();
            void showMaze();

            // variables
            short height;
            short width;
            std::vector<std::vector<short>> grid;
            std::vector<std::vector<Tree*>> sets;
            std::vector<std::vector<short>> edges;
            static const short N = 1, S = 2, E = 4, W = 8;
            static std::map<short, short> DX;
            static std::map<short, short> DY;
            static std::map<short, short> OPPOSITE;
    };
} 