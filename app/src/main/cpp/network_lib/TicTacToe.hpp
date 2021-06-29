#include <vector>
#include <string>
#include "GameManager.h"
#include "Messages.hpp"
#include <android/log.h>

#ifndef TEAMPRAKTIKUM_TICTACTOE_HPP
#define TEAMPRAKTIKUM_TICTACTOE_HPP


#define MSG_TTT_CLICK 100
#define MSG_TTT_END 101
#define MSG_TTT_BOARD_UPDATE 102

#define FIELD_EMPTY 0
#define PLAYER_O 1
#define PLAYER_X 2

/**
 * class that holds the game board state
 */
class TTTBoard {
public:
    char **board;

    /**
     * Constructor
     */
    TTTBoard() {
        board = new char *[3];
        board[0] = new char[3];
        board[1] = new char[3];
        board[2] = new char[3];

        reset();
    }

    /**
     * Destructor
     */
    ~TTTBoard() {
        delete[] board;
    }

    void reset() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = FIELD_EMPTY;
            }
        }
    }

    void set(int x, int y, char state) {
        board[x][y] = state;
    }

    char get(int x, int y) {
        return board[x][y];
    }

    /**
     * compares the string representation of the board to the actual board
     * @param str string with minimum length of 9 containing 'x', 'o' representing the board state.
     * Any other character means "don´t care".
     * The first 3 characters are the top row of the board from left to right.
     * Then comes the middle row and then the bottom row.
     * @return true if the board has x and o on the given positions
     * false otherwise
     * e. g. the string "........." would always return true because all fields are ignored
     * "ooo......" would return true if the top rows is all o.
     * ".x..x..x." would return true if the middle column is all x.
     */
    bool equals(std::string str) {
        int i = 0;

        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                switch (str[i]) {
                    case 'x':
                        if (board[x][y] != PLAYER_X) return false;
                        break;
                    case 'o':
                        if (board[x][y] != PLAYER_O) return false;
                        break;
                }

                i++;
            }
        }

        return true;
    }

    /**
     * checks if someone has won
     * @return 0 means no winner yet, 1 means player O and 2 means player X
     */
    char check_win() {
        if (equals("xxx......")) return PLAYER_X;
        if (equals("...xxx...")) return PLAYER_X;
        if (equals("......xxx")) return PLAYER_X;

        if (equals("ooo......")) return PLAYER_O;
        if (equals("...ooo...")) return PLAYER_O;
        if (equals("......ooo")) return PLAYER_O;

        if (equals("x..x..x..")) return PLAYER_X;
        if (equals(".x..x..x.")) return PLAYER_X;
        if (equals("..x..x..x")) return PLAYER_X;

        if (equals("o..o..o..")) return PLAYER_O;
        if (equals(".o..o..o.")) return PLAYER_O;
        if (equals("..o..o..o")) return PLAYER_O;

        if (equals("x...x...x")) return PLAYER_X;
        if (equals("..x.x.x..")) return PLAYER_X;

        if (equals("o...o...o")) return PLAYER_O;
        if (equals("..o.o.o..")) return PLAYER_O;

        return 0; //No one has won yet
    }

    /**
     * checks if a player can use the given board position
     * @param x x-coord of board position
     * @param y y-coord of board position
     */
    bool isValidMove(int x, int y) {
        return board[x][y] == FIELD_EMPTY;
    }
};

/**
 * class for storing the click message for a Tic Tac Toe board
 */
class TTTClickMessage : public BaseMessage {
public:
    const char TYPE = MSG_TTT_CLICK;

    unsigned char pos_x, pos_y;

    void set_coords(char x, char y) {
        pos_x = x;
        pos_y = y;
    }

    virtual int to_bytes(char *buffer) override {
        __android_log_print(ANDROID_LOG_VERBOSE, "TTTClick -> to_bytes", "%d", TYPE);
        buffer[0] = TYPE;
        buffer[1] = pos_x;
        buffer[2] = pos_y;
        return 3;
    }

    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;
        this->pos_x = buffer[1];
        this->pos_y = buffer[2];
        return true;
    }
};

