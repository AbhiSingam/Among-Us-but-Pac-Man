# Among Us (definitely not Pac-Man) in OpenGL
## Concept
This game is a mix of two popular games, Among Us and Pac-Man. It takes attributes such as tasks and impostors from Among Us and takes attributes like the maze and the obstacles and coins from Pac-Man. 

## Rules
You spawn in a 19x19 maze with an impostor whose sole objective is to eliminate you. If the impostor comes in contact with your character, you lose.  
To win the game, you must complete two tasks and reach the end of the maze before the impostor catches you and before the time limit runs out.
### The tasks:
There are two tasks that you must complete to be able to win:  
1. Kill the impostor: To kill the impostor you must reach the green button which will instantly vaporise the impostor.
2. Release the Coins and Obstacles: To complete this task, you must reach the grey button. Upon reaching this button, bombs and coins will drop. Coins will give you 70 points and Bombs will reduce your score by 30 points.

The objective of the game is to complete both tasks, collect as many points as possible, and exit the maze before time runs out or the impostor catches you.

## Controls:
 - Movement - WASD
 - Exit game - X or esc

## Running the game
Once you've unzipped and entered the downloaded file, run the following commands:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./Hello-World
```
