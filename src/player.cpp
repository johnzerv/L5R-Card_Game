#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include "../include/player.hpp"

using namespace std;

int Stronghold::ID = 0;

Player::Player() : money(0), numberOfProvinces(NO_OF_PROVINCES) {
  decks.createFateDeck();
  decks.createDynastyDeck();
  decks.deckShuffler();

  // Initialize the player's "provinces", "hand" and "holdings" lists

  for (int i = 0; i < numberOfProvinces; i++)
    drawDynastyCard(provinces.begin());    

  for (int i = 0; i < NO_HAND_CARDS; i++)
    drawFateCard();

  // Harvest for Stronghold between 5-9

  string StrongholdName = "Stronghold" + to_string(Stronghold::nextID());
  holdings.push_back(new Stronghold(StrongholdName, rand() % 7, rand() % 5,
                     rand() % 5 + 5 , rand() % 6 + 4));

  list<Holding *>::iterator strongholdIt = holdings.begin();
  initialDefence = ((Stronghold *) (*strongholdIt))->getInitDefence();
}

Player::~Player() {

  // Step 1: deallocate the provinces (cards) of the player
  list<BlackCard *>::iterator blackIt = provinces.begin();
  BlackCard *tempBCardPtr;

  while (blackIt != provinces.end()) {
    if ((tempBCardPtr = dynamic_cast<Personality *> (*blackIt)) == nullptr)
      tempBCardPtr = (Holding *) *blackIt;

    delete tempBCardPtr;
    blackIt++;
  }

  provinces.clear();

  // Step 2: deallocate the army (cards) of the player
  list<Personality *>::iterator persIt = army.begin();

  while (persIt != army.end()) {
    delete *persIt;
    persIt++;
  }

  army.clear();

  // Step 3: deallocate the activated personalities (cards) of the player
  persIt = activatedPersonalities.begin();

  while (persIt != activatedPersonalities.end()) {
    delete *persIt;
    persIt++;
  }

  activatedPersonalities.clear();

  // Step 4: deallocate the hand (cards) of the player
  list<GreenCard *>::iterator greenIt = hand.begin();
  GreenCard *tempGCardPtr;

  while (greenIt != hand.end()) {
    if ((tempGCardPtr = dynamic_cast<Item *> (*greenIt)) == nullptr)
      tempGCardPtr = (Follower *) *greenIt;

    delete tempGCardPtr;
    greenIt++;
  }

  hand.clear();
}

void Player::untapEverything() {
  list<Holding *>::iterator holdIt = holdings.begin();

  while (holdIt != holdings.end()) {
    (*holdIt)->untap();
    holdIt++;
  }

  list<Personality *>::iterator persIt = army.begin();

  while (persIt != army.end()) {
    (*persIt)->untap();
    persIt++;
  }
}

void Player::drawFateCard() {
  list<GreenCard *>::iterator greenIt = decks.getGreen()->begin();
  hand.push_back(*greenIt); // Add the new GreenCard to hand
  decks.getGreen()->pop_front(); // Remove the drawn card from FateDeck
}

void Player::drawDynastyCard(list<BlackCard *>::iterator positionIt) {
  list<BlackCard *>::iterator blackIt = decks.getBlack()->begin();

  // Add the new BlackCard to provinces at the right position
  provinces.insert(positionIt, *blackIt);

  decks.getBlack()->pop_front(); // Remove the drawn card from DynastyDeck
}

void Player::revealProvinces() {
  list<BlackCard *>::iterator blackIt = provinces.begin();

  while (blackIt != provinces.end()) {
    (*blackIt)->setRevealedTrue();
    blackIt++;
  }
}

void Player::printHoldings() {
  list<Holding *>::iterator holdIt = holdings.begin();
  cout << "Printing Holdings :" << endl << endl;

  while (holdIt != holdings.end()) {
    (*holdIt)->print();
    holdIt++;
  }
}

void Player::printHand() {
  list<GreenCard *>::iterator greenIt = hand.begin();
  cout << "Printing Hand :" << endl << endl;

  while (greenIt != hand.end()) {
    (*greenIt)->print();
    greenIt++;
  }
}

