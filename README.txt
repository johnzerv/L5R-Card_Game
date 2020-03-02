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

• Project structure


