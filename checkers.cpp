#include <algorithm>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <time.h>
#include "checkers.h"
using namespace std;

void printMove(Move m);

Board::Board() {
    setUpBoard();
}

Board::Board(PieceType** b) {
    numBlack = 0;
    numWhite = 0;
    numBlackKings = 0;
    numWhiteKings = 0;
    board = b;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            PieceType piece = getPiece(i, j);
            if (piece == BLACK || piece == BLACK_KING) {
                numBlack++;
                if (piece == BLACK_KING) {
                    numBlackKings++;
                }
            } else if (piece == WHITE || piece == WHITE_KING) {
                numWhite++;
                if (piece == WHITE_KING) {
                    numWhiteKings++;
                }
            }
        }
    }
}

PieceType Board::getPiece(int i, int j) {
    return board[i][j];
}

void Board::setUpBoard() {
    numBlack = 12;
    numWhite = 12;
    numBlackKings = 0;
    numWhiteKings = 0;
    board = new PieceType*[8];
    for (int i = 0; i < 8; i++) {
        board[i] = new PieceType[8];
    }
    for (int i = 0; i < 8; i++) {
        int first = 0;
        if (i % 2 == 0) {
            first = 1;
        }
        PieceType piece = EMPTY;
        if (i <= 2) {
            piece = WHITE;
        } else if (i >= 5) {
            piece = BLACK;
        }
        for (int j = first; j < 8; j += 2) {
            board[i][j] = piece;
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!(board[i][j] == BLACK || board[i][j] == WHITE) || (i > 2 && i < 5)) {
                board[i][j] = EMPTY;
            }
        }
    }

    // FOR TESTING PURPOSES
    // board[4][1] = WHITE;
}

int Board::getNumBlack() {
    return numBlack;
}

int Board::getNumWhite() {
    return numWhite;
}

int Board::getNumBlackKings() {
    return numBlackKings;
}

int Board::getNumWhiteKings() {
    return numWhiteKings;
}

void Board::genAllMoves(Side side, vector<Move>& moves) {
    PieceType normal = side == BLACK_SIDE ? BLACK : WHITE;
    PieceType king = side == BLACK_SIDE ? BLACK_KING : WHITE_KING;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            PieceType piece = getPiece(i, j);
            if (piece == normal || piece == king) {
                // genValidMoves(i, j, side, moves);
                genValidJumpMoves(i, j, side, moves);
            }
        }
    }
    if (moves.empty()) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                PieceType piece = getPiece(i, j);
                if (piece == normal || piece == king) {
                    // genValidMoves(i, j, side, moves);
                    genValidMoves(i, j, side, moves);
                }
            }
        }
    }
}

void Board::genValidMoves(int r, int c, Side side, vector<Move> &moves) {
    PieceType piece = board[r][c];
    if (piece == EMPTY) {
        cout << "EMPTY PIECE\n";
        throw Failure();
    }
    // vector<Move> jump_moves;
    // genValidJumpMoves(r, c, side, jump_moves);
    // if (!jump_moves.empty()) {
    //     moves = jump_moves;
    //     return;
    // }
    if (piece == WHITE || piece == BLACK) {
        int rowChange = piece == BLACK ? -1 : 1;
        int newRow = r + rowChange;
        if (newRow >= 0 && newRow < 8) {
            int newCol = c + 1;
            if (newCol < 8 && getPiece(newRow, newCol) == EMPTY) {
                moves.push_back({r, c, newRow, newCol});
            }
            newCol = c - 1;
            if (newCol >= 0 && getPiece(newRow, newCol) == EMPTY) {
                moves.push_back({r, c, newRow, newCol});
            }
        }
    } else {
        int newRow = r + 1;
        if (newRow < 8) {
            int newCol = c + 1;
            if (newCol < 8 && getPiece(newRow, newCol) == EMPTY) {
                moves.push_back({r, c, newRow, newCol});
            }
            newCol = c - 1;
            if (newCol >= 0 && getPiece(newRow, newCol) == EMPTY) {
                moves.push_back({r, c, newRow, newCol});
            }
        }
        newRow = r - 1;
        if (newRow >= 0) {
            int newCol = c + 1;
            if (newCol < 8 && getPiece(newRow, newCol) == EMPTY) {
                moves.push_back({r, c, newRow, newCol});
            }
            newCol = c - 1;
            if (newCol >= 0 && getPiece(newRow, newCol) == EMPTY) {
                moves.push_back({r, c, newRow, newCol});
            }
        }
    }
}

