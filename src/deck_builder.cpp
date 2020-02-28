#include "../include/deck_builder.hpp"
#include "../include/blackcards.hpp"
#include "../include/greencards.hpp"

using namespace std;

DeckBuilder::DeckBuilder() {
  black = new list<BlackCard *>();
  green = new list<GreenCard *>();
}

DeckBuilder::~DeckBuilder() {
  list<BlackCard *>::iterator blackIt = black->begin();
  list<GreenCard *>::iterator greenIt = green->begin();

  while (blackIt != black->end()) {
    delete *blackIt;
    blackIt++;
  }

  while (greenIt != green->end()) {
    delete *greenIt;
    greenIt++;
  }

  delete black;
  delete green;
}

void DeckBuilder::createFateDeck() {
  int i;

  // Create followers
  for (i = 0; i < NO_FOOTSOLDIER; i++)
    green->push_back(new Footsoldier("Peasants"));
  for (i = 0; i < NO_ARCHER; i++)
    green->push_back(new Archer("Elite Troops"));
  for (i = 0; i < NO_SIEGER; i++)
    green->push_back(new Sieger("Immortal Bushi"));
  for (i = 0; i < NO_CAVALRY; i++)
    green->push_back(new Cavalry("Unicorn Horses"));
  for (i = 0; i < NO_NAVAL; i++)
    green->push_back(new Atakebune("Yomi Ships"));
  for (i = 0; i < NO_BUSHIDO; i++)
    green->push_back(new Bushido("7 Samurai"));

  // Create items
  for (i = 0; i < NO_KATANA; i++)
    green->push_back(new Katana("Daijtso"));
  for (i = 0; i < NO_BOW; i++)
    green->push_back(new Bow("Yumi"));
  for (i = 0; i < NO_SPEAR; i++)
    green->push_back(new Spear("Kama"));
  for (i = 0; i < NO_NINJATO; i++)
    green->push_back(new Ninjato("Butterfly Dagger"));
  for (i = 0; i < NO_WAKIZASHI; i++)
    green->push_back(new Wakizashi("Kusanagi"));
}

void DeckBuilder::createDynastyDeck() {
  int i;

  // Create personalities
  for (i = 0; i < NO_DEFENSIVE; i++)
    black->push_back(new Defender("Footsoldiers"));
  for (i = 0; i < NO_ATTACKING; i++)
    black->push_back(new Attacker("Steadfast bushi"));

  black->push_back(new Shogun("Akodo Kaneka"));
  black->push_back(new Shogun("Isawa Shi en"));

  black->push_back(new Chancellor("Hida Shozen"));
  black->push_back(new Chancellor("Mirimuto Enishi"));

  black->push_back(new Champion("Hida Kisada"));

  // Create holdings
  for (i = 0; i < NO_PLAIN; i++)
    black->push_back(new Plain("Rice Fields"));
  for (i = 0; i < NO_FARM_LANDS; i++)
    black->push_back(new Farmland("Fish Farms"));
  for (i = 0; i < NO_MINE; i++)
    black->push_back(new Mine("Ashio Copper Mine"));
  for (i = 0; i < NO_GOLD_MINE; i++)
    black->push_back(new GoldMine("Konomai Gold Mine"));
  for (i = 0; i < NO_CRYSTAL_MINE; i++)
    black->push_back(new CrystalMine("Ichinokawa Crystal Mine"));
  for (i = 0; i < NO_GIFT_FAVOURS; i++)
    black->push_back(new GiftsFavour("Gifts and Favors"));
}

void DeckBuilder::deckShuffler(list<BlackCard *> *black) {
  vector<BlackCard *> vect;
  list<BlackCard *>::iterator blackItL = black->begin();

  while (blackItL != black->end()) {
    vect.push_back(*blackItL);
    blackItL++;
  }

  random_shuffle(vect.begin(), vect.end());

  black->clear();

  vector<BlackCard *>::iterator blackItV = vect.begin();
  while (blackItV != vect.end()) {
    black->push_back(*blackItV);
    blackItV++;
  }
}

void DeckBuilder::deckShuffler(list<GreenCard *> *green) {
  vector<GreenCard *> vect;
  list<GreenCard *>::iterator greenItL = green->begin();
  
  while (greenItL != green->end()) {
    vect.push_back(*greenItL);
    greenItL++;
  }

  random_shuffle(vect.begin(), vect.end());

  green->clear();

  vector<GreenCard *>::iterator greenItV = vect.begin();
  while (greenItV != vect.end()) {
    green->push_back(*greenItV);
    greenItV++;
  }
}
