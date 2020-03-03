#pragma once

#include <iostream>
#include <string>
#include <list>

#include "constants.hpp"

// Abstract class!

class Card {
private:
  std::string name;
  int cost;
  bool isTapped;

public:
  Card(std::string nam, int cst)
  : name(nam), cost(cst), isTapped(false)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  virtual ~Card() {}

  std::string getName() { return name; }
  int getCost() { return cost; }
  bool getIsTapped() { return isTapped; }

  void untap() { isTapped = false; }
  void tap() { isTapped = true; }

  virtual void print() {
    std::cout << "Card's attributes:" << std::endl
              << "Name: " << name << std::endl
              << "Cost: " << cost << std::endl
              << "Is tapped: " << (isTapped ? "true" : "false") << std::endl;
  }

  virtual cardType getType() = 0;
};

// Abstract class!

class GreenCard : public Card {
private:
  int attackBonus;
  int defenceBonus;
  int minimumHonour;
  std::string cardText;

  bool isUpgraded;
  int effectBonus;
  int effectCost;

public:
  GreenCard(std::string name, int cost, int attBonus, int defBonus,
            int minHonour, std::string cardTxt, int effBonus, int effCost)
  : Card(name, cost), attackBonus(attBonus), defenceBonus(defBonus),
    minimumHonour(minHonour), cardText(cardTxt), isUpgraded(false),
    effectBonus(effBonus), effectCost(effCost)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  int getAtkBonus() { return attackBonus; }
  int getDefBonus() { return defenceBonus; }
  int getMinHonour() { return minimumHonour; }
  std::string getCardTxt() { return cardText; }

  bool getIsUpgraded() { return isUpgraded; }
  int getEffectBonus() { return effectBonus; }
  int getEffectCost() { return effectCost; }

  void Upgrade() { isUpgraded = true; }

  virtual void print() {
    Card::print();
    std::cout << "Green card" << std::endl;
    std::cout << "Attack bonus: " << attackBonus << std::endl
              << "Defence bonus: " << defenceBonus << std::endl
              << "Minimum honour: " << minimumHonour << std::endl
              << "Card text: " << cardText << std::endl
              << "Effect bonus: " << effectBonus << std::endl
              << "Effect cost: " << effectCost << std::endl << std::endl;
  }

  virtual cardType getType() = 0;
};

class Follower : public GreenCard {
public:
  Follower(std::string name, int cost, int attBonus, int defBonus,
           int minHonour, std::string cardTxt, int effBonus, int effCost)
  : GreenCard(name, cost, attBonus, defBonus, minHonour, cardTxt, effBonus,
              effCost)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  virtual void print() {
    std::cout << "Follower" << std::endl;
    GreenCard::print();
  }

  void detach() { delete this; }

  virtual cardType getType() { return FOLLOWER; }
};

class Item : public GreenCard {
private:
  int durability;

public:
  Item(std::string name, int cost, int attBonus, int defBonus, int minHonour,
       std::string cardTxt, int effBonus, int effCost, int durab)
  : GreenCard(name, cost, attBonus, defBonus, minHonour, cardTxt, effBonus,
              effCost),
    durability(durab)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  int getDurability() { return durability; }
  void reduceDurability() { durability--; }
  void detach() { delete this; }

  virtual void print() {
    std::cout << "Item" << std::endl;
    GreenCard::print();
    std::cout << "Item's attribute :" << std::endl;
    std::cout << "Durability: " << durability << std::endl << std::endl;
  }

  virtual cardType getType() { return ITEM; }
};

// Abstract class!

class BlackCard : public Card {
private:
  bool isRevealed;

public:
  BlackCard(std::string name, int cost)
  : Card(name, cost), isRevealed(false)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  bool getRevealed() { return isRevealed; }

  void setRevealedFalse() { isRevealed = false; }
  void setRevealedTrue() { isRevealed = true; }

  virtual void print() {
     Card::print();
     std::cout << "BlackCard's attributes :" << std::endl;
     std::cout << "Is revealed: " << (isRevealed ? "true" : "false")
               << std::endl;
  }

  virtual cardType getType() = 0;
};

class Personality : public BlackCard {
private:
  int attack;
  int defence;
  int honour;

  std::list<Follower *> followers;
  std::list<Item *> items;

public:
  Personality(std::string name, int cost, int att, int def, int hon)
  : BlackCard(name, cost), attack(att), defence(def), honour(hon)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  ~Personality() {

    // Step 1: Detach all followers
    std::list<Follower *>::iterator followerIt = followers.begin();

    while (followerIt != followers.end()) {
      (*followerIt)->detach();
      followerIt++;
    }

    followers.clear();

    // Step 2: Detach all items
    std::list<Item *>::iterator itemIt = items.begin();

    while (itemIt != items.end()) {
      (*itemIt)->detach();
      itemIt++;
    }

    items.clear();
  }

  int getAtk() { return attack; }
  int getDef() { return defence; }
  int getHonour() { return honour; }

  void reduceHonour() { honour--; }

  std::list<Follower *> getFollowers() { return followers; }
  std::list<Item *> getItems() { return items; }

  bool hasMaxFollowers() { return followers.size() >= MAX_FOLLOWERS; }
  bool hasMaxItems() { return items.size() >= MAX_ITEMS; }

