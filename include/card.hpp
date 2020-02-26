#pragma once

#include <iostream>
#include <string>
#include <list>

#include "enums.hpp"

/* Abstract class */
class Card {
private:
  std::string name;
  unsigned cost;
  bool isTapped;

public:
  Card(std::string nam, unsigned cst)
  : name(nam), cost(cst), isTapped(false)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  std::string getName() const { return name; }
  unsigned getCost() const { return cost; }
  bool getIsTapped() const { return isTapped; }

  void untap() { isTapped = false; }
  void tap() { isTapped = true; }

  virtual void print() const {
    std::cout << "Card:" << std::endl
              << "Name: " << name << std::endl
              << "Cost: " << cost << std::endl
              << "isTapped: " << (isTapped ? "true" : "false") << std::endl;
  }
  
  virtual cardType getType() const = 0;
};

/* Abstract class */
class GreenCard : public Card {
private:
  unsigned attackBonus;
  unsigned defenceBonus;
  unsigned minimumHonour;
  std::string cardText;

  bool isUpgraded;
  unsigned effectBonus;
  unsigned effectCost;

public:
  GreenCard(std::string name, unsigned cost, unsigned attBonus,
            unsigned defBonus, unsigned minHonour, std::string cardTxt,
            unsigned effBonus, unsigned effCost)
  : Card(name, cost), attackBonus(attBonus), defenceBonus(defBonus),
    minimumHonour(minHonour), cardText(cardTxt), isUpgraded(false),
    effectBonus(effBonus), effectCost(effCost)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  unsigned getAtkBonus() const { return attackBonus; }
  unsigned getDefBonus() const { return defenceBonus; }
  unsigned getMinHonour() const { return minimumHonour; }
  std::string getCardTxt() const { return cardText; }

  bool getIsUpgraded()const{ return isUpgraded; }
  unsigned getEffectBonus() const { return effectBonus; }
  unsigned getEffectCost() const { return effectCost; }

  void Upgrade(){ isUpgraded = true; }

  virtual void print() const {
    std::cout << "GreenCard - ";
    Card::print();
    std::cout << "attackBonus: " << attackBonus << std::endl
              << "defenceBonus: " << defenceBonus << std::endl
              << "minimumHonour: " << minimumHonour << std::endl
              << "cardText: " << cardText << std::endl
              << "effectBonus: " << effectBonus << std::endl
              << "effectCost: " << effectCost << std::endl;
  }

  virtual cardType getType() const = 0;
};

class Follower : public GreenCard {
public:
  Follower(std::string name, unsigned cost, unsigned attBonus,
           unsigned defBonus, unsigned minHonour, std::string cardTxt,
           unsigned effBonus, unsigned effCost)
  : GreenCard(name, cost, attBonus, defBonus, minHonour, cardTxt, effBonus,
              effCost)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  virtual void print() const {
    std::cout << "Follower - ";
    GreenCard::print();
  }

  void detach(){ delete(this); }

  virtual cardType getType() const { return FOLLOWER; }
};

class Item : public GreenCard {
private:
  unsigned durability;

public:
  Item(std::string name, unsigned cost, unsigned attBonus, unsigned defBonus,
       unsigned minHonour, std::string cardTxt, unsigned effBonus,
       unsigned effCost, unsigned durab)
  : GreenCard(name, cost, attBonus, defBonus, minHonour, cardTxt, effBonus,
              effCost),
    durability(durab)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  unsigned getDurability() const { return durability; }
  void reduceDurability() { durability--; }
  void detach(){ delete(this); }

  virtual void print() const {
    std::cout << "Item - ";
    GreenCard::print();
    std::cout << "durability: " << durability << std::endl;
  }

  virtual cardType getType() const { return ITEM; }
};

/* Abstract class */
class BlackCard : public Card {
private:
  bool isRevealed;

public:
  BlackCard(std::string name, unsigned cost)
  : Card(name, cost), isRevealed(false)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  virtual ~BlackCard(){}

  bool getRevealed() const { return isRevealed; }

  void setRevealedFalse() { isRevealed = false; }
  void setRevealedTrue() { isRevealed = true; }

  virtual void print() const {
     std::cout << "BlackCard - ";
     Card::print();
     std::cout << "isRevealed: " << (isRevealed ? "true" : "false") << std::endl;
  }

  virtual cardType getType() const = 0;
};

#include "../auxiliary_files/DeckBuilder.hpp"

class Personality : public BlackCard {
private:
  unsigned attack;
  unsigned defence;
  unsigned honour;

  bool isDead;

