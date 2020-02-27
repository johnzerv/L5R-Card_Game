#include <iostream>
#include <string>
#include <sstream>

#include "../include/gameboard.hpp"

using namespace std;

GameBoard::~GameBoard(){
  list<Player *>::iterator it;

  for(it = players.begin(); it != players.end(); it++)
    delete(*it);
  players.clear();
}

void GameBoard::initializeGameBoard(unsigned int playersNumber = 2){
  numberOfPlayers = playersNumber;

  for(int i = 0; i < playersNumber; i++)
    players.push_back(new Player);
}

void GameBoard::startingPhase(Player &myPlayer){
  cout << endl << "-------- STARTING PHASE ----------" << endl << endl;
  myPlayer.untapEverything();
  myPlayer.drawFateCard();
  myPlayer.revealProvinces();
  myPlayer.printHand();
  myPlayer.printProvinces();
}

void GameBoard::finalPhase(Player &myPlayer){
  cout << endl << "-------- FINAL PHASE ----------" << endl << endl;
  myPlayer.discardSurplusFateCards();
  // myplayer.printGameStatistics();
  myPlayer.printHand();
  myPlayer.printProvinces();
  myPlayer.printArena();
  myPlayer.printHoldings();
}

void GameBoard::equipPhase(Player &myPlayer){
  cout << endl << "-------- EQUIPMENT PHASE ----------" << endl << endl;

  if(!myPlayer.getArmy().empty()){
    myPlayer.printArmy();
    myPlayer.printHand();

    string selectedCardStr, personalityPosStr;
    int selectedCard, personalityPos;
      cout << "Put the number of card you want to buy or type 'ok' to continue : ";
      cin >> selectedCard;
      cout << endl << "Put the number of the personality you want to expand or type 'ok' to continue";
      cin >> personalityPos;
      cout << endl << endl;

    while(selectedCardStr != "ok" && personalityPosStr != "ok"){
      stringstream temp1(selectedCardStr), temp2(personalityPosStr);
      temp1 >> selectedCard;
      temp2 >> personalityPos;


      if(selectedCard > 0 && selectedCard <= myPlayer.getSizeOfHand()){

        if(personalityPos > 0 && personalityPos <= myPlayer.getArmy().size())
          myPlayer.buyGreenCard(selectedCard, personalityPos);
        else
          cout << "The number of personality position that was given is out of bounds";   
      }
      else
        cout << "Input for card is out of bounds" << endl;

      cout << endl << endl;
      myPlayer.printArmy();
      myPlayer.printHand();

      cout << "Put the number of card you want to buy or type 'ok' to continue : ";
      cin >> selectedCard;
      cout << endl << "Put the number of the personality you want to expand or type 'ok' to continue";
      cin >> personalityPos;
      cout << endl << endl;
    }
  }
}

void GameBoard::economyPhase(Player &myPlayer) {
  cout << endl << "-------- ECONOMY PHASE ----------" << endl << endl;

  myPlayer.revealProvinces();
  myPlayer.printProvinces();

  std::list <BlackCard *> provinces = myPlayer.getProvinces();
  bool available[4] = {0}; // available[i] -> i-th province can be bought

  list <BlackCard*> :: iterator itProvince = provinces.begin();
  for (int i = 0; i < myPlayer.getNumberOfProvinces(); i++, itProvince++)
    available[i] = ((*itProvince)->getRevealed() == true);

  int targetProvince;
  for (int i = 0; i < myPlayer.getNumberOfProvinces(); i++) {
    cout << "Select a province (0 to 3) to buy (enter -1"
         << " if you wish to continue to the next phase)" << endl;
    cin >> targetProvince;

    if (targetProvince == -1) return;
    if (targetProvince < 0 || targetProvince > 3) {
      cout << "Invalid province index, please try again" << endl;
      i--;
      continue;
    }

    if (available[targetProvince]) {
      available[targetProvince] = false;
      myPlayer.buyBlackCard(targetProvince);
    }
    else {
      cout << "This province is unavailable, please choose another one" << endl;
      i--;
      continue;
    }
  }
}

void GameBoard::battlePhase(Player &myPlayer){
  cout << endl << "-------- BATTLE PHASE ----------" << endl << endl;
  myPlayer.activatePersonalities();

  for(int i = 0; i < numberOfPlayers; i++)
    cout << "Player " << to_string(i) << "  ";

  string input;
  cout << endl << "Choose a player to attack (0 - " 
       << numberOfPlayers << ") or type 'ok' to continue:"; /* prepei na meiwnetai to number of players otan pethainei kapoios paiktis */
  cin >> input;

  int chosenPlayerInt, i;

  if(input != "ok"){
    stringstream temp(input);
    temp >> chosenPlayerInt;

    list<Player*>::iterator playerIt, tempIt;
    for(i = 0, playerIt = players.begin(); i < chosenPlayerInt; i++, playerIt++);
    Player *chosenPlayer = (*playerIt); 

    chosenPlayer->printProvinces();
    int chosenProvince;
    cout << endl << "Choose a province to attack (0 - " << chosenPlayer->getNumberOfProvinces() << "): ";
    cin >> chosenProvince;
    cout << endl << endl;

    // No error-handling
    int attackerPoints = myPlayer.calculateAttackPoints();
    int defencerPoints = chosenPlayer->calculateDefencePoints();

    if(attackerPoints - defencerPoints - chosenPlayer->getInitialDefence()
                                       > chosenPlayer->getInitialDefence()){

      if(chosenPlayer->destroyProvince(chosenProvince)){
        delete(*playerIt);
        players.erase(playerIt);
        cout << "Player " << chosenPlayer << " is dead !" << endl << endl;
        reducePlayersNumber();
      }

      chosenPlayer->reduceProvinces();
      chosenPlayer->destroyActPers();
    }
    else{
      int difference = attackerPoints - defencerPoints - chosenPlayer->getInitialDefence();

      if(difference > 0){
        chosenPlayer->destroyActPers();
        myPlayer.discardActPCards(difference);
        myPlayer.reduceActPersHonour();
      }
      else if(attackerPoints == defencerPoints){
        myPlayer.destroyActPers();
        chosenPlayer->destroyActPers();
      }
      else if(difference < 0){
        myPlayer.destroyActPers();
        chosenPlayer->discardActPCards(difference);
        chosenPlayer->reduceActPersHonour();
      }
      myPlayer.battleReverberations();
    }

  }

}