# Conway

A C++ implementation of the famous "Conway's Game of Life"

## **How to use:**
Settings of the simulation are determined in a "settings.txt" file that should be located in the same folder.
If no file is found, a default one will be generated.
Once the settings are loaded, you can choose to load an initial game state from a text file or make it random.

After the simulation is finished you can:
	- restart the simulation. Resets stats.
	- continue the simulation. Continues simulation for another amount of generations specified in settings.
	- save and exit the program. Saves the game state to a text file
	- exit the program

## **Game save:**
The game save should consist of zeroes (empty) and ones (cell) divided by commas.
For example:

0,0,1,0,0,0,0,0,0,0  
0,0,1,0,0,0,0,1,0,0  
1,1,0,1,0,0,0,1,1,1  
0,1,1,0,0,0,1,0,0,1  
0,1,1,1,0,0,0,0,1,1  
0,1,0,0,0,0,0,0,0,0  
1,1,0,0,0,1,0,0,0,1  
1,0,0,0,0,1,0,0,0,0  
1,1,0,0,0,0,0,0,0,0  
0,0,1,1,0,0,0,0,0,0  

## **Default settings:**

height=25  
width=25  
lower_p=25  
upper_p=30  
rules_survival=23  
rules_birth=3  
number_of_generations=10000  
speed_multiplier=1  
periodic=0  
display_style=0  

## **Settings explained:**

- height, width - determine the size of the game board.


- lower_p, upper_p - percentage boundaries determining how many cells will be initially alive.

		   EXAMPLE:
			lower_p = 25, upper_p = 30
			Initial number of alive cells will be between 25% and 30% of the total game board size.


- rules_survival - how many neighbors does a cell need to stay alive.

		   EXAMPLE:
		   	Value "23" would mean that the cell needs 2 or 3 neighbors.

		   SPECIAL CONDITION:
		   	Value "9" is a lack of rules of survival.


- rules_birth - how many neighbors does a dead cell need to be born again.

	           EXAMPLE:
	                Value "3" would mean that the cell needs 3 neighbors.

	      	   SPECIAL CONDITION:
		        Value "9" is a lack of rules of birth .


- number_of_generations - how many consecutive generations you want to simulate.


- speed_multiplier - multiplier of the speed of the simulation.

		   ExAMPLES:		   
			Value 3 would result in a simulation being 3 times faster.
		  	Value 0.5 would result in a simulation being 2 times slower.


- periodic - Takes 0 or 1. Periodic boundary conditions.
	     0 - cells can't cross borders of the game board.
	     1 - allows the cells to cross borders of the game board and show up on the opposite site.


- display_style - Takes 0 or 1. Defines the graphic style.
		  0 - is a basic style.
		  1 - uses extended ASCII characters and changes console's background color (it looks clearer)
