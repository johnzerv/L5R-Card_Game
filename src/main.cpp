#include <iostream>

#include "../include/gameboard.hpp"

using namespace std;

int main(void){
  GameBoard gameboard;

  gameboard.initializeGameBoard();

  gameboard.gameplay();

  return 0;
}