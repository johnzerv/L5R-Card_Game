#include <iostream>
#include <string>
#include <sstream>

#include "../include/gameboard.hpp"
#include "../include/card.hpp"

using namespace std;

GameBoard::~GameBoard() {
  list<Player *>::iterator playerIt = players.begin();

  while (playerIt != players.end()) {
    delete *playerIt;
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

    (*playerIt)->printHand();
    (*playerIt)->printProvinces();
    (*playerIt)->printArena();
    (*playerIt)->printHoldings();
  }
}

bool comparePlayers(const Player *a, const Player *b) {
  return a->getHonour() > b->getHonour();
}

void GameBoard::gameplay() {

  // Sort players in decreasing order, based on their honour
  players.sort(comparePlayers);

  bool gameover = false;

  while (!gameover) {
    list<Player *>::iterator playerIt = players.begin();

    int playerNO = 0;
    while (playerIt != players.end()) {

      cout << "\n\nPlayer's number " << playerNO << " turn !" << endl;

      startingPhase(**playerIt);
      equipPhase(**playerIt);
      battlePhase(**playerIt, playerNO);

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
  printGameStatistics();
  player.printHand();
  player.printProvinces();
  player.printHoldings();
}

void GameBoard::equipPhase(Player &player) {
  cout << "\n-------- EQUIPMENT PHASE ----------\n" << endl;

  if (player.getArmy().empty()) {
    cout << "Can't equip items right now" << endl;
    return;
  }

  player.printArena();
  player.printHand();

  // Keeps track of the player's balance for this phase
  int balance = player.getBalance();

  string selectedCardStr, personalityPosStr;
  int selectedCard, personalityPos;

  do {
    cout << "Balance: " << balance << endl;

    cout << "Enter the index of the card you want to buy"
       << " (or type ok to continue): ";

    cin >> selectedCardStr;
    if (selectedCardStr == "ok")
      break;

    cout << "\nEnter the index of the personality you want to expand"
         << " (or type ok to continue): ";

    cin >> personalityPosStr;
    if (personalityPosStr == "ok")
      break;
    cout << endl << endl;

    stringstream temp1(selectedCardStr), temp2(personalityPosStr);
    temp1 >> selectedCard;
    temp2 >> personalityPos;

    if (selectedCard >= 0 && selectedCard < player.getSizeOfHand()) {

      if (personalityPos >= 0 && personalityPos < player.getArmy().size())
        player.buyGreenCard(selectedCard, personalityPos, balance);
      else
        cout << "Out of bounds personality index";   
    }
    else
      cout << "Out of bounds card index" << endl;

    cout << endl << endl;
    player.printArena();
    player.printHand();
  } while (true);

  player.emptyWallet();
}

void GameBoard::economyPhase(Player &player) {
  cout << "\n-------- ECONOMY PHASE ----------\n" << endl;

  player.revealProvinces();

  int balance = player.getBalance();

  list<BlackCard *> provinces = player.getProvinces();
  bool available[4] = {0}; // available[i] -> i-th province can be bought

  list<BlackCard*>::iterator itProvince = provinces.begin();

  // Only revealed provinces will be available for purchase by default
  for (int i = 0; i < player.getNumberOfProvinces(); i++, itProvince++)
    available[i] = ((*itProvince)->getRevealed() == true);

  string targetProvinceStr;
  int targetProvince;

  for (int i = 0; i < player.getNumberOfProvinces(); i++) {
    player.printProvinces();

    cout << "Balance: " << balance << endl;

    cout << "Select a province (0 to 3) to buy (enter ok"
         << " if you wish to continue to the next phase): ";
    cin >> targetProvinceStr;

    if (targetProvinceStr == "ok") break;

    stringstream temp(targetProvinceStr);
    temp >> targetProvince;

    if (targetProvince < 0 || targetProvince > 3) {
      cout << "Invalid province index, please try again" << endl;
      i--;
      continue;
    }

    if (available[targetProvince]) {
      // 
      available[targetProvince] = false;

      player.buyBlackCard(targetProvince, balance);
    }
    else {
      cout << "This province is unavailable, please choose "
           << "another one\n" << endl;
      i--;
      continue;
    }
  }

  player.emptyWallet();
}

void GameBoard::battlePhase(Player &player, int playerNO) {
  cout << "\n-------- BATTLE PHASE ----------\n" << endl;

  if (player.getArmy().empty() && player.getActivatedPers().empty()) {
    cout << "Can't participate in a battle yet" << endl;
    return;
  }

  player.activatePersonalities();

  for (int i = 0; i < numberOfPlayers; i++){
    if (i == playerNO)
      continue;

    cout << "Player " << to_string(i) << "  ";
  }

  string input;
  cout << endl << "Choose a player to attack (0 - " 
       << numberOfPlayers - 1 << ") or type 'ok' to continue :";
  cin >> input;
  cout << endl;
  while(input == to_string(playerNO)){
    cout << "Can't attack to yourself, please try again" << endl;
    cin >> input;
  }

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
         << chosenPlayer->getNumberOfProvinces() - 1 << "): ";

    cin >> chosenProvince;
    cout << endl << endl;

    // No error-handling
    int attackerPoints = player.calculateAttackPoints();
    int defencerPoints = chosenPlayer->calculateDefencePoints();

    if (attackerPoints - defencerPoints - chosenPlayer->getInitialDefence()
                                       > chosenPlayer->getInitialDefence()) {

      cout << "You WON the battle !!!" << endl << endl;

      if (chosenPlayer->destroyProvince(chosenProvince)) {
        delete *playerIt;
        players.erase(playerIt);
        cout << "Player " << chosenPlayerInt << " is dead !\n" << endl;
        reducePlayersNumber();
      }

      chosenPlayer->reduceProvinces();
      chosenPlayer->destroyActPers();
    }
    else {
      int difference = attackerPoints - defencerPoints
                      - chosenPlayer->getInitialDefence();

      if (difference > 0) {
      cout << "You didn't manage to destroy the defencer province "
           << "but you destroy enemy's activated army !" << endl << endl;

        chosenPlayer->destroyActPers();
        player.discardActPCards(difference);
        player.reduceActPersHonour();
      }
      else if (!difference) {
              cout << "You and your enemy lost activated "
                   << "army after this battle !" << endl << endl;

        player.destroyActPers();
        chosenPlayer->destroyActPers();
      }
      else if (difference < 0) {
        cout << "You didn't manage to destroy the defencer province,"
             << "and you also lost your activated army !" << endl << endl;

        player.destroyActPers();
        chosenPlayer->discardActPCards(difference);
        chosenPlayer->reduceActPersHonour();
      }
      player.battleReverberations();
    }
  }
}
