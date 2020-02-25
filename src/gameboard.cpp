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
  myPlayer.untapEverything();
  myPlayer.drawFateCard();
  myPlayer.revealProvinces();
  myPlayer.printHand();
  myPlayer.printProvinces();
}

void GameBoard::finalPhase(Player &myPlayer){
  myPlayer.discardSurplusFateCards();
  // myplayer.printGameStatistics();
  myPlayer.printHand();
  myPlayer.printProvinces();
  myPlayer.printArena();
  myPlayer.printHoldings();
}

void GameBoard::equipPhase(Player &myPlayer){
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
  myPlayer.revealProvinces();
  myPlayer.printProvinces();

  std::list <BlackCard *> provinces = myPlayer.getProvinces();
  bool available[4] = {0}; // available[i] -> i-th province can be bought

  list <BlackCard*> :: iterator itProvince = provinces.begin();
  for (int i = 0; i < myPlayer.getNumberOfProvinces(); i++, itProvince++)
    available[i] = ((*itProvince)->getRevealed() == true);

  int target_province;
  for (int i = 0; i < myPlayer.getNumberOfProvinces(); i++) {
    cout << "Select a province (0 to 3) to buy (enter -1"
         << " if you wish to continue to the next phase)" << endl;
    cin >> target_province;

    if (target_province == -1) return;
    if (target_province < 0 || target_province > 3) {
      cout << "Invalid province index, please try again" << endl;
      i--;
      continue;
    }

    if (available[target_province]) {
      available[target_province] = false;
      myPlayer.buyBlackCard(target_province);
    }
    else {
      cout << "This province is unavailable, please choose another one" << endl;
      i--;
      continue;
    }
  }
}
