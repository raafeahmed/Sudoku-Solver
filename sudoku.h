#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <vector>

class Sudoku{
private:
    std::vector<std::vector<uint32_t>> grid;
public:
    Sudoku();
    void readInput();
    void sudoku_solver(uint32_t row, uint32_t col);
    bool isFull();
    bool promising(uint32_t row, uint32_t col, uint32_t choice);
    void print_solution();
    ~Sudoku();
};

#endif