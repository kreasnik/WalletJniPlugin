#define TAG "wallet-hal"

#include <errno.h>
#include <string.h>
#include <cutils/log.h>
#include "ioctl_wallet.h"
#include "hw_wallet.h"
#include "english.h"
#include "sha2.h"

#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

const char *mnemonic_from_data(const UINT8 *data, int len, char* language){
	if (len != PRIV_KEY_BIT) {
	    LOGD("mnemonic_from_data - must be 32byte");
		return 0;
	}

    UINT8 checksum[PRIV_KEY_BIT] = {0};
	UINT8 bits[PRIV_KEY_BIT + 1] = {0};

	memcpy(bits, data, len);
	sha256(bits, ENTROPY_128, checksum);
	bits[ENTROPY_128] = (bits[ENTROPY_128] & 0x0f) | (checksum[0] & 0xf0); //00001111 11110000
	sha256(bits, ENTROPY_160, checksum);
	bits[ENTROPY_160] = (bits[ENTROPY_160] & 0x07) | (checksum[0] & 0xf8); //00000111 11111000
	sha256(bits, ENTROPY_192, checksum);
	bits[ENTROPY_192] = (bits[ENTROPY_192] & 0x03) | (checksum[0] & 0xfc); //00000011 11111100
	sha256(bits, ENTROPY_224, checksum);
	bits[ENTROPY_224] = (bits[ENTROPY_224] & 0x01) | (checksum[0] & 0xfe); //00000001 11111110
	sha256(bits, ENTROPY_256, checksum);
	bits[ENTROPY_256] = (bits[ENTROPY_256] & 0x01) | (checksum[0] & 0xff); //00000000 11111111

	int mlen = len * 3 / 4;
	static char mnemo[MNEMO_LEN * 10];
	int i, j, idx;
	char *p = mnemo;
	for (i = 0; i < mlen; i++) {
		idx = 0;
		for (j = 0; j < 11; j++) {
			idx <<= 1;
			idx += (bits[(i * 11 + j) / 8] & (1 << (7 - ((i * 11 + j) % 8)))) > 0;
		}
		strcpy(p, wordlist[idx]);
		p += strlen(wordlist[idx]);
		*p = (i < mlen - 1) ? ' ' : 0;
		p++;
	}

	return mnemo;
}

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
    char *mnemonic;
    LOGD("wallet_create_mnemonic - passphrase = %s, language = %s, number = %d", passphrase, language, number);
    result = ioc_create_mnemonic(&entropy);
    //entropy -> mnemonic
    mnemonic = mnemonic_from_data(entropy, strlen(entropy), language);
    LOGD("wallet_create_mnemonic - mnemonic = %s", mnemonic);
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