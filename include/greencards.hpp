#pragma once

#include <string>

#include "card.hpp"

// The constructor arguments corresponding to a card's stats are
// filled arbitrarily, according to the given .txt files

// Follower types

class Footsoldier : public Follower {
public:
  Footsoldier(std::string name)
  : Follower(name, 2, 2, 0, 1, "Footsoldier", 1, 2) {}
};

class Archer : public Follower {
public:
  Archer(std::string name)
  : Follower(name, 0, 0, 2, 1, "Archer", 1, 2) {}
};

class Sieger : public Follower {
public:
  Sieger(std::string name)
  : Follower(name, 5, 3, 3, 2, "Sieger", 2, 3) {}
};

class Cavalry : public Follower {
public:
  Cavalry(std::string name)
  : Follower(name, 3, 4, 2, 3, "Cavalry", 3, 4) {}
};

class Atakebune : public Follower {
public:
  Atakebune(std::string name)
  : Follower(name, 3, 2, 4, 3, "Atakebune", 3, 4) {}
};

class Bushido : public Follower {
public:
  Bushido(std::string name)
  : Follower(name, 8, 8, 8, 6, "Bushido", 3, 8) {}
};

// Item types

class Katana : public Item {
public:
  Katana(std::string name)
  : Item(name, 0, 2, 0, 1, "Katana", 1, 2, 3) {}
};

class Spear : public Item {
public:
  Spear(std::string name)
  : Item(name, 0, 0, 2, 1, "Spear", 1, 2, 3) {}
};

class Bow : public Item {
public:
  Bow(std::string name)
  : Item(name, 2, 2, 2, 2, "Bow", 3, 4, 5) {}
};

class Ninjato : public Item {
public:
  Ninjato(std::string name)
  : Item(name, 4, 3, 3, 3, "Ninjato", 2, 2, 4) {}
};

class Wakizashi : public Item {
public:
  Wakizashi(std::string name)
  : Item(name, 8, 5, 5, 3, "Wakizashi", 3, 3, 8) {}
};