  std::list <Follower *> followers;
  std::list <Item *> items;

public:
  Personality(std::string name, unsigned cost, unsigned att, unsigned def,
              unsigned hon)
  : BlackCard(name, cost), attack(att), defence(def), honour(hon), isDead(false)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  ~Personality(){
    std::list<Follower *>::iterator it1;
    for(it1 = followers.begin(); it1 != followers.end(); it1++)
      (*it1)->detach();
    followers.clear();

    std::list<Item *>::iterator it2;
    for(it2 = items.begin(); it2 != items.end(); it2++)
      (*it2)->detach();
    items.clear();
  }

  unsigned getAtk() const { return attack; }
  unsigned getDef() const { return defence; }
  unsigned getHonour() const { return honour; }
  bool getIsDead() const { return isDead; }

  void reduceHonour(){ honour--; }

  std::list <Follower *> getFollowers() const { return followers; }
  std::list <Item *> getItems() const { return items; }

  bool hasMaxFollowers() const{ return followers.size() <= MAX_FOLLOWERS; }
  bool hasMaxItems() const{ return items.size() <= MAX_ITEMS; }

  void expandPersonality(Card *CardPtr, unsigned type) {
    if(type == FOLLOWER)
      followers.push_back((Follower *) CardPtr);
    else if(type == ITEM)
      items.push_back((Item *) CardPtr);
  }

  unsigned calculateAttackPoints(){
    unsigned totalPoints = attack;
    std::list<Item *>::iterator it1;

    for(it1 = items.begin(); it1 != items.end(); it1++){
      totalPoints += (*it1)->getAtkBonus();
      if((*it1)->getIsUpgraded())
        totalPoints += (*it1)->getEffectBonus();
    }

    std::list<Follower *>::iterator it2;
    for(it2 = followers.begin(); it2 != followers.end(); it2++){
      totalPoints += (*it2)->getAtkBonus();
      if(((*it2)->getIsUpgraded()))
        totalPoints += (*it2)->getEffectBonus();
    }

    return totalPoints;
  }

  unsigned calculateDefencePoints(){
    unsigned totalPoints = attack;
    std::list<Item *>::iterator it1;

    for(it1 = items.begin(); it1 != items.end(); it1++){
      totalPoints += (*it1)->getDefBonus();
      if((*it1)->getIsUpgraded())
        totalPoints += (*it1)->getEffectBonus();
    }

    std::list<Follower *>::iterator it2;
    for(it2 = followers.begin(); it2 != followers.end(); it2++){
      totalPoints += (*it2)->getDefBonus();
      if(((*it2)->getIsUpgraded()))
        totalPoints += (*it2)->getEffectBonus();
    }

    return totalPoints;
  }

  void performSeppuku(){ delete(this); }

  virtual void print() const {
    std::cout << "Personality - ";
    BlackCard::print();
    std::cout << "attack: " << attack << std::endl
              << "defence: " << defence << std::endl
              << "honour: " << honour << std::endl
              << "isDead: " << (isDead ? "true" : "false") << std::endl;
  }

  virtual cardType getType() const { return PERSONALITY; }
};

class Holding : public BlackCard {
private:
  unsigned harvestValue;

  Holding *upperHolding;
  Holding *subHolding;

public:
  Holding(std::string name, unsigned cost, unsigned harv)
  : BlackCard(name, cost), harvestValue(harv), upperHolding(nullptr),
    subHolding(nullptr)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  unsigned getHarvestValue() const { return harvestValue; }

  Holding * getUpperHolding() const { return upperHolding; }
  Holding * getSubHolding() const { return subHolding; }

  /* Change this so that it will return the right amount, if chains exist */
  unsigned tap() {
    Card::tap();
    return harvestValue;
  }

  virtual void print() const {
    std::cout << "Holding - ";
    BlackCard::print();
    std::cout << "harvestValue: " << harvestValue << std::endl;

    if (upperHolding != nullptr) {
      std::cout << "upperHolding - ";
      upperHolding->print();
    }

    if (subHolding != nullptr) {
      std::cout << "subHolding - ";
      upperHolding->print();
    }
  }

  virtual cardType getType() const { return HOLDING; }
};

class Stronghold : public Holding {
private:
  static unsigned int ID;
  unsigned honour;
  unsigned initialDefence;

public:
  Stronghold(std::string name, unsigned cost, unsigned hnr, unsigned harv,
             unsigned initDef)
  : Holding(name, cost, harv), honour(hnr), initialDefence(initDef)
  { /* Empty constructor body (on purpose, nothing to do here)! */ }

  virtual void print() const {
    std::cout << "Stronghold - ";
    Holding::print();
    std::cout << "honour: " << honour << std::endl
              << "initialDefence: " << initialDefence << std::endl;
  }
  
  static unsigned int getID() { return ID; }

  cardType getType() const{ return HOLDING; };  
};