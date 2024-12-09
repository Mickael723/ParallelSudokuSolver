#include <iostream>
#include "classes.h"

int main() {
    Puzzle p(9);
    p.readFile("test.txt");
    p.printPuzzle();
    cout << p.checkPlacement(1,0,3) << endl;
    cout << p.checkPlacement(1,0,1) << endl;
    p.printPuzzle();
}