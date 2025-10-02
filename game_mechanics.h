#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#define BOARD_LEN 9
#define EMPTY '.'

typedef uint8_t u8;
typedef int8_t int8;
typedef struct{
  char  board[BOARD_LEN];
  u8 current_player; // 0 for player X and 1 for player O
}Gamestate;

u8 GAME_OWL[8][3]={
  {0,1,2},{3,4,5},{6,7,8},{0,3,6},
  {1,4,7},{2,5,8},{0,4,8},{2,4,6}
};

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

void init_game(Gamestate *);
void display_board(Gamestate *);
void  apply_move(Gamestate *,int8);
u8 check_game_over(Gamestate *,char *);
int8 count_opening_winining_lines(Gamestate *,char);
int8 game_heuristic(Gamestate *);
void cannonical_form(Gamestate * ,char *);

/*====================== impl =============================*/

void init_game(Gamestate *state){
  u8 x=0;
  while(x < BOARD_LEN)
    state->board[x++]=EMPTY;
  state->current_player=0;
}

void display_board(Gamestate *state){
  u8 row=3;
  for(u8 i =0 ;i < row; i++){
    printf("%c%c%c ",state->board[row * i],state->board[row * i +1],state->board[row * i +2]);
    printf("%d%d%d\n",row*i, row*i+1, row*i+2); 
   }
  printf("\n");
}


void  apply_move(Gamestate *state,int8 move){
  if (move < 0 || move > 8 || state->board[move] != EMPTY)
    return;
  state->board[move] = (state->current_player == 0) ? 'X' :'O';
  state->current_player = !state->current_player;
}


u8 check_game_over(Gamestate *state,char *winner){
  for(u8 row=0 ; row<3 ; row++){
    if(state->board[row*3] != EMPTY && state->board[row * 3] == state->board[row * 3+1] &&
       state->board[row*3] == state->board[row*3+2]){
      *winner=state->board[row*3];
      return 1;
    }
  }

 for(u8 i=0 ; i < 3 ; i++){
    if(state->board[i] != EMPTY && state->board[i] == state->board[i+3]  &&
       state->board[i] == state->board[i+6]){
      *winner=state->board[i];
      return 1;
    }
  }
   
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
  
  u8 empty_cells=0;
  for(u8 i =0 ; i< BOARD_LEN;i++){
    if(state->board[i] == EMPTY)
      empty_cells++;
  }
  
  if(!empty_cells){
    *winner='T';
    return 1;
  }
  return 0; 
}

/* count number of  opening winning lines for Gamestate`s current_player according to this
 * definition : An OWL is a line which contains at least one
 * of the player’s marks and none of the opponent’s.
 */
int8 count_opening_winining_lines(Gamestate *state,char NEG){
  int8 count=0;
  int8 empty_space=0;
  for(u8 i=0;i<8;i++){
    if(state->board[GAME_OWL[i][0]] != NEG && state->board[GAME_OWL[i][1]] != NEG
       &&  state->board[GAME_OWL[i][2]] != NEG)
      count++;
    if(state->board[GAME_OWL[i][0]] == EMPTY && state->board[GAME_OWL[i][1]] == EMPTY
       && state->board[GAME_OWL[i][2]] == EMPTY)
      empty_space++;	 					  
  }
  return (count - empty_space);
}

/* Game heuristic function : A function which assigns
 * a value to  each position in a game. This implementation  follows this
 * simple heuristic to assign values; Heuristic h is the number of open winning lines
 * (OWLs) for player 'X' minus the number of OWLs for 'O'
 */
int8 game_heuristic(Gamestate *state){
  return (count_opening_winining_lines(state,'O') - count_opening_winining_lines(state,'X'));
}

void cannonical_form(Gamestate *state,char *result){
  char temp[9];
  for(u8 i=0; i < 9; i++)
    result[i]=state->board[i]; 

  for(u8 j=1; j < 8;j++){
    for(u8 k=0; k < 9 ;k++)
      temp[k]=state->board[transformations[j][k]];

    for(u8 p=0; p < 9; p++){
      if(temp[p] < result[p]){
	for(u8 i=0;i< 9;i++)
	  result[i]=temp[i];
	break;
      }else if(temp[p] > result[p]){
	break;
       }
     } 
  }
  
}
