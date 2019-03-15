Jasmine Duerk
February 1, 2019


The Game of Life is played on a rectangular grid of cells, so that each cell has eight neighbors (adjacent cells).
Each cell is either occupied by an organism or not. A pattern of occupied and unoccupied cells in the grid is called a generation.
The rules for deriving a new generation from the previous generation are these:

1. Death. If an occupied cell has 0, 1, 4, 5, 6, 7, or 8 occupied neighbors, the organism dies (0 or 1 of loneliness; 4 thru 8 of overcrowding).
2. Survival. If an occupied cell has two or three neighbors, the organism survives to the next generation.
3. Birth. If an unoccupied cell has precisely three occupied neighbors, it becomes occupied by a new organism.

The program should be invoked from the command line with the following arguments:
./HW2 NR NC gens inputfile [print] [pause]
where	
    • NR and NC are unsigned integers indicating the number of elements in the y and x directions of the grid, respectively. 
      In other words, the number of rows and columns, respectively, in the grid
    • gens is the number of generations to play. This value must be greater than zero. 
    • inputfile is the name of a file containing a sequence of lines, each consisting of a sequence of 'x' and 'o' characters, indicating the occupied and unoccupied cells of the initial configuration. 
      An ‘x’ indicates an occupied cell, an ‘o’ or a blank indicates an unoccupied cell. If a line is shorter than the width of the grid, cells to the right are considered unoccupied. 
      If there are fewer lines in the file than the height of the grid, cells below are considered unoccupied. 
    • print is an optional argument with value of 'y' or 'n' indicating whether each generation (including generation 0) should be printed or displayed before proceeding to the next generation. 
      If this item is missing, it defaults to 'n'. 
    • pause is an optional argument with value of 'y' or 'n' indicating whether a keystroke is needed between generations. 
      If this and/or the print item is missing, it defaults to 'n'. It is not possible to specify a value for pause if you do not also specify a value for print.
      
