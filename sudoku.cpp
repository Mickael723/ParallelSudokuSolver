
#include "classes.h"

int main() {
    Puzzle p(9);
    std::queue<std::shared_ptr<Puzzle> > puzzle_queue;
    p.readFile("test.txt");
    cout << "Starting Puzzle:" << endl;
    p.printPuzzle();
    std::shared_ptr<Puzzle> startingPuzzle = std::make_shared<Puzzle>(p);;
    puzzle_queue.push(startingPuzzle);
    auto start = std::chrono::high_resolution_clock::now();
    while (!puzzle_queue.empty()) {
        auto puzzle = puzzle_queue.front();
        cout << "Popped puzzle from queue:" << endl;
        puzzle->printPuzzle();
        cout << endl;
        puzzle_queue.pop();
        if (puzzle->isSolved()) {
            cout << "The puzzle has been solved!: " << endl;
            puzzle->printPuzzle();
            break;
        }
        puzzle->enqueuePuzzles(puzzle_queue);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "This process took: " << duration.count() << " milliseconds." << endl;
}