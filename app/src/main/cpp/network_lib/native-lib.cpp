#include <jni.h>
#include <string>


#include <android/log.h>

#include "TicTacToe.hpp"
#include "NetworkInterface.h"

TTTMaster master;
TTTSlave slave;

bool hosting = false;

int fieldGetStatusAt(int x,int y) {
    //__android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikumNetwork","fieldGetStatusAt()");
    if (hosting) {
        return  master.board.get((int) x, (int) y);
    }
    return slave.board.get((int) x, (int) y);
}

void tick(){
    if(hosting){
        master.tick();
    }else{
        slave.tick();
    }
}



extern "C" {


JNIEXPORT void
Java_com_example_teampraktikum_ARCoreActivity_startConnection(JNIEnv *env, jobject /* this */,
                                                              jstring ip) {
    const char *ip_chars = env->GetStringUTFChars(ip, NULL);
    std::string ip_string = std::string(ip_chars);
    env->ReleaseStringUTFChars(ip, ip_chars);

    if (ip_string.empty()) {
        // HOST
        hosting = true;
        return;
    }
    // CLIENT
    hosting = false;
    slave.connect_to_master(ip_string, 7080);
}

JNIEXPORT void
Java_com_example_teampraktikum_TicTacToeActivity_boardPressedAt(JNIEnv *env, jobject /* this */, jint x,
                                                           jint y) {
    TTTClickMessage clickMessage;
    clickMessage.pos_x = x;
    clickMessage.pos_y = y;

    if (hosting) {
        master.send_to(clickMessage, master.get_connected_slaves()[0]);
        master.my_turn = false;
        master.board.set(x, y, PLAYER_X);
        return;
    }
    slave.send(clickMessage);
    slave.my_turn = false;
    slave.board.set(x, y, PLAYER_O);
}

JNIEXPORT void
Java_com_example_teampraktikum_TicTacToeActivity_startButtonPressed(JNIEnv *env, jobject /* this */,
                                                               jstring ip) {
    const char *ip_chars = env->GetStringUTFChars(ip, NULL);
    std::string ip_string = std::string(ip_chars);
    env->ReleaseStringUTFChars(ip, ip_chars);

    if (ip_string.empty()) {
        // HOST
        hosting = true;
        return;
    }
    // CLIENT
    hosting = false;
    slave.connect_to_master(ip_string, 7080);
}

JNIEXPORT jint
Java_com_example_teampraktikum_TicTacToeActivity_getStatusAt(JNIEnv *env, jobject /* this */, jint x,
                                                        jint y) {
    if (hosting) {
        return (jint) master.board.get((int) x, (int) y);
    }
    return (jint) slave.board.get((int) x, (int) y);
}

JNIEXPORT jboolean
Java_com_example_teampraktikum_TicTacToeActivity_getMyTurn(JNIEnv *env, jobject /* this */) {
    if (hosting) {
        return (jboolean) master.my_turn;
    }
    return (jboolean) slave.my_turn;
}

JNIEXPORT jboolean
Java_com_example_teampraktikum_TicTacToeActivity_getGameRunning(JNIEnv *env, jobject /* this */) {
    if (hosting) {
        return (jboolean) master.get_connected_slaves().size() > 0;
    }
    return (jboolean) slave.connected;
}
}