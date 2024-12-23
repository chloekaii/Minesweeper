//
// Created by chloe on 4/13/2023.
//
#include "button.h"

void Button::OnClick() {
    if (clicked) {
        clicked = false;
    } else {
        clicked = true;
    }
}

bool Button::ButtonWasClicked(int x, int y) const {
    if ((xCoord < x and x < xCoord + 64) and (yCoord < y and y < yCoord + 64)) {
        return true;
    } else {
        return false;
    }
}

Button::Button(int xCoord, int yCoord) {
    this->xCoord = xCoord;
    this->yCoord = yCoord;
    clicked = false;
}

Button::Button() {
    xCoord = 0;
    yCoord = 0;
    clicked = false;
}
