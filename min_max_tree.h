#include "game_mechanics.h"
#define MAX_CHILDREN 9
#define UNDEFINED -578949949
#define INFINITY 255255255

typedef struct node{
  Gamestate state;
  int  value;
  struct node *children[MAX_CHILDREN];
  int8 count;
  int move;  // store move made
}Node;

Node *create_node(Gamestate *);
void add_child(Node *,Node *);
void build_tree(Gamestate *, Node *,int8);
void traverse(Node *,int8);
void free_tree(Node *);
Node *min_max(Node *,int8,int *);

/*====================== impl =============================*/

Node *create_node(Gamestate *state){
  Node *temp=(struct node *)malloc(sizeof (Node));
  temp->state=*state;
  temp->value=UNDEFINED;
  temp->count=0;
  temp->move=UNDEFINED;
  return temp;
}


void add_child(Node *parent,Node *child){
  if(parent)
    if(parent->count < MAX_CHILDREN)
      parent->children[parent->count++]=child; 
}

/*
 * depth first recursive algorithm to build the min max
 * tree . We use the board symetric properties to reduce
 * the number of moves we explore eg an 'X' in the top top left diagnonal
 * after a rotation (90 degrees) is the same as an 'X' in the top right  diagonal
 * this properties reduce our state space .
 * check void cannonical_form(Gamestate * ,char *)
 */  
void build_tree(Gamestate *state, Node *root,int8 depth){
  char winner;
  char result[9];
  if(depth == 0 || (check_game_over(state,&winner) == 1)){
    root->value=game_heuristic(state);
    return;
  }
  char seen[9][10];
  u8 seen_count=0;
  for(u8 i = 0 ; i < BOARD_LEN ; i++){
    if(state->board[i] == EMPTY){
      Gamestate temp_state=*state;
      apply_move(state,i); 
      cannonical_form(state,result);
      result[9]='\0';
      u8 flag=0;
      for(u8 j=0 ; j < seen_count ; j++){
	if(strcmp(seen[j],result)==0){
	  flag=1;
	  break;
	}
      }
      if(flag){
	*state =temp_state;
	continue;
      }
      strcpy(seen[seen_count++],result);
      Node *child=create_node(state);
      child->move=i;
      add_child(root,child);
      build_tree(state,child,depth -1);
      *state=temp_state;
    }
  }  
}

void traverse(Node *node, int8 depth) {
    if (node == NULL) return;
    for (u8 i = 0; i < depth; i++) printf("  ");
    printf("Value: %d\n", node->value);
    for (u8 i = 0; i < node->count; i++)
      traverse(node->children[i], depth + 1);
}


void free_tree(Node *node){
  if(node == NULL)
    return;
  for(u8 i= 0 ; i < node->count; i++)
    free_tree(node->children[i]);
  free(node);
}

/* The Minimax algorithm is best explained
 * here https://www.math.umd.edu/~immortal/CMSC351/notes/minimax.pdf
 * the implementation here follows the pseudocode in the same resource
 * but with one addition,that when building the tree each node kept the
 * index of its resulting gamestate when the algorithm returns the node  representing
 * the actions of either 'Max' or 'Min' the index is returned as the third argument
 * of min_max(Node *,int depth,int *) 
 */
Node *min_max(Node *node,int8 depth,int *move){
  if(node->value != UNDEFINED)
    return node;
  else
    if((depth % 2 )== 0){
      int best = -INFINITY;
      for(u8 i=0;i<node->count;i++){
	Node *n=min_max(node->children[i],depth +1,move);
        if(n->value > best){
	  best=n->value;
	  *move=n->move;
	}
      }
      node->value=best;
    }else{
      int  best=+INFINITY;
      for(u8 i=0;i<node->count;i++){
	Node *n= min_max(node->children[i],depth +1,move);
        if(n->value < best){
	  best=n->value;
	  *move=n->move;
	}
      }      
      node->value=best;
    }
}

