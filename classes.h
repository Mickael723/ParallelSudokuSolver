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
#include <mutex>
#include <condition_variable>
#include <optional>
#include <atomic>
#include <thread>

using std::vector;
using std::cout;
using std::endl;
using std::string;

template<typename T>
class TSQueue {
    private:
        std::queue<T> queue;
        mutable std::mutex mtx;
        std::condition_variable cond_var;
    public:
        void push(T object) {
            {
                std::scoped_lock lock(mtx);
                queue.push(std::move(object));
            }
            cond_var.notify_one();
        }
        std::optional<T> pop() {
            std::unique_lock<std::mutex> lock(mtx);
            cond_var.wait(lock, [this] { return !queue.empty(); });
            
            if (queue.empty()) {
                return std::nullopt; // Return empty optional if queue is empty
            }

            T object = std::move(queue.front());
            queue.pop();
            return object;
        }

        bool empty() const {
            std::scoped_lock lock(mtx);
            return queue.empty();
        }
};

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
        void enqueuePuzzles(TSQueue<std::shared_ptr<Puzzle> > &queue) const;
        void printPuzzle() const;
};