/**
 * this message class stores who has won the game
 */
class TTTGameEndMessage : public BaseMessage {
public:
    const char TYPE = MSG_TTT_END;

    bool host_won = false;

    virtual int to_bytes(char *buffer) override {
        buffer[0] = TYPE;
        if (host_won) buffer[1] = 1;
        else buffer[1] = 0;
        return 2;
    }

    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;
        this->host_won = buffer[1] == 1;
        return true;
    }
};


/**
 * Stores the TicTacToe board state to be sent via network
 * also stores if it is the hosts turn or not
 */
class TTTBoardUpdateMessage : public BaseMessage {
public:
    const char TYPE = MSG_TTT_BOARD_UPDATE;

    TTTBoard *board;

    char hosts_turn = 0;

    /**
     * Constructor
     * @param board_ptr pointer to the board of a TTTBoard to update
     */
    TTTBoardUpdateMessage(TTTBoard *board_ptr) {
        board = board_ptr;
    }

    virtual int to_bytes(char *buffer) override {
        buffer[0] = TYPE;
        buffer[1] = hosts_turn;
        int i = 2;
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                buffer[i] = board->board[x][y];
                i++;
            }
        }
    }

    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;
        this->hosts_turn = buffer[1];
        int i = 2;
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                this->board->board[x][y] = buffer[i];
                i++;
            }
        }
        return true;
    }
};


class TTTMaster : public Master {
public:
    TTTBoard board;

    bool my_turn = true;

    TTTMaster() : Master("master", 7080, 1) {}

    virtual void on_game_specific_message(RawContainer container) override {
        TTTClickMessage clickMessage;
        if (clickMessage.from_bytes(container.buffer)) {
            //Master is always X and Slave always O
            if (!my_turn) {
                board.set(clickMessage.pos_x, clickMessage.pos_y, PLAYER_O);
                my_turn = true;
                broadCastBoardUpdate();
            }
        }
    }

    /**
     * broadcasts the current board state to the slave
     */
    void broadCastBoardUpdate() {
        TTTBoardUpdateMessage *bum = new TTTBoardUpdateMessage(&board);
        bum->hosts_turn = my_turn;
        broadcast(bum);
    }

    /**
     * restarts the game and broadcasts the clear board
     */
    void restartGame() {
        board.reset();
        my_turn = true;
        broadCastBoardUpdate();
    }

    /**
     * lets the master make a move and place his X at the given position (x, y) on the board.
     * 0, 0 means top left and 2, 2 bottom right.
     * If the move is not allowed or it is not the masters turn, nothing happens.
     * @param x x-coord of position
     * @param y y-coord of position
     */
    void makeMove(int x, int y) {
        if (my_turn && board.isValidMove(x, y)) {
            board.set(x, y, PLAYER_X);
            my_turn = false;

            broadCastBoardUpdate();
        }
    }
};

class TTTSlave : public Slave {
public:
    TTTBoard board;

    bool my_turn = false;

    TTTSlave() : Slave("slave", 7081) {}

    /**
     * handles incoming messages that are specific to the TicTacToe game.
     * @param containers contains the raw message data.
     */
    virtual void on_game_specific_message(RawContainer container) override {
        // __android_log_print(ANDROID_LOG_VERBOSE,"TTTSlave","Got an fieldUpdate");
        TTTBoardUpdateMessage bum(&board);
        if (bum.from_bytes(container.buffer)) {
            // if incoming message is TTTBoardUpdateMessage the board is updated now
            my_turn = bum.hosts_turn == 0;
        }
    }

    /**
     * lets the slave make a move and place his O at the given position (x, y) on the board.
     * 0, 0 means top left and 2, 2 bottom right.
     * If the move is not allowed or it is not the slaves turn, nothing happens.
     * All checks are made on the masters side.
     * @param x
     * @param y
     */
    void makeMove(int x, int y) {
        TTTClickMessage * cm = new TTTClickMessage();
        cm->pos_x = x;
        cm->pos_y = y;

        send(cm);
    }
};


#endif //TEAMPRAKTIKUM_TICTACTOE_HPP