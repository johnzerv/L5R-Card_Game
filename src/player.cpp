#include <iostream>
#include <list>

#include "../include/player.hpp"

using namespace std;

Player::Player() : numberOfProvinces(NO_OF_PROVINCES){
  decks.createFateDeck();
  decks.createDynastyDeck();

  unsigned int i;  // Temp variable
  list <BlackCard*> :: iterator itBlack;
  for(i = 0, itBlack = decks.getBlack()->begin(); i < numberOfProvinces; i++, itBlack++)
    provinces.push_back(*itBlack);

  list <GreenCard*> :: iterator itGreen;
  for(i = 0, itGreen = decks.getGreen()->begin(); i < NO_HAND_CARDS; i++, itGreen++)
    hand.push_back(*itGreen);

  string StrongholdName = "Stronghold" + to_string(Stronghold::getID()+1);
  holdings.push_back(new Stronghold(StrongholdName, rand() % 7,
                     rand() % 5, rand() % 10, rand() % 6));
}

Player::~Player(){
  list <BlackCard *>::iterator it1;
  BlackCard *tempBCardPtr;
  for(it1 = provinces.begin(); it1 != provinces.end(); it1++){
    if((tempBCardPtr = dynamic_cast<Personality *>(*it1)) == nullptr)
      tempBCardPtr = ((Holding *)(*it1));
    delete (tempBCardPtr);
  }
  provinces.clear();

  list <Personality *>::iterator it2;
  for(it2 = army.begin(); it2 != army.end(); it2++)
    delete (*it2);
  army.clear();

  list <GreenCard *>::iterator it3;
  GreenCard *tempGCardPtr;
  for(it3 = hand.begin(); it3 != hand.end(); it3++){
    if((tempGCardPtr = dynamic_cast<Item *>(*it1)) == nullptr)
      tempGCardPtr = ((Follower *)(*it1));
    delete (tempGCardPtr);
  }
  hand.clear();
}

void Player::untapEverything(){
  list <Holding *>::iterator it1;
  for(it1 = holdings.begin(); it1 != holdings.end(); it1++)
    (*it1)->untap();

  list <Personality *>::iterator it2;
  for(it2 = army.begin(); it2 != army.end(); it2++)
    (*it2)->untap();
}

void Player::drawFateCard(){
  list<GreenCard *>::iterator it = decks.getGreen()->begin();
  hand.push_back(*it); /* Add the new GreenCard to hand */
  decks.getGreen()->pop_front();  /* Remove the drawn card from FateDeck */
}

void Player::drawDynastyCard() {
  list<BlackCard *>::iterator it = decks.getBlack()->begin();
  provinces.push_back(*it); /* Add the new BlackCard to provinces */
  decks.getBlack()->pop_front();  /* Remove the drawn card from DynastyDeck */
}

void Player::revealProvinces(){
  list <BlackCard *>::iterator it;

  for(it = provinces.begin(); it != provinces.end(); ++it)
    (*it)->setRevealedTrue();
}

void Player::printHoldings(){
  list <Holding *>::iterator it;

  for(it = holdings.begin(); it != holdings.end(); ++it)
    (*it)->print();
}

void Player::printHand(){
  list <GreenCard *>::iterator it;
  for(it = hand.begin(); it != hand.end(); ++it)
    (*it)->print();
}

void Player::printArmy(){
  list <Personality *>::iterator it;
  for(it = army.begin(); it != army.end(); ++it)
    (*it)->print();
}

inline void Player::printArena(){ printArmy(); }

void Player::printProvinces() {
  list <BlackCard *>::/* const_ */iterator it;
  for(it = provinces.begin(); it != provinces.end(); it++)
    (*it)->print();
}

void Player::discardSurplusFateCards(){
  int mustPoppedCards = hand.size() - NO_HAND_CARDS;  /* Cards that have to get popped */
  if(mustPoppedCards > 0) /* If they exist */
    for(int i = 0; i < mustPoppedCards; i++){
      int randomCard = rand() % hand.size();  /* Pick a random card from hand and pop it */
      list <GreenCard *>::iterator it;
      for(it = hand.begin(), i = 0; i < randomCard; it++, i++);
      hand.erase(it);
    }
}

bool Player::checkWinningCondition(Player *players, unsigned int playersNumber) const{
  bool hasWon = true;
  for (unsigned int i = 0; i < playersNumber; i++)
    if (&players[i] != this)
      hasWon = hasWon && !(players[i].hasProvinces());

  return (hasWon && this->hasProvinces());
}

/* iterator Player::getSpecifiedCard(int position){
  list <GreenCard *>::iterator it = hand.begin();
  for(int i = 0, it = hand.begin(); i < position; i++, it++);
  return it;
} */

bool Player::isMoneyEnough(Card *myCard){
  return (money >= myCard->getCost());
}

