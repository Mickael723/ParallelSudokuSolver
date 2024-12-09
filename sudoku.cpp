
#include "classes.h"

std::mutex mtx;

int main() {

    Puzzle p(9);
    std::queue<std::shared_ptr<Puzzle> > puzzle_queue;
    p.readFile("test.txt");
    cout << "Starting Puzzle:" << endl;
    p.printPuzzle();
    std::shared_ptr<Puzzle> startingPuzzle = std::make_shared<Puzzle>(p);;
    puzzle_queue.push(startingPuzzle);

    bool solutionFound = false;

    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel

    while (!puzzle_queue.empty() && !solutionFound) {

        std::lock_guard<std::mutex> lock(mtx);
        auto puzzle = puzzle_queue.front();
        puzzle_queue.pop();
        if (puzzle->isSolved()) {
            #pragma omp critical
                {
                    if (!solutionFound) {
                        solutionFound = true;
                        std::cout << "The puzzle has been solved!" << std::endl;
                        puzzle->printPuzzle();
                    }
                }
            break;
        }
        puzzle->enqueuePuzzles(puzzle_queue);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double> >(end - start);
    cout << "This process took: " << duration.count() << " ticks." << endl;
}