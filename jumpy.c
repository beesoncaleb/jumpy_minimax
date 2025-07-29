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

// Struct so that board state can be passed up in recursion
typedef struct {
  int val;
  int *board;
  int *best_choice;
} Val_Board;

// Function declarations
int static_estimator(int[], int);
int compute_win(int[]);
Val_Board minimax(int[], int, int, int, int, int);
void compute_move(int, int, int, int, int, int*);


// White is a maximizer, black is a minimizer
Val_Board minimax(int *board, int alpha, int beta, int player_turn, int depth, int max_depth) {
  if (depth == max_depth) {
    return (Val_Board){static_estimator(board, player_turn), board, 0};
  }

  int checkwin = compute_win(board);
  if (checkwin != 0) {
    return (Val_Board){checkwin, board, 0};
  }

  if (player_turn == WHITE_TURN) {
    Val_Board result = {NEG_INF, board, 0};
    if (board[0] != 9) {
      int possible_board[BOARD_LEN];
      compute_move(board[0], board[1], board[2], board[3], WHITE_TURN, possible_board);
      Val_Board compare = minimax(possible_board, alpha, beta, BLACK_TURN, depth+1, max_depth);

      if (compare.val > result.val) {
        result.val = compare.val;
        result.best_choice = compare.board;
      }

      if (result.val >= beta) {
        return result;
      }
      alpha = (result.val > alpha) ? result.val : alpha;
    }

    if (board[1] != 9) {
      int possible_board[BOARD_LEN];
      compute_move(board[1], board[0], board[2], board[3], WHITE_TURN, possible_board);
      Val_Board compare = minimax(possible_board, alpha, beta, BLACK_TURN, depth+1, max_depth);

      if (compare.val > result.val) {
        result.val = compare.val;
        result.best_choice = compare.board;
      }

      if (result.val >= beta) {
        return result;
      }
      alpha = (result.val > alpha) ? result.val : alpha;
    }

    return result;
  }
  else if (player_turn == BLACK_TURN) {
    Val_Board result = {POS_INF, board, 0};
    if (board[2] != 0) {
      int possible_board[BOARD_LEN];
      compute_move(board[2], board[3], board[0], board[1], BLACK_TURN, possible_board);
      Val_Board compare = minimax(possible_board, alpha, beta, WHITE_TURN, depth+1, max_depth);

      if (compare.val < result.val) {
        result.val = compare.val;
        result.best_choice = compare.board;
      }

      if (result.val <= alpha) {
        return result;
      }
      beta = (result.val < beta) ? result.val : beta;
    }

    if (board[3] != 0) {
      int possible_board[BOARD_LEN];
      compute_move(board[3], board[2], board[0], board[1], BLACK_TURN, possible_board);
      Val_Board compare = minimax(possible_board, alpha, beta, WHITE_TURN, depth+1, max_depth);

      if (compare.val < result.val) {
        result.val = compare.val;
        result.best_choice = compare.board;
      }

      if (result.val <= alpha) {
        return result;
      }
      beta = (result.val < beta) ? result.val : beta;
    }

    return result;
  }
  else {
    return (Val_Board){0};
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

void compute_move(int dynamic_plr, int static_plr, int enemy_plr1, int enemy_plr2, int player_turn, int *return_board) {
  int potential_move = dynamic_plr + player_turn;   // player_turn used as direction of movement
  bool e1jumped = false;
  bool e2jumped = false;

  while (static_plr == potential_move || enemy_plr1 == potential_move || enemy_plr2 == potential_move) {
    if (enemy_plr1 == potential_move) {
      e1jumped = true;
    }
    else if (enemy_plr2 == potential_move) {
      e2jumped = true;
    }
    else if (static_plr == potential_move && (static_plr == 0 || static_plr == 9)) {
      break;
    }

    potential_move += player_turn;
  }
  dynamic_plr = potential_move;

  int reset_start;
  if (player_turn == WHITE_TURN) {
    reset_start = 8;
  }
  else {
    reset_start = 0;
  }

  if (e1jumped) {
    while (dynamic_plr == reset_start || static_plr == reset_start || enemy_plr2 == reset_start) {
      reset_start -= player_turn;
    }
    enemy_plr1 = reset_start;
  }

  if (e2jumped) {
    while (dynamic_plr == reset_start || static_plr == reset_start || enemy_plr1 == reset_start) {
      reset_start -= player_turn;
    }
    enemy_plr2 = reset_start;
  }

  if (player_turn == WHITE_TURN) {
    if (dynamic_plr < static_plr) {
      return_board[0] = dynamic_plr;
      return_board[1] = static_plr;
    }
    else {
      return_board[0] = static_plr;
      return_board[1] = dynamic_plr;
    }
    if (enemy_plr1 < enemy_plr2) {
      return_board[2] = enemy_plr1;
      return_board[3] = enemy_plr2;
    }
    else {
      return_board[2] = enemy_plr2;
      return_board[3] = enemy_plr1;
    }
  }
  else {
    if (enemy_plr1 < enemy_plr2) {
      return_board[0] = enemy_plr1;
      return_board[1] = enemy_plr2;
    }
    else {
      return_board[0] = enemy_plr2;
      return_board[1] = enemy_plr1;
    }
    if (dynamic_plr < static_plr) {
      return_board[2] = dynamic_plr;
      return_board[3] = static_plr;
    }
    else {
      return_board[2] = static_plr;
      return_board[3] = dynamic_plr;
    }
  }
}

int main() {
  int board[] = {3, 5, 6, 7};
  Val_Board move = minimax(board, NEG_INF, POS_INF, WHITE_TURN, 0, 16);

  printf("Starting Position: %d%d%d%d\n\n", board[0], board[1], board[2], board[3]);
  printf("Chosen Move: %d%d%d%d\n", move.best_choice[0], move.best_choice[1], move.best_choice[2], move.best_choice[3]);
  printf("Value: %d\n", move.val);
  printf("Moves Consider: %d\n", static_counter);
}
