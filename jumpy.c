#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Global variables and definitions
#define BOARD_LEN 4
#define WHITE_TURN 1
#define BLACK_TURN -1
#define POS_INF 999999
#define NEG_INF -999999
int static_counter = 0;
int computer_board[BOARD_LEN];

// Function declarations
int static_estimator(int[], int);
int compute_win(int[]);
int minimax(int[], int, int);
void white_maxi(int[], int, int, int, int);
void black_mini(int[], int, int, int, int);
void compute_moves(int[], int, int[][BOARD_LEN]) {
void print_board(int*);
int handle_playerturn(int*);

int minimax(int* board, int depth, int player_turn) {
  if (depth == 0) {
    return static_estimator(board, player_turn);
  }

  int wincheck = compute_win(board);
  if (wincheck != 0){
    return wincheck;
  }

  int alpha = NEG_INF;
  int beta = POS_INF;
  if (player_turn == WHITE_TURN) {
    int computed_moves[2][BOARD_LEN];
    compute_moves(board, player_turn, computed_moves);
    for (int i=0; i<2; i++) {
      if (computed_moves[i][0] == -1) { continue; }

      
    }
  }
  else {
  }
}

int white_maxi(int* board, int alpha, int beta, int depth, int player_turn) {
  return;
}

int black_mini(int* board, int alpha, int beta, int depth, int player_turn) {
  return;
}

// REFACTOR WITH HELPER LATER
void compute_moves(int* board, int player_turn, int return_board[][BOARD_LEN]) {
  int w1 = board[0];
  int w2 = board[1];
  int b1 = board[2];
  int b2 = board[3];

  // mark w1 in both boards in case no new computed move available
  return_board[0][0] = -1;
  return_board[1][0] = -1;

  if (player_turn == WHITE_TURN) {
    if (w1 != 9) {
      int temp_w1 = w1;
      int temp_w2 = w2;
      int temp_b1 = b1;
      int temp_b2 = b2;

      int pos = temp_w1 + 1;
      bool b1jump = false;
      bool b2jump = false;

      while ((pos == temp_w2 && temp_w2 != 9) || pos == temp_b1 || pos == temp_b2) {
        if (pos == temp_b1) {
          b1jump = true;
        }
        if (pos == temp_b2) {
          b2jump = true;
        }
        pos++;
      }
      temp_w1 = pos;

      if (b2jump) {
        int start = 8;
        while (start == temp_w1 || start == temp_w2 || start == temp_b1){
          start--;
        }
        temp_b2 = start;
      }
      if (b1jump) {
        int start = 8;
        while (start == temp_w1 || start == temp_w2 || start == temp_b2){
          start--;
        }
        temp_b1 = start;
      }

      // return board in a sorted order
      if (temp_w1 < temp_w2) {
        return_board[0][0] = temp_w1;
        return_board[0][1] = temp_w2;
      }
      else {
        return_board[0][0] = temp_w2;
        return_board[0][1] = temp_w1;
      }
      if (temp_b1 < temp_b2) {
        return_board[0][2] = temp_b1;
        return_board[0][3] = temp_b2;
      }
      else {
        return_board[0][2] = temp_b2;
        return_board[0][3] = temp_b1;
      }
    }
    
    if (w2 != 9) {
      int temp_w1 = w1;
      int temp_w2 = w2;
      int temp_b1 = b1;
      int temp_b2 = b2;

      int pos = temp_w2 + 1;
      bool b1jump = false;
      bool b2jump = false;

      while ((pos == temp_w1 && temp_w1 != 9) || pos == temp_b1 || pos == temp_b2) {
        if (pos == temp_b1) {
          b1jump = true;
        }
        if (pos == temp_b2) {
          b2jump = true;
        }
        pos++;
      }
      temp_w2 = pos;

      if (b2jump) {
        int start = 8;
        while (start == temp_w1 || start == temp_w2 || start == temp_b1){
          start--;
        }
        temp_b2 = start;
      }
      if (b1jump) {
        int start = 8;
        while (start == temp_w1 || start == temp_w2 || start == temp_b2){
          start--;
        }
        temp_b1 = start;
      }

      // return board in a sorted order
      if (temp_w1 < temp_w2) {
        return_board[1][0] = temp_w1;
        return_board[1][1] = temp_w2;
      }
      else {
        return_board[1][0] = temp_w2;
        return_board[1][1] = temp_w1;
      }
      if (temp_b1 < temp_b2) {
        return_board[1][2] = temp_b1;
        return_board[1][3] = temp_b2;
      }
      else {
        return_board[1][2] = temp_b2;
        return_board[1][3] = temp_b1;
      }
    }
  }

  else {
    if (b1 != 0) {
      int temp_w1 = w1;
      int temp_w2 = w2;
      int temp_b1 = b1;
      int temp_b2 = b2;

      int pos = temp_b1 - 1;
      bool w1jump = false;
      bool w2jump = false;

      while ((pos == temp_b2 && temp_b2 != 0) || pos == temp_w1 || pos == temp_w2) {
        if (pos == temp_w1) {
          w1jump = true;
        }
        if (pos == temp_w2) {
          w2jump = true;
        }
        pos--;
      }
      temp_b1 = pos;

      if (w1jump) {
        int start = 1;
        while (start == temp_b1 || start == temp_b2 || start == temp_w2){
          start++;
        }
        temp_w1 = start;
      }
      if (w2jump) {
        int start = 1;
        while (start == temp_b1 || start == temp_b2 || start == temp_w1){
          start++;
        }
        temp_w2 = start;
      }

      // return board in a sorted order
      if (temp_w1 < temp_w2) {
        return_board[0][0] = temp_w1;
        return_board[0][1] = temp_w2;
      }
      else {
        return_board[0][0] = temp_w2;
        return_board[0][1] = temp_w1;
      }
      if (temp_b1 < temp_b2) {
        return_board[0][2] = temp_b1;
        return_board[0][3] = temp_b2;
      }
      else {
        return_board[0][2] = temp_b2;
        return_board[0][3] = temp_b1;
      }
    }
    if (b2 != 0) {
      int temp_w1 = w1;
      int temp_w2 = w2;
      int temp_b1 = b1;
      int temp_b2 = b2;

      int pos = temp_b2 - 1;
      bool w1jump = false;
      bool w2jump = false;

      while ((pos == temp_b1 && temp_b1 != 0) || pos == temp_w1 || pos == temp_w2) {
        if (pos == temp_w1) {
          w1jump = true;
        }
        if (pos == temp_w2) {
          w2jump = true;
        }
        pos--;
      }
      temp_b2 = pos;

      if (w1jump) {
        int start = 1;
        while (start == temp_b1 || start == temp_b2 || start == temp_w2){
          start++;
        }
        temp_w1 = start;
      }
      if (w2jump) {
        int start = 1;
        while (start == temp_b1 || start == temp_b2 || start == temp_w1){
          start++;
        }
        temp_w2 = start;
      }

      // return board in a sorted order
      if (temp_w1 < temp_w2) {
        return_board[1][0] = temp_w1;
        return_board[1][1] = temp_w2;
      }
      else {
        return_board[1][0] = temp_w2;
        return_board[1][1] = temp_w1;
      }
      if (temp_b1 < temp_b2) {
        return_board[1][2] = temp_b1;
        return_board[1][3] = temp_b2;
      }
      else {
        return_board[1][2] = temp_b2;
        return_board[1][3] = temp_b1;
      }
    }
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

void print_board(int* board) {
  int w1 = board[0];
  int w2 = board[1];
  int b1 = board[2];
  int b2 = board[3];

  printf("\n   Current Board\n");
  printf(" |1|2|3|4|5|6|7|8| \n");
  char pieces[] = "                   ";   

  pieces[w1*2] = 'w';
  pieces[w2*2] = 'w';
  pieces[b1*2] = 'b';
  pieces[b2*2] = 'b';
  printf("%s\n", pieces);
}

// come back and add error handling
inline int handle_playerturn(int* board_ret) {
  printf("\nPLAYER_MOVE");
  char board_str[6];
  printf("\nPlayer enter a board position: ");
  fgets(board_str, 6, stdin);

  board_ret[0] = board_str[0]-48;
  board_ret[1] = board_str[1]-48;
  board_ret[2] = board_str[2]-48;
  board_ret[3] = board_str[3]-48;
  print_board(board_ret);
  return 0;
}

int main() {
  printf("\033[2J\033[H");
  printf("Welcome to Jumpy\n\n");
  printf("Which side will you play as? (W/B) ");
  while (true) {
    // get player turn
    char turn_selection[3];
    int player_turn;
    fgets(turn_selection, 3, stdin);
    while (true) {
      if (turn_selection[0] == 'W' || turn_selection[0] == 'w') {
        player_turn = WHITE_TURN;
        break;
      }
      else if (turn_selection[0] == 'B' || turn_selection[0] == 'b') {
        player_turn = BLACK_TURN;
        break;
      }
      else {
        printf("Must choose black (B/b) or white (W/w) ");
        fgets(turn_selection, 3, stdin);
      }
    }

    printf("\033[2J\033[H");
    printf("GAME START\n");
    int board[] = {1, 2, 7, 8};
    print_board(board);
    if (player_turn == WHITE_TURN) {
      while (true) {
        handle_playerturn(board);

        // COMPUTER MOVE
        printf("\nCOMPUTER MOVE\n");
        int comp_board[BOARD_LEN];
        break;
      }
    }
    else {
      while (true) {
        // COMPUTER MOVE
        printf("\nCOMPUTER MOVE\n");
        break;

        handle_playerturn(board);
      }
    }
    break;
  }
}