void Player::buyGreenCard(int position, int personalityPos){
  list<GreenCard*>::iterator cardIt; int i;
  for(i = 0, cardIt = hand.begin(); i < position; i++, cardIt++);

  list<Personality *>::iterator personalityIt;
  for(i = 0, personalityIt = army.begin(); i < personalityPos ; personalityIt++);

  if((*cardIt)->getType() == FOLLOWER)
    if(!hasMaxFollowers(*personalityIt))
      if((*personalityIt)->getHonour() >= (*cardIt)->getMinHonour()){
        if(isMoneyEnough(*cardIt)){
          (*personalityIt)->expandPersonality(*cardIt, FOLLOWER);
          money -= (*cardIt)->getCost();
          
          if(wantToUpgrade())
            if(!upgradeGreenCard(*cardIt))
              cout << "Unfortunately you can't upgrade your card because you haven't enough money or holdings" << endl;

          hand.erase(cardIt);
        }
        else{
          if(tapHoldings(*cardIt)){
            (*personalityIt)->expandPersonality(*cardIt, FOLLOWER);
            money -= (*cardIt)->getCost();

            if(wantToUpgrade())
              if(!upgradeGreenCard(*cardIt))
                cout << "Unfortunately you can't upgrade your card because you haven't enough money or holdings" << endl;

            hand.erase(cardIt);
          }
          else
            cout << "There weren't enough holdings to reach the requested amount of money";
        }
      }
      else
        cout << "Current personality hasn't the required honour" << endl;
    else
      cout << "Current personality has the max number of followers" << endl;
  else
    if(!hasMaxItems(*personalityIt))
      if((*personalityIt)->getHonour() > (*cardIt)->getMinHonour()){
        if(isMoneyEnough(*cardIt)){
          (*personalityIt)->expandPersonality(*cardIt, ITEM);
          money -= (*cardIt)->getCost();

          if(wantToUpgrade())
            if(!upgradeGreenCard(*cardIt))
              cout << "Unfortunately you can't upgrade your card because you haven't enough money or holdings" << endl;

          hand.erase(cardIt);
        }
        else{
          if(tapHoldings(*cardIt)){
            (*personalityIt)->expandPersonality(*cardIt, ITEM);
            money -= (*cardIt)->getCost();

            if(wantToUpgrade())
              if(!upgradeGreenCard(*cardIt))
                cout << "Unfortunately you can't upgrade your card because you haven't enough money or holdings" << endl;

            hand.erase(cardIt);
          }
          else
            cout << "There weren't enough holdings to reach the requested amount of money";
        }
      }
      else
        cout << "Current personality hasn't the required honour" << endl;
    else
      cout << "Current personality has the max number of items" << endl;
}

void Player::buyBlackCard(int target_province) {
  list<BlackCard*>::iterator cardIt = provinces.begin();
  for(int i = 0; i < target_province; i++, cardIt++);

  if (isMoneyEnough(*cardIt)) {
    money -= (*cardIt)->getCost();

    if ((*cardIt)->getType() == PERSONALITY)
      army.push_back((Personality*) *cardIt);
    else
      holdings.push_back((Holding*) *cardIt);

    provinces.erase(cardIt);
    drawDynastyCard();
  }
  else if (tapHoldings(*cardIt)) {
    money -= (*cardIt)->getCost();

    if ((*cardIt)->getType() == PERSONALITY)
      army.push_back((Personality *) *cardIt);
    else
      holdings.push_back((Holding *) *cardIt);

    provinces.erase(cardIt);
    drawDynastyCard();
  }
  else
    cout << "There weren't enough holdings to reach the requested amount of money" << endl;
}

bool Player::tapHoldings(Card *CardPtr){
  if(holdings.empty())
    return false;

  unsigned tempMoney = money;
  list<Holding *>::iterator it;
  it = holdings.begin();
  tempMoney += (*it)->getHarvestValue(); /* The first holding is the Stronghold */

  while(!isMoneyEnough(CardPtr)){
    if(!(*it)->getIsTapped())
      tempMoney += (*it)->tap();

    it++;
    if(it == holdings.end()){  /* If there wasn't possible to reach the requested amount of money  */
      list<Holding *>::iterator tempIt;
      it--;
      for(tempIt = holdings.begin(); tempIt != it; tempIt++)  /* Untap all the holdings you tapped */
        (*tempIt)->untap();
      return false; /* And return the result of collecting harvesValue's */
    }
  }
  money = tempMoney;  /* If there are enough holdings to reach the amount
                        by tapping them, update the money of player */
  return true;
}

bool Player::hasMaxFollowers(Personality *myPersonality){ 
  /* Checks if the personality at position personalityPos has max followers */
  list <GreenCard *>::iterator it;
  return myPersonality->hasMaxFollowers();
}

bool Player::hasMaxItems(Personality *myPersonality){
  /* Checks if the personality at position personalityPos has max items */
  return myPersonality->hasMaxItems();
}

bool Player::upgradeGreenCard(GreenCard *cardPtr){
  if(isMoneyEnough(cardPtr)){
    cardPtr->Upgrade();
    money -= cardPtr->getEffectCost();
    return true;
  }
  else{
    if(tapHoldings(cardPtr)){
      cardPtr->Upgrade();
      money -= cardPtr->getEffectCost();
      return true;
    }
    else
      return false;
  }
}

bool Player::wantToUpgrade() const{
  string input;
  cout << "If you want to upgrade your new item type y, or n if not :";
  cin >> input; cout << endl;

  while(input != "y" && input != "n"){
    cout << "If you want to upgrade your new item type y, or n if not :";
    cin >> input; cout << endl;
  }

  if(input == "y")
    return true;
  else  /* No error-handling */
    return false;
}