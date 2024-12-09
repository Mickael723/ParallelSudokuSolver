#include "classes.h"

extern std::mutex mtx;

// Constructor: create empty puzzle space
Puzzle::Puzzle(const int size) {
    this->size = size;
    puzzle_space.resize(size);
    for (int i = 0; i < size; ++i) {
        puzzle_space[i].resize(size, 0);
    }
}

bool Puzzle::checkPlacement(const int row, const int col, const int value) const{
    // Validate the input
        if (row < 0 || row >= 9 || col < 0 || col >= 9 || value < 1 || value > 9 || puzzle_space[row][col] != 0) {
            throw std::out_of_range("Row or column or value out of range (must be between 0 and 8) or cell if full.");
        }
    // Check same row
    for (int i = 0; i < size; i++) {
        if (puzzle_space[row][i] == value) {
            return false;
        }
    }
    // Check same column
    for (int i = 0; i < size; i++) {
        if (puzzle_space[i][col] == value) {
            return false;
        }
    }

    // Check smaller grid
    int root = std::sqrt(size);
    int startRow = (row / root) * root;
    int startCol = (col / root) * root;
    for (int i = startRow; i < startRow + root; ++i) {
        for (int j = startCol; j < startCol + root; ++j) {
            if (puzzle_space[i][j] == value) {
                return false;
            }
        }
    }
    
    return true;
}

void Puzzle::placeValue(const int row, const int col, const int value) {
    puzzle_space[row][col] = value;
}

bool Puzzle::isSolved() const {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (puzzle_space[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

void Puzzle::readFile(const string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    string line;
    int row = 0;

    while (getline(file, line)) {
        if (row >= 9) {
            throw std::runtime_error("File contains more than 9 rows.");
        }
        std::istringstream lineStream(line);
        int value, col = 0;

        while (lineStream >> value) {
            if (col >= 9 || value < 0 || value > 9) {
                throw std::runtime_error("Puzzle File is invalid");
            }

            puzzle_space[row][col] = value;
            ++col;
        }
        if (col != 9) {
            throw std::runtime_error("Puzzle File is invalid");
        }
        ++row;
    }
    if (row != 9) {
        throw std::runtime_error("Puzzle File is invalid");
    }
    file.close();
}

std::shared_ptr<Puzzle> Puzzle::generateNewPuzzle(const int row, const int col, const int value) const{
    std::shared_ptr<Puzzle> newPuzzle(new Puzzle(size));
    newPuzzle->puzzle_space = this->puzzle_space;
    newPuzzle->placeValue(row, col, value);
    return newPuzzle;
}

void Puzzle::enqueuePuzzles(std::queue<std::shared_ptr<Puzzle> > &queue) const {
    // Look for empty space
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (puzzle_space[i][j] == 0) {
                // Attempt to fill space
                for (int k = 1; k < size + 1; ++k) {
                    if (checkPlacement(i,j,k)) {
                        std::shared_ptr<Puzzle> newPuzzle = generateNewPuzzle(i,j,k);
                        std::lock_guard<std::mutex> lock(mtx);
                        queue.push(newPuzzle);
                        
                    }
                }
                return;
            }
        }
    }
}

void Puzzle::printPuzzle() const {
    for (const auto& row : puzzle_space) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}
