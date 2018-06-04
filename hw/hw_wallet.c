#define TAG "wallet-hal"

#include "ioctl_wallet.h"
#include "hw_wallet.h"
#include <errno.h>
#include <string.h>
#include <cutils/log.h>

#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

static int wallet_init(struct wallet *dev) {

    LOGD("wallet_init - init");
    return open_dev();
}

static int wallet_deinit(struct wallet *dev) {

    LOGD( "wallet_deinit - deinit");
    return close_dev();
}

static int wallet_set_passphrase(struct wallet *dev, char *passphrase){
    int result = 0;
    LOGD("wallet_set_passphrase - passphrase = %s", passphrase);
    result = ioc_set_passphrase(passphrase);
    return result;
}

static int wallet_create_mnemonic(struct wallet *dev, char *passphrase, char *language, UINT16 number){
    int result = 0;
    char entropy[PRIV_KEY_BIT] = {0};
    char *mnemonic = "point cabin meat buffalo skull arm sense tilt shrimp distance nose medal";
    LOGD("wallet_create_mnemonic - passphrase = %s, language = %s, number = %d", passphrase, language, number);
    result = ioc_create_mnemonic(&entropy);
    //entropy -> mnemonic
    result = ioc_save_mnemonic(passphrase, language, number, mnemonic);
    return result;
}

static int wallet_save_mnemonic(struct wallet *dev, char *passphrase, char *language, UINT16 number, char *mnemonic){
    int result = 0;
    result = ioc_save_mnemonic(passphrase, language, number, mnemonic);
    return result;
}

static int wallet_recovery_mnemonic(struct wallet *dev, char *passphrase, char *language, UINT16 number, char *mnemonic){
    int result = 0;
    result = ioc_recovery_mnemonic(passphrase, language, number, mnemonic);
    return result;
}

static int wallet_start_derive(struct wallet *dev, char *passphrase, char *path,
                                UINT16 deriveAlgoId, UINT16 signAlgoId, UINT16 number,
                                char *pubkey){
    int result = 0;
    LOGD("wallet_start_derive - passphrase = %s, path = %s, number = %d", passphrase, path, number);
    result = ioc_start_derive(passphrase, path, deriveAlgoId, signAlgoId, number, pubkey);
    return result;
}

static int wallet_start_sign(struct wallet *dev, char *passphrase, char *path,
                                UINT16 deriveAlgoId, UINT16 signAlgoId, UINT16 number,
                                char *transhash, char *pubkey, char *signhash){
    int result = 0;
    LOGD("wallet_start_sign - passphrase = %s, path = %s, number = %d, transhash = %s", passphrase, path, number, transhash);
    result = ioc_start_sign(passphrase, path, deriveAlgoId, signAlgoId, number, transhash, pubkey, signhash);
    return result;
}

static int wallet_delete_wallet(struct wallet *dev, char *passphrase){
    int result = 0;
    LOGD("wallet_delete_wallet - passphrase = %s", passphrase);
    result = ioc_delete_wallet(passphrase);
    return result;
}
/*
 * Generic device handling
 */

static int wallet_close(hw_device_t *dev) {
    free(dev);
    return 0;
}

static int wallet_open(const hw_module_t* module, const char* name,
        hw_device_t** device) {
    if (device == NULL) {
        LOGE("NULL device on open");
        return -EINVAL;
    }

    LOGD("Device open");
    wallet_t *dev = malloc(sizeof(wallet_t));
    memset(dev, 0, sizeof(wallet_t));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*) module;
    dev->common.close = wallet_close;
#if 1
    dev->wallet_init = wallet_init;
    dev->wallet_deinit = wallet_deinit;
    dev->wallet_set_passphrase = wallet_set_passphrase;
    dev->wallet_create_mnemonic = wallet_create_mnemonic;
    dev->wallet_recovery_mnemonic = wallet_recovery_mnemonic;
    dev->wallet_start_derive = wallet_start_derive;
    dev->wallet_start_sign = wallet_start_sign;
    dev->wallet_delete_wallet = wallet_delete_wallet;
#endif
    *device = (hw_device_t*) dev;
    return 0;
}


static struct hw_module_methods_t wallet_module_methods = {
        .open = wallet_open,
};

wallet_module_t HAL_MODULE_INFO_SYM = {
        .common = {
                .tag = HARDWARE_MODULE_TAG,
                .module_api_version = WALLET_MODULE_API_VERSION_1_1,
                .hal_api_version = HARDWARE_HAL_API_VERSION,
                .id = WALLET_HARDWARE_MODULE_ID,
                .name = "wallet",
                .author = "The Android Open Source Project",
                .methods = &wallet_module_methods,
        },
};