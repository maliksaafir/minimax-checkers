#include <vector>
#include <string>
using namespace std;

enum PieceType {
    EMPTY,
    BLACK,
    WHITE,
    BLACK_KING,
    WHITE_KING
};

enum Side {
    BLACK_SIDE,
    WHITE_SIDE
};

enum Status {
    COMPLETED,
    FAILED_INVALID_PIECE,
    FAILED_INVALID_DEST,
    GAME_OVER
};

struct Point {
    int row;
    int col;
};

struct Move {
    Point start;
    Point end;
};

class Failure {};

class Board {
private:
    PieceType** board;
    int numBlack;
    int numWhite;
    int numBlackKings;
    int numWhiteKings;
public:
    Board();
    Board(PieceType** b);
    bool isOpponentPiece(Side side, PieceType);
    bool isOwnPiece(int r, int c, Side side);
    bool movesEqual(Move one, Move two);
    int getNumBlack();
    int getNumWhite();
    int getNumBlackKings();
    int getNumWhiteKings();
    Board* clone();
    PieceType getPiece(int i, int j);
    PieceType getPiece(Point p);
    Point getMidSquare(Move move);
    Status makeMove(Move move, Side side);
    void drawBoard();
    void genAllMoves(Side side, vector<Move>& moves);
    void genValidJumpMoves(int r, int c, Side side, vector<Move>& moves);
    void genValidMoves(int r, int c, Side side, vector<Move>& moves);
    void setUpBoard();
};

class Player {
protected:
    Side side;
    string name;
public:
    Player(string n, Side s);
    Side getSide();
    virtual Status makeMove(Move move, Board b);
};

class AI : public Player {
private:
    int depth;
public:
    AI(string name, Side side) : Player(name, side){
        depth = 3;
    };
    Status makeMove(Board board);
    Move minimaxStart(Board board, Side side, bool maximizing);
    double minimax(Board board, Side side, bool maximizing, int depth, int alpha, int beta);
    double getHeuristic(Board board);
    Side switchSide(Side side);
};