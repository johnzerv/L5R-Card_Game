#pragma once 

#include "../include/player.hpp"

class GameBoard {
  unsigned int numberOfPlayers;
  Player *players;

  void startingPhase(Player &);
  void equipPhase(Player &);
  void battlePhase(Player &);
  void economyPhase(Player &);
  void finalPhase(Player &);
  
public:
  GameBoard(){ players = nullptr; }
  ~GameBoard();

  void initializeGameBoard(unsigned int);

  void printGameStatistics();

  void gameplay(/*...*/);

  inline unsigned int getNumberOfPlayers() const{ return numberOfPlayers; }
};