void Board::genValidJumpMoves(int r, int c, Side side, vector<Move>& moves) {
    Point start = {r, c};
    vector<Point> possible_moves;
    if (side == WHITE_SIDE && getPiece(r, c) == WHITE) {
        possible_moves.push_back({r + 2, c + 2});
        possible_moves.push_back({r + 2, c - 2});
    } else if (side == BLACK_SIDE && getPiece(r, c) == BLACK) {
        possible_moves.push_back({r - 2, c + 2});
        possible_moves.push_back({r - 2, c - 2});
    } else if (getPiece(r, c) == BLACK_KING || getPiece(r, c) == WHITE_KING) {
        possible_moves.push_back({r + 2, c + 2});
        possible_moves.push_back({r + 2, c - 2});
        possible_moves.push_back({r - 2, c + 2});
        possible_moves.push_back({r - 2, c - 2});
    }
    int n = possible_moves.size();
    for (int i = 0; i < n; i++) {
        Point temp = possible_moves[i];
        Move m = {start, temp};
        Point mid = getMidSquare(m);
        if (temp.row >= 0 && temp.row < 8 && temp.col >= 0 && temp.col < 8 && getPiece(temp.row, temp.col) == EMPTY && isOpponentPiece(side, getPiece(mid))) {
            moves.push_back(m);
        }
    }
}

PieceType Board::getPiece(Point p) {
    return board[p.row][p.col];
}

Point Board::getMidSquare(Move m) {
    Point middle = {(m.start.row + m.end.row) / 2, (m.start.col + m.end.col) / 2};
    return middle;
}

bool Board::isOpponentPiece(Side side, PieceType piece) {
    if (side == BLACK_SIDE) {
        return piece == WHITE || piece == WHITE_KING;
    }
    return piece == BLACK || piece == BLACK_KING;
}

bool Board::isOwnPiece(int r, int c, Side side) {
    PieceType piece = getPiece(r, c);
    if (side == BLACK_SIDE) {
        return piece == BLACK || piece == BLACK_KING;
    }
    return piece == WHITE || piece == WHITE_KING;
}

Status Board::makeMove(Move move, Side side) {
    // TODO: CHECK FOR GAME OVER
    if (move.start.row == -1) {
        return GAME_OVER;
    }
    Point start = move.start;
    int startRow = start.row;
    int startCol = start.col;
    Point end = move.end;
    int endRow = end.row;
    int endCol = end.col;
    PieceType current_piece = getPiece(startRow, startCol);
    if (!isOwnPiece(startRow, startCol, side) || getPiece(startRow, startCol) == EMPTY) {
        return FAILED_INVALID_PIECE;
    }
    vector<Move> possible_moves;
    // genValidJumpMoves(startRow, startCol, side, possible_moves);
    // if (possible_moves.empty()) {
    //     genValidMoves(startRow, startCol, side, possible_moves);
    // }
    genAllMoves(side, possible_moves);
    bool move_in_list = false;
    int n = possible_moves.size();
    for (int i = 0; i < n && !move_in_list; i++) {
        move_in_list = movesEqual(possible_moves[i], move);
    }
    if (move_in_list) {
        bool jump = false;
        if (startRow + 1 == endRow || startRow - 1 == endRow) {
            board[startRow][startCol] = EMPTY;
            board[endRow][endCol] = current_piece;
        } else {
            jump = true;
            board[startRow][startCol] = EMPTY;
            board[endRow][endCol] = current_piece;
            Point middle_square = getMidSquare(move);
            PieceType middle_piece = getPiece(middle_square);
            if (middle_piece == BLACK) {
                numBlack--;
            } else if (middle_piece == WHITE) {
                numWhite--;
            } else if (middle_piece == BLACK_KING) {
                numBlack--;
                numBlackKings--;
            } else if (middle_piece == WHITE_KING) {
                numWhite--;
                numWhiteKings--;
            }
            board[middle_square.row][middle_square.col] = EMPTY;
        }
        if (endRow == 0 && side == BLACK_SIDE) {
            board[endRow][endCol] = BLACK_KING;
            numBlackKings++;
        } else if (endRow == 7 && side == WHITE_SIDE) {
            board[endRow][endCol] = WHITE_KING;
            numWhiteKings++;
        }
        return COMPLETED;
    } else {
        return FAILED_INVALID_DEST;
    }
}

