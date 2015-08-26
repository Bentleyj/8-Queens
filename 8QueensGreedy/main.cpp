//
//  main.cpp
//  8QueensGreedy
//
//  Created by James Bentley on 8/24/15.
//  Copyright (c) 2015 James Bentley. All rights reserved.
//

#include <iostream>

//basic piece class with a row and a column
class Piece {
public:
    Piece(int row, int col) {
        row = row;
        col = col;
    }
    int row, col;
    bool takes(Piece *piece);
};


//queen, inherits from piece with it's particular "takes" method
class Queen: public Piece {
public:
    Queen(int row, int col):Piece(row, col){};
    
    //check another piece to see if this piece takes it
    bool takes(Piece *piece) {
        //check for horizontal take
        if(piece->row == row) {
            return 1;
        }
        //check for vertical take
        if(piece->col == col) {
            return 1;
        }
        //check for diagonal take with m = 1
        if(piece->col - col == 1*(piece->row - row)) {
            return 1;
        }
        //check for diagonal ake with m = -1
        if(piece->col - col == -1*(piece->row - row)) {
            return 1;
        }
        return 0;
    }
};

//return random int between two values
int random(int min, int max) {
    return (rand()%(max-min) + min);
}

//utility function for drawing the board
void drawBoard(Queen** queens, int size) {
    printf("  0  1  2  3  4  5  6  7\n");
    for(int row=0; row<size; row++) {
        printf("%i", row);
        for(int j=0; j<size; j++) {
            if(queens[j]->row == row)
                printf(" Q ");
            else
                printf(" _ ");
        }
        printf("\n");
    }
    printf("\n");
}

//compare two boards to see if they are the same
bool compare(Queen** queens1, Queen** queens2, int size) {
    for(int i=0; i<size; i++) {
        if(queens1[i]->row != queens2[i]->row || queens1[i]->col != queens2[i]->col) {
            return 0;
        }
    }
    return 1;
}

//main function
int main(int argc, const char * argv[]) {
    //array of correct boards
    Queen* correctPositions[92][8];
    //initialize correct board positions with 0, 0 values
    for(int i=0; i<92; i++) {
        for(int j=0; j<8; j++) {
            correctPositions[i][j] = new Queen(0, 0);
        }
    }
    //board to work with
    Queen* queens[8];
    //initialize working board
    for(int i=0; i<8; i++) {
        queens[i] = new Queen(i, i);
        queens[i]->row = i;
        queens[i]->col = i;
    }
    //last queen and best row to check if we're making the same optimization twice
    int lastWorstQueen = -1;
    int lastBestRow = -1;
    //number for boards found
    int numSuccesses = 0;
    //loop to find successful boards
    while(1) {
        //find the worst queen ie the queen with the most takes
        int maxTakes = 0;
        int worstQueen = -1;
        for(int i=0; i<8; i++) {
            int numTakes = 0;
            for(int j=0; j<8; j++) {
                if(queens[i]->takes(queens[j]))
                    if(i!=j) numTakes++;
            }
            if(numTakes > maxTakes) {
                maxTakes = numTakes;
                worstQueen = i;
            }
        }
        //if worst queen is -1 it means no queens had mroe than 0 takes so we've found a solution!
        if(worstQueen < 0) {
            printf("SUCCESS!! %i\n", numSuccesses);
            //check if the board was already in our list of solutions
            bool found = false;
            for(int i=0; i<numSuccesses; i++) {
                found = compare(queens, correctPositions[i], 8);
                if(found) break;
            }
            //if it wasn't found add it to the list of solutions!
            if(!found) {
                for(int i=0; i<8; i++) {
                    correctPositions[numSuccesses][i]->row = queens[i]->row;
                    correctPositions[numSuccesses][i]->col = queens[i]->col;
                }
                numSuccesses++;
            }
            //if we've found all the solutions we win!
            if(numSuccesses >= 92) {
                printf("All solutions found!\n");
                for(int i = 0; i < 92; i++) {
                    printf("%i\n", i);
                    drawBoard(correctPositions[i], 8);
                }
                break;
            }
            //start with a new random board
            for(int i=0; i<8; i++) {
                queens[i]->row = random(0, 8);
            }
            
        } else {
            //find the best row to move the worst queen to
            int minTakes = 9;
            int bestRow = -1;
            for(int i=0; i<8; i++) {
                int numTakes = 0;
                queens[worstQueen]->row = i;
                for(int j=0; j<8; j++) {
                    if(queens[worstQueen]->takes(queens[j]))
                        if(worstQueen!=j)numTakes++;
                }
                if(numTakes < minTakes) {
                    minTakes = numTakes;
                    bestRow = i;
                }
            }
            //move the wirst queen to it'c best row
            queens[worstQueen]->row = bestRow;
            //if we've repeated a move, ie we've found a local maxima, reshuffle the starting positions and start again
            if(lastWorstQueen == worstQueen && lastBestRow == bestRow) {
                for(int i=0; i<8; i++) {
                    queens[i]->row = random(0, 8);
                }
            }
            //save last move to check for local maxima
            lastWorstQueen = worstQueen;
            lastBestRow = bestRow;
        }
    }
    return 0;
}
