#include "min_max_tree.h"

void play_game();

int main(){
  while(1) {
   char play_again;
   play_game();
   printf("Play again? (y/n): ");
   scanf(" %c", &play_again);
   if (play_again != 'y' && play_again != 'Y') break;
    }
  return 0;
}


/* the default is is 'Max' in the algorithm sense but
 * one can change to 'Min' with slight modifications 
 */

void play_game(){
  Gamestate state;
  char winner;
  init_game(&state);
  printf("Welcome to Tic Tac Toe! You are O, the computer is X.\n");
  printf("Enter positions as numbers from 0 to 8 (see picture).\n");
  while(!check_game_over(&state,&winner)){
    display_board(&state);
    if(state.current_player == 0){
      printf("Computer turn\n");
      Node *root=create_node(&state);
      build_tree(&state,root,2);
      int move=-1;
      min_max(root,0,&move);
      apply_move(&state,move);
      printf("Computer placed O at position %d\n", move);
      free_tree(root);
    }else{
      int move;
      char movec;
      printf("Your move (0-8): ");
      scanf(" %c",&movec);
      move=movec - '0';
      if (move < 0 || move > 8 || state.board[move] != '.'){
            printf("Invalid move! Try again.\n");
            continue;
            }
       apply_move(&state,move);
    }
    
  }
  display_board(&state);
  if(winner == 'O'){
    printf("You win!\n");
  }else if(winner == 'X'){
    printf("Computer wins!\n");
  }else{
    printf("Its a tie!\n");
  }
  
}
