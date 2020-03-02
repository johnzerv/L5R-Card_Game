#include <iostream>
#include <cstdlib>

#include "../include/gameboard.hpp"

using namespace std;

int main() {
  // A constant seed is used so that the game's output is reproducible
  srand(0);

  GameBoard gameboard;

  // Receives an optional argument, denoting the number of players that
  // will participate in the game

  gameboard.initializeGameBoard();

  // Starts the game
  gameboard.gameplay();

  return 0;
}
