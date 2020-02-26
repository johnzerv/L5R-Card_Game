#include <iostream>
#include <string>
#include <sstream>

#include "../include/gameboard.hpp"

using namespace std;

GameBoard::~GameBoard(){
  delete[] players;
}

void GameBoard::initializeGameBoard(unsigned int playersNumber = 2){
  numberOfPlayers = playersNumber;
  players = new Player[numberOfPlayers];
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
       << numberOfPlayers << ") or type 'ok' to continue:";
  cin >> input;

  int chosenPlayer;

  if(input != "ok"){
    stringstream temp(input);
    temp >> chosenPlayer;

    int chosenProvince;
    cout << endl << "Choose a province to attack (0 - " << myPlayer.getNumberOfProvinces() << "): ";
    cin >> chosenProvince;
    cout << endl << endl;

    // No error-handling
    int attackerPoints = myPlayer.calculateAttackPoints();
    int defencerPoints = players[chosenPlayer].calculateDefencePoints();

    if(attackerPoints - defencerPoints - myPlayer.getInitialDefence()
                                       > myPlayer.getInitialDefence()){
      players[chosenPlayer].destroyProvince(chosenProvince);
      players[chosenPlayer].reduceProvinces();
      players[chosenPlayer].destroyActPers();
    }
    else{
      int difference = attackerPoints - defencerPoints;

      if(difference > 0){
        players[chosenPlayer].destroyActPers();
        myPlayer.discardActPCards(difference);
      }
      else if(attackerPoints == defencerPoints){
        myPlayer.destroyActPers();
        players[chosenPlayer].destroyActPers();
      }
      else if(difference < 0){
        myPlayer.destroyActPers();
        players[chosenPlayer].discardActPCards(difference);
      }
      myPlayer.battleReverberations();
    }

  }

}