/*
 * Copyright 2009-2011 Cedric Priscal
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include <utils/Log.h>
#include "wallet.h"
static const char *TAG="WalletJniPort";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)
#ifdef __cplusplus
extern "C" {
#endif
#define EC_SIGNATURE_LEN     64
#define BIP32_SERIALIZED_LEN 78
namespace android {

static const char *classPathNameRx = "com/android/walletport/WalletJniPort";

static char* jstringToChar(JNIEnv* env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

static jint wallet_init(JNIEnv *env, jobject obj) {

    return 1;
}

static jint wallet_deinit(JNIEnv *env, jobject obj) {

    return 1;
}
static jint set_passphrase(JNIEnv *env, jobject obj, jstring passphrase){
    LOGD("hal Device set_passphrase \n");
    return change_pwd();
}

static jint create_mnemonic(JNIEnv *env, jobject obj, jstring passphrase, jstring language, jint number){
    LOGD("hal Device create_mnemonic \n");
    return create_wallet(number);
}

static jint recovery_mnemonic(JNIEnv *env, jobject obj, jstring passphrase, jstring language, jint number, jstring mnemonic){
    LOGD("hal Device recovery_mnemonic \n");
    return import_wallet(number, jstringToChar(env, mnemonic));
}

static jint start_derive(JNIEnv *env, jobject obj, jstring passphrase, jstring path, jint deriveAlgoId, jint signAlgoId, jint number, jstring callback){
    jint ret = 0;
    unsigned char pubkey[BIP32_SERIALIZED_LEN] = {0};
    jint pubkey_len = BIP32_SERIALIZED_LEN;
    LOGD("hal Device start_derive \n");
    ret = get_root_pubkey(jstringToChar(env, path), deriveAlgoId, signAlgoId, number, pubkey, &pubkey_len);
    jclass clazz = env->GetObjectClass(obj);
    LOGD("start_derive callback \n");
    jbyteArray jarray = env->NewByteArray(BIP32_SERIALIZED_LEN);
    env->SetByteArrayRegion(jarray, 0, BIP32_SERIALIZED_LEN, (jbyte*)pubkey);
    jmethodID mID = env->GetMethodID(clazz, "deriveCallback", "([B)V");
    env->CallVoidMethod(obj, mID, jarray);
    return ret;
}

static jint start_sign(JNIEnv *env, jobject obj, jstring passphrase, jstring path, jint deriveAlgoId, jint signAlgoId, jint number, jstring transhash, jstring callback){
    jint ret = 0;
    char pubkey[BIP32_SERIALIZED_LEN] = {0};
    char signhash[EC_SIGNATURE_LEN] = {0};
    LOGD("hal Device start_sign \n");
    ret = sign_transaction(jstringToChar(env, path), deriveAlgoId, signAlgoId, number, jstringToChar(env, transhash), pubkey, signhash);
    jclass clazz = env->GetObjectClass(obj);
    LOGD("start_sign callback \n");
    jbyteArray jpubkeyarray = env->NewByteArray(BIP32_SERIALIZED_LEN);
    jbyteArray jsignarray = env->NewByteArray(EC_SIGNATURE_LEN);
    env->SetByteArrayRegion(jpubkeyarray, 0, BIP32_SERIALIZED_LEN, (jbyte*)pubkey);
    env->SetByteArrayRegion(jsignarray, 0, EC_SIGNATURE_LEN, (jbyte*)signhash);
    jmethodID mID = env->GetMethodID(clazz, "signCallback", "([B[B)V");
    env->CallVoidMethod(obj, mID, jpubkeyarray, jsignarray);
    return ret;
}

static jint delete_wallet(JNIEnv *env, jobject obj, jstring passphrase){
    LOGD("hal Device delete_wallet \n");
    return remove_wallet();
}

void test_port(JNIEnv *env, jobject thiz){
    LOGD("jni test_port");
}

static JNINativeMethod methodsRx[] = {
    {"test", "()V", (void*)test_port},
    {"wallet_init", "()I", (void*)wallet_init},
    {"wallet_deinit", "()I", (void*)wallet_deinit},
    {"set_passphrase", "(Ljava/lang/String;)I", (void*)set_passphrase},
    {"create_mnemonic", "(Ljava/lang/String;Ljava/lang/String;I)I", (void*)create_mnemonic},
    {"recovery_mnemonic", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)I", (void*)recovery_mnemonic},
    {"start_derive", "(Ljava/lang/String;Ljava/lang/String;IIILjava/lang/String;)I", (void*)start_derive},
    {"start_sign", "(Ljava/lang/String;Ljava/lang/String;IIILjava/lang/String;Ljava/lang/String;)I", (void*)start_sign},
    {"delete_wallet", "(Ljava/lang/String;)I", (void*)delete_wallet}
};

/*
 * Register several native methods for one class.
 */
static jint registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    LOGD("%s, success\n", __func__);
    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static jint registerNatives(JNIEnv* env)
{
    jint ret = JNI_FALSE;

    if (registerNativeMethods(env, classPathNameRx,methodsRx,
        sizeof(methodsRx) / sizeof(methodsRx[0]))) {
        ret = JNI_TRUE;
    }

    LOGD("%s, done\n", __func__);
    return ret;
}

// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    LOGI("JNI_OnLoad");

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        goto fail;
    }
    env = uenv.env;

    if (registerNatives(env) != JNI_TRUE) {
        LOGE("ERROR: registerNatives failed");
        goto fail;
    }
    result = JNI_VERSION_1_4;

fail:
    return result;
}

}

#ifdef __cplusplus
}
#endif
