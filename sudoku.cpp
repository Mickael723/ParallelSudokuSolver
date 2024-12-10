
#include "classes.h"

void execute(TSQueue<std::shared_ptr<Puzzle>>& puzzle_queue, std::atomic<bool>& puzzleSolved, 
                std::mutex& print_mtx) {
    while (!puzzle_queue.empty() && !puzzleSolved) {
        auto maybePuzzle = puzzle_queue.pop();
        if (!maybePuzzle.has_value()) {continue;}
        auto puzzle = maybePuzzle.value();
        if (puzzle->isSolved()) {
            puzzleSolved = true;
            std::cout << "The puzzle has been solved!" << std::endl;
            puzzle->printPuzzle();
            break;
        }
        puzzle->enqueuePuzzles(puzzle_queue);
    }
}

// Usage ./sudoku (int)puzzle_size (string)txt_file_name (int)number_of_threads

int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " (int)puzzle_size (string)txt_file_name (int)number_of_threads" << std::endl;
        return 1;
    }

    int puzzle_size = std::stoi(argv[1]);
    std::string file_name = argv[2];
    int num_threads = std::stoi(argv[3]);

    // Validate inputs
    if (puzzle_size <= 0) {
        std::cerr << "Error: Puzzle size must be a positive integer." << std::endl;
        return 1;
    }
    if (num_threads <= 0) {
        std::cerr << "Error: Number of threads must be a positive integer." << std::endl;
        return 1;
    }

    Puzzle p(puzzle_size);
    TSQueue<std::shared_ptr<Puzzle> > puzzle_queue;
    std::atomic<bool> puzzleSolved(false);
    std::mutex print_mtx;

    p.readFile(file_name);

    cout << "Starting Puzzle:" << endl;
    p.printPuzzle();

    std::shared_ptr<Puzzle> startingPuzzle = std::make_shared<Puzzle>(p);;
    puzzle_queue.push(startingPuzzle);

    auto start = std::chrono::high_resolution_clock::now();

    // Create and start threads
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(execute, std::ref(puzzle_queue), std::ref(puzzleSolved), std::ref(print_mtx));
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double> >(end - start);
    cout << "This process took: " << duration.count() << " ticks." << endl;
}