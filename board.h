#ifndef BOARD_H
#define BOARD_H

#include <Arduino.h>

// Chessboard representation
extern char board[8][8];

// Movement tracker for all pieces
extern bool hasMoved[8][8]; // Tracks whether a piece has moved

// Function to print the chessboard
void printBoard();
void printBoard(char boardState[8][8]); // Overloaded version

// Function to handle a move
void makeMove(int fromX, int fromY, int toX, int toY);

// Serial terminal function to process input commands
void serial_terminal();

// Function to evaluate all possible moves for a specified side
void evaluateMoves();

void random_move();

void toggle_led();

float evaluatePosition();
#endif
