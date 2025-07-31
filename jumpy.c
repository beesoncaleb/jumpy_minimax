#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Global variables and definitions
#define BOARD_LEN 4
#define WHITE_TURN 1
#define BLACK_TURN -1
#define POS_INF 999999
#define NEG_INF -999999
int static_counter = 0;
int best_move[BOARD_LEN];

// singly linked list of moves
typedef struct movelist{
  int w1;
  int w2;
  int b1;
  int b2;
  struct movelist* next;
} movelist;

// Function declarations
int static_estimator(int[], int);
int compute_win(int[]);
int white_maximizer(int[], int, int, int);
int black_minimizer(int[], int, int, int);
int minimax(int[], int, int);
movelist* compute_moves(int[], int);

int minimax(int board[], int depth, int player_turn) {
  if (depth < 0) {
    best_move[0] = -1;
    best_move[1] = -1;
    best_move[2] = -1;
    best_move[3] = -1;
    return 0;
  }

  int checkwin = compute_win(board);
  if (checkwin != 0) {
    best_move[0] = -2;
    best_move[1] = -2;
    best_move[2] = -2;
    best_move[3] = -2;
    return 0;
  }
  movelist* moves = compute_moves(board, player_turn);
  
  if (player_turn == WHITE_TURN) {
    int value = NEG_INF;
    movelist* curr = moves;

    while (curr != NULL) {
      int current_move[] = {curr->w1, curr->w2, curr->b1, curr->b2};
      int estimate = black_minimizer(current_move, value, POS_INF, depth-1);   //At root level so alpha is equivalent to value in this case
      if (estimate > value) {
        value = estimate; 
        best_move[0] = curr->w1;
        best_move[1] = curr->w2;
        best_move[2] = curr->b1;
        best_move[3] = curr->b2;
      }
      
      // iteration
      movelist* garbage = curr;
      curr = curr->next;
      free(garbage);
    }
    return value;
  }
  else { 
    int value = POS_INF;
    movelist* curr = moves;

    while (curr != NULL) {
      int current_move[] = {curr->w1, curr->w2, curr->b1, curr->b2};
      int estimate = white_maximizer(current_move, NEG_INF, value, depth-1);   //At root level so beta is equivalent to value in this case
      if (estimate < value) {
        value = estimate; 
        best_move[0] = curr->w1;
        best_move[1] = curr->w2;
        best_move[2] = curr->b1;
        best_move[3] = curr->b2;
      }
      
      // iteration
      movelist* garbage = curr;
      curr = curr->next;
      free(garbage);
    }
    return value;
  }
}

int white_maximizer(int board[], int alpha, int beta, int depth) {
  if (depth == 0) {
    return static_estimator(board, WHITE_TURN);
  }

  int checkwin = compute_win(board);
  if (checkwin != 0) { return checkwin; }

  movelist* moves = compute_moves(board, WHITE_TURN);
  int value = NEG_INF;
  movelist* curr = moves;

  while (curr != NULL) {
    int current_move[] = {curr->w1, curr->w2, curr->b1, curr->b2};
    int estimate = black_minimizer(current_move, alpha, beta, depth-1);
    if (estimate > value) {
      value = estimate; 
    }

    // if beta test fails, clean up memory and return early
    if (value >= beta) {
      while(curr != NULL) {
        movelist* garbage = curr;
        curr = curr->next;
        free(garbage);
      }
      return value;
    }
    alpha = value > alpha ? value : alpha;
    
    // iteration
    movelist* garbage = curr;
    curr = curr->next;
    free(garbage);
  }
  return value;
}

int black_minimizer(int board[], int alpha, int beta, int depth) {
  if (depth == 0) {
    return static_estimator(board, BLACK_TURN);
  }

  int checkwin = compute_win(board);
  if (checkwin != 0) { return checkwin; }

  movelist* moves = compute_moves(board, BLACK_TURN);
  int value = POS_INF;
  movelist* curr = moves;

  while (curr != NULL) {
    int current_move[] = {curr->w1, curr->w2, curr->b1, curr->b2};
    int estimate = white_maximizer(current_move, alpha, beta, depth-1);
    if (estimate < value) {
      value = estimate; 
    }

    // if alpha test fails, clean up memory and return early
    if (value <= alpha) {
      while(curr != NULL) {
        movelist* garbage = curr;
        curr = curr->next;
        free(garbage);
      }
      return value;
    }
    beta = value < beta ? value : beta;
    
    // iteration
    movelist* garbage = curr;
    curr = curr->next;
    free(garbage);
  }
  return value;
}

