#include "evaluator.h"
#include "board.h"






int minimax(char boardState[8][8], int depth, bool isMaximizingPlayer) {
  if (depth == 0) {
    float eval = evaluatePosition(boardState);
    // Print out the evaluation and board for the leaf node
    Serial.print("Depth 0 Evaluation: ");
    Serial.println(eval);
 //   printBoard(boardState);
    return eval;
  }
    toggle_led();
  if (isMaximizingPlayer) {
    int bestEval = 100000;  // Start with a high value
    for (int fromX = 0; fromX < 8; ++fromX) {
      for (int fromY = 0; fromY < 8; ++fromY) {
        char piece = boardState[fromX][fromY];
        if (piece >= 'a' && piece <= 'z') {
          for (int toX = 0; toX < 8; ++toX) {
            for (int toY = 0; toY < 8; ++toY) {
              if (isLegalMove(boardState, piece, fromX, fromY, toX, toY)) {
                char boardCopy[8][8];
                memcpy(boardCopy, boardState, 8 * 8 * sizeof(char));

                boardCopy[toX][toY] = boardCopy[fromX][fromY];
                boardCopy[fromX][fromY] = ' ';

                int eval = minimax(boardCopy, depth - 1, false);
                bestEval = min(bestEval, eval);  // Minimize for the machine

                // Print out the evaluation and board for each move
                Serial.print("Maximizing Player - Move from ");
                Serial.print((char)('a' + fromY));
                Serial.print(8 - fromX);
                Serial.print(" to ");
                Serial.print((char)('a' + toY));
                Serial.print(8 - toX);
                Serial.print(": Eval = ");
                Serial.println(eval);
           //     printBoard(boardCopy);
              }
            }
          }
        }
      }
    }
    return bestEval;
  } else {
    int bestEval = -100000;  // Start with a low value
    for (int fromX = 0; fromX < 8; ++fromX) {
      for (int fromY = 0; fromY < 8; ++fromY) {
        char piece = boardState[fromX][fromY];
        if (piece >= 'A' && piece <= 'Z') {
          for (int toX = 0; toX < 8; ++toX) {
            for (int toY = 0; toY < 8; ++toY) {
              if (isLegalMove(boardState, piece, fromX, fromY, toX, toY)) {
                char boardCopy[8][8];
                memcpy(boardCopy, boardState, 8 * 8 * sizeof(char));

                boardCopy[toX][toY] = boardCopy[fromX][fromY];
                boardCopy[fromX][fromY] = ' ';

                int eval = minimax(boardCopy, depth - 1, true);
                bestEval = max(bestEval, eval);  // Maximize for the opponent

                // Print out the evaluation and board for each move
                Serial.print("Minimizing Player - Move from ");
                Serial.print((char)('a' + fromY));
                Serial.print(8 - fromX);
                Serial.print(" to ");
                Serial.print((char)('a' + toY));
                Serial.print(8 - toX);
                Serial.print(": Eval = ");
                Serial.println(eval);
             //   printBoard(boardCopy);
              }
            }
          }
        }
      }
    }
    return bestEval;
  }
}





// Knight's move offsets
const int knightMoves[8][2] = {
  { -2, -1}, { -2, 1}, { -1, -2}, { -1, 2},
  {1, -2},  {1, 2},  {2, -1},  {2, 1}
};




bool isPathClear(char boardState[8][8], int fromX, int fromY, int toX, int toY) {
  int dx = (toX - fromX) == 0 ? 0 : (toX - fromX) / abs(toX - fromX);
  int dy = (toY - fromY) == 0 ? 0 : (toY - fromY) / abs(toY - fromY);

  int x = fromX + dx;
  int y = fromY + dy;

  while (x != toX || y != toY) {
    if (boardState[x][y] != ' ') {
      return false;
    }
    x += dx;
    y += dy;
  }
  return true;
}





// Helper function to check if the target piece belongs to the opponent
bool isOpponentPiece(char piece, char target) {
  if (piece >= 'A' && piece <= 'Z') {
    // White piece
    return target >= 'a' && target <= 'z';
  } else {
    // Black piece
    return target >= 'A' && target <= 'Z';
  }
}

