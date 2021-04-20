#include <jni.h>
#include <string>

#include "TicTacToe.hpp"

TTTMaster master;
TTTSlave slave;

bool hosting = false;

extern "C" {

JNIEXPORT void
Java_com_example_teampraktikum_MainActivity_boardPressedAt(JNIEnv *env, jobject /* this */, jint x,
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
Java_com_example_teampraktikum_MainActivity_startButtonPressed(JNIEnv *env, jobject /* this */,
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
    slave.connect_to_master(ip_string, 8080);
}

JNIEXPORT jint
Java_com_example_teampraktikum_MainActivity_getStatusAt(JNIEnv *env, jobject /* this */, jint x,
                                                        jint y) {
    if (hosting) {
        return (jint) master.board.get((int) x, (int) y);
    }
    return (jint) slave.board.get((int) x, (int) y);
}

JNIEXPORT jboolean
Java_com_example_teampraktikum_MainActivity_getMyTurn(JNIEnv *env, jobject /* this */) {
    if (hosting) {
        return (jboolean) master.my_turn;
    }
    return (jboolean) slave.my_turn;
}

JNIEXPORT jboolean
Java_com_example_teampraktikum_MainActivity_getGameRunning(JNIEnv *env, jobject /* this */) {
    if (hosting) {
        return (jboolean) master.get_connected_slaves().size() > 0;
    }
    return (jboolean) slave.connected;
}
}