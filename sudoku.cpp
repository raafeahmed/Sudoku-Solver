#include "sudoku.h"

Sudoku::Sudoku(){}

//initalizes Sudoku grid with values read in
void Sudoku::readInput(){

    uint32_t number;
    grid.resize(9, std::vector<uint32_t>(9, 0));

    for(uint32_t i = 0; i < grid.size(); ++i){
        for(uint32_t j = 0; j < grid[i].size(); ++j){
            std::cin >> number;
            grid[i][j] = number;
        }
    }
}

//traverses each empty cell, filling in a value from 1-9, checking to see if the choice selected violates constraints
//recursive backtracking algorithm
void Sudoku::sudoku_solver(uint32_t row, uint32_t col){

    //if current solution leads to a full, valid solution, then print the solution
    if(isFull()){
        print_solution();
        return;
    }
    
    //enumerates over all possible choices to insert in a given cell
    for(uint32_t choice = 1; choice <= 9; ++choice){

        while(grid[row][col] != 0){ //traverses the grid until it finds the first cell that is blank, aka contains 0.

            if(col == grid[row].size() - 1){ //if col is at the end of grid, then restart at new row, 0th column
                col = 0;
                row++;
            }
            else{
                col++;
            }
        }
        
        grid[row][col] = choice; //once empty cell is found, assign current choice to be the value 

        if(promising(row, col, choice)){ //checks that current choice doesn't violate the constraints

            //if choice is promising, then recurse on next empty cell
            if(col == grid[row].size() - 1){
                uint32_t temp = 0;
                sudoku_solver(row + 1, temp);
            }
            else{
                sudoku_solver(row, col + 1);
            }
        }
        grid[row][col] = 0; //if a branch leads to an invalid solution, then backtrack on previous cell by resetting value to 0, and go back through for loop with next choice
    }
}

//checks whether entire board has been filled 
bool Sudoku::isFull(){

    for(uint32_t i = 0; i < grid.size(); ++i){
        for(uint32_t j = 0; j < grid[i].size(); ++j){
            if(grid[i][j] == 0){
                return false;
            }
        }
    }
    return true;
}

//check whether choice selected at the given row and column violates constraints
bool Sudoku::promising(uint32_t row, uint32_t col, uint32_t choice){
    
    //2 pairs representing the start and end coordinates of a subgrid. Initially set to row and column values.
    std::pair<uint32_t, uint32_t> subgrid_start;
    std::pair<uint32_t, uint32_t> subgrid_end;
    subgrid_start.first = row;
    subgrid_start.second = col;
    subgrid_end.first = row;
    subgrid_end.second = col;
    
    uint32_t count = 0;

    //At most it will take 2 iterations from the given row and column to reach the start and end coordinates of the specific subgrid that needs to be checked.
    //subgrid_start is initally set to given row and column coordinates, so to reach the start of that specific subgrid, subtract by 1 until the coordinates are any combination
    //of 0, 3, 6.
    //0, 3, 6 are the only possible row and column pairs that can be the start of a subgrid. Ex: (0,0), (0,6), (3,6), (6,3), etc.
    //subgrid_end is initally set to given row and column coordinates, and to reach the end of the specific subgrid, add by 1 until the coordinates are any combination of 2, 5, 8.
    //2, 5, 8 are the only possible row and column pairs that can be the end of a subgrid. Ex: (2,2), (2,8), (5,8), (8,5), etc.
    while(count < 2){
        if(subgrid_start.first == 6 || subgrid_start.first == 3 || subgrid_start.first == 0){
            break;
        }
        subgrid_start.first--;
        count++;
    }

    count = 0;
    while(count < 2){
        if(subgrid_start.second == 6 || subgrid_start.second == 3 || subgrid_start.second == 0){
            break;
        }
        subgrid_start.second--;
        count++;
    }

    count = 0;
    while(count < 2){
        if(subgrid_end.first == 8 || subgrid_end.first == 5 || subgrid_end.first == 2){
            break;
        }
        subgrid_end.first++;
        count++;
    }

    count = 0;
    while(count < 2){
        if(subgrid_end.second == 8 || subgrid_end.second == 5 || subgrid_end.second == 2){
            break;
        }
        subgrid_end.second++;
        count++;
    }

    //traverses the subgrid
    for(; subgrid_start.first <= subgrid_end.first; ++subgrid_start.first){
        for(uint32_t start = subgrid_start.second; start <= subgrid_end.second; ++start){
            if(subgrid_start.first == row && start == col){ //skip over the given row,column pair that we are checking every other cell against
                continue;
            }
            if(grid[subgrid_start.first][start] == choice){ //if choice is found at any other location, constraint has been violated
                return false;
            }
        }
    }
    
    //searches every row north of row,column for any violations
    for(uint32_t n = row; n > 0; --n){ 
        if(grid[n - 1][col] == choice){
            return false;
        }
    }
    
    //searches every column east of row,column for any violations
    for(uint32_t e = col; e < 8; ++e){ 
        if(grid[row][e + 1] == choice){
            return false;
        }
    }
    
    //searches every row south of row,column for any violations
    for(uint32_t s = row; s < 8; ++s){ 
        if(grid[s + 1][col] == choice){
            return false;
        }
    }

    //searches every column west of row,column for any violatons
    for(uint32_t w = col; w > 0; --w){ 
        if(grid[row][w - 1] == choice){
            return false;
        }
    }

    return true;
}

//print complete, valid solution to the screen
void Sudoku::print_solution(){

    for(uint32_t i = 0; i < grid.size(); ++i){
        for(uint32_t j = 0; j < grid[i].size(); ++j){
            std::cout << grid[i][j] << " ";
        }
        std::cout << "\n";
    }
}

Sudoku::~Sudoku(){}

int main(int argc, char * argv[]){

    std::ios_base::sync_with_stdio(false);

    if(argc == 2){
        std::string argv1 = argv[1];
        if(argv1 == "--help"){
            std::cout << "Welcome! This program solves any given sudoku puzzle.\n" <<
            "To use the program, Enter a text file containing a 9x9 grid into the command line. Example: ./sudoku < inputfile.txt\n";
            exit(0);
        }
    }

    Sudoku s;
    uint32_t row = 0;
    uint32_t col = 0;
    s.readInput();
    s.sudoku_solver(row, col);
}