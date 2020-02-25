#pragma once

// #ifndef ENUMS
// #define ENUMS

typedef enum{ATTACKER, DEFENDER, SHOGUN, CHANCELLOR, CHAMPION} Personalities;
typedef enum{PLAIN, MINE, GOLD_MINE, CRYSTAL_MINE, FARMS, SOLO, STRONGHOLD} Holdings;
typedef enum{FOOTSOLDIER, ARCHER, SIEGER, CAVALRY, NAVAL, BUSHIDO} Followers;
typedef enum{KATANA, SPEAR, BOW, NINJATO, WAKIZASHI} Items;

typedef enum{PERSONALITY = 1, HOLDING, FOLLOWER, ITEM} cardType;

// #endif