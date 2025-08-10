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

// ASCII art
char game_start[] =
    R"(   ______       _       ____    ____  ________     ______   _________     _       _______   _________  )" "\n"
    R"( .' ___  |     / \     |_   \  /   _||_   __  |  .' ____ \ |  _   _  |   / \     |_   __ \ |  _   _  | )" "\n"
    R"(/ .'   \_|    / _ \      |   \/   |    | |_ \_|  | (___ \_||_/ | | \_|  / _ \      | |__) ||_/ | | \_| )" "\n"
    R"(| |   ____   / ___ \     | |\  /| |    |  _| _    _.____`.     | |     / ___ \     |  __ /     | |     )" "\n"
    R"(\ `.___]  |_/ /   \ \_  _| |_\/_| |_  _| |__/ |  | \____) |   _| |_  _/ /   \ \_  _| |  \ \_  _| |_    )" "\n"
    R"( `._____.'|____| |____||_____||_____||________|   \______.'  |_____||____| |____||____| |___||_____|   )" "\n";

// Function declarations
int static_estimator(int[], int);
int compute_win(int[]);
int minimax(int[], int, int);
int white_maxi(int[], int, int, int, int);
int black_mini(int[], int, int, int, int);
void compute_moves(int[], int, int[][BOARD_LEN]);
void single_move_helper(int, int, int, int, int, int[]);
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
  return 0;
}

int black_mini(int* board, int alpha, int beta, int depth, int player_turn) {
  return 0;
}

// helper that can caculate a single move for a1 regardless of color
// use player_turn as direction increment (+1 for white, -1 for black)
inline void single_move_helper(int a1, int a2, int e1, int e2, int player_turn, int ret_board[]) {
  int pos = a1 += player_turn;
  int goal = player_turn == WHITE_TURN ? 9 : 0;
  bool a2jump = false;
  bool e1jump = false;
  bool e2jump = false;

  // move a1
  while (pos == e1 || pos == e2 || (pos == a2 && pos != goal)) {
    if (pos == e1) { e1jump = true; }
    if (pos == e2) { e2jump = true; }
    if (pos == a2) { a2jump = true; }
    pos += player_turn;
  }
  a1 = pos;

  // reset any piece if able
  int reset_start = player_turn == WHITE_TURN ? 8 : 1;
  if (e1jump && !e2jump && !a2jump){
    while (reset_start == e2 || reset_start == a1 || reset_start == a2){
      reset_start -= player_turn;
    }
    e1 = reset_start;
  }
  if (e2jump && !e1jump && !a2jump){
    while (reset_start == e1 || reset_start == a1 || reset_start == a2){
      reset_start -= player_turn;
    }
    e2 = reset_start;
  }

  ret_board[0] = a1;
  ret_board[1] = a2;
  ret_board[2] = e1;
  ret_board[3] = e2;
}

void compute_moves(int board[], int player_turn, int ret_boards[][BOARD_LEN]) {
  int w1 = board[0];
  int w2 = board[1];
  int b1 = board[2];
  int b2 = board[3];

  // flag w1 pieces in both boards for moves that don't exist
  ret_boards[0][0] = -1;
  ret_boards[1][0] = -1;

  if (player_turn == WHITE_TURN) {
    if (w1 != 9){
      int tmp[BOARD_LEN];
      single_move_helper(w1, w2, b1, b2, player_turn, tmp);
      ret_boards[0][0] = tmp[0];
      ret_boards[0][1] = tmp[1];
      ret_boards[0][2] = tmp[2];
      ret_boards[0][3] = tmp[3];
    }
    if (w2 != 9){
      int tmp[BOARD_LEN];
      single_move_helper(w2, w1, b1, b2, player_turn, tmp);
      ret_boards[1][0] = tmp[1];
      ret_boards[1][1] = tmp[0];
      ret_boards[1][2] = tmp[2];
      ret_boards[1][3] = tmp[3];
    }
  }
  else {
    if (b1 != 0) {
      int tmp[BOARD_LEN];
      single_move_helper(b1, b2, w1, w2, player_turn, tmp);
      ret_boards[0][0] = tmp[2];
      ret_boards[0][1] = tmp[3];
      ret_boards[0][2] = tmp[0];
      ret_boards[0][3] = tmp[1];
    }
    if (b2 != 0) {
      int tmp[BOARD_LEN];
      single_move_helper(b2, b1, w1, w2, player_turn, tmp);
      ret_boards[1][0] = tmp[3];
      ret_boards[1][1] = tmp[2];
      ret_boards[1][2] = tmp[0];
      ret_boards[1][3] = tmp[1];
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

  printf("\nCurrent Board\n");
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
    printf("%s", game_start);
    int board[] = {1, 2, 7, 8};
    print_board(board);
    if (player_turn == WHITE_TURN) {
      while (true) {
        handle_playerturn(board);

        // COMPUTER MOVE
        printf("\nCOMPUTER MOVE\n\n");
        int comp_boards[2][BOARD_LEN];
        compute_moves(board, BLACK_TURN, comp_boards);
        if (comp_boards[0][0] != -1) {
          printf("Move 1:");
          print_board(comp_boards[0]);
        }
        if (comp_boards[1][0] != -1) {
          printf("Move 2:");
          print_board(comp_boards[1]);
        }
        break;
      }
    }
    else {
      // FOR TESTING, REORDER AFTER DONE
      while (true) {
        handle_playerturn(board);

        // COMPUTER MOVE
        printf("\nCOMPUTER MOVE\n\n");
        int comp_boards[2][BOARD_LEN];
        compute_moves(board, WHITE_TURN, comp_boards);
        if (comp_boards[0][0] != -1) {
          printf("Move 1:");
          print_board(comp_boards[0]);
        }
        if (comp_boards[1][0] != -1) {
          printf("Move 2:");
          print_board(comp_boards[1]);
        }
        break;
      }
    }
    break;
  }
}
