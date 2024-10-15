#pragma once

#include <vector>

namespace map{

    struct Cell{
        int x;
        int y;

        bool isObstacle;
        Cell()
            :x{0}, y{0}, isObstacle{false} {}

        Cell(int x, int y, bool isObstacle = false)
            :x{x}, y{y}, isObstacle{isObstacle} {}

        Cell(const Cell& other)
        {
            this->x = other.x;
            this->y = other.y;
            this->isObstacle = other.isObstacle;
        }
        
        
        bool operator == (const Cell& other) const
        {
            return (x==other.x && y==other.y && isObstacle == other.isObstacle);
        }

        Cell& operator=(const Cell& other) {
            if (this == &other) {
                return *this;  // Obsługa przypisania do samego siebie
            }
            x = other.x;
            y = other.y;
            isObstacle = other.isObstacle;
            return *this;
        }
    };

    struct Graph
    {
        std::vector<std::vector<Cell>> cells;
        int width, height;

        Graph() = default;

        Graph(Graph& g)
        {
            this->cells = g.cells;
            this->width = g.width;
            this->height = g.height;
        }

        Graph& operator=(const Graph& other) {
            if (this != &other) {
                this->cells = other.cells;
                this->width = other.width;
                this->height = other.height;
            }
            return *this;
        }

        Graph(int width, int height)
            :width(width), height(height)
        {
            cells.resize(width, std::vector<Cell>(height, Cell(0, 0)));
            for (int i = 0; i < width; ++i) 
                for (int j = 0; j < height; ++j) 
                    cells[i][j] = Cell(i, j, false);
        }
    };

};
