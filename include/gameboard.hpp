#pragma once 

#include <list>

#include "player.hpp"

class GameBoard {
  int numberOfPlayers;
  std::list<Player *> players;

  void startingPhase(Player &);
  void equipPhase(Player &);
  void battlePhase(Player &);
  void economyPhase(Player &);
  void finalPhase(Player &);
  
  void reducePlayersNumber() { numberOfPlayers--; }

public:
  ~GameBoard();

  // Basic functionalities of our game
  void initializeGameBoard(int = 2);
  void printGameStatistics();
  void gameplay();

  // Getters
  int getNumberOfPlayers() { return numberOfPlayers; }
  std::list<Player *> getPlayersList() { return players; }
};

bool comparePlayers(const Player *a, const Player *b);
