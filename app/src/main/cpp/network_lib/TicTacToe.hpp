#include <vector>
#include <string>
#include "GameManager.h"
#include "Messages.hpp"

#ifndef TEAMPRAKTIKUM_TICTACTOE_HPP
#define TEAMPRAKTIKUM_TICTACTOE_HPP


#define MSG_TTT_CLICK 100
#define MSG_TTT_END 101

#define FIELD_EMPTY 0
#define PLAYER_O 1
#define PLAYER_X 2

/**
 * class that holds the game board state
 */
class TTTBoard {
public:
    char board[3][3];

    TTTBoard() {
        reset();
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
};

/**
 * class for storing the click message for a Tic Tac Toe board
 */
class TTTClickMessage : public BaseMessage {
public:
    static const char TYPE = MSG_TTT_CLICK;

    char pos_x, pos_y;

    void set_coords(char x, char y) {
        pos_x = x;
        pos_y = y;
    }

    virtual int to_bytes(char *buffer) {
        buffer[0] = TYPE;
        buffer[1] = pos_x;
        buffer[2] = pos_y;
        return 3;
    }

    static bool from_bytes(char *buffer, TTTClickMessage *msg_obj) {
        if (buffer[0] != TYPE) return false;
        msg_obj->pos_x = buffer[1];
        msg_obj->pos_y = buffer[2];
        return true;
    }
};

/**
 * this message class stores who has won the game
 */
class TTTGameEndMessage : public BaseMessage {
public:
    static const char TYPE = MSG_TTT_END;

    bool host_won = false;

    virtual int to_bytes(char *buffer) {
        buffer[0] = TYPE;
        if (host_won) buffer[1] = 1;
        else buffer[1] = 0;
        return 2;
    }

    static bool from_bytes(char *buffer, TTTGameEndMessage *msg_obj) {
        if (buffer[0] != TYPE) return false;
        msg_obj->host_won = buffer[1] == 1;
        return true;
    }
};


class TTTMaster : public Master {
public:
    TTTBoard board;

    bool my_turn = true;

    TTTMaster() : Master("master", 7080, 1) {}

    virtual void handleMessages(std::vector<RawContainer> &containers) {
        for (auto container : containers) {
            TTTClickMessage clickMessage;
            if (TTTClickMessage::from_bytes(container.buffer, &clickMessage)) {
                //Master is always X and Slave always O
                board.set(clickMessage.pos_x, clickMessage.pos_y, PLAYER_O);

                my_turn = true;
            }
        }

        TTTGameEndMessage em;

        switch (board.check_win()) {
            case PLAYER_O:
                em.host_won = false;
                if (slaves.size() > 0) send_to(em, slaves[0]);
                board.reset();
                break;
            case PLAYER_X:
                em.host_won = true;
                if (slaves.size() > 0) send_to(em, slaves[0]);
        }
    }
};

class TTTSlave : public Slave {
public:
    TTTBoard board;

    bool my_turn = false;

    TTTSlave() : Slave("slave", 7081) {}

    virtual void handleMessages(std::vector<RawContainer> &containers) {
        for (auto container : containers) {
            TTTClickMessage clickMessage;
            if (TTTClickMessage::from_bytes(container.buffer, &clickMessage)) {
                //Master is always X and Slave always O
                board.set(clickMessage.pos_x, clickMessage.pos_y, PLAYER_X);

                my_turn = true;
            }

            TTTGameEndMessage em;
            if (TTTGameEndMessage::from_bytes(container.buffer, &em)) {
                LeaveMessage lm;
                send(lm);
                board.reset();
            }
        }
    }
};


#endif //TEAMPRAKTIKUM_TICTACTOE_HPP