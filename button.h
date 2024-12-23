//
// Created by chloe on 4/8/2023.
//
#include <iostream>
#include <functional>
#include <utility>
using namespace std;

struct Button {
    int xCoord;
    int yCoord;
    bool clicked;
    Button();
    Button(int xCoord, int yCoord);
    void OnClick();
    bool ButtonWasClicked(int x, int y) const;
};

