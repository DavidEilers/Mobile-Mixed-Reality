//
// Created by Oskar on 28.09.2021.
//

#ifndef TEAMPRAKTIKUM_FOUR_HPP
#define TEAMPRAKTIKUM_FOUR_HPP

#include <vector>
#include <string>
#include "GameManager.h"
#include "Messages.hpp"
#include <android/log.h>

#define MSG_FOUR_CLICK 200
#define MSG_FOUR_END 201
#define MSG_FOUR_BOARD_UPDATE 202

#define FIELD_EMPTY 0
#define PLAYER_1 1
#define PLAYER_2 2

/**
 * class that holds the game board state
 */
class FourBoard {
public:
    char **board;

    char width = 8;
    char height = 6;

    /**
     * Constructor
     */
    FourBoard() {
        board = new char *[width];

        for (int i = 0; i < width; i++) {
            board[i] = new char[height];
        }

        reset();
    }

    /**
     * Destructor
     */
    ~FourBoard() {
        delete[] board;
    }

    void reset() {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
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
     * checks if someone has won
     * @return 0 means no winner yet, 1 means player O and 2 means player X
     */
    char check_win() {
        for (int x = 0; x < width; x++) {
            int vertical_counter_p1 = 0;
            int vertical_counter_p2 = 0;

            for (int y = 0; y < height; y++) {
                if (board[x][y] == PLAYER_1) vertical_counter_p1++;
                else vertical_counter_p1 = 0;

                if (board[x][y] == PLAYER_2) vertical_counter_p2++;
                else vertical_counter_p2 = 0;

                if (vertical_counter_p1 >= 4) return PLAYER_1;
                if (vertical_counter_p2 >= 4) return PLAYER_2;
            }
        }

        for (int y = 0; y < height; y++) {
            int horizontal_counter_p1 = 0;
            int horizontal_counter_p2 = 0;

            for (int x = 0; x < width; x++) {
                if (board[x][y] == PLAYER_1) horizontal_counter_p1++;
                else horizontal_counter_p1 = 0;

                if (board[x][y] == PLAYER_2) horizontal_counter_p2++;
                else horizontal_counter_p2 = 0;

                if (horizontal_counter_p1 >= 4) return PLAYER_1;
                if (horizontal_counter_p2 >= 4) return PLAYER_2;
            }
        }

        return 0; //No one has won yet
    }

    void insert_at(int x, char player) {
        if (x < 0) return;
        if (x >= width) return;

        for (int y = height - 1; y >= 0; y--) {
            if (board[x][y] == FIELD_EMPTY) {
                board[x][y] = player;
                return;
            }
        }
    }

    int amountInRow(int x){
        if(x<0||x>width)return -1;
        size_t counter=0;
        for(int y=0;y<height;y++){
            if(board[x][y]!=FIELD_EMPTY)counter++;
        }
        return counter;
    }

    bool is_column_full(int x) {
        if (x < 0) return false;
        if (x >= width) return false;

        for (int y = height - 1; y >= 0; y--) {
            if (board[x][y] == FIELD_EMPTY) {
                return false;
            }
        }

        return true;
    }
};

/**
 * class for storing the click message for a Tic Tac Toe board
 */
class FourClickMessage : public BaseMessage {
public:
    const char TYPE = MSG_FOUR_CLICK;

    unsigned char pos_x;

    void set_coord(char x) {
        pos_x = x;
    }

    virtual int to_bytes(char *buffer) override {
        __android_log_print(ANDROID_LOG_VERBOSE, "TTTClick -> to_bytes", "%d", TYPE);
        buffer[0] = TYPE;
        buffer[1] = pos_x;
        return 2;
    }

    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;
        this->pos_x = buffer[1];
        return true;
    }
};

/**
 * this message class stores who has won the game
 */
class FourGameEndMessage : public BaseMessage {
public:
    const char TYPE = MSG_FOUR_END;

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
class FourBoardUpdateMessage : public BaseMessage {
public:
    const char TYPE = MSG_FOUR_BOARD_UPDATE;

