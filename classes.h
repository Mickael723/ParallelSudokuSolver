#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

using std::vector;
using std::cout;
using std::endl;
using std::string;

class Puzzle {
    private:
        int size;
        vector<vector<int> > puzzle_space;
    public:
        // Constructor: create empty puzzle space
        Puzzle(const int size = 0, const std::vector<std::vector<int>>& initialGrid) : size(size), puzzle_space(initialGrid) {}
        void readFile(const string& filename);
        // Checks if a value can be placed at the specified location
        bool checkPlacement(const int row, const int col, const int value);
        // Checks if puzzle is solved
        bool isSolved();
        void placeValue(const int row, const int col, const int value);
        // Generates new puzzle states given the current state.
        Puzzle generateNewPuzzle(const int row, const int col, const int value);
        void printPuzzle();
};