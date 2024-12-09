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
        Puzzle(const int size = 0);
        void readFile(const string& filename);
        bool checkPlacement(const int row, const int col, const int value);
        void printPuzzle();
};