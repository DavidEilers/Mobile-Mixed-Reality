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
    BaseMessage bm;
    bm.payload = "hello test string";
    server.send_message(bm, "192.168.43.53", 8080);
}

JNIEXPORT jobjectArray
Java_com_example_teampraktikum_MainActivity_getMessagesAsString(JNIEnv *env, jobject /* this */) {
    vector<RawContainer> msgs = server.get_messages();

    jobjectArray ret = (jobjectArray) env->NewObjectArray(msgs.size(),
                                                          env->FindClass("java/lang/String"),
                                                          env->NewStringUTF(""));

    for (int i = 0; i < msgs.size(); i++) {
        char s[1023];

        BaseMessage bm;

        if (BaseMessage::from_bytes(msgs[i].buffer, &bm)) {
            sprintf(s, "Base Message Object: PAYLOAD: %s\n", bm.payload.c_str());
        } else {
            sprintf(s, "Another Message Object");
        }

        env->SetObjectArrayElement(ret, i, env->NewStringUTF(s));
    }

    return ret;
}

}