#pragma once

typedef enum {ATTACKER, DEFENDER, SHOGUN, CHANCELLOR, CHAMPION} Personalities;
typedef enum {PLAIN, MINE, GOLD_MINE, CRYSTAL_MINE, FARM_LAND, GIFT_FAVOUR,
              STRONGHOLD} Holdings;
typedef enum {FOOTSOLDIER, ARCHER, SIEGER, CAVALRY, NAVAL, BUSHIDO} Followers;
typedef enum {KATANA, SPEAR, BOW, NINJATO, WAKIZASHI} Items;

typedef enum {PERSONALITY = 1, HOLDING, FOLLOWER, ITEM} cardType;

#define NO_HAND_CARDS      6
#define NO_OF_PROVINCES    4

#define DECKSIZE          40
#define MAXDECKSIZE       60

#define NO_PERSONALITIES 25
#define NO_DEFENSIVE     10
#define NO_ATTACKING     10
#define NO_SHOGUN         2
#define NO_CHANCELLOR     2
#define NO_CHAMPION       1

#define NO_HOLDINGS      17
#define NO_PLAIN          3
#define NO_MINE           3
#define NO_GOLD_MINE      3
#define NO_CRYSTAL_MINE   3
#define NO_FARM_LANDS     3
#define NO_GIFT_FAVOURS   2

#define NO_FOLLOWERS     28
#define NO_FOOTSOLDIER    6
#define NO_ARCHER         6
#define NO_SIEGER         4
#define NO_CAVALRY        5
#define NO_NAVAL          5
#define NO_BUSHIDO        2

#define NO_ITEMS         12
#define NO_KATANA         4
#define NO_SPEAR          3
#define NO_BOW            2
#define NO_NINJATO        2
#define NO_WAKIZASHI      1

#define MAX_ITEMS         6
#define MAX_FOLLOWERS    14
