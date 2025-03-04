![image](https://github.com/user-attachments/assets/4b08cd8d-1933-47b8-8b30-7dcd90550f1f)


To program your arduino-compatible with this software, use the arduino IDE, load all files, and click upload. It doesn't use any special libraries.
Locate the com port belonging to your microcontroller's serial connection and set the baud rate to 2000000, or your preferred rate,
which is set in the Setup() function. 



This program is a very basic chess computer that implements the Minimax algorithm.
It checks piece by piece, move by move the evaluate the position resulting from each move.
it then does the same for each of the opponents possible reply moves, and recurses twice. 

whichever move leads to the highest position evaluation is chosen as a result of the evaluation and it waits for your next move.

The evaluation function is based purely on material gains and losses. Moves resulting in a captured King are highly evaluated.
Moves resulting in loss of the King are strongly avoided.

Missing features include checkmate / captured king / end of game logic. 

Next steps would be adding an Alpha-Beta 'pruning' step to the Minimax function in order to lighten it's workload.

Modern chess-playing algorithms use 'monte carlo' (random) move generation to look far into the future albiet on a subset of the total possible moves.
I think this microcontroller could be a far 'smarter' opponant if, after 2 rounds of Minimax, it used Monte Carlo move generation to explore promising avenues from there.

The evaluation function would be smarter if specific positions could be given specific weights and biases. 
This is an obvious place to apply neural networks to detect advantagous or poor positions and inform the move-selector.
Historically, chess computers like Deep Blue have been hard coded to detectg specific positions and trigger specific plans of attack from there.