movelist* compute_moves(int board[], int player_turn) {
  int w1 = board[0];
  int w2 = board[1];
  int b1 = board[2];
  int b2 = board[3];

  if (player_turn == WHITE_TURN) {
    movelist *move1 = malloc(sizeof(movelist));
    move1->next = NULL;
    move1->w1 = -1;   // use w1 to signal whether ally2 should extend linkedlist or not
    if (w1 != 9) {
      int tw1 = w1;
      int tw2 = w2;
      int tb1 = b1;
      int tb2 = b2;
      int potential_move = tw1 + 1;
      bool b1jumped = false;
      bool b2jumped = false;

      while ((potential_move ==  tw2 && tw2 != 9) || potential_move == tb1 || potential_move == tb2) {
        if (potential_move == tb1) {
          b1jumped = true;
        }
        if (potential_move == tb2) {
          b2jumped = true;
        }
        potential_move++;
      }
      tw1 = potential_move;

      if (b1jumped) {
        int curr = 8;
        while (curr == tw1 || curr == tw2 || curr == tb2) {
          curr--;
        }
        tb1 = curr;
      }
      if (b2jumped) {
        int curr = 8;
        while (curr == tw1 || curr == tw2 || curr == tb1) {
          curr--;
        }
        tb2 = curr;
      }

      if (tw1 < tw2) {
        move1->w1 = tw1;
        move1->w2 = tw2;
      }
      else {
        move1->w1 = tw2;
        move1->w2 = tw1;
      }
      if (tb1 < tb2) {
        move1->b1 = tb1;
        move1->b2 = tb2;
      }
      else {
        move1->b1 = tb2;
        move1->b2 = tb1;
      }
    }
    if (w2 != 9) {
      movelist *move2 = malloc(sizeof(movelist));
      move2->next = NULL;
      int tw1 = w1;
      int tw2 = w2;
      int tb1 = b1;
      int tb2 = b2;
      int potential_move = tw2 + 1;
      bool b1jumped = false;
      bool b2jumped = false;

      while ((potential_move ==  tw1 && tw1 != 9) || potential_move == tb1 || potential_move == tb2) {
        if (potential_move == tb1) {
          b1jumped = true;
        }
        if (potential_move == tb2) {
          b2jumped = true;
        }
        potential_move++;
      }
      tw2 = potential_move;

      if (b1jumped) {
        int curr = 8;
        while (curr == tw1 || curr == tw2 || curr == tb2) {
          curr--;
        }
        tb1 = curr;
      }
      if (b2jumped) {
        int curr = 8;
        while (curr == tw1 || curr == tw2 || curr == tb1) {
          curr--;
        }
        tb2 = curr;
      }

      if (tw1 < tw2) {
        move2->w1 = tw1;
        move2->w2 = tw2;
      }
      else {
        move2->w1 = tw2;
        move2->w2 = tw1;
      }
      if (tb1 < tb2) {
        move2->b1 = tb1;
        move2->b2 = tb2;
      }
      else {
        move2->b1 = tb2;
        move2->b2 = tb1;
      }

      if (move1->w1 == -1) {
        free(move1);
        return move2;
      }
      else {
        move1->next = move2;
      }
    }
    return move1;
  }
  else {
    movelist *move1 = malloc(sizeof(movelist));
    move1->next = NULL;
    move1->w1 = -1;   // use w1 to signal whether ally2 should extend linkedlist or not
    if (b1 != 0) {
      int tw1 = w1;
      int tw2 = w2;
      int tb1 = b1;
      int tb2 = b2;
      int potential_move = tb1 - 1;
      bool w1jumped = false;
      bool w2jumped = false;

      while ((potential_move ==  tb2 && tb2 != 0) || potential_move == tw1 || potential_move == tw2) {
        if (potential_move == tw1) {
          w1jumped = true;
        }
        if (potential_move == tw2) {
          w2jumped = true;
        }
        potential_move--;
      }
      tb1 = potential_move;

      if (w1jumped) {
        int curr = 1;
        while (curr == tb1 || curr == tb2 || curr == tw2) {
          curr++;
        }
        tw1 = curr;
      }
      if (w2jumped) {
        int curr = 1;
        while (curr == tb1 || curr == tb2 || curr == tw1) {
          curr++;
        }
        tw2 = curr;
      }

      if (tw1 < tw2) {
        move1->w1 = tw1;
        move1->w2 = tw2;
      }
      else {
        move1->w1 = tw2;
        move1->w2 = tw1;
      }
      if (tb1 < tb2) {
        move1->b1 = tb1;
        move1->b2 = tb2;
      }
      else {
        move1->b1 = tb2;
        move1->b2 = tb1;
      }
    }
    if (b2 != 0) {
      movelist *move2 = malloc(sizeof(movelist));
      move2->next = NULL;
      int tw1 = w1;
      int tw2 = w2;
      int tb1 = b1;
      int tb2 = b2;
      int potential_move = tb2 - 1;
      bool w1jumped = false;
      bool w2jumped = false;

      while ((potential_move ==  tb1 && tb1 != 0) || potential_move == tw1 || potential_move == tw2) {
        if (potential_move == tw1) {
          w1jumped = true;
        }
        if (potential_move == tw2) {
          w2jumped = true;
        }
        potential_move--;
      }
      tb2 = potential_move;

      if (w1jumped) {
        int curr = 1;
        while (curr == tb1 || curr == tb2 || curr == tw2) {
          curr++;
        }
        tw1 = curr;
      }
      if (w2jumped) {
        int curr = 1;
        while (curr == tb1 || curr == tb2 || curr == tw1) {
          curr++;
        }
        tw2 = curr;
      }

      if (tw1 < tw2) {
        move2->w1 = tw1;
        move2->w2 = tw2;
      }
      else {
        move2->w1 = tw2;
        move2->w2 = tw1;
      }
      if (tb1 < tb2) {
        move2->b1 = tb1;
        move2->b2 = tb2;
      }
      else {
        move2->b1 = tb2;
        move2->b2 = tb1;
      }

      if (move1->w1 == -1) {
        free(move1);
        return move2;
      }
      else {
        move1->next = move2;
      }
    }
    return move1;
  }
}