// Helper function to check if the destination square contains a piece of the same color
bool isOwnPiece(char piece, char target) {
  if (piece >= 'A' && piece <= 'Z') {
    // White piece
    return target >= 'A' && target <= 'Z';
  } else {
    // Black piece
    return target >= 'a' && target <= 'z';
  }
}

bool isLegalMove(char boardState[8][8], char piece, int fromX, int fromY, int toX, int toY) {
  int dx = toX - fromX;
  int dy = toY - fromY;

  // Ensure target coordinates are within bounds
  if (toX < 0 || toX > 7 || toY < 0 || toY > 7) {
    return false;
  }

  // Prevent moving to a square occupied by a piece of the same color
  if (isOwnPiece(piece, boardState[toX][toY])) {
    return false;
  }

  switch (piece) {
    case 'P': { // White Pawn
      if (dy == 0) {
        if (dx == -1 && boardState[toX][toY] == ' ') {
          return true; // One step directly forward
        }
        if (dx == -2 && fromX == 6 && !hasMoved[fromX][fromY] &&
            boardState[fromX - 1][fromY] == ' ' && boardState[toX][toY] == ' ') {
          return true; // Two steps forward on first move
        }
      }
      if (dx == -1 && abs(dy) == 1 && isOpponentPiece(piece, boardState[toX][toY])) {
        return true; // Diagonal capture
      }
      break;
    }
    case 'p': { // Black Pawn
      if (dy == 0) {
        if (dx == 1 && boardState[toX][toY] == ' ') {
          return true; // One step forward
        }
        if (dx == 2 && fromX == 1 && !hasMoved[fromX][fromY] &&
            boardState[fromX + 1][fromY] == ' ' && boardState[toX][toY] == ' ') {
          return true; // Two steps forward on first move
        }
      }
      if (dx == 1 && abs(dy) == 1 && isOpponentPiece(piece, boardState[toX][toY])) {
        return true;
      }
      break;
    }
    case 'R': case 'r': // Rook
      if ((dx == 0 || dy == 0) && isPathClear(boardState, fromX, fromY, toX, toY)) {
        return true;
      }
      break;
    case 'B': case 'b': // Bishop
      if (abs(dx) == abs(dy) && isPathClear(boardState, fromX, fromY, toX, toY)) {
        return true;
      }
      break;
    case 'Q': case 'q': // Queen
      if ((dx == 0 || dy == 0 || abs(dx) == abs(dy)) && isPathClear(boardState, fromX, fromY, toX, toY)) {
        return true;
      }
      break;
    case 'N': case 'n': // Knight
      for (int i = 0; i < 8; i++) {
        int newX = fromX + knightMoves[i][0];
        int newY = fromY + knightMoves[i][1];
        if (newX == toX && newY == toY && !isOwnPiece(piece, boardState[toX][toY])) {
          return true;
        }
      }
      break;
    case 'K': case 'k': // King
      if (abs(dx) <= 1 && abs(dy) <= 1 && !isOwnPiece(piece, boardState[toX][toY])) {
        return true;
      }
      break;
  }
  return false;
}


void evaluateAllMachineMoves() {
  // Array to store moves and their evaluations
  MoveEvaluation moveEvals[256]; // Assuming max 256 possible moves
  int moveCount = 0;

  // Traverse the board and collect all legal moves for machine's pieces
  for (int fromX = 0; fromX < 8; ++fromX) {
    for (int fromY = 0; fromY < 8; ++fromY) {
      char piece = board[fromX][fromY];

      // Check if the piece belongs to the machine
      if (piece >= 'a' && piece <= 'z') {
        for (int toX = 0; toX < 8; ++toX) {
          for (int toY = 0; toY < 8; ++toY) {
            if (isLegalMove(board, piece, fromX, fromY, toX, toY)) { // updated
              // Create a copy of the board
              char boardCopy[8][8];
              memcpy(boardCopy, board, 8 * 8 * sizeof(char));

              // Make the move on the board copy
              boardCopy[toX][toY] = boardCopy[fromX][fromY];
              boardCopy[fromX][fromY] = ' ';

              // Evaluate the new position
              float eval = evaluatePosition(boardCopy);

              // Store the move and its evaluation
              moveEvals[moveCount++] = {fromX, fromY, toX, toY, eval};
            }
          }
        }
      }
    }
  }

  // Print the evaluations
  Serial.println("Evaluations of all possible machine moves:");
  for (int i = 0; i < moveCount; ++i) {
    MoveEvaluation mv = moveEvals[i];
    Serial.print("Move ");
    Serial.print((char)('a' + mv.fromY)); // From column
    Serial.print(8 - mv.fromX);           // From row
    Serial.print(" to ");
    Serial.print((char)('a' + mv.toY));   // To column
    Serial.print(8 - mv.toX);             // To row
    Serial.print(" - Evaluation: ");
    Serial.println(mv.evaluation);
  }
}


