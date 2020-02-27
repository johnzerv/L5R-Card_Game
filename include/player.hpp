#pragma once

#include <iostream>
#include <string>
#include <list>

#include "../auxiliary_files/DeckBuilder.hpp"
#include "../include/card.hpp"
#include "../include/enums.hpp"

#define NO_HAND_CARDS 6
#define NO_OF_PROVINCES 4

class Player {
private:
  unsigned money;
  unsigned honour;
  unsigned initialDefence;

  DeckBuilder decks;

  unsigned numberOfProvinces;
  std::list <BlackCard *> provinces;

  std::list <GreenCard *> hand;

  std::list <Holding *> holdings;
  std::list <Personality *> army;
  std::list <Personality *> activatedPersonalities;

  void printUntappedArmy();

  bool hasProvinces() const{ return !(provinces.empty()); }
 
 /* Useful methods for equipment/economy Phase */
  bool isMoneyEnough(Card*);
  bool tapHoldings(Card*);
  bool upgradeGreenCard(GreenCard *);
  bool hasMaxFollowers(Personality *);
  bool hasMaxItems(Personality *);
  bool wantToUpgrade() const;

  /*Getters/Setters*/
  std::list <BlackCard *> getProvinces() const{ return provinces; }
  std::list <GreenCard *> getHand() const{ return hand; }
  std::list <Holding *> getHoldings() const{ return holdings; }
  std::list <Personality *> getArmy() const{ return army; }

  unsigned getSizeOfHand() const{ return hand.size(); }
  unsigned getSizeOfArmy() const{ return army.size(); }
  unsigned getMoney() const{ return money; }
  unsigned getHonour() const{ return honour; }
  unsigned getInitialDefence() const{ return initialDefence; }
  unsigned getNumberOfProvinces() const { return numberOfProvinces; }

public:
  Player();
  ~Player();

  /* Methods for starting/final Phase */
  void untapEverything();
  void drawFateCard();
  void drawDynastyCard();
  void revealProvinces();
  void printHoldings();
  void printHand();
  void printArena();
  void printProvinces();
  void printArmy();
  void discardSurplusFateCards();

  void buyGreenCard(int, int);
  void buyBlackCard(int);

/* Methods added for battlePhase */
  void activatePersonalities();
  unsigned calculateAttackPoints();
  unsigned calculateDefencePoints();
  bool destroyProvince(int);
  void reduceProvinces() { numberOfProvinces--; }
  void destroyActPers();
  void discardActPCards(int);
  void battleReverberations();
  void reduceActPersHonour();

  bool checkWinningCondition(Player *, unsigned int) const;
};