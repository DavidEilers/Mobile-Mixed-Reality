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

    /**
     * Clears the board. All fields are set to FIELD_EMPTY.
     */
    void reset() {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                board[i][j] = FIELD_EMPTY;
            }
        }
    }

    /**
     * Sets the given position in the board to a given state
     * @param x column position
     * @param y row position
     * @param state FIELD_EMPTY, PLAYER_1 or PLAYER_2
     */
    void set(int x, int y, char state) {
        board[x][y] = state;
    }

    /**
     * Gets the state of a given board position
     * @param x column position
     * @param y row position
     * @return FIELD_EMPTY, PLAYER_1 or PLAYER_2
     */
    char get(int x, int y) {
        return board[x][y];
    }

    /**
     * checks if someone has won
     * @return 0 means no winner yet, 1 means player 1, 2 means player 2 and 3 means the board is full
     */
    char check_win() {
        int used_positions_counter = 0;

        for (int x = 0; x < width; x++) {
            int vertical_counter_p1 = 0;
            int vertical_counter_p2 = 0;

            for (int y = 0; y < height; y++) {
                if (board[x][y] != FIELD_EMPTY) used_positions_counter++;
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

        if (used_positions_counter >= width * height) {
            return 3 //Board is full
        }
        return 0; //No one has won yet
    }

    /**
     * Inserts a disc into a given column
     * @param x column position
     * @param player owner of the disc
     */
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

    /**
     * Returns the count of discs in a column
     * @param x column position
     * @return count of discs in this column
     */
    int amountInColumn(int x) {
        if (x < 0 || x > width)return -1;
        size_t counter = 0;
        for (int y = 0; y < height; y++) {
            if (board[x][y] != FIELD_EMPTY)counter++;
        }
        return counter;
    }

    /**
     * Tells you if the given column is already full of discs
     * @param x column position
     * @return true -> column is full, false column has empty positions
     */
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
 * class for storing the click message for a four in a row board
 */
class FourClickMessage : public BaseMessage {
public:
    const char TYPE = MSG_FOUR_CLICK;

    unsigned char pos_x;

    /**
     * Sets the column positon the click happend in
     * @param x column position
     */
    void set_coord(char x) {
        pos_x = x;
    }

    /**
     * Serializes this Object and stores it inside the given buffer
     * @param buffer buffer to store the object in
     * @return count of written bytes
     */
    virtual int to_bytes(char *buffer) override {
        __android_log_print(ANDROID_LOG_VERBOSE, "FourClick -> to_bytes", "%d", TYPE);
        buffer[0] = TYPE;
        buffer[1] = pos_x;
        return 2;
    }

    /**
     * Deserializes a buffer into this Object. Configures this object with the information in the buffer.
     * @param buffer buffer to read from
     * @return true if buffer could be deserialized
     */
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

    /**
     * Serializes this Object and stores it inside the given buffer
     * @param buffer buffer to store the object in
     * @return count of written bytes
     */
    virtual int to_bytes(char *buffer) override {
        buffer[0] = TYPE;
        if (host_won) buffer[1] = 1;
        else buffer[1] = 0;
        return 2;
    }

    /**
     * Deserializes a buffer into this Object. Configures this object with the information in the buffer.
     * @param buffer buffer to read from
     * @return true if buffer could be deserialized
     */
    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;
        this->host_won = buffer[1] == 1;
        return true;
    }
};


/**
 * Stores the Four in a Row board state to be sent via network
 * also stores if it is the hosts turn or not
 */
class FourBoardUpdateMessage : public BaseMessage {
public:
    const char TYPE = MSG_FOUR_BOARD_UPDATE;

    FourBoard *board;

    char hosts_turn = 0;
    char new_game = 0;

    /**
     * Constructor
     * @param board_ptr pointer to the board of a FourBoard to update
     */
    FourBoardUpdateMessage(FourBoard *board_ptr) {
        board = board_ptr;
    }

    /**
     * Serializes this Object and stores it inside the given buffer
     * @param buffer buffer to store the object in
     * @return count of written bytes
     */
    virtual int to_bytes(char *buffer) override {
        buffer[0] = TYPE;
        buffer[1] = hosts_turn;
        buffer[2] = new_game;
        int i = 3;
        for (int x = 0; x < board->width; x++) {
            for (int y = 0; y < board->height; y++) {
                buffer[i] = board->board[x][y];
                i++;
            }
        }
        return 12;
    }

    /**
     * Deserializes a buffer into this Object. Configures this object with the information in the buffer.
     * @param buffer buffer to read from
     * @return true if buffer could be deserialized
     */
    virtual bool from_bytes(char *buffer) override {
        if (buffer[0] != TYPE) return false;
        this->hosts_turn = buffer[1];
        this->new_game = buffer[2];
        int i = 3;
        for (int x = 0; x < this->board->width; x++) {
            for (int y = 0; y < this->board->height; y++) {
                this->board->board[x][y] = buffer[i];
                i++;
            }
        }
        return true;
    }
};

/**
 * Represents the Master in a four in a row game
 */
class FourMaster : public Master {
public:
    FourBoard board;

    bool my_turn = true;

    /**
     * Constructor
     */
    FourMaster() : Master("master", 7080, 1) {}

    /**
     * Handles the game specific messages
     * @param container container with the message
     */
    virtual void on_game_specific_message(RawContainer container) override {
        FourClickMessage clickMessage;
        if (clickMessage.from_bytes(container.buffer)) {
            //Master is always X and Slave always O
            if (!my_turn) {
                board.insert_at(clickMessage.pos_x, PLAYER_2);
                my_turn = true;
                broadCastBoardUpdate(false);
            }
        }
    }

    /**
     * broadcasts the current board state to the slave
     * @param new_game true if you want to broadcast that a new game started
     */
    void broadCastBoardUpdate(bool new_game) {
        FourBoardUpdateMessage *bum = new FourBoardUpdateMessage(&board);
        bum->hosts_turn = my_turn;
        if (new_game) bum->new_game = 1;
        broadcast(bum);
    }

    /**
     * restarts the game and broadcasts the clear board
     */
    void restartGame() {
        board.reset();
        my_turn = true;
        broadCastBoardUpdate(true);
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

            broadCastBoardUpdate(false);
        }
    }
};

