//
// Created by chloe on 4/13/2023.
//
#include <vector>
#include "tile.h"

using namespace std;

struct Board {
    int numCols;
    int numRows;
    int numMines;
    vector<vector<Tile>> tiles;
    Board(const int numColumns, const int numRows, const int numMines) {
        this->numMines = numMines;
        this->numCols = numColumns;
        this->numRows = numRows;
    }
};

