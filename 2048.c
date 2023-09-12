#include "game.h"
/*
the purpose of this code is to provide funtions that combine with main to make the 2048 game. to do this there needed to be 8 functions finished.
the first being make game. This essentially allocated enough memory for the game to be played. this created the space for the same board
the second is remake_game. This goes through all the allocated memories and sets each location to -1 to indicate an empty cell
the third is get_cell. This returns the address of a specific cell within the game. But this also makes sure that the cell is within the game boundaries
the fourth, fifth, sixth and seventh cells are all very similar. these are the inputs to the game.
they are the functions that occur when you press w,a,s,d respectively/
the way I do this is by creating anther board which indicates whether or not a cell has already been merged or not. I loop through this array and mark all cells as unmerged. After this I then loop through the current game and check each cell to see if it is filled with a number, If this is true then I pass this cell through another loop which checks the adjacent cell (dependent on the key but for w it checks the cell above). I set a flag to 0 which indicated a cell has not already been shifted. then if the cell above is empty I mark my valid move flag to 1 indicating that there is a valid move and i set my shift flag to 1 aswell. after this I move the cell up by setting the cell above to the cell below and then i set the cell below to empty (-1). after this I check to see if a cell can be merged. of it can then I set the cell above to twice the current cell (because it moves up and combines) then i clear the current cell and I break from this loop.

lastly there is the eigth function which checks to see if there are any legal moves left. It does this by checking each cell to see if it is either empty or if a cell above, below, left, or right are able to be merged. If this is true then we know there is a legal move. otherwise ther is not a legal move and the game should end.


 */
game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    mygame->cols = cols;
    mygame->rows = rows;
    mygame->score = 0;
    for(int i = 0; i < rows; i++){ //loops through rows ance cells and sets the data to -1
      for(int j = 0; j < cols; j++){
	mygame->cells[(i*cols)+j] = -1;
      }
    }
    
    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct

	(*_cur_game_ptr)->rows = new_rows;
	(*_cur_game_ptr)->cols = new_cols;
	(*_cur_game_ptr)->score = 0;
	for(int i =0; i < new_rows; i++){ //loops through new game board and sets all the values equal to -1
	  for(int j = 0; j < new_cols; j++){
	    (*_cur_game_ptr)->cells[(i*new_cols)+j] = -1;
	  }
	}

	
	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
  if(row < 0 || col < 0 || (cur_game->cols) < col || (cur_game->rows) < row){ //if outside boundaries then return null
    return NULL;
  }
  return &cur_game->cells[(row * (cur_game->cols)) + col]; //otherwise return the address
    return NULL;
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    //YOUR CODE STARTS HERE
  int valid_move = 0; //CONSTANT FOR ALL WASD
  int i, j, k;
  int cols = cur_game->cols, rows = cur_game->rows;
  int mergedboard[rows][cols]; //creates a copy board to see if a position has been merged yet
  for(i = 0; i < rows; i++){ //loops through each position
    for(j = 0; j < cols; j++){
      mergedboard[i][j] = 0; //marks each position as unmerged
    }
  }
  for(i = 0; i < rows; i++){
    for(j = 0; j < cols; j++){ //loop though entire board
      if(cur_game->cells[(i * cols) + j] != -1){
	for(k = 0; k <= i; k++){ //loops through rows again
	  int shift_flag = 0;
	  if(cur_game->cells[(k * cols) + j] == -1){//if a position is empty then there is a valid move
	    valid_move = 1; //is a valid move
	    shift_flag = 1; //can be shifted
	    cur_game->cells[(k * cols) + j] = cur_game->cells[(i * cols) + j];//move cell up
	    cur_game->cells[(i * cols) + j] = -1; //set cell to empty
	  }
	  if(k == i || shift_flag == 1){
	    if(mergedboard[k - 1][j] == 0 && k != 0 && cur_game->cells[(k * cols) + j] == cur_game->cells[((k-1) * cols) + j]){
	      valid_move = 1;
	      cur_game->cells[((k-1) * cols) + j] = cur_game->cells[(k * cols) + j] * 2; //replaces cell above with double the cell below
	      cur_game->score = cur_game->score + (cur_game->cells[(k * cols) + j] * 2); //adds the new cells score to the score counter
	      cur_game->cells[(k * cols) + j] = -1;// marks cell as empty
	      mergedboard[k - 1][j] = 1; //marks new position as merged
	    }
	    break;//break from k for loop
	  }
	}
      }
    }
  }
	  
    return valid_move;
}

