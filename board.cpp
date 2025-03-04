#include "board.h"
#include "evaluator.h"
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For seeding random number generator

int led_state = 1;



void toggle_led() {
  pinMode(LED_BUILTIN, OUTPUT);
  led_state = !led_state;

  if (led_state) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}





void random_move() {
  MoveEvaluation bestMove;
  float bestEval = 100000;  // Start with a high value

  for (int fromX = 0; fromX < 8; ++fromX) {
    for (int fromY = 0; fromY < 8; ++fromY) {
      char piece = board[fromX][fromY];
      if (piece >= 'a' && piece <= 'z') {
        for (int toX = 0; toX < 8; ++toX) {
          for (int toY = 0; toY < 8; ++toY) {
            if (isLegalMove(board, piece, fromX, fromY, toX, toY)) {
              char boardCopy[8][8];
              memcpy(boardCopy, board, 8 * 8 * sizeof(char));

              boardCopy[toX][toY] = boardCopy[fromX][fromY];
              boardCopy[fromX][fromY] = ' ';

              float eval = minimax(boardCopy, 2, false);  // Depth set to 1

              // Print move evaluation
              Serial.print("Considering move from ");
              Serial.print((char)('a' + fromY));
              Serial.print(8 - fromX);
              Serial.print(" to ");
              Serial.print((char)('a' + toY));
              Serial.print(8 - toX);
              Serial.print(": Eval = ");
              Serial.println(eval);

              if (eval < bestEval) {  // Minimize bestEval
                bestEval = eval;
                bestMove = {fromX, fromY, toX, toY, eval};
                Serial.print("New best move found: from ");
                Serial.print((char)('a' + fromY));  // Column
                Serial.print(8 - fromX);            // Row
                Serial.print(" to ");
                Serial.print((char)('a' + toY));    // Column
                Serial.print(8 - toX);              // Row
                Serial.print(": Eval = ");
                Serial.println(bestEval);
              }
            }
          }
        }
      }
    }
  }

  // Print out the final best move chosen
  Serial.print("Final best move chosen: from ");
  Serial.print((char)('a' + bestMove.fromY));  // Column
  Serial.print(8 - bestMove.fromX);            // Row
  Serial.print(" to ");
  Serial.print((char)('a' + bestMove.toY));    // Column
  Serial.print(8 - bestMove.toX);              // Row
  Serial.print(": Eval = ");
  Serial.println(bestEval);

  // Execute the best move if found
  if (bestMove.fromX != -1 && bestMove.fromY != -1) {
    makeMove(bestMove.fromX, bestMove.fromY, bestMove.toX, bestMove.toY);
    Serial.print("Machine moved from ");
    Serial.print((char)('a' + bestMove.fromY));  // Column
    Serial.print(8 - bestMove.fromX);            // Row
    Serial.print(" to ");
    Serial.print((char)('a' + bestMove.toY));    // Column
    Serial.println(8 - bestMove.toX);            // Row
  } else {
    Serial.println("No valid move found!");
  }
}





// Initialize the chessboard
char board[8][8] = {
  {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
  {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
  {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};





// Movement tracker for all pieces
bool hasMoved[8][8] = {false}; // Initially, no pieces have moved



void makeMove(int fromX, int fromY, int toX, int toY) {
  char piece = board[fromX][fromY];
  char target = board[toX][toY]; // The piece being captured (if any)

  if (piece == ' ') {
    Serial.println("Invalid move: No piece at the selected position.");
    return;
  }

  // Validate the move using the new isLegalMove
  if (isLegalMove(board, piece, fromX, fromY, toX, toY)) { // updated
    // Check if this is a capture
    if (target != ' ') {
      Serial.print("Capture detected! ");
      Serial.print(piece);
      Serial.print(" captures ");
      Serial.println(target);
    }

    // Update the movement tracker
    hasMoved[fromX][fromY] = true;

    // Move the piece
    board[fromX][fromY] = ' ';
    board[toX][toY] = piece;

    // Display the updated board
    Serial.println("Move made:");
    printBoard();
  } else {
    Serial.println("Invalid move: Not a legal move for this piece.");
  }
}


void printBoard() {
    Serial.println("    a b c d e f g h");
  Serial.println("    _ _ _ _ _ _ _ _");
   // Serial.print();

    
 // Serial.println("  a b c d e f g h");
  for (int i = 0; i < 8; i++) {
    Serial.print(8 - i); // Row labels
    Serial.print("  |");
    for (int j = 0; j < 8; j++) {
      if (board[i][j] == ' ') { // For visuals, replace blank spots with points
        Serial.print('_');
      } else {
        Serial.print(board[i][j]);
      }
      Serial.print("|");
    }
        Serial.print("  ");
        Serial.print(8 - i); // Row labels

    Serial.println();
  }
    Serial.println();
      Serial.println("    a b c d e f g h");
  Serial.println();
}

// Overloaded printBoard function that accepts a board state
void printBoard(char boardState[8][8]) {
  Serial.println("  a b c d e f g h");
  for (int i = 0; i < 8; i++) {
    Serial.print(8 - i); // Row labels
    Serial.print(" ");
    for (int j = 0; j < 8; j++) {
      if (boardState[i][j] == ' ') { // For visuals, replace blank spots with points
        Serial.print('.');
      } else {
        Serial.print(boardState[i][j]);
      }
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}




// Serial terminal function to process input commands
void serial_terminal() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    Serial.println();     Serial.println();     Serial.println();     Serial.println();     Serial.println();
    command.trim(); // Remove any trailing whitespace
    if (command.length() == 4) {
      // Parse the input command (e.g., "e2e4")
      int fromX = 8 - (command[1] - '0'); // Convert row (e.g., '2') to array index
      int fromY = command[0] - 'a';       // Convert column (e.g., 'e') to array index
      int toX = 8 - (command[3] - '0');   // Convert destination row
      int toY = command[2] - 'a';         // Convert destination column

      // Attempt to make the move
      makeMove(fromX, fromY, toX, toY);
      delay(1000);
      // Pass the global board to evaluateMoves
      evaluateMoves(board);
      evaluateAllMachineMoves();

      random_move();
    } else {
      Serial.println("Invalid command format. Use the format 'e2e4'.");
    }

    float score = evaluatePosition();
    Serial.print("Position score: ");
    Serial.println(score);
  }
}
