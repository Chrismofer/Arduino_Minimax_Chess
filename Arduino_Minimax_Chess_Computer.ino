//PicoChess chess playing algorithm

#include "board.h"
#include "evaluator.h"




void setup() {
  // Initialize serial communication

  Serial.begin(2000000);
  while (!Serial) {
    delay(10);
  }
  Serial.println();
  Serial.println("RP2040/2350 Chess Computer by Chris B.");
  Serial.println();    
  Serial.println("Implements 'naive' Minimax algorithm (brute force position search) and only looks 2 moves & replies ahead");
  Serial.println("Evaluation function is entirely materialistic. each piece's importance is weighted.");
  Serial.println("With no other optimizations (alpha-beta pruning, dual-core and parallel processing),");
  Serial.println("it takes ~4 seconds to evaluate 2 moves deep and 5 minutes to evaluate 3 moves deep.");
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(">first move? try e2e4<");
  Serial.println();   
  Serial.println();   
  Serial.println();
  Serial.println("SHALL WE PLAY A GAME?");
  Serial.println();
  // Display the initial chessboard
  printBoard();
}






void loop() {
  // Process player commands via the serial terminal
  serial_terminal();

}
