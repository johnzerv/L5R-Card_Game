#pragma once

#include <iostream>
#include <string>
#include <list>

#include "card.hpp"
#include "deck_builder.hpp"

class Player {
private:
  int money;
  int honour;
  int initialDefence;
  int numberOfProvinces;

  DeckBuilder decks;

  std::list<BlackCard *> provinces;
  std::list<GreenCard *> hand;

  std::list<Holding *>     holdings;
  std::list<Personality *> army;
  std::list<Personality *> activatedPersonalities;

  bool hasProvinces() { return !provinces.empty(); }
 
  // Auxiliary methods for equipment/economy phase
  bool isMoneyEnough(Card *);
  bool tapHoldings(int);
  bool upgradeGreenCard(GreenCard *);
  bool wantToUpgrade();
  void formMineChain(Holding *);
  int getMineType(Holding *);

  void printUntappedArmy(); // Used in activatePersonalities()

public:
  Player();
  ~Player();

  // Methods for starting/final phase
  void untapEverything();
  void drawFateCard();
  void drawDynastyCard(std::list<BlackCard *>::iterator);
  void revealProvinces();
  void printHoldings();
  void printHand();
  void printArena();
  void printProvinces();
  void printArmy();
  void discardSurplusFateCards();

  void buyGreenCard(int, int, int &);
  void buyBlackCard(int, int &);

  // Methods for battle phase
  void activatePersonalities();
  int calculateAttackPoints();
  int calculateDefencePoints();
  bool destroyProvince(int);
  void reduceProvinces() { numberOfProvinces--; }
  void destroyActPers();
  void discardActPCards(int);
  void battleReverberations();
  void reduceActPersHonour();
  void emptyWallet() { money = 0; }
  int getBalance();

  bool checkWinningCondition(std::list<Player *>);

  // Getters
  std::list<BlackCard *> getProvinces() { return provinces; }
  std::list<GreenCard *> getHand() { return hand; }
  std::list<Holding *> getHoldings() { return holdings; }
  std::list<Personality *> getArmy() { return army; }

  int getSizeOfHand() { return hand.size(); }
  int getSizeOfArmy() { return army.size(); }
  int getMoney() { return money; }
  int getHonour() const { return honour; }
  int getInitialDefence() { return initialDefence; }
  int getNumberOfProvinces()  { return numberOfProvinces; }
};