void Player::printArena() {
  list<Personality *>::iterator persIt = army.begin();
  cout << "Printing Arena :" << endl << endl;

  while (persIt != army.end()) {
    (*persIt)->print();
    persIt++;
  }
}

int Player::getBalance() {
  int balance = 0;

  list<Holding *>::iterator holdIt = holdings.begin();

  if (holdIt != holdings.end())

  while (holdIt != holdings.end()) {
    if (!(*holdIt)->getIsTapped())
      balance += (*holdIt)->tap(false);

    holdIt++;
  }

  return balance;
}

void Player::printUntappedArmy() {
  list<Personality *>::iterator persIt = army.begin();
  cout << "Printing Untapped Personalities of Army :" << endl << endl;

  while (persIt != army.end()) {
    if (!(*persIt)->getIsTapped())
      (*persIt)->print();

    persIt++;
  }
}

void Player::printProvinces() {
  list<BlackCard *>::iterator blackIt = provinces.begin();
  cout << "Printing Provinces :" << endl << endl;

  while (blackIt != provinces.end()) {
    (*blackIt)->print();
    blackIt++;
  }
}

void Player::discardSurplusFateCards() {
  int n_cards_to_pop = hand.size() - NO_HAND_CARDS;

  if (n_cards_to_pop > 0)
    for (int i = 0; i < n_cards_to_pop; i++) {

      // choose a random card to pop from hand
      int randomCard = rand() % hand.size();

      list<GreenCard *>::iterator greenIt = hand.begin();
      for (int j = 0; j < randomCard; greenIt++, j++);

      hand.erase(greenIt);
    }
}

bool Player::checkWinningCondition(list<Player *> players) {
  bool hasWon = true;
  list<Player *>::iterator playerIt = players.begin();

  while (playerIt != players.end()) {
    if (*playerIt != this)
      hasWon = hasWon && !(*playerIt)->hasProvinces();

    playerIt++;
  }

  return (hasWon && this->hasProvinces());
}

bool Player::isMoneyEnough(Card *card) {
  return (money >= card->getCost());
}

void Player::buyGreenCard(int position, int personalityPos, int &balance) {
  list<GreenCard*>::iterator greenIt = hand.begin();

  // Find a pointer to the target card
  for (int i = 0; i < position; i++, greenIt++);

  list<Personality *>::iterator persIt = army.begin();

  // Find the personality to which we will equip the to-be-bought card
  for (int i = 0; i < personalityPos; i++, persIt++);

  cardType card_type = (*greenIt)->getType();

  // Check if the maximum cards of card_type have been reached
  if (card_type == FOLLOWER && (*persIt)->hasMaxFollowers()) {
    cout << "Can't attach another follower to personality" << "\n\n";
    return;
  }
  else if (card_type == ITEM && (*persIt)->hasMaxItems()) {
    cout << "Can't attach another item to personality" << "\n\n";
    return;
  }

  // Check if the personality's honour is sufficient to buy card
  if ((*persIt)->getHonour() < (*greenIt)->getMinHonour()) {
    cout << "Unable to attach card to personality (insufficient honour)\n\n";
    return;
  }

  // Check if the player's balance is sufficient to buy card
  if (!tapHoldings((*greenIt)->getCost())) {
    cout << "Not enough money to buy card" << "\n\n";
    return;
  }

  cout << "Purchase completed" << "\n\n";

  (*persIt)->expandPersonality(*greenIt, card_type);
  money -= (*greenIt)->getCost();

  if (wantToUpgrade()) {
    if (upgradeGreenCard(*greenIt)) {
      cout << "Upgrade completed\n\n";
      balance -= (*greenIt)->getEffectCost();
    }
    else
      cout << "Not enough money to upgrade card\n\n";
  }

  balance -= (*greenIt)->getCost();
  hand.erase(greenIt);
}

