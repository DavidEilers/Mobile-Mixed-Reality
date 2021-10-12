#include <jni.h>
#include <string>


#include <android/log.h>

#include "TicTacToe.hpp"
#include "FourInARow.hpp"
#include "NetworkInterface.h"

Master * master= NULL;// = new TTTMaster();
Slave *slave = NULL;// = new TTTSlave();
std::string gameMode_string="TicTacToe";


bool hosting = false;


extern "C" {

jlong getTTTGame(){
    // master->tick();
    if(slave!= nullptr){
        //return (jlong) new TTTGame((TTTSlave*) (slave) );
    }else{
        // return (jlong) new TTTGame((TTTMaster*) (master));
    }
    return (jlong) nullptr;
}

jlong getFourGame(){
    // master->tick();
    if(slave!= nullptr){
        return (jlong) new FourGame((FourSlave*) (slave) );
    }else{
        return (jlong) new FourGame((FourMaster*) (master));
    }
}

JNIEXPORT jlong
Java_com_example_teampraktikum_HostGameActivity_getTTTGame(JNIEnv *env, jobject /* this */) {
    return getTTTGame();
}

JNIEXPORT jlong
Java_com_example_teampraktikum_HostGameActivity_getFourGame(JNIEnv *env, jobject /* this */) {
    return getFourGame();
}

JNIEXPORT jlong
Java_com_example_teampraktikum_JoinGameActivity_getTTTGame(JNIEnv *env, jobject /* this */) {
   return getTTTGame();
}

JNIEXPORT jlong
Java_com_example_teampraktikum_JoinGameActivity_getFourGame(JNIEnv *env, jobject /* this */) {
   return getFourGame();
}

JNIEXPORT jlong
Java_com_example_teampraktikum_HostGameActivity_getMaster(JNIEnv *env, jobject /* this */) {
   // master->tick();
    return (jlong) master;
}
JNIEXPORT jboolean
Java_com_example_teampraktikum_HostGameActivity_isSlaveConnected(JNIEnv *env, jobject /* this */,jstring gameMode) {
//master->tick();
    const char *gameMode_chars = env->GetStringUTFChars(gameMode, NULL);
    gameMode_string = std::string(gameMode_chars);
    if(master!=NULL){
        return master->slaves.size()>0?JNI_TRUE:JNI_FALSE;
    }
    if(gameMode_string=="TICTACTOE"){
        master = new TTTMaster();
    }else if(gameMode_string=="FOURINAROW"){
        master = new FourMaster();
    }else{
        return JNI_FALSE;//ERROR gameMode should be one of the above
    }
    return JNI_FALSE;

}

JNIEXPORT void
Java_com_example_teampraktikum_JoinGameActivity_slaveConnectToMaster(JNIEnv *env, jobject /* this */,jstring ip ,jstring gameMode) {
    const char *ip_chars = env->GetStringUTFChars(ip, NULL);
    std::string ip_string = std::string(ip_chars);
    const char *gameMode_chars = env->GetStringUTFChars(gameMode, NULL);
    gameMode_string = std::string(gameMode_chars);
    if(gameMode_string=="TICTACTOE"){
        slave = new TTTSlave();
    }else if(gameMode_string=="FOURINAROW"){
        slave = new FourSlave();
    }else{
        return;//ERROR gameMode should be one of the above
    }
    slave->connect_to_master(ip_string,7080);
    //slave->tick();
}

JNIEXPORT jboolean
Java_com_example_teampraktikum_JoinGameActivity_hasSlaveConnectToMasterSucceeded(JNIEnv *env, jobject /* this */) {
   // slave->tick();
   if(slave!=NULL){
    return slave->connected?JNI_TRUE:JNI_FALSE;
   }
   return JNI_FALSE;
}

JNIEXPORT jlong
Java_com_example_teampraktikum_JoinGameActivity_getSlave(JNIEnv *env, jobject /* this */) {
    return (jlong) slave;
}




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
    slave->connect_to_master(ip_string, 7080);
}

JNIEXPORT void
Java_com_example_teampraktikum_TicTacToeActivity_boardPressedAt(JNIEnv *env, jobject /* this */, jint x,
                                                           jint y) {
    TTTClickMessage clickMessage;
    clickMessage.pos_x = x;
    clickMessage.pos_y = y;
    if (hosting) {
        master->send_to(&clickMessage, master->get_connected_slaves()[0]);
        master->my_turn = false;
        master->board.set(x, y, PLAYER_X);
        return;
    }
    slave->send(&clickMessage);
    slave->my_turn = false;
    slave->board.set(x, y, PLAYER_O);
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
    slave->connect_to_master(ip_string, 7080);
}

JNIEXPORT jint
Java_com_example_teampraktikum_TicTacToeActivity_getStatusAt(JNIEnv *env, jobject /* this */, jint x,
                                                        jint y) {
    if (hosting) {
        return (jint) master->board.get((int) x, (int) y);
    }
    return (jint) slave->board.get((int) x, (int) y);
}

JNIEXPORT jboolean
Java_com_example_teampraktikum_TicTacToeActivity_getMyTurn(JNIEnv *env, jobject /* this */) {
    if (hosting) {
        return (jboolean) master->my_turn;
    }
    return (jboolean) slave->my_turn;
}

JNIEXPORT jboolean
Java_com_example_teampraktikum_TicTacToeActivity_getGameRunning(JNIEnv *env, jobject /* this */) {
    if (hosting) {
        return (jboolean) master->get_connected_slaves().size() > 0;
    }
    return (jboolean) slave->connected;
}
}