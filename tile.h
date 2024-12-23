//
// Created by chloe on 4/13/2023.
//
#include <iostream>
#include <vector>
using namespace std;

class Tile {
    bool hasMine = false;
    bool isRevealed = false;
    bool flagged = false;
    bool HorizOrVertical(int origRow, int origCol) const;
public:
    int xCoord;
    int yCoord;
    int row;
    int col;
    vector<Tile*> adjacentTiles;
    Tile(int row, int col);
    void OnClick();
    void OnRightClick();
    bool TileWasClicked(int x, int y);
    bool TileIsRevealed();
    bool IsFlagged() const;
    void SetMine();
    bool TileHasMine() const;
    int AdjacentMines();
};