void Player::buyBlackCard(int target_province, int &balance) {
  list<BlackCard*>::iterator blackIt = provinces.begin();

  // Find a pointer to the target card
  for (int i = 0; i < target_province; i++, blackIt++);

  // Check if the player's balance is sufficient to buy card
  if (!tapHoldings((*blackIt)->getCost())) {
    cout << "Not enough money to buy card" << "\n\n";
    return;
  }

  money -= (*blackIt)->getCost();
  balance -= (*blackIt)->getCost();

  // If the card we bought is a holding, we can't tap it during this round
  // to get its harvest value. We need to wait until the next round

  (*blackIt)->tap();

  cout << "Purchase completed\n\n";

  if ((*blackIt)->getType() == PERSONALITY)
    army.push_back((Personality *) *blackIt);
  else {
    formMineChain((Holding *) *blackIt);
    holdings.push_back((Holding *) *blackIt);
  }

  // Erase the purchased card and get the right position for
  // the new black card (tempBlackIt)

  list<BlackCard *>::iterator tempBlackIt = provinces.erase(blackIt);

  drawDynastyCard(tempBlackIt);
}

bool Player::tapHoldings(int cost) {
  int tempBalance = money;
  list<Holding *>::iterator holdIt = holdings.begin();

  // Accumulate the harvest values of the player's holdings until
  // (a) the needed amount is reached or (b) we run out of holdings
  //
  // Note: we don't tap any of the holdings until we make sure the
  // requested amount has been reached

  while (tempBalance < cost && holdIt != holdings.end()) {
    if (!(*holdIt)->getIsTapped())
      tempBalance += (*holdIt)->tap(false);

    holdIt++;
  }

  // If we have managed to reach the requested amount, proceed to tap
  // all of the needed holdings, traversing the holding list backwards
  //
  // Note: we don't have to do the above if we alredy had the requested
  // amount of money (acquired in a previous transaction)

  if (tempBalance >= cost) {
    if (tempBalance == money) return true; // don't have to tap holdings

    list<Holding *>::iterator tempHoldIt = holdings.begin();

    while (tempHoldIt != holdIt) {
      if (!(*tempHoldIt)->getIsTapped())
        (*tempHoldIt)->tap();

      tempHoldIt++;
    }

    money = tempBalance;
    return true; // Signal that the purchase succeeded
  }

  return false; // Signal that the purchase failed
}

bool Player::upgradeGreenCard(GreenCard *card) {
  if (tapHoldings(card->getEffectCost())) {
    card->Upgrade();
    money -= card->getEffectCost();
    return true; // Signal that the upgrade succeeded
  }

  return false; // Signal that the upgrade failed
}

bool Player::wantToUpgrade() {
  string input;

  do {
    cout << "Upgrade new item (y/n)? ";
    cin >> input;
    cout << endl;
  } while (input != "y" && input != "n");

  return (input == "y");
}

void Player::activatePersonalities() {
  int selectedPersonality;
  string input;

  printUntappedArmy();

  cout << "Enter the index of the personality you want to activate"
       << " (or type ok to continue): ";

  cin >> input;

  while (input != "ok") {
    selectedPersonality = stoi(input);

    if (selectedPersonality >= 0 && selectedPersonality < army.size()) {
      list<Personality *>::iterator persIt = army.begin();

      for (int i = 0; i < selectedPersonality; persIt++)
        if (!(*persIt)->getIsTapped()) // Bypass tapped personalities
          i++;

      activatedPersonalities.push_back(*persIt);
      army.erase(persIt);
    }
    else cout << "Wrong input, try again!" << endl << endl;

    printUntappedArmy();
    cout << "Enter the index of the personality you want to activate"
         << " (or type ok to continue): ";
    cin >> input;
    cout << endl;
  }
}

int Player::calculateAttackPoints() {
  int totalPoints = 0;
  list<Personality *>::iterator playerIt = activatedPersonalities.begin();

  while (playerIt != activatedPersonalities.end()) {
    totalPoints += (*playerIt)->calculateAttackPoints();
    playerIt++;
  }

  return totalPoints;
}

int Player::calculateDefencePoints() {
  int totalPoints = 0;
  list<Personality *>::iterator personalityIt = activatedPersonalities.begin();

  while (personalityIt != activatedPersonalities.end()) {
    totalPoints += (*personalityIt)->calculateDefencePoints();
    personalityIt++;
  }

  return totalPoints;
}

