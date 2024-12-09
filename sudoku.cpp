#include <iostream>
#include "classes.h"

int main() {
    vector<vector<int> > s();
    Puzzle p(9);
    p.readFile("test.txt");
    p.printPuzzle();
    if (!p.isSolved()) { cout << "Puzzle not solved." << endl;}
    p.printPuzzle();
}