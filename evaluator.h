#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "board.h"

// Function to check if a move is legal for a given piece
bool isLegalMove(char boardState[8][8], char piece, int fromX, int fromY, int toX, int toY);

// Helper function to check if the path between two points is clear (for sliding pieces)
bool isPathClear(char boardState[8][8], int fromX, int fromY, int toX, int toY);

// Helper function to check if the target square contains an opponent's piece
bool isOpponentPiece(char piece, char target);

// Helper function to check if the target square contains a piece of the same color
bool isOwnPiece(char piece, char target);

struct MoveEvaluation {
  int fromX, fromY, toX, toY; // Move details
  float evaluation;           // Evaluation score of the resulting position
};

float evaluatePosition(char boardState[8][8]);
float evaluatePosition();
void evaluateAllMachineMoves();
int minimax(char boardState[8][8], int depth, bool isMaximizingPlayer);
void evaluateMoves(char boardState[8][8]);  // Ensure the prototype matches the definition

extern const int knightMoves[8][2];

#endif