int static_estimator(int board[], int player_turn) {
  static_counter++;
  int wincheck = compute_win(board);
  if (wincheck != 0) { return wincheck; }

  // Evaluate clustering bias
  int cluster_estimation = 0;
  for (int i = 0; i < BOARD_LEN; i++) {
    int piece = board[i];

    // |X|X|X| | | | | |
    if (1 <= piece && piece <= 3) {
        cluster_estimation -= 2;
    }

    // | |X|X|X| | | | |
    if (2 <= piece && piece <= 4) {
        cluster_estimation -= 1;
    }

    // | | | | |X|X|X| |
    if (5 <= piece && piece <= 7) {
        cluster_estimation += 1;
    }

    // | | | | | |X|X|X|
    if (6 <= piece && piece <= 8) {
        cluster_estimation += 2;
    }

    if (piece == 0) {
        cluster_estimation -= 3;
    }
    if (piece == 9) {
        cluster_estimation += 3;
    }
  }

  // Evaluate turn bias and enemy cluster bias
  int w1 = board[0];
  int w2 = board[1];
  int b1 = board[2];
  int b2 = board[3];
  int enemy_cluster_bias = 0;
  int turn_bias = 0;
  if (player_turn == WHITE_TURN) { 
    turn_bias = 2; 
    if (abs(b1-b2) == 1 && (b1 != 0 || b2 != 0)) {
      enemy_cluster_bias = 1;
    }
  }
  else {
    turn_bias = -2; 
    if (abs(w1-w2) == 1 && (w1 != 9 || w2 != 9)) {
      enemy_cluster_bias = -1;
    }
  }

  return cluster_estimation + turn_bias + enemy_cluster_bias;
}

int compute_win(int board[]) {
  int w1 = board[0];
  int w2 = board[1];
  int b1 = board[2];
  int b2 = board[3];

  if (w1 == 9 && w2 == 9) {
    return 100;
  }
  else if (b1 == 0 && b2 == 0) {
    return -100;
  }
  else {
    return 0;
  }
}

int main() {
  int board[] = {3, 6, 7, 8};
  int player_turn = WHITE_TURN;
  int value = minimax(board, 64, player_turn);

  printf("Starting Board: %d%d%d%d\n", board[0], board[1], board[2], board[3]);
  if (player_turn == WHITE_TURN) {
    printf("Player Turn: WHITE\n\n");
  }
  else {
    printf("Player Turn: BLACK\n\n");
  }
  printf("Best Move: %d%d%d%d\n", best_move[0], best_move[1], best_move[2], best_move[3]);
  printf("Value: %d\n", value);
  printf("Leaf nodes analyzed: %d", static_counter);
}
