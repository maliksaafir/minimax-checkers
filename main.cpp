//
//  main.cpp
//  Phase I
//

#include <iostream>
#include "PrototypeSpecificationFile.h"
#include "PrototypeSpecificationFine.cpp"

using namespace std;

Player game;

bool check (bool winner)
{
    int countx = 0, counto = 0;

    for (int x = 0; x < 8 ; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (game.board[x][y] == 'X')
            {
                countx++;
            }
            else if (game.board[x][y] == 'O')
            {
                counto++;
            }
        }
    }

    if (countx == 0)
    {
        winner = true;
        cout << "The O player wins!" << endl;
    }
    else if (counto == 0)
    {
        winner = true;
        cout << "The X player wins!" << endl;
    }
    else
    {
        winner = false;
    }

    return winner;
}


void piecevalidation() // Moves user piece.
{
    int x, y, a, b;
    cout << "First enter your current row position, then your column position (Press enter in between numbers): " << endl;
    cin >> x >> y;
    cout << "Then enter your new row position, then your new column position location (Press enter in between numbers): " << endl;
    cin >> a >> b;

    if (game.validateDirection(x, y, a, b) == true && game.turn == false) // X-turn
    {
        cout << "Move successful." << endl;
        if (game.board[a - 1][b - 1] == 'O')
        {
            game.board[a - 1][b - 1] = ' ';
        }
        else if (game.board[a - 1][b + 1] == 'O')
        {
            game.board[a - 1][b + 1] = ' ';
        }
        game.turn = true;
        game.movePiece(x, y, a, b);
        game.board[x][y] = ' ';
        game.board[a][b] = 'X';
    }
    else if (game.validateDirection(x, y, a, b) == true && game.turn == true) // O-turn
    {
        if (game.board[a + 1][b + 1] == 'X')
        {
            game.board[a + 1][b + 1] = ' ';
        }
        else if (game.board[a + 1][b - 1] == 'X')
        {
            game.board[a + 1][b - 1] = ' ';
        }
        game.turn = false;
        game.movePiece(x, y, a, b);
        game.board[x][y] = ' ';
        game.board[a][b] = 'O';
    }
    else
    {
        cout<< "Not a valid move." << endl;
    }
}

int main ()
{
    game.appendNode(0, 0, false, 'X');
    game.appendNode(0, 2, false, 'X');
    game.appendNode(0, 4, false, 'X');
    game.appendNode(0, 6, false, 'X');

    game.appendNode(1, 1, false, 'X');
    game.appendNode(1, 3, false, 'X');
    game.appendNode(1, 5, false, 'X');
    game.appendNode(1, 7, false, 'X');

    game.appendNode(2, 0, false, 'X');
    game.appendNode(2, 2, false, 'X');
    game.appendNode(2, 4, false, 'X');
    game.appendNode(2, 6, false, 'X');

    game.appendNode(5, 1, true, 'O');
    game.appendNode(5, 3, true, 'O');
    game.appendNode(5, 5, true, 'O');
    game.appendNode(5, 7, true, 'O');

    game.appendNode(6, 0, true, 'O');
    game.appendNode(6, 2, true, 'O');
    game.appendNode(6, 4, true, 'O');
    game.appendNode(6, 6, true, 'O');

    game.appendNode(7, 1, true, 'O');
    game.appendNode(7, 3, true, 'O');
    game.appendNode(7, 5, true, 'O');
    game.appendNode(7, 7, true, 'O');

    game.makeboard();

    bool winner = false;

    while (winner == false)
    {
        if (game.turn == false)
        {
            cout << "It's The X player's turn!" << endl << endl;
        }
        else if (game.turn == true)
        {
            cout << "It's The O player's turn!" << endl << endl;
        }

        game.printboard();
        piecevalidation();
        check(winner);
    }

    bool finish = false;
    char answer;

    do
    {
        cout << "Would you like to play again? Enter 'Y' for 'Yes' or 'N' for 'No': " << endl;
        cin >> answer;
        answer = toupper(answer);

        if (answer == 'Y')
        {
            finish = false;
        }
        else if (answer == 'N')
        {
            finish = true;
        }
        else
        {
            cout << "Incorrect answer, try again. " << endl;
        }
    }
    while (answer != 'N' && answer != 'Y');

    return 0;
}

