Authors
-------

John Zervakis (Γιάννης Ζερβάκης) - sdi1800048
George Sittas (Γιώργος Σίττας)   - sdi1800179

Compilation and Execution
-------------------------

The following commands work inside the directory project
(i.e. make sure you're in the right directory before typing them in!)

- To compile & run the executable, type: make run
- To clean the directory, type:          make clean

- If you wish only to compile the project, type: make
  (the executable file's name is "L5R")

Project Goal
------------

In this project we've tried to simulate the trading cards game
"Legend of the Five Rings". The project's description can be
found in the file "OOPproj_2020.pdf".

Project Details
---------------

• Game Interface
  --------------

  The class GameBoard (see include/gameboard.hpp) is the interface through
  which a user can start playing the game. Specifically, in order to start
  playing, the user has to create an object of type GameBoard, invoke the
  initializeGameBoard() method and then invoke the gameplay() method.

  After that is done, the game will start and the players that participate
  in it will start being prompted for their moves on the terminal window.

• Project Structure
  -----------------

  - blackcards.hpp, greencards.hpp, constants.hpp : auxiliary .hpp files
  - cards.hpp : contains the (inline) definition of the Card (class) hierarchy

  - deck_builder.* : definition of class DeckBuilder
  - gameboard.*    : definition of class GameBoard
  - player.*       : definition of class Player

• Design Choices
  --------------

  - To start off, each player is instantiated inside the initializeGameBoard()
    method. This method takes an optional argument (how many players to create,
    2 by default) and creates the corresponding Player objects (in the heap).
    These Player objects are then pushed in a list<Player *> container (STL),
    which is a private member of the GameBoard class.

  - Inside the GameBoard scope, we have created a method for each of the game's
    phases (i.e. Starting phase, Equipment phase, etc.). Each of these methods
    receives a Player argument and basically simulates the corresponding phase
    for that player. These methods have been kept private to this scope, as the
    user doesn't need to know anything about them.

  - GameBoard::printGameStatistics() is trivial: it just prints each player's
    cards (i.e. Holdings, Hand, Provinces and Army).

  - In GameBoard::gameplay(), the player's are first sorted in decreasing order,
    based on their honour (sort() STL list method). After that, there's the game
    loop which determines the game flow: each player makes a move for the corre-
    sponding phase of the game, and a gameover check is made after each battle
    phase. If a winner is found, the program will print his ID and exit the loop.





