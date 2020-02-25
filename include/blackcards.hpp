// #ifndef BLACKCARDS
// #define BLACKCARDS
#pragma once

#include <string>

#include "card.hpp"

class Defender : public Personality {
public:
  Defender(std::string name) : Personality(name, 5, 2, 3, 2) {}
};

class Attacker : public Personality {
public:
  Attacker(std::string name) : Personality(name, 5, 3, 2, 2) {}
};

class Shogun : public Personality {
public:
  Shogun(std::string name) : Personality(name, 15, 10, 5, 8) {}
};

class Chancellor : public Personality {
public:
  Chancellor(std::string name) : Personality(name, 15, 5, 10, 8) {}
};

class Champion : public Personality {
public:
  Champion(std::string name) : Personality(name, 30, 20, 20, 12) {}
};

class Plain : public Holding {
public:
  Plain(std::string name) : Holding(name, 2, 2) {}
};

class Farmland : public Holding {
public:
  Farmland(std::string name) : Holding(name, 3, 4) {}
};

class GiftsandFavour : public Holding {
public:
  GiftsandFavour(std::string name) : Holding(name, 2, 2) {}
};

class Farms : public Holding {
public:
  Farms(std::string name) : Holding(name, 3, 4) {}
};

class Mine : public Holding {
public:
  Mine(std::string name) : Holding(name, 5, 3) {}
};

class GoldMine : public Holding {
public:
  GoldMine(std::string name) : Holding(name, 7, 5) {}
};

class CrystalMine : public Holding {
public:
  CrystalMine(std::string name) : Holding(name, 12, 6) {}
};

// #endif