
#include "classes.h"

void execute(TSQueue<std::shared_ptr<Puzzle>>& puzzle_queue, std::atomic<bool>& puzzleSolved, 
                std::atomic<int>& puzzlesPopped) {
    while (!puzzle_queue.empty() && !puzzleSolved) {
        auto maybePuzzle = puzzle_queue.pop(); // Need to make sure thread popped a puzzle to avoid seg faults.
        if (!maybePuzzle.has_value()) {continue;}
        puzzlesPopped++;
        auto puzzle = maybePuzzle.value();
        if (puzzle->isSolved()) {
            puzzleSolved = true;
            cout << "The puzzle has been solved!" << endl;
            puzzle->printPuzzle();
            cout << "Looked at " << puzzlesPopped << " states in total." << endl;
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

    // Create initial puzzle state and the queue.
    Puzzle p(puzzle_size);
    TSQueue<std::shared_ptr<Puzzle> > puzzle_queue;

    std::atomic<bool> puzzleSolved(false);

    p.readFile(file_name);

    cout << "Starting Puzzle:" << endl;
    p.printPuzzle();

    std::shared_ptr<Puzzle> startingPuzzle = std::make_shared<Puzzle>(p);;
    puzzle_queue.push(startingPuzzle);
    std::atomic<int> puzzlesPopped(0); 

    cout << "Solving..." << endl;

    auto start = std::chrono::high_resolution_clock::now();

    // Create and start threads
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(execute, std::ref(puzzle_queue), std::ref(puzzleSolved), std::ref(puzzlesPopped));
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double> >(end - start);
    cout << "This process took: " << duration.count() << " ticks." << endl;
}