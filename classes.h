#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <queue>
#include <chrono>
#include <memory>
#include <omp.h>
#include <mutex>

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

        Puzzle(const Puzzle& other) : size(other.size), puzzle_space(other.puzzle_space) {}

        Puzzle& operator=(const Puzzle& other) {
            if (this == &other) {
                return *this;
            }
            size = other.size;
            puzzle_space = other.puzzle_space;

            return *this; 
        }
        void readFile(const string& filename);
        // Checks if a value can be placed at the specified location
        bool checkPlacement(const int row, const int col, const int value) const;
        // Checks if puzzle is solved
        bool isSolved() const;
        void placeValue(const int row, const int col, const int value);
        // Generates new puzzle states given the current state.
        std::shared_ptr<Puzzle> generateNewPuzzle(const int row, const int col, const int value) const;
        void enqueuePuzzles(std::queue<std::shared_ptr<Puzzle> > &queue) const;
        void printPuzzle() const;
};