  void expandPersonality(Card *CardPtr, int type) {
    if (type == FOLLOWER)
      followers.push_back((Follower *) CardPtr);
    else if (type == ITEM)
      items.push_back((Item *) CardPtr);
  }

  int calculateAttackPoints() {
    int totalPoints = attack;

    std::list<Item *>::iterator itemIt = items.begin();
    while (itemIt != items.end()) {
      totalPoints += (*itemIt)->getAtkBonus();

      if ((*itemIt)->getIsUpgraded())
        totalPoints += (*itemIt)->getEffectBonus();

      itemIt++;
    }

    std::list<Follower *>::iterator followerIt = followers.begin();
    while (followerIt != followers.end()) {
      totalPoints += (*followerIt)->getAtkBonus();

      if (((*followerIt)->getIsUpgraded()))
        totalPoints += (*followerIt)->getEffectBonus();

      followerIt++;
    }

    return totalPoints;
  }

  int calculateDefencePoints() {
    int totalPoints = defence;

    std::list<Item *>::iterator itemIt = items.begin();
    while (itemIt != items.end()) {
      totalPoints += (*itemIt)->getDefBonus();

      if ((*itemIt)->getIsUpgraded())
        totalPoints += (*itemIt)->getEffectBonus();

      itemIt++;
    }

    std::list<Follower *>::iterator followerIt = followers.begin();
    while (followerIt != followers.end()) {
      totalPoints += (*followerIt)->getDefBonus();

      if (((*followerIt)->getIsUpgraded()))
        totalPoints += (*followerIt)->getEffectBonus();

      followerIt++;
    }

    return totalPoints;
  }

  void performSeppuku() { delete this; }

  virtual void print() {
    std::cout << "Personality" << std::endl;
    BlackCard::print();
    std::cout << "Personality attributes :" << std::endl;
    std::cout << "Attack: " << attack << std::endl
              << "Defence: " << defence << std::endl
              << "Honour: " << honour << std::endl << std::endl;
  }

  virtual cardType getType() { return PERSONALITY; }
};

class Holding : public BlackCard {
private:
  int harvestValue;
  Holdings holdingType; // e.g. PLAIN, MINE, etc. (see constants.h)

  Holding *upperHolding;
  Holding *subHolding;

public:
  Holding(std::string name, int cost, int harv, Holdings holdType)
  : BlackCard(name, cost), harvestValue(harv), holdingType(holdType),
    upperHolding(NULL), subHolding(NULL)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  int getHarvestValue() { return harvestValue; }
  Holdings getHoldingType() { return holdingType; }

  Holding * getUpperHolding() { return upperHolding; }
  Holding * getSubHolding() { return subHolding; }

  // Calculates possible bonuses, if a chain exists
  int tap(bool callSuperTap=true) {
    if (callSuperTap) Card::tap();

    if (holdingType == MINE && upperHolding != NULL)
      return harvestValue + 2; // MINE <-> GOLD_MINE 

    if (holdingType == GOLD_MINE) {
      if (subHolding != NULL && upperHolding != NULL)
        return harvestValue * 2; // MINE <-> GOLD_MINE <-> CRYSTAL_MINE
      if (subHolding != NULL)
        return harvestValue + 4; // MINE <-> GOLD_MINE
      if (upperHolding != NULL)
        return harvestValue + 5; // GOLD_MINE <-> CRYSTAL_MINE
    }

    if (holdingType == CRYSTAL_MINE) {
      if (subHolding != NULL && subHolding->subHolding != NULL)
        return harvestValue * 3; // MINE <-> GOLD_MINE <-> CRYSTAL_MINE
      if (subHolding != NULL)
        return harvestValue * 2; // GOLD_MINE <-> CRYSTAL_MINE
    }

    return harvestValue; // No chains exist
  }

  void setUpperHolding(Holding *upperHold) { upperHolding = upperHold; }
  void setSubHolding(Holding *subHold) { subHolding = subHold; }

  virtual void print() {
    std::cout << "Holding" << std::endl;
    BlackCard::print();
    std::cout << "Holding's attributes :" << std::endl;
    std::cout << "Harvest value: " << harvestValue << std::endl;

    if (upperHolding != NULL) {
      std::cout << "Upper holding :" << std::endl;
      std::cout << upperHolding->getName() << std::endl;
    }

    if (subHolding != NULL) {
      std::cout << "Sub holding :" << std::endl;
      std::cout << subHolding->getName() << std::endl;
    }

    std::cout << std::endl;
  }

  virtual cardType getType() { return HOLDING; }
};

class Stronghold : public Holding {
private:
  static int ID;

  int honour;
  int initialDefence;

public:
  Stronghold(std::string name, int cost, int hnr, int harv, int initDef)
  : Holding(name, cost, harv, STRONGHOLD), honour(hnr), initialDefence(initDef)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  virtual void print() {
    std::cout << "Stronghold" << std::endl;
    Holding::print();
    std::cout << "Stronghold's attributes :" << std::endl; 
    std::cout << "Honour: " << honour << std::endl
              << "Initial defence: " << initialDefence << "\n\n";
  }

  int getInitDefence() { return initialDefence; }
  static int nextID() { return ++ID; }

  virtual cardType getType() { return HOLDING; }
};
