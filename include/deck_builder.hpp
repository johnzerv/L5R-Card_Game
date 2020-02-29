#pragma once

#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cstdlib>

#include "card.hpp"

class DeckBuilder {
private:
  std::list<GreenCard *> *green;
  std::list<BlackCard *> *black;

public:
  DeckBuilder();
  ~DeckBuilder();

  void createFateDeck();
  void createDynastyDeck();

  void deckShuffler();
  void deckShuffler(std::list<BlackCard *> *black);
  void deckShuffler(std::list<GreenCard *> *green);

  std::list<GreenCard*> * getGreen() { return green; }
  std::list<BlackCard*> * getBlack() { return black; }
};