bool Board::movesEqual(Move one, Move two) {
    return one.start.row == two.start.row && one.start.col == two.start.col && one.end.row == two.end.row && one.end.col == two.end.col;
}

Board* Board::clone() {
    PieceType** new_board = new PieceType*[8];
    for (int i = 0; i < 8; i++) {
        new_board[i] = new PieceType[8];
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            new_board[i][j] = board[i][j];
        }
    }
    Board* b = new Board(new_board);
    return b;
}

void Board::drawBoard()
{
    cout << "    ";
    for (int i = 0; i < 8; i++) {
        cout << "0" << i << " ";
    }
    cout << "\n";
    for (int i = 0; i <= 16; i++) {
        if (i % 2 == 0) {
            cout << "    ";
            for (int j = 0; j < 23; j++) {
                cout << "-";
            }
            cout << " ";
        } else {
            cout << "0" << i/2 << " ";
            for (int j = 0; j < 8; j++) {
                cout << "|";
                PieceType p = board[i/2][j];
                if (p == BLACK) {
                    cout << "BP";
                } else if (p == BLACK_KING) {
                    cout << "BK";    
                } else if (p == WHITE) {
                    cout << "WP";
                } else if (p == WHITE_KING) {
                    cout << "WK";
                } else if (p == EMPTY) {
                    cout << "  ";
                }
            }
            cout << "| 0" << i/2;
        }
        cout << "\n";
    }
    cout << "    ";
    for (int i = 0; i < 8; i++) {
        cout << "0" << i << " ";
    }
    cout << "\n\nBP = Black Piece / BK = Black King / WP = White Piece / WK = White King\n\n\n";
}

Player::Player(string n, Side s) {
    name = n;
    side = s;
}

Status Player::makeMove(Move move, Board board)
{
    return board.makeMove(move, side);
}

Side Player::getSide()
{
    return side;
}

Status AI::makeMove(Board board)
{
    Move move = minimaxStart(board, getSide(), true);
    cout << "AI made move: ";
    printMove(move);
    cout << "\n";
    Status move_status = board.makeMove(move, getSide());
    return move_status;
}

Move AI::minimaxStart(Board board, Side s, bool maximizing)
{
    int alpha = -1000, beta = 1000;
    vector<Move> possible_moves;
    board.genAllMoves(s, possible_moves);
    vector<double> heuristics;
    if (possible_moves.empty()) {
        cout << "NO POSSIBLE MOVES\n";
        return {-1};
    }
    Board tempBoard;
    int num_moves = possible_moves.size();
    for (int i = 0; i < num_moves; i++) {
        tempBoard = *board.clone();
        Status status = tempBoard.makeMove(possible_moves[i], s);
        // if (status != COMPLETED && status != GAME_OVER) {
        //     continue;
        // }
        heuristics.push_back(minimax(tempBoard, switchSide(s), !maximizing, depth - 1, alpha, beta));
    }
    double max_heuristics = -1000;
    int num_heuristics = heuristics.size();
    for (int i = num_heuristics - 1; i >= 0; i--) {
        if (heuristics[i] >= max_heuristics) {
            max_heuristics = heuristics[i];
        }
    }
    for (int i = 0; i < num_heuristics; i++) {
        // cout << "number of heuristics: " << num_heuristics << endl;
        // cout << "i: " << i << endl;
        if (heuristics[i] < max_heuristics) {
            heuristics.erase(heuristics.begin() + i);
            possible_moves.erase(possible_moves.begin() + i);
            i--;
            num_heuristics--;
        }
    }
    num_moves = possible_moves.size();
    // cout << "Number of moves: " << num_moves << endl;
    Move final_move = possible_moves[rand() % num_moves];
    // cout << "FINAL MOVE: " << final_move.start.row << " " << final_move.start.col << " to " << final_move.end.row << " " << final_move.end.col << "\n";
    return final_move;
}

