#pragma once 

#include "../include/player.hpp"

class GameBoard {
  unsigned int numberOfPlayers;
  std::list<Player *> players;

  void startingPhase(Player &);
  void equipPhase(Player &);
  void battlePhase(Player &);
  void economyPhase(Player &);
  void finalPhase(Player &);
  
  void reducePlayersNumber(){ numberOfPlayers--; }

public:
  ~GameBoard();

  void initializeGameBoard(unsigned int);

  void printGameStatistics();

  void gameplay(/*...*/);

  unsigned int getNumberOfPlayers() const{ return numberOfPlayers; }
  std::list<Player *> getPlayersList() const {return players; }
};