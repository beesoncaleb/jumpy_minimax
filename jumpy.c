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
char jumpy[] =
  R"(    _____                                      )" "\n"
  R"(   |_   _|                                     )" "\n"
  R"(     | | __   _   _ .--..--.  _ .--.   _   __  )" "\n"
  R"( _   | |[  | | | [ `.-. .-. |[ '/'`\ \[ \ [  ] )" "\n"
  R"(| |__' | | \_/ |, | | | | | | | \__/ | \ '/ /  )" "\n"
  R"(`.____.' '.__.'_/[___||__||__]| ;.__/[\_:  /   )" "\n"
  R"(                             [__|     \__.'    )" "\n";

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
int white_maxi(int[], int, int, int);
int black_mini(int[], int, int, int);
void compute_moves(int[], int, int[][BOARD_LEN]);
void single_move_helper(int, int, int, int, int, int[]);
void print_board(int*);
void handle_playerturn(int*);

int minimax(int* board, int depth, int player_turn) {
  static_counter = 0;
  if (depth == 0) {
    return static_estimator(board, player_turn);
  }

  int wincheck = compute_win(board);
  if (wincheck != 0){
    return wincheck;
  }

  if (player_turn == WHITE_TURN) {
    int computed_moves[2][BOARD_LEN];
    compute_moves(board, player_turn, computed_moves);
    int value = NEG_INF;
    for (int i=0; i<2; i++) {
      if (computed_moves[i][0] == -1) { continue; }
      
      int curr = black_mini(computed_moves[i], NEG_INF, POS_INF, depth-1);
      if (curr > value) {
        value = curr;
        computer_board[0] = computed_moves[i][0];
        computer_board[1] = computed_moves[i][1];
        computer_board[2] = computed_moves[i][2];
        computer_board[3] = computed_moves[i][3];
      }
    }
    return value;
  }
  else {
    int computed_moves[2][BOARD_LEN];
    compute_moves(board, player_turn, computed_moves);
    int value = POS_INF;
    for (int i=0; i<2; i++) {
      if (computed_moves[i][0] == -1) { continue; }
      
      int curr = white_maxi(computed_moves[i], NEG_INF, POS_INF, depth-1);
      if (curr < value) {
        value = curr;
        computer_board[0] = computed_moves[i][0];
        computer_board[1] = computed_moves[i][1];
        computer_board[2] = computed_moves[i][2];
        computer_board[3] = computed_moves[i][3];
      }
    }
    return value;
  }
}

int white_maxi(int board[], int alpha, int beta, int depth) {
  if (depth == 0) {
    return static_estimator(board, WHITE_TURN);
  }

  int wincheck = compute_win(board);
  if (wincheck != 0){
    return wincheck;
  }

  int computed_moves[2][BOARD_LEN];
  compute_moves(board, WHITE_TURN, computed_moves);
  int value = NEG_INF;
  for (int i=0; i<2; i++) {
    if (computed_moves[i][0] == -1) { continue; }

    int curr = black_mini(computed_moves[i], alpha, beta, depth-1);
    value = curr > value ? curr : value;

    if (value >= beta) {
      break;
    }
    alpha = value > alpha ? value : alpha;
  }
  return value;
}

int black_mini(int board[], int alpha, int beta, int depth) {
  if (depth == 0) {
    return static_estimator(board, BLACK_TURN);
  }

  int wincheck = compute_win(board);
  if (wincheck != 0){
    return wincheck;
  }

  int computed_moves[2][BOARD_LEN];
  compute_moves(board, BLACK_TURN, computed_moves);
  int value = POS_INF;
  for (int i=0; i<2; i++) {
    if (computed_moves[i][0] == -1) { continue; }

    int curr = white_maxi(computed_moves[i], alpha, beta, depth-1);
    value = curr < value ? curr : value;

    if (value <= alpha) {
      break;
    }
    beta = value < beta ? value : beta;
  }
  return value;
}

// helper that can caculate a single move for a1 regardless of turn
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
      ret_boards[1][0] = tmp[2];
      ret_boards[1][1] = tmp[3];
      ret_boards[1][2] = tmp[1];
      ret_boards[1][3] = tmp[0];
    }
  }
}

int static_estimator(int board[], int player_turn) {
  static_counter++;

  int w1 = board[0];
  int w2 = board[1];
  int b1 = board[2];
  int b2 = board[3];

  int wincheck = compute_win(board);
  if (wincheck != 0) {
    return wincheck; 
  }
  else {
    int cluster_bias = 0;
    if (player_turn == WHITE_TURN && ((b1 - b2) == 1 || (b1 - b2) == -1) && (b1 != 0 || b2 != 0)) {
      cluster_bias = 1;
    }
    if (player_turn == BLACK_TURN && ((w1 - w2) == 1 || (w1 - w2) == -1) && (w1 != 9 || w2 != 9)) {
      cluster_bias = -1;
    }
    return cluster_bias + w1 + w2 + b1 + b2 - 18;
  }
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

  printf("| |1|2|3|4|5|6|7|8| |\n");

  char pieces[] = "                   ";   
  pieces[w1*2+1] = 'w';
  pieces[w2*2+1] = 'w';
  pieces[b1*2+1] = 'b';
  pieces[b2*2+1] = 'b';
  printf("%s\n", pieces);
  printf("%d%d%d%d\n", w1, w2, b1, b2);
}

// come back and add error handling
inline void handle_playerturn(int* board_ret) {
  printf("\033[34mPLAYER MOVE\033[0m\n");
  char board_str[6];
  printf("Player enter a board position: ");
  fgets(board_str, 6, stdin);

  board_ret[0] = board_str[0]-48;
  board_ret[1] = board_str[1]-48;
  board_ret[2] = board_str[2]-48;
  board_ret[3] = board_str[3]-48;
  print_board(board_ret);
}

int main() {
  printf("\033[2J\033[H");
  printf("%s\n\n", jumpy);
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
    printf("\n\nStarting Board\n");
    print_board(board);
    printf("-----------------------------------------------------------\n");
    if (player_turn == WHITE_TURN) {
      while (true) {
        handle_playerturn(board);

        //comp move
        printf("\n\033[31mCOMPUTER MOVE\033[0m\n");
        int value = minimax(board, 60, BLACK_TURN);
        for (int i=0; i<BOARD_LEN; i++) { board[i] = computer_board[i]; } // computer_board is global and stores current comp move
        print_board(board);
        printf("\nEstimated Value: %d\n", value);
        printf("Static estimates: %d\n", static_counter);
        printf("-----------------------------------------------------------\n");

        if (value == -100) {
          printf("\nCOMPUTER WINS");
          break;
        }
      }
    }
    else {
      while (true) {
        //comp move
        printf("\033[31mCOMPUTER MOVE\033[0m\n");
        int value = minimax(board, 60, WHITE_TURN);
        for (int i=0; i<BOARD_LEN; i++) { board[i] = computer_board[i]; } // computer_board is global and stores current comp move
        print_board(board);
        printf("\nEstimated Value: %d\n", value);
        printf("Static estimates: %d\n\n", static_counter);
        if (value == 100) {
          printf("-----------------------------------------------------------\n");
          printf("\nCOMPUTER WINS");
          break;
        }

        handle_playerturn(board);
        printf("-----------------------------------------------------------\n");

        if (value == -100) {
          printf("\nCOMPUTER WINS");
          break;
        }
      }
    }
    break;
  }
}
