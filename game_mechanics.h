#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define EMPTY '.'

typedef struct{
  char  board[9];
  int current_player; // 0 for player X and 1 for player O
}Gamestate;

/* winning lines of the game */
int GAME_OWL[8][3]={
  {0,1,2},
  {3,4,5},
  {6,7,8},
  {0,6,3},
  {7,4,1},
  {8,5,2},
  {0,4,8},
  {2,4,6}
};

/* all tic tac toe board transformations as indices to
 * help in observing board symetries by rotating
 * and reflecting the board
 */
const int transformations[8][9]={
  {0,1,2,3,4,5,6,7,8},
  {2,5,8,1,4,7,0,3,6},
  {8,7,6,5,4,3,2,1,0},
  {6,3,0,7,4,1,8,5,2},
  {0,3,6,1,4,7,2,5,8},
  {6,7,8,3,4,5,0,1,2},
  {2,1,0,5,4,3,8,7,6},
  {8,5,2,7,4,1,6,3,0}
};

void init_game(Gamestate *state){
  memset(state->board,EMPTY,9);
  state->current_player=0;
}

void display_board(Gamestate *state){
  int row=3;
  for(int i =0 ;i<row;i++){
    printf("%c%c%c ",state->board[row * i],state->board[row * i+1],state->board[row * i+2]);
    printf("%d%d%d\n",row*i,row*i+1,row*i+2); // display numbers on the side
  }
  printf("\n");
}

/* if move is invalid just return if successful switch players */
void  apply_move(Gamestate *state,int move){
       if (move < 0 || move > 8 || state->board[move] != EMPTY)
	 return;
       state->board[move] = (state->current_player == 0) ? 'X' :'O';
       state->current_player = !state->current_player;
}

/* crude method but works */
int check_game_over(Gamestate *state,char *winner){
  //checking rows
  for(size_t row=0;row<3;row++){
    if(state->board[row*3] != EMPTY && state->board[row * 3] == state->board[row * 3+1] &&
       state->board[row*3] == state->board[row*3+2]){
      *winner=state->board[row*3];
      return 1;
    }
  }
  //check columns
  for(size_t i=0;i<3;i++){
    if(state->board[i] != EMPTY && state->board[i] == state->board[i+3]  &&
       state->board[i] == state->board[i+6]){
      *winner=state->board[i];
      return 1;
    }
  }
  //check diagonals
  if(state->board[0] != EMPTY && state->board[0] == state->board[4] &&
     state->board[0] == state->board[8]){
    *winner=state->board[0];
    return 1;
  }
  if(state->board[2] != EMPTY && state->board[2] == state->board[4] &&
     state->board[2] == state->board[6]){
    *winner=state->board[2];
    return 1;
  }
  
  int empty_cells=0;
  for(size_t i=0 ; i< 9;i++){
    if(state->board[i] == EMPTY)
      empty_cells++;
  }
  if(!empty_cells){
    *winner='T';
    return 1;
  }
  return 0; //joue 
}

/* count number of  opening winning lines for Gamestate`s current_player according to this
 * definition : An OWL is a line which contains at least one
 * of the player’s marks and none of the opponent’s.
 */
int count_owl(Gamestate *state,char NEG){
  int count=0;
  int empty_space=0;
  for(size_t i=0;i<8;i++){
    if(state->board[GAME_OWL[i][0]] != NEG && state->board[GAME_OWL[i][1]] != NEG &&  state->board[GAME_OWL[i][2]] != NEG)
      count++;
    if(state->board[GAME_OWL[i][0]] == EMPTY && state->board[GAME_OWL[i][1]] == EMPTY && state->board[GAME_OWL[i][2]] == EMPTY)
      empty_space++;	 					  
  }
  return (count - empty_space);
}

/* Game heuristic function : A function which assigns
 * a value to  each position in a game. This implementation  follows this
 * simple heuristic to assign values; Heuristic h is the number of open winning lines
 * (OWLs) for player 'X' minus the number of OWLs for 'O'
 */
int game_heuristic(Gamestate *state){
  int x_score,y_score;
  x_score =count_owl(state,'O');
  y_score = count_owl(state,'X');
  return (x_score - y_score );
}

/* perform board rotations and reflections and return
 * the board in its smallest lexicographical form
 */
void cannonical_form(Gamestate *state,char *result){
  char temp[9];
  for(size_t i =0 ;i< 9;i++)
    result[i]=state->board[i];
  
  for(size_t i =1;i<8;i++){
    for(size_t j= 0;j<9;j++)
      temp[j]=state->board[transformations[i][j]];
 
    for(size_t t=0;t<9;t++){
      if(temp[t] > result[t])
        for(size_t i =0 ;i< 9;i++)
	   result[i]=temp[i];
      else if(temp[t] < result[t])
           break;
    }
  } 
}