bool Player::destroyProvince(int chosenProvince) {
  list<BlackCard *>::iterator blackIt = provinces.begin();
  BlackCard *tempBCardPtr;

  // Find a pointer to the target card
  for (int i = 0; i < chosenProvince; i++, blackIt++);

  if ((tempBCardPtr = dynamic_cast<Personality *> (*blackIt)) == nullptr)
    tempBCardPtr = (Holding *) *blackIt;
  
  delete tempBCardPtr;
  provinces.erase(blackIt);

  // True if player has lost in the game, false otherwise
  return provinces.empty();
}

void Player::destroyActPers() {
  list<Personality *>::iterator persIt = activatedPersonalities.begin();

  while (persIt != activatedPersonalities.end()) {
    delete *persIt;
    persIt++;
  }

  activatedPersonalities.clear();
}

void Player::discardActPCards(int difference) {
  list<Personality *>::iterator persIt = activatedPersonalities.begin();
  list<Follower *>::iterator followerIt;

  while (persIt != activatedPersonalities.end()) {
    if (difference <= 0) return;

    followerIt = (*persIt)->getFollowers().begin();

    while (followerIt != (*persIt)->getFollowers().end()) {
      if (difference <= 0) return;

      delete *followerIt;
      followerIt = (*persIt)->getFollowers().erase(followerIt);
    }

    delete *persIt;
    persIt = activatedPersonalities.erase(persIt);
  }
}

void Player::battleReverberations() {
  list<Personality *>::iterator persIt = activatedPersonalities.begin();
  list<Follower *>::iterator followerIt;
  list<Item *>::iterator itemIt;

  while (persIt != activatedPersonalities.end()) {
    followerIt = (*persIt)->getFollowers().begin();

    while (followerIt != (*persIt)->getFollowers().end()) {
      (*followerIt)->tap();
      followerIt++;
    }

    (*persIt)->tap();

    itemIt = (*persIt)->getItems().begin();

    while (itemIt != (*persIt)->getItems().end()) {
      (*itemIt)->reduceDurability();

      if (!(*itemIt)->getDurability()) {
        (*itemIt)->detach();
        itemIt = (*persIt)->getItems().erase(itemIt);
        itemIt--; // Don't skip any item
      }

      itemIt++;
    }

    persIt++;
  }
}

void Player::reduceActPersHonour() {
  list<Personality *>::iterator persIt = activatedPersonalities.begin();

  while (persIt != activatedPersonalities.end()) {
    (*persIt)->reduceHonour();

    if (!(*persIt)->getHonour()) {
      (*persIt)->performSeppuku();
      persIt = activatedPersonalities.erase(persIt);
      persIt--; // Don't skip any activated Personality
    }

    persIt++;
  }
}

void Player::formMineChain(Holding *holding) {
  bool upperSet = false, subSet = false;

  list<Holding *>::iterator holdIt = holdings.begin();

  int newHoldingType = getMineType(holding);
  int currentType;

  // For each holding in the "holdings" list, check:
  // (a) if its type is either MINE, GOLD_MINE or CRYSTAL_MINE, and
  // (b) if it can form a chain with the new holding (argument)

  while (holdIt != holdings.end() && !(upperSet && subSet)) {
    if ((currentType = getMineType(*holdIt)) != 0) {
      if (newHoldingType > currentType && !subSet
       && (*holdIt)->getUpperHolding() == nullptr) {
        subSet = true;
        holding->setSubHolding(*holdIt);
        (*holdIt)->setUpperHolding(holding);
      }
      else if (newHoldingType < currentType && !upperSet
       && (*holdIt)->getSubHolding() == nullptr) {
        upperSet = true;
        holding->setUpperHolding(*holdIt);
        (*holdIt)->setSubHolding(holding);
      }
    }

    holdIt++;
  }
}

int Player::getMineType(Holding *holding) {
  switch (holding->getHoldingType()) {
    case MINE:          return 1;
    case GOLD_MINE:     return 2;
    case CRYSTAL_MINE:  return 3;
    default:            return 0;
  }
}
