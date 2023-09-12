#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
/* Logan Wonnacott
The purpose of this code is to provide funtions to solve a maze
the first being maze_t. This opens the file that contains the unsolved maze and it gathers the lenght and width of the input. It then allocates enough memory for the size of the board. After this is create a replica board by copying the input and pasting that to my own board. While I do this I check to see if a certain positions is either the start or the end. If they are then I mark those locations in the data structure.
The second being destroyMaze. This loops through each row and deallocates memory and frees it back to the system.
the third being printMaze. This loops through the replica board and prints the board to the screen.
The last being solveMazeDFS. This is the recursive funtion. First it checks the three base cases. First I check to make sure We are within the mazes boundaries. Next I check to see if we are at the start or end AND that position is unmarked. The last base case is checking if we are at the end. If so then return 1. Otherwise I call this function and check if I cam move left. If you can then do so. Otherwise it checks right, then up, then down. If it can move it places a * . If it gets stuck then it back tracks and places a ~.

 */

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
  FILE *fileName1;
    fileName1 = fopen(fileName, "r"); //opens fileName file to read
    maze_t * maze = malloc(sizeof(maze_t));
    
  int width, height;
  fscanf(fileName1, "%d %d", &width, &height); //scans input file and puts into width and height

  maze->width = width; //puts parameters into maze structure
  maze->height = height;

  maze->cells = malloc(sizeof(maze->cells)*(maze->height)); //allocates memory for rows

  for(int i = 0; i< maze->height; i++){
    maze->cells[i] = malloc(sizeof(maze->cells[i]) * maze->width); // for each row allocate memory for the col
  }

 
  //scan file and print the input such
  char input;
  fscanf(fileName1, "%c", &input);//new line after width and height
  for(int i = 0; i < maze->height; i++){ //loop through each position of the maze
    for(int j = 0; j < maze->width; j++){
     
      fscanf(fileName1, "%c", &input); //scans input
      maze->cells[i][j] = input; //sets location to whatever the input was

      //check to see if it is start or end location
      if(maze->cells[i][j] == 'S' || maze->cells[i][j] == 'E'){ //checks if each position is start of end
	if(maze->cells[i][j] == 'S'){ //start case
	  maze->startRow = i;
	  maze->startColumn = j;
	}
	if(maze->cells[i][j] == 'E'){// end case
	  maze->endRow = i;
	  maze->endColumn = j;
	}
      }
    }
    fscanf(fileName1, "%c", &input);//new line
  }
      
  fclose(fileName1);
  return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
  // free(maze); //frees maze
  for(int i =0; i<maze->height; i++){
    free(maze->cells[i]);
  }
  free(maze->cells);
  free(maze);
      
    return;
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
  int width = maze->width, height = maze->height;
  for(int i = 0; i < height; i++){ //loops through board
    for(int j = 0; j < width; j++){
      printf("%c", maze->cells[i][j]); //prints character thats in each position
    }
    printf("\n"); //prints a new line
  }
  return;      
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.

  //Base case 1: if outside of  the board return 0
  if(col<0 || row<0 || col>=maze->width || row>=maze->height){
    return 0;
  }
  //Base case 2: if at start or end are unmarked return 0
  if((maze->cells[row][col]) != 'S' && (maze->cells[row][col]) != 'E'){
      if((maze->cells[row][col]) != ' '){
    return 0;
    }
  }
  //Base case 3: if we are at the end return 1
  if((maze->cells[row][col]) == 'E'){
    maze->cells[maze->startRow][maze->startColumn] = 'S';
    return 1;
  }

  maze->cells[row][col] = '*';
  //recursive cases

  //if you can move left do so
  if(solveMazeDFS(maze, col, row+1)){
      return 1;
    }
  //if you can move right do so
  if(solveMazeDFS(maze, col, row-1)){
    return 1;
      }
  //If you can move up do so
  if(solveMazeDFS(maze, col+1, row)){
    return 1;
  }
  //If you can move down do so
  if(solveMazeDFS(maze, col-1, row)){
    return 1;
  }

  //other wise mark cell as ~ and return 0
  maze->cells[row][col] = '~';
  return 0;
  
}
