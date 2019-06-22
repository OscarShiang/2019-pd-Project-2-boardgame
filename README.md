# QtChess Game
## Intro
This is my project in 2019 Program Design. The whole project is write in C++/Qt and can be play as one player and play with other online. This project had 3 main mode to play:

### 2 Player Playing Offline
Using the basic rule of the chess reference from wikipedia to write the game. And when you pressed the chess you want to move, the possible blocks you can move will be shown. 

### 2 Player PLaying Online
The online playing mode is implement by using TCP protocle, the built-in header file of Qt which are TCP server and TCP socket to connect 2 players. But the data the game send to communicate is "string", so there are still some bugs when applying the special rules of the chess.

### Editing Puzzle
The whole chess board can br edited to the status you want and start to play.

## Playing Screen


## Problems wait to solve
1. The king on the board is not limited not to suicide.
2. When playing online, the special rule like castling and promotion will not display properly.