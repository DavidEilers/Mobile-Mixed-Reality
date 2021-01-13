#include <jni.h>
#include <string>

#include "Network.h"

Server server(8080);

extern "C" JNIEXPORT jstring Java_com_example_teampraktikum_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) {
    std::string hello = "";
    if(server.isListening()){
        hello = "listening";
    }else{
        hello = "not listening";
    }
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void Java_com_example_teampraktikum_MainActivity_startListening(JNIEnv* env, jobject /* this */) {
    server.startListening();
}

extern "C" JNIEXPORT void Java_com_example_teampraktikum_MainActivity_stopListening(JNIEnv* env, jobject /* this */) {
    server.stopListening();
}

extern "C" JNIEXPORT void Java_com_example_teampraktikum_MainActivity_sendString(JNIEnv* env, jobject /* this */) {
    sendString("hallo test string", "192.168.43.53", 8080);
}

extern "C" JNIEXPORT jobjectArray Java_com_example_teampraktikum_MainActivity_getMessages(JNIEnv* env, jobject /* this */) {
    vector<message> msgs = server.getMessages();

    jobjectArray ret = (jobjectArray)env->NewObjectArray(msgs.size(),env->FindClass("java/lang/String"),env->NewStringUTF(""));

    for(int i = 0; i < msgs.size(); i++){
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(msgs[i].text.c_str()));
    }

    return ret;
}