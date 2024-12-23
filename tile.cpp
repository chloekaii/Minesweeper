//
// Created by chloe on 4/13/2023.
//
#include "tile.h"

Tile::Tile(int row, int col) {
    this->row = row;
    this->col = col;
    this->xCoord = col * 32;
    this->yCoord = row * 32;
}

void Tile::OnClick() {
    if (!flagged) {
        isRevealed = true;

        if (!TileHasMine() and AdjacentMines() == 0) {
            for (int i=0; i < adjacentTiles.size(); i++) {
                if (!adjacentTiles[i]->TileHasMine() and !adjacentTiles[i]->isRevealed) {
                    isRevealed = true;
                    adjacentTiles[i]->OnClick();
                }
            }
        }
    }
}

void Tile::OnRightClick() {
    if (flagged) {
        flagged = false;
    } else if (!flagged) {
        flagged = true;
    }
}

bool Tile::TileWasClicked(int x, int y) {
    if ((xCoord < x and x < xCoord + 32) and (yCoord < y and y < yCoord + 32)) {
        return true;
    } else {
        return false;
    }
}

bool Tile::TileIsRevealed() {
    if (isRevealed) {
        return true;
    } else {
        return false;
    }
}

bool Tile::IsFlagged() const {
    if (flagged) {
        return true;
    } else {
        return false;
    }
}

void Tile::SetMine() {
    hasMine = true;
}

bool Tile::TileHasMine() const {
    if (hasMine) {
        return true;
    } else {
        return false;
    }
}

int Tile::AdjacentMines() {

    int adjacentMines = 0;
    for (int i=0; i < adjacentTiles.size(); i++) {
        if (adjacentTiles[i]->hasMine) {
            adjacentMines ++;
        }
    }
    return adjacentMines;
}

bool Tile::HorizOrVertical(int origRow, int origCol) const {
    if ((row == origRow) and (col == origCol - 1 or col == origCol + 1)){
        return true;
    } else if ((col == origCol) and (row == origRow - 1 or row == origRow + 1)) {
        return true;
    } else {
        return false;
    }
}
