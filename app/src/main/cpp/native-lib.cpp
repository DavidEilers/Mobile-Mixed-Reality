#include <jni.h>
#include <string>

#include "Network.h"

Server server(8080);

extern "C" {

JNIEXPORT jstring
Java_com_example_teampraktikum_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "";
    if (server.is_listening()) {
        hello = "listening";
    } else {
        hello = "not listening";
    }
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void
Java_com_example_teampraktikum_MainActivity_startListening(JNIEnv *env, jobject /* this */) {
    server.start_listening();
}

JNIEXPORT void
Java_com_example_teampraktikum_MainActivity_stopListening(JNIEnv *env, jobject /* this */) {
    server.stop_listening();
}

JNIEXPORT void
Java_com_example_teampraktikum_MainActivity_sendString(JNIEnv *env, jobject /* this */) {
    server.send_string(0, "hallo test string", "192.168.43.53", 8080);
}

JNIEXPORT jobjectArray
Java_com_example_teampraktikum_MainActivity_getMessagesAsString(JNIEnv *env, jobject /* this */) {
    vector<MessageContainer> msgs = server.get_messages();

    jobjectArray ret = (jobjectArray) env->NewObjectArray(msgs.size(),
                                                          env->FindClass("java/lang/String"),
                                                          env->NewStringUTF(""));

    for (int i = 0; i < msgs.size(); i++) {
        char s[100];

        u_int16_t TYPE = msgs[i].message->TYPE;
        u_int16_t LENGTH = msgs[i].message->LENGTH;

        sprintf(s, "Message Object:\nTYPE: %d\nLENGTH: %d\nPAYLOAD: %s\n", TYPE, LENGTH,
                msgs[i].message->payload);

        env->SetObjectArrayElement(ret, i, env->NewStringUTF(s));
    }

    return ret;
}

}