#include <iostream>
#include <string>
#include <sstream>

#include "../include/gameboard.hpp"
#include "../include/card.hpp"

using namespace std;

GameBoard::~GameBoard() {
  list<Player *>::iterator playerIt = players.begin();

  while (playerIt != players.end()) {
    delete(*playerIt);
    playerIt++;
  }

  players.clear();
}

void GameBoard::initializeGameBoard(int playersNumber) {
  numberOfPlayers = playersNumber;

  for (int i = 0; i < playersNumber; i++)
    players.push_back(new Player);
}

void GameBoard::printGameStatistics() {
  list<Player *>::iterator playerIt = players.begin();

  for (int i = 0; i < numberOfPlayers; i++, playerIt++) {
    cout << "\nPrinting stats for player " << i << endl;

    (*playerIt)->printHoldings();
    (*playerIt)->printHand();
    (*playerIt)->printArena();
    (*playerIt)->printProvinces();
    (*playerIt)->printArmy();
  }
}

bool comparePlayers(const Player *a, const Player *b) {
  return a->getHonour() > b->getHonour();
}

void GameBoard::gameplay() {
  players.sort(comparePlayers);

  bool gameover = false;

  while (!gameover) {
    list<Player *>::iterator playerIt = players.begin();

    int playerNO = 0;
    while (playerIt != players.end()) {

      cout << "\n\nPlayer's number " << playerNO << " turn !" << endl;

      startingPhase(**playerIt);
      equipPhase(**playerIt);
      battlePhase(**playerIt);

      if ((*playerIt)->checkWinningCondition(players)) {
        cout << "Player " << playerNO << " has won!" << endl;
        gameover = true;
        break;
      }

      economyPhase(**playerIt);
      finalPhase(**playerIt);

      playerIt++;
      playerNO++;
    }
  }
}

void GameBoard::startingPhase(Player &player) {
  cout << "\n-------- STARTING PHASE ----------\n" << endl;

  player.untapEverything();
  player.drawFateCard();
  player.revealProvinces();
  player.printHand();
  player.printProvinces();
}

void GameBoard::finalPhase(Player &player) {
  cout << "\n-------- FINAL PHASE ----------\n" << endl;

  player.discardSurplusFateCards();
  // player.printGameStatistics();
  player.printHand();
  player.printProvinces();
  player.printArena();
  player.printHoldings();
}

void GameBoard::equipPhase(Player &player) {
  cout << "\n-------- EQUIPMENT PHASE ----------\n" << endl;

  if (!player.getArmy().empty()) {
    player.printArmy();
    player.printHand();

    string selectedCardStr, personalityPosStr;
    int selectedCard, personalityPos;

    do {
      cout << "Enter the index of the card you want to buy"
         << " (or type ok to continue): ";

      cin >> selectedCard;
      cout << "\nEnter the index of the personality you want to expand"
           << " (or type ok to continue): ";

      cin >> personalityPos;
      cout << endl << endl;

      stringstream temp1(selectedCardStr), temp2(personalityPosStr);
      temp1 >> selectedCard;
      temp2 >> personalityPos;

      if (selectedCard > 0 && selectedCard <= player.getSizeOfHand()) {

        if (personalityPos > 0 && personalityPos <= player.getArmy().size())
          player.buyGreenCard(selectedCard, personalityPos);
        else
          cout << "Out of bounds personality index";   
      }
      else
        cout << "Out of bounds card index" << endl;

      cout << endl << endl;
      player.printArmy();
      player.printHand();
    } while (selectedCardStr != "ok" && personalityPosStr != "ok");
  }
}

void GameBoard::economyPhase(Player &player) {
  cout << "\n-------- ECONOMY PHASE ----------\n" << endl;

  player.revealProvinces();
  player.printProvinces();

  list<BlackCard *> provinces = player.getProvinces();
  bool available[4] = {0}; // available[i] -> i-th province can be bought

  list<BlackCard*> :: iterator itProvince = provinces.begin();
  for (int i = 0; i < player.getNumberOfProvinces(); i++, itProvince++)
    available[i] = ((*itProvince)->getRevealed() == true);

  int targetProvince;
  for (int i = 0; i < player.getNumberOfProvinces(); i++) {
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
      player.buyBlackCard(targetProvince);
    }
    else {
      cout << "This province is unavailable, please choose another one" << endl;
      i--;
      continue;
    }
  }
}

void GameBoard::battlePhase(Player &player) {
  cout << "\n-------- BATTLE PHASE ----------\n" << endl;
  player.activatePersonalities();

  for (int i = 0; i < numberOfPlayers; i++)
    cout << "Player " << to_string(i) << "  ";

  // numberOfPlayers must be decremented each time a player dies

  string input;
  cout << endl << "Choose a player to attack (0 - " 
       << numberOfPlayers << ") or type 'ok' to continue :";
  cin >> input;
  cout << endl;

  int chosenPlayerInt;

  if (input != "ok") {
    stringstream temp(input);
    temp >> chosenPlayerInt;

    list<Player*>::iterator playerIt = players.begin();

    // Find a pointer to the target player
    for (int i = 0; i < chosenPlayerInt; i++, playerIt++);

    Player *chosenPlayer = *playerIt; 

    chosenPlayer->printProvinces();

    int chosenProvince;
    cout << "\nChoose a province to attack (0 - "
         << chosenPlayer->getNumberOfProvinces() << "): ";

    cin >> chosenProvince;
    cout << endl << endl;

    // No error-handling
    int attackerPoints = player.calculateAttackPoints();
    int defencerPoints = chosenPlayer->calculateDefencePoints();

    if (attackerPoints - defencerPoints - chosenPlayer->getInitialDefence()
                                       > chosenPlayer->getInitialDefence()) {

      if (chosenPlayer->destroyProvince(chosenProvince)) {
        delete *playerIt;
        players.erase(playerIt);
        cout << "Player " << chosenPlayer << " is dead !\n" << endl;
        reducePlayersNumber();
      }

      chosenPlayer->reduceProvinces();
      chosenPlayer->destroyActPers();
    }
    else {
      int difference = attackerPoints - defencerPoints
                      - chosenPlayer->getInitialDefence();

      if (difference > 0) {
        chosenPlayer->destroyActPers();
        player.discardActPCards(difference);
        player.reduceActPersHonour();
      }
      else if (attackerPoints == defencerPoints) {
        player.destroyActPers();
        chosenPlayer->destroyActPers();
      }
      else if (difference < 0) {
        player.destroyActPers();
        chosenPlayer->discardActPCards(difference);
        chosenPlayer->reduceActPersHonour();
      }
      player.battleReverberations();
    }
  }
}