float evaluatePosition() {
  return evaluatePosition(board);
}


float evaluatePosition(char boardState[8][8]) {
  float score = 0.0;

  // Evaluate material for both sides
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      char piece = boardState[i][j];
      switch (piece) {
        case 'P': score += 1.0; break;
        case 'N': score += 3.0; break;
        case 'B': score += 3.0; break;
        case 'R': score += 5.0; break;
        case 'Q': score += 9.0; break;
        case 'K': score += 100.0; break; // High value for king
        case 'p': score -= 1.0; break;
        case 'n': score -= 3.0; break;
        case 'b': score -= 3.0; break;
        case 'r': score -= 5.0; break;
        case 'q': score -= 9.0; break;
        case 'k': score -= 100.0; break; // High value for king
        default: break;
      }
    }
  }

  // You can add more evaluation criteria here (mobility, pawn structure, etc.)

  return score;
}





void evaluateMoves(char boardState[8][8]) {
  // Arrays to tally legal moves for each player
  int playerMoveCounts[8][8] = {0};
  int machineMoveCounts[8][8] = {0};

  // Evaluate player moves
  for (int fromX = 0; fromX < 8; ++fromX) {
    for (int fromY = 0; fromY < 8; ++fromY) {
      char piece = boardState[fromX][fromY];

      // Player's pieces are uppercase
      if (piece >= 'A' && piece <= 'Z') {
        for (int toX = 0; toX < 8; ++toX) {
          for (int toY = 0; toY < 8; ++toY) {
            if (isLegalMove(boardState, piece, fromX, fromY, toX, toY)) {
              playerMoveCounts[fromX][fromY]++;
            }
          }
        }
      }
    }
  }

  // Evaluate machine moves
  for (int fromX = 0; fromX < 8; ++fromX) {

    for (int fromY = 0; fromY < 8; ++fromY) {
      char piece = boardState[fromX][fromY];

      // Machine's pieces are lowercase
      if (piece >= 'a' && piece <= 'z') {
        for (int toX = 0; toX < 8; ++toX) {
          for (int toY = 0; toY < 8; ++toY) {
            if (isLegalMove(boardState, piece, fromX, fromY, toX, toY)) {
              machineMoveCounts[fromX][fromY]++;
            }
          }
        }
      }
    }
  }

  // Print the combined results
  Serial.println("Legal move counts:");
  Serial.println("    a b c d e f g h");
  Serial.println("  _| | | | | | | | |");
  for (int i = 0; i < 8; ++i) {
    Serial.print(8 - i); // Row labels
    Serial.print(" _ ");
    for (int j = 0; j < 8; ++j) {
      // Display player and machine move counts in one cell
      if (playerMoveCounts[i][j] > 0 && machineMoveCounts[i][j] > 0) {
        // Both players have moves on this square
        Serial.print(playerMoveCounts[i][j]);
        Serial.print("/");
        Serial.print(machineMoveCounts[i][j]);
      } else if (playerMoveCounts[i][j] > 0) {
        // Only player has moves
        Serial.print(playerMoveCounts[i][j]);
      } else if (machineMoveCounts[i][j] > 0) {
        // Only machine has moves
        Serial.print(machineMoveCounts[i][j]);
      } else {
        // No moves for either player
        Serial.print(".");
      }
      Serial.print(" "); // Space between columns
    }
    Serial.println();
  }
  Serial.println();

  int playerMobility = 0, machineMobility = 0;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      playerMobility += playerMoveCounts[i][j];
      machineMobility += machineMoveCounts[i][j];
    }
  }
  Serial.print("Player mobility: ");
  Serial.println(playerMobility);
  Serial.print("Machine mobility: ");
  Serial.println(machineMobility);
}