    FourBoard *board;

    char hosts_turn = 0;

    /**
     * Constructor
     * @param board_ptr pointer to the board of a FourBoard to update
     */
    FourBoardUpdateMessage(FourBoard *board_ptr) {
        board = board_ptr;
    }

    virtual int to_bytes(char *buffer) override {
        buffer[0] = TYPE;
        buffer[1] = hosts_turn;
        int i = 2;
        for (int x = 0; x < board->width; x++) {
            for (int y = 0; y < board->height; y++) {
                buffer[i] = board->board[x][y];
                i++;
            }
        }
        return 11;
    }

    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;
        this->hosts_turn = buffer[1];
        int i = 2;
        for (int x = 0; x < this->board->width; x++) {
            for (int y = 0; y < this->board->height; y++) {
                this->board->board[x][y] = buffer[i];
                i++;
            }
        }
        return true;
    }
};

class FourMaster : public Master {
public:
    FourBoard board;

    bool my_turn = true;

    FourMaster() : Master("master", 7080, 1) {}

    virtual void on_game_specific_message(RawContainer container) override {
        FourClickMessage clickMessage;
        if (clickMessage.from_bytes(container.buffer)) {
            //Master is always X and Slave always O
            if (!my_turn) {
                board.insert_at(clickMessage.pos_x, PLAYER_2);
                my_turn = true;
                broadCastBoardUpdate();
            }
        }
    }

    /**
     * broadcasts the current board state to the slave
     */
    void broadCastBoardUpdate() {
        FourBoardUpdateMessage *bum = new FourBoardUpdateMessage(&board);
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
     * lets the master make a move and place his dot at the given column position.
     * 0, 0 means top left.
     * If the move is not allowed or it is not the masters turn, nothing happens.
     * @param x x-coord of the column
     */
    void makeMove(int x) {
        if (my_turn && !board.is_column_full(x)) {
            board.insert_at(x, PLAYER_1);
            my_turn = false;

            broadCastBoardUpdate();
        }
    }
};

class FourSlave : public Slave {
public:
    FourBoard board;

    bool my_turn = false;

    FourSlave() : Slave("slave", 7081) {}

    /**
     * handles incoming messages that are specific to the TicTacToe game.
     * @param containers contains the raw message data.
     */
    virtual void on_game_specific_message(RawContainer container) override {
        // __android_log_print(ANDROID_LOG_VERBOSE,"TTTSlave","Got an fieldUpdate");
        FourBoardUpdateMessage bum(&board);
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
    void makeMove(int x) {
        FourClickMessage *cm = new FourClickMessage();
        cm->pos_x = x;

        send(cm);
    }
};


class FourGame {
public:

    FourBoard* board;

    FourGame(FourMaster *master) {
        this->is_master = true;
        this->master = master;
        board = &(master->board);
    }

    FourGame(FourSlave *slave) {
        this->is_master = false;
        this->slave = slave;
        board = &(slave->board);
    }

    FourGame(FourMaster* master_, FourSlave* slave_){
        this->master=master_;
        this->slave=slave_;
        board = &(master->board);
        this->is_master = true;
        this->debug=true;

    }

    ~FourGame() {

    }

    void makeMove(int x) {
        if (is_master) {
            master->makeMove(x);
        } else {
            slave->makeMove(x);
        }
        if(debug){
            this->is_master = !this->is_master;
        }
    }

    char checkWin() {
        if (is_master) {
            return master->board.check_win();
        } else {
            return slave->board.check_win();
        }
    }

    bool myTurn() {
        if (is_master) {
            return master->my_turn;
        } else {
            return slave->my_turn;
        }
    }

    char whoAmI(){
        if(is_master){
            return PLAYER_1;
        }else{
            return PLAYER_2;
        }
    }

    void restartGame() {
        if (is_master) {
            master->restartGame();
        }
    }

private:
    FourMaster *master;
    FourSlave *slave;

    bool is_master;
    bool debug=false;
};


#endif //TEAMPRAKTIKUM_FOUR_HPP