/**
 * Represents the slave in a four in a row game
 */
class FourSlave : public Slave {
public:
    FourBoard board;

    bool my_turn = false;

    /**
     * Constructor
     */
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

            if (bum.new_game != 0) restarted = true;
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

    /**
     * Tells you if the game was restarted
     * @return true or false
     */
    bool isGameRestarted() {
        if (restarted) {
            restarted = false;
            return true;
        }

        return false;
    }

private:

    bool restarted = false;
};

/**
 * Unifies FourMaster and FourSlave into one Object to use for network communication
 */
class FourGame {
public:

    FourBoard *board;

    /**
     * Constructor
     * @param master pointer to an initialised master instance
     */
    FourGame(FourMaster *master) {
        this->is_master = true;
        this->master = master;
        board = &(master->board);
    }

    /**
     * Constructor
     * @param slave pointer to an initialised slave instance
     */
    FourGame(FourSlave *slave) {
        this->is_master = false;
        this->slave = slave;
        board = &(slave->board);
    }

    /**
     * Constructor to play locally
     * @param master_
     * @param slave_
     */
    FourGame(FourMaster *master_, FourSlave *slave_) {
        this->master = master_;
        this->slave = slave_;
        board = &(master->board);
        this->is_master = true;
        this->debug = true;

    }

    /**
     * Destructor
     */
    ~FourGame() {

    }

    /**
     * Places a disc in column x.
     * @param x Column position
     */
    void makeMove(int x) {
        if (is_master) {
            master->makeMove(x);
        } else {
            slave->makeMove(x);
        }
        if (debug) {
            this->is_master = !this->is_master;
        }
    }

    /**
     * Checks if somebody has won
     * @return 0 means no winner yet, 1 / PLAYER_1 1 won and 2 / PLAYER_2 means player 2 won
     */
    char checkWin() {
        if (is_master) {
            return master->board.check_win();
        } else {
            return slave->board.check_win();
        }
    }

    /**
     * Tells you if it is your turn
     * @return true means your turn, false means not your turn
     */
    bool myTurn() {
        if (is_master) {
            return master->my_turn;
        } else {
            return slave->my_turn;
        }
    }

    /**
     * Tells you who you are
     * @return if you are master -> PLAYER_1, if you are slave -> PLAYER_2
     */
    char whoAmI() {
        if (is_master) {
            return PLAYER_1;
        } else {
            return PLAYER_2;
        }
    }

    /**
     * Resets the board
     */
    void restartGame() {
        if (is_master) {
            restarted = true;
            master->restartGame();
        }
    }

    /**
     * Tells you if the game was restarted
     * @return true or false
     */
    bool isGameRestarted() {
        if (is_master) {
            if (restarted) {
                restarted = false;
                return true;
            }

            return false;
        } else {
            return slave->isGameRestarted();
        }
    }

private:
    FourMaster *master;
    FourSlave *slave;

    bool is_master;
    bool debug = false;
    bool restarted = false;
};


#endif //TEAMPRAKTIKUM_FOUR_HPP