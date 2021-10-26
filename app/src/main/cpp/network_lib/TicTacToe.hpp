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
#define BOARD_FULL 3

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

    /**
     * Resets the board, every position is set to FIELD_EMPTY
     */
    void reset() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = FIELD_EMPTY;
            }
        }
    }

    /**
     * Sets the state at a given board position
     * @param x x position
     * @param y y position
     * @param state PLAYER_X, PLAYER_O or FIELD_EMPTY
     */
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

        int used_field_counter = 0;

        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                if (board[x][y] != FIELD_EMPTY) used_field_counter++;
            }
        }

        if (used_field_counter >= 9) {
            return BOARD_FULL;
        }

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

    /**
     * Set the coordinates at which the click happened
     * @param x x position
     * @param y y position
     */
    void set_coords(char x, char y) {
        pos_x = x;
        pos_y = y;
    }

    /**
     * Serializes this Object and stores it inside the given buffer
     * @param buffer buffer to store the object in
     * @return count of written bytes
     */
    virtual int to_bytes(char *buffer) override {
        __android_log_print(ANDROID_LOG_VERBOSE, "TTTClick -> to_bytes", "%d", TYPE);
        buffer[0] = TYPE;
        buffer[1] = pos_x;
        buffer[2] = pos_y;
        return 3;
    }

    /**
     * Deserializes a buffer into this Object. Configures this object with the information in the buffer.
     * @param buffer buffer to read from
     * @return true if buffer could be deserialized
     */
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
 * Stores the TicTacToe board state to be sent via network
 * also stores if it is the hosts turn or not
 */
class TTTBoardUpdateMessage : public BaseMessage {
public:
    const char TYPE = MSG_TTT_BOARD_UPDATE;

    TTTBoard *board;

    char hosts_turn = 0;

    char new_game = 0;

    /**
     * Constructor
     * @param board_ptr pointer to the board of a TTTBoard to update
     */
    TTTBoardUpdateMessage(TTTBoard *board_ptr) {
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
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                buffer[i] = board->board[x][y];
                i++;
            }
        }
        return i;
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
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                this->board->board[x][y] = buffer[i];
                i++;
            }
        }
        return true;
    }
};

/**
 * Represents a master in a tic tac toe game
 */
class TTTMaster : public Master {
public:
    TTTBoard board;

    bool my_turn = true;

    /**
     * Constructor
     */
    TTTMaster() : Master("master", 7080, 1) {}

    /**
     * Handles game specific messages
     * @param container container with the message
     */
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

    /**
     * Tells you if it is your turn
     * @return true -> yes, false -> no
     */
    bool isMyTurn() {
        return my_turn;
    }
};

/**
 * Represents a slave in a tic tac toe game
 */
class TTTSlave : public Slave {
public:
    TTTBoard board;

    bool my_turn = false;

    /**
     * Constructor
     */
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

            if (bum.new_game > 0) restarted = true;
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
        TTTClickMessage *cm = new TTTClickMessage();
        cm->pos_x = x;
        cm->pos_y = y;

        send(cm);
    }

    /**
     * Tells you if it is your turn
     * @return true -> yes, false -> no
     */
    bool isMyTurn() {
        return my_turn;
    }

    bool isGameRestarted() {
        if (restarted) {
            restarted = false;
            return true;
        } else {
            return false;
        }
    }

private:
    bool restarted = false;
};

/**
 * Unifies FourMaster and FourSlave into one Object to use for network communication
 */
class TTTGame {
public:
    TTTBoard *board;

    /**
     * Constructor
     * @param master pointer to an initialised master instance
     */
    TTTGame(TTTMaster *master) {
        this->is_master = true;
        this->master = master;
        board = &(master->board);
    }

    /**
     * Constructor
     * @param slave pointer to an initialised slave instance
     */
    TTTGame(TTTSlave *slave) {
        this->is_master = false;
        this->slave = slave;
        board = &(slave->board);
        if (slave == NULL) {
            __android_log_print(ANDROID_LOG_VERBOSE, "TTTGame", "Slave==NULL");
        }
    }

    /**
     * Constructor to play locally
     * @param master_
     * @param slave_
     */
    TTTGame(TTTMaster *master_, TTTSlave *slave_) {
        this->master = master_;
        this->slave = slave_;
        board = &(slave->board);
        this->debug = true;
        this->is_master = true;
    }

    /**
     * Used to make a game move
     * @param x x position
     * @param y y position
     */
    void makeMove(int x, int y) {
        if (debug) {
            this->is_master = !this->is_master;
        }
        if (is_master) {
            master->makeMove(x, y);
        } else {
            slave->makeMove(x, y);
        }
    }

    /**
     * Tells you if someone has won the game already
     * @return 0 -> no winner yet, 1 -> PLAYER_O, 2 -> PLAYER_X, 3 -> board is full
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
     * @return true -> yes, false -> no
     */
    bool myTurn() {
        if (is_master) {
            return master->my_turn;
        } else {
            return slave->my_turn;
        }
    }

    /**
     * Tells you which player you are
     * @return PLAYER_O or PLAYER_X
     */
    char whoAmI() {
        if (is_master) {
            return PLAYER_O;
        } else {
            return PLAYER_X;
        }
    }

    /**
     * Restarts the game if you are the master
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
    TTTMaster *master;
    TTTSlave *slave;

    bool is_master;
    bool debug = false;
    bool restarted = false;
};

#endif //TEAMPRAKTIKUM_TICTACTOE_HPP