#include <bits/stdc++.h>
using namespace std;

class Board {
private:
    char arr[3][3];
    char winner = ' ';
    int moves = 0;

public:
    Board() { reset(); }

    void reset() {
        char c = '0';
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                arr[i][j] = c++;
        winner = ' ';
        moves = 0;
    }

    void print() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j)
                cout << (arr[i][j] == 'O' ? '@' : arr[i][j]) << "   ";
            cout << "\n\n\n";
        }
    }

    bool makeMove(int k, char player) {
        if (arr[k / 3][k % 3] != 'X' && arr[k / 3][k % 3] != 'O') {
            arr[k / 3][k % 3] = player;
            ++moves;
            return true;
        }
        return false;
    }

    void undoMove(int k) {
        arr[k / 3][k % 3] = '0' + k;
        --moves;
    }

    bool isFull() { return moves == 9; }

    int evaluate(char player) {
        for (int i = 0; i < 3; ++i)
            if (arr[i][0] == arr[i][1] && arr[i][1] == arr[i][2])
                return (arr[i][0] == player) ? 10 : -10;
        for (int i = 0; i < 3; ++i)
            if (arr[0][i] == arr[1][i] && arr[1][i] == arr[2][i])
                return (arr[0][i] == player) ? 10 : -10;
        if (arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2])
            return (arr[0][0] == player) ? 10 : -10;
        if (arr[0][2] == arr[1][1] && arr[1][1] == arr[2][0])
            return (arr[0][2] == player) ? 10 : -10;
        return 0;
    }
};

class Player {
public:
    char sign;
    Player(char sign) : sign(sign) {}
    virtual int move(Board &b) = 0;
};

class Human : public Player {
public:
    Human(char sign) : Player(sign) {}
    int move(Board &b) override {
        char c;
        cout << "Enter Position: ";
        cin >> c;
        return c - '0';
    }
};

class Easy : public Player {
public:
    Easy(char sign) : Player(sign) {}
    int move(Board &b) override {
        int pos;
        do {
            pos = rand() % 9;
        } while (!b.makeMove(pos, sign));
        return pos;
    }
};

class Hard : public Player {
public:
    Hard(char sign) : Player(sign) {}
    int move(Board &b) override {
        int bestVal = -1000, bestMove = -1;
        for (int i = 0; i < 9; ++i) {
            if (b.makeMove(i, sign)) {
                int moveVal = minimax(b, 0, false, sign);
                b.undoMove(i);
                if (moveVal > bestVal) {
                    bestMove = i;
                    bestVal = moveVal;
                }
            }
        }
        b.makeMove(bestMove, sign);
        return bestMove;
    }

private:
    int minimax(Board &b, int depth, bool isMax, char player) {
        int score = b.evaluate(player);
        if (score == 10 || score == -10) return score;
        if (b.isFull()) return 0;

        if (isMax) {
            int best = -1000;
            for (int i = 0; i < 9; ++i) {
                if (b.makeMove(i, player)) {
                    best = max(best, minimax(b, depth + 1, !isMax, player));
                    b.undoMove(i);
                }
            }
            return best;
        } else {
            int best = 1000;
            char oppo = (player == 'X') ? 'O' : 'X';
            for (int i = 0; i < 9; ++i) {
                if (b.makeMove(i, oppo)) {
                    best = min(best, minimax(b, depth + 1, !isMax, player));
                    b.undoMove(i);
                }
            }
            return best;
        }
    }
};

class Game {
private:
    Board board;
    Player *p1, *p2;
    bool turn = true;

public:
    Game(Player *p1, Player *p2) : p1(p1), p2(p2) {}

    void play() {
        board.reset();
        while (true) {
            board.print();
            int move = (turn) ? p1->move(board) : p2->move(board);
            if (!board.makeMove(move, (turn) ? p1->sign : p2->sign)) {
                cout << "Invalid move, try again.\n";
                continue;
            }

            if (board.evaluate(p1->sign) == 10 || board.evaluate(p2->sign) == 10) {
                board.print();
                cout << "WINNER --> " << ((turn) ? p1->sign : p2->sign) << "\n";
                break;
            }

            if (board.isFull()) {
                board.print();
                cout << "---- DRAW ----\n";
                break;
            }

            turn = !turn;
        }
    }
};

int main() {
    srand(time(0));
    char choice;
    while (true) {
        cout << "Select mode:\n1. Human vs Easy\n2. Easy vs Human\n3. Human vs Hard\n4. Hard vs Human\n5. Human vs Human\nq. Quit\n";
        cin >> choice;

        if (choice == 'q') break;

        Player *p1, *p2;

        if (choice == '1') {
            p1 = new Human('X');
            p2 = new Easy('O');
        } else if (choice == '2') {
            p1 = new Easy('X');
            p2 = new Human('O');
        } else if (choice == '3') {
            p1 = new Human('X');
            p2 = new Hard('O');
        } else if (choice == '4') {
            p1 = new Hard('X');
            p2 = new Human('O');
        } else if (choice == '5') {
            p1 = new Human('X');
            p2 = new Human('O');
        } else {
            cout << "Invalid choice.\n";
            continue;
        }

        Game game(p1, p2);
        game.play();

        delete p1;
        delete p2;
    }

    return 0;
}
