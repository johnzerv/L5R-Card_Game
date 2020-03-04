Authors
-------

- John Zervakis

- [George Sittas](https://github.com/GeorgeSittas)

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
    user doesn't need to know anything about them, and their implementation is
    easy to follow, since it's basically a translation of their description to
    code.

  - One note about the equipment phase: a player can buy Follower/Item cards
    during this phase, only if he has non active personalities (i.e. only if
    his army list is non empty). Basically, we've made the design decision that
    once a Personality card is activated (see battle phase), it can no longer
    equip any Green cards.

  - GameBoard::printGameStatistics() is trivial: it just prints each player's
    cards (i.e. Holdings, Hand, Provinces and Army).

  - In GameBoard::gameplay(), the player's are first sorted in decreasing order,
    based on their honour (sort() STL list method). After that, there's the game
    loop which determines the game flow: each player makes a move for the corre-
    sponding phase of the game, and a "gameover check" is made after each battle
    phase. If a winner is found, the program will print his ID and exit the loop.

  - The Card (class) hierarchy is shown below:

                                Card (abstract)
                              /               \
                    GreenCard (abstract)   BlackCard (abstract)
                     /             \          /             \
                Follower          Item     Holding      Personality
                   |                |         |               |
                 [...]            [...] Stronghold, [...]    [...]

    where [...] denotes a set of trivial classes (see [blackcards|greencards].hpp).
    Each class in the Card (class) hierarchy contains a getType() method, which is
    used to identify it from the other classes in the hierarchy.

  - Each Personality has two lists (STL): one for its followers and one for its
    items.

  - The deck_builder.* files have been slightly edited, so that they can be adapted
    to the needs of our project (changes are easy to follow).

  - Each player has a unique stronghold, since each stronghold card has a _unique_
    ID (see the Stronghold class definition).

  - Each player has lists (STL) for his: provinces, hand cards, holdings and army.
    Additionally, each player has an auxiliary list "activatedPersonalities", which
    is needed for the implementation of the battle phase (see problem description).

  - The Player class's methods are generally easy to follow. Some choices we made:

    • When a player exceeds the hand cards limit, the surplus cards are discarded
      randomly.

    • The tapHoldings(cost) method checks whether a player's holdings can provide
      him with the sufficient harvest value (i.e. so that the 'cost' value is
      reached). If that's the case, it returns true, otherwise false.

General Notes
-------------

- A simple driver program has been included in the project (main.cpp), in order
  to showcase the game's flow through the gameplay() method.

- We have added comments extensively, so that the code will be easier to follow.
  In any case, most of the work done is trivial, so that shouldn't be a problem.

- Even though there are places in the code where we have tried to incorporate
  error handling, such as carefully chosen error diagnostic messages and out
  of bounds checks, in general we haven't paid much attention to this matter
  (i.e. there could be places where error checking is absent from the code).

- We've debugged the program to the point where one can (hopefully) use it without
  any nuisances, but there could still be some hidden bugs underneath!
  Any feedback on this matter will be appreciated! :)

TO DO
-----

- Handle the case where a player runs out of cards during gameplay
  (trivial solution: generate more cards through DeckBuilder)

- Print player's atk/def stats during Battle Phase, so that the
  player who is making a move can decide whether or not he wants
  to attack/defence someone/against someone.