int move_s(game * cur_game) //slide down
{
    //YOUR CODE STARTS HERE
  int valid_move = 0; //CONSTANT FOR ALL WASD
  int i, j, k;
  int cols = cur_game->cols, rows = cur_game->rows;
  int mergedboard[rows][cols]; //creates a copy board to see if a position has been merged yet
  for(i = 0; i < rows; i++){ //loops through each position
    for(j = 0; j < cols; j++){
      mergedboard[i][j] = 0; //marks each position as unmerged
    }
  }
  for(i = rows - 1; i >= 0; i--){
    for(j = 0; j < cols; j++){ //loop though entire board
      if(cur_game->cells[(i * cols) + j] != -1){
	for(k = rows - 1; k >= i; k--){ //loops through rows again
	  int shift_flag = 0;
	  if(cur_game->cells[(k * cols) + j] == -1){
	    valid_move = 1;
	    shift_flag = 1;
	    cur_game->cells[(k * cols) + j] = cur_game->cells[(i * cols) + j];//move cell up
	    cur_game->cells[(i * cols) + j] = -1; //set cell to empty
	  }
	  if(k == i || shift_flag == 1){
	    if(mergedboard[k + 1][j] == 0 && k != (rows-1) && cur_game->cells[(k * cols) + j] == cur_game->cells[((k+1) * cols) + j]){
	      valid_move = 1;
	      cur_game->cells[((k+1) * cols) + j] = cur_game->cells[(k * cols) + j] * 2; //replaces cell above with double the cell below
	      cur_game->score = cur_game->score + (cur_game->cells[(k * cols) + j] * 2); //adds the new cells score to the score counter
	      cur_game->cells[(k * cols) + j] = -1;// marks cell as empty
	      mergedboard[k + 1][j] = 1; //marks new position as merged
	    }
	    break;//break from k for loop
	  }
	}
      }
    }
  }
    return valid_move;
}

int move_a(game * cur_game) //slide left
{
    //YOUR CODE STARTS HERE
  int valid_move = 0; //CONSTANT FOR ALL WASD
  int i, j, k;
  int cols = cur_game->cols, rows = cur_game->rows;
  int mergedboard[rows][cols]; //creates a copy board to see if a position has been merged yet
  for(i = 0; i < rows; i++){ //loops through each position
    for(j = 0; j < cols; j++){
      mergedboard[i][j] = 0; //marks each position as unmerged
    }
  }
  for(i = 0; i < rows; i++){
    for(j = 0; j < cols; j++){ //loop though entire board
      if(cur_game->cells[(i * cols) + j] != -1){
	for(k = 0; k <= j; k++){ //loops through cols again
	  int shift_flag = 0;
	  if(cur_game->cells[(i * cols) + k] == -1){
	    valid_move = 1;
	    shift_flag = 1;
	    cur_game->cells[(i * cols) + k] = cur_game->cells[(i * cols) + j];//move cell up
	    cur_game->cells[(i * cols) + j] = -1; //set cell to empty
	  }
	  if(k == j || shift_flag == 1){
	    if(mergedboard[i][k - 1] == 0 && k != 0 && cur_game->cells[(i * cols) + k] == cur_game->cells[(i * cols) + (k-1)]){
	      valid_move = 1;
	      cur_game->cells[(i * cols) + k -1] = cur_game->cells[(i * cols) + k] * 2; //replaces cell above with double the cell below
	      cur_game->score = cur_game->score + (cur_game->cells[(i * cols) + k] * 2); //adds the new cells score to the score counter
	      cur_game->cells[(i * cols) + k] = -1;// marks cell as empty
	      mergedboard[i][k - 1] = 1; //marks new position as merged
	    }
	    break;//break from k for loop
	  }
	}
      }
    }
  }
    return valid_move;
}

int move_d(game * cur_game){ //slide to the right
    //YOUR CODE STARTS HERE
  int valid_move = 0; //CONSTANT FOR ALL WASD
  int i, j, k;
  int cols = cur_game->cols, rows = cur_game->rows;
  int mergedboard[rows][cols]; //creates a copy board to see if a position has been merged yet
  for(i = 0; i < rows; i++){ //loops through each position
    for(j = 0; j < cols; j++){
      mergedboard[i][j] = 0; //marks each position as unmerged
    }
  }

  for(i = 0; i < rows; i++){
    for(j = cols -1; j >= 0; j--){ //loop though entire board
      if(cur_game->cells[(i * cols) + j] != -1){
	for(k = cols-1; k >= j; k--){ //loops through cols again
	  int shift_flag = 0;
	  if(cur_game->cells[(i * cols) + k] == -1){
	    valid_move = 1;
	    shift_flag = 1;
	    cur_game->cells[(i * cols) + k] = cur_game->cells[(i * cols) + j];//move cell up
	    cur_game->cells[(i * cols) + j] = -1; //set cell to empty
	  }
	  if(k == j || shift_flag == 1){
	    if(mergedboard[i][k + 1] == 0 && k != cols-1 && cur_game->cells[(i * cols) + k] == cur_game->cells[(i * cols) + (k+1)]){
	      valid_move = 1;
	      cur_game->cells[(i * cols) + k +1] = cur_game->cells[(i * cols) + k] * 2; //replaces cell above with double the cell below
	      cur_game->score = cur_game->score + (cur_game->cells[(i * cols) + k] * 2); //adds the new cells score to the score counter
	      cur_game->cells[(i * cols) + k] = -1;// marks cell as empty
	      mergedboard[i][k + 1] = 1; //marks new position as merged
	    }
	    break;//break from k for loop
	  }
	}
      }
    }
  }
    return valid_move;
}

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE
  int col = cur_game->cols, row = cur_game->rows;
  int flag = 0;
  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      if(cur_game->cells[(i*col) +j] == -1){//checks to see if a space is empty
	flag = 1;
      }
      if(i < row -1 && cur_game->cells[(i*col) + j] == cur_game->cells[((i+ 1)*col) + j]){//checks spaces above and below to see if there is the ability to merge
	flag = 1;
      }
      if(j < col -1 && cur_game->cells[(i*col) + j] == cur_game->cells[(i * col) + j + 1]){//checks spaces left and right to see if there is the ability to merge
	flag = 1;
      }
    }
  }   
    return flag;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