double AI::minimax(Board board, Side s, bool maximizing, int depth, int alpha, int beta) {
    // cout << "------------minimax----------\n";
    // cout << "depth: " << depth << "\n";
    // if (depth == 2) {
    //     board.drawBoard();
    //     char c;
    //     cin >> c;
    // }
    if (depth == 0) {
        return getHeuristic(board);
    }
    vector<Move> possible_moves;
    board.genAllMoves(s, possible_moves);
    int n = possible_moves.size();
    int initial = 0;
    Board tempBoard;
    if (maximizing) {
        for (int i = 0; i < n; i++) {
            tempBoard = *board.clone();
            Status status = tempBoard.makeMove(possible_moves[i], s);
            if (status != COMPLETED && status != GAME_OVER) {
                continue;
            }
            int result = minimax(tempBoard, switchSide(s), !maximizing, depth - 1, alpha, beta);
            initial = max(result, initial);
            alpha = max(initial, alpha);
            if (alpha >= beta) {
                break;
            }
        }
    } else {
        initial = 1000;
        for (int i = 0; i < n; i++) {
            tempBoard = *board.clone();
            Status status = tempBoard.makeMove(possible_moves[i], s);
            if (status != COMPLETED && status != GAME_OVER) {
                continue;
            }
            int result = minimax(tempBoard, switchSide(s), !maximizing, depth - 1, alpha, beta);
            initial = min(result, initial);
            beta = min(beta, initial);
            if (alpha >= beta) {
                break;
            }
        }
    }
    return initial;
}

double AI::getHeuristic(Board board) {
    double king_weight = 1.5;
    if (getSide() == BLACK_SIDE) {
        return (king_weight * board.getNumBlackKings() + board.getNumBlack()) - (king_weight * board.getNumWhiteKings() + board.getNumWhite());
    }
    return (king_weight * board.getNumWhiteKings() + board.getNumWhite()) - (king_weight * board.getNumBlackKings() + board.getNumBlack());
}

Side AI::switchSide(Side side) {
    if (side == BLACK_SIDE) {
        return WHITE_SIDE;
    }
    return BLACK_SIDE;
}

void printMove(Move m) {
    cout << "Move: " << m.start.col << " " << m.start.row << " to " << m.end.col << " " << m.end.row << "\n";
}

void clear() {
    system("CLS");
}

void playGame() {
    char p, dummy;
    Side player, cpu;
    do {
        cout << "Pick a side (b or w) ";
        cin >> p;
    } while (!(p == 'b' || p == 'w'));
    clear();
    if (p == 'b') {
        player = BLACK_SIDE;
        cpu = WHITE_SIDE;
    } else if (p == 'w') {
        player = WHITE_SIDE;
        cpu = BLACK_SIDE;
    }
    Player one("Player", player);
    AI two("CPU", cpu);
    bool player_turn = true;
    Board board;
    Status msg = COMPLETED;
    while (msg != GAME_OVER) {
        clear();
        cout << "\n\n";
        if (player_turn) {
            cout << "------------Player Move------------\n";
        } else {
            cout << "-------------CPU Move--------------\n";
        }
        cout << "\n\n";
        board.drawBoard();
        if (!player_turn) {
            system("pause");
        }
        if (player_turn) {
            vector<Move> player_moves;
            board.genAllMoves(one.getSide(), player_moves);
            if (player_moves.empty()) {
                cout << "No possible player moves, you lose\n";
                break;
            } else {
                int num_moves = player_moves.size();
                cout << "Possible moves (must make jump moves when possible):\n";
                for (int i = 0; i < num_moves; i++) {
                    printMove(player_moves[i]);
                }
                cout << "\n\n";
            }
            Move move;
            do {
                cout << "Enter your move (four space-separated integers x1 y1 x2 y2): ";
                cin >> move.start.col >> move.start.row >> move.end.col >> move.end.row;
                msg = one.makeMove(move, board);
                if (msg == FAILED_INVALID_DEST) {
                    cout << "Invalid move\n";
                } else if (msg == FAILED_INVALID_PIECE) {
                    cout << "Invalid chosen piece\n";
                }
            } while (msg != COMPLETED && msg != GAME_OVER);
            player_turn = !player_turn;
        } else {
            vector<Move> ai_moves;
            board.genAllMoves(two.getSide(), ai_moves);
            if (ai_moves.empty()) {
                cout << "No possible cpu moves, you win\n";
            }
            msg = two.makeMove(board);
            player_turn = !player_turn;
        }
    }
}

int main() {
    srand(time(NULL));
    clear();
    cout << "\n\n\n-------------- Welcome to AI Checkers --------------\n";
    cout << "Morehouse Data Structures & Algorithms Final Project\n";
    cout << "    Developed by Malcolm Maddox and Malik Saafir\n\n\n";
    char play_again;
    do {
        playGame();
        do {
            cout << "Would you like to play again? (y/n) ";
            cin >> play_again;
        } while (!(play_again == 'y' || play_again == 'n'));
    } while (play_again == 'y');
    return 0;
}