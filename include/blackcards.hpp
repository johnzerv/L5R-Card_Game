#pragma once

#include <string>

#include "card.hpp"

// The constructor arguments corresponding to a card's stats are
// filled arbitrarily (according to the given .txt files)

// Personality types: Attacker, Defender, Champion, Chancellor, Shogun

class Attacker : public Personality {
public:
  Attacker(std::string name)
  : Personality(name, 5, 3, 2, 2) {}
};

class Defender : public Personality {
public:
  Defender(std::string name)
  : Personality(name, 5, 2, 3, 2) {}
};

class Champion : public Personality {
public:
  Champion(std::string name)
  : Personality(name, 30, 20, 20, 12) {}
};

class Chancellor : public Personality {
public:
  Chancellor(std::string name)
  : Personality(name, 15, 5, 10, 8) {}
};

class Shogun : public Personality {
public:
  Shogun(std::string name)
  : Personality(name, 15, 10, 5, 8) {}
};

// Holding types: Plain, Mine, Gold mine, Crystal mine, Farmland, Gifts&Favour
// (Stronghold is also a Holding type, but is defined in card.hpp)

class Plain : public Holding {
public:
  Plain(std::string name)
  : Holding(name, 2, 2, PLAIN) {}
};

class Mine : public Holding {
public:
  Mine(std::string name)
  : Holding(name, 5, 3, MINE) {}
};

class GoldMine : public Holding {
public:
  GoldMine(std::string name)
  : Holding(name, 7, 5, GOLD_MINE) {}
};

class CrystalMine : public Holding {
public:
  CrystalMine(std::string name)
  : Holding(name, 12, 6, CRYSTAL_MINE) {}
};

class Farmland : public Holding {
public:
  Farmland(std::string name)
  : Holding(name, 2, 2, FARM_LAND) {}
};

class GiftsFavour : public Holding {
public:
  GiftsFavour(std::string name)
  : Holding(name, 2, 2, GIFTS_FAVOUR) {}
};
