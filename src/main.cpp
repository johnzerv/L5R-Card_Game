#include <iostream>

#include "../include/gameboard.hpp"

using namespace std;

int main(void){
  srand(time(NULL));

  GameBoard gameboard;

  gameboard.initializeGameBoard();

  gameboard.gameplay();

  return 0;
}