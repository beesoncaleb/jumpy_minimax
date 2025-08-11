# int static_estimator(int board[], int player_turn) {
#   static_counter++;
# 
#   int w1 = board[0];
#   int w2 = board[1];
#   int b1 = board[2];
#   int b2 = board[3];
# 
#   int wincheck = compute_win(board);
#   if (wincheck != 0) {
#     return wincheck; 
#   }
#   else {
#     int cluster_bias = 0;
#     if (player_turn == WHITE_TURN && ((b1 - b2) == 1 || (b1 - b2) == -1) && (b1 != 0 || b2 != 0)) {
#       cluster_bias = 1;
#     }
#     if (player_turn == BLACK_TURN && ((w1 - w2) == 1 || (w1 - w2) == -1) && (w1 != 9 || w2 != 9)) {
#       cluster_bias = -1;
#     }
#     return cluster_bias + w1 + w2 + b1 + b2 - 18;
#   }
# }

def compute_win(board):
    w1, w2, b1, b2 = board
    
    if w1 == 9 and w2 == 9:
        return 100
    elif b1 == 0 and b2 == 0:
        return -100
    else:
        return 0


def static_estimator(board, white_turn):
    w1, w2, b1, b2 = board

    win = compute_win(board)
    if win != 0:
        return win
    else:
        cluster_bias = 0
        if white_turn and abs(b1-b2) == 1 and (b1 != 0 or b2 != 0):
            cluster_bias = 2
        if not white_turn and abs(w1-w2) == 1 and (w1 != 9 or w2 != 9):
            cluster_bias = -2

        return cluster_bias + w1 + w2 + b1 + b2 - 18


if __name__ == "__main__":
    with open("white_static_value.txt", "w") as white, open("black_static_value.txt", "w") as black:
        for x in range(0, 10000):
            w1 = x // 1000
            w2 = (x // 100) % 10
            b1 = (x // 10) % 10
            b2 = x % 10

            white_value = static_estimator([w1, w2, b1, b2], True)
            black_value = static_estimator([w1, w2, b1, b2], False)

            white.write(f"{white_value}\n")
            black.write(f"{black_value}\n")

    with open("white_static_value.txt", "r") as white, open("black_static_value.txt", "r") as black:
        for x in range(0, 10000):
            w1 = x // 1000
            w2 = (x // 100) % 10
            b1 = (x // 10) % 10
            b2 = x % 10

            expected_white_value = static_estimator([w1, w2, b1, b2], True)
            expected_black_value = static_estimator([w1, w2, b1, b2], False)

            white_value = int(white.readline().strip())
            black_value = int(black.readline().strip())

            if expected_white_value != white_value:
                print(f"white value: {white_value}\texpected value: {expected_white_value}")
                exit()

            if expected_black_value != black_value:
                print(f"white value: {black_value}\texpected value: {expected_black_value}")
                exit()
