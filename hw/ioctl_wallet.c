//
// Created by xuejin on 18-6-2.
//

#include "ioctl_wallet.h"
#define TAG "wallet-hal"

#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

int open_dev(){
    LOGD("open_dev\n");
    fd = open(NODE,O_RDWR);
    if (fd < 0){
        LOGE("Open Dev %s Error", NODE);
        return 0;
    }
    return 1;
}

int close_dev(){
    if(fd < 0){
        return 0;
    }else{
        close(fd);
    }
    return 1;
}

int ioc_spi_read(){
    memset(&message, 0, sizeof(message));
    if (ioctl(fd, IOC_SPI_READ, &message) < 0){
        return 0;
    }
    return 1;
}

int ioc_spi_write(){
    if (ioctl(fd, IOC_SPI_WRITE, &message) < 0){
        return 0;
    }
    return 1;
}

int ioc_create_mnemonic(char *entropy){
    int result = 0;
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x00;
    message.header.id = AT_M2S_GEN_WALLET;
    message.header.len = 0;
    result = ioc_spi_write();
    LOGD("ioc_create_mnemonic result = %d\n", result);
    if(result == 1){
        ioc_spi_read();
        if(message.header.is_ready == 0x55 && message.header.id == AT_S2M_GEN_WALLET_RSP){
            strncpy(entropy, message.para, message.header.len);
            result = 1;
        }else{
            result = 0;
        }
    }
    return result;
}

int ioc_set_passphrase(char *passphrase){
    int result = 0;
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x00;
    message.header.id = AT_M2S_SET_PWD;
    message.header.len = strlen(passphrase);
    strncpy(message.para, passphrase, strlen(passphrase));
    result = ioc_spi_write();
    LOGD("ioc_set_passphrase result = %d\n", result);
    if(result == 1){
        ioc_spi_read();
        if(message.header.is_ready == 0x55 && message.header.id == AT_S2M_SET_PWD_RSP){
            result = 1;
        }else{
            result = 0;
        }
    }
    return result;
}

int ioc_save_mnemonic(char *passphrase, char *language, UINT16 number, char *mnemonic){
    int result = 0;
    UINT16 mne_len;
    mne_len = strlen(mnemonic);
    char st[UINT_LEN] = {0};
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x00;
    message.header.id = AT_M2S_SAVE_MNEMONIC;
    strncpy(message.para, passphrase, strlen(passphrase));
    sprintf(st, "%d", number);
    strncat(message.para, st, sizeof(st));
    sprintf(st, "%d", mne_len);
    strncat(message.para, st, sizeof(st));
    strncat(message.para, mnemonic, strlen(mnemonic));
    message.header.len = strlen(message.para);
    result = ioc_spi_write();
    LOGD("ioc_save_mnemonic result = %d\n", result);
    if(result == 1){
        ioc_spi_read();
        if(message.header.is_ready == 0x55 && message.header.id == AT_S2M_SAVE_MNEMONIC_RSP){
            result = 1;
        }else{
            result = 0;
        }
    }
    return result;
}

int ioc_recovery_mnemonic(char *passphrase, char *language, UINT16 number, char *mnemonic){
    int result = 0;
    UINT16 mne_len;
    mne_len = strlen(mnemonic);
    char st[UINT_LEN] = {0};
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x00;
    message.header.id = AT_M2S_RECOVER_WALLET;
    strncpy(message.para, passphrase, strlen(passphrase));
    sprintf(st, "%d", number);
    strncat(message.para, st, sizeof(st));
    sprintf(st, "%d", mne_len);
    strncat(message.para, st, sizeof(st));
    strncat(message.para, mnemonic, strlen(mnemonic));
    message.header.len = strlen(message.para);
    result = ioc_spi_write();
    LOGD("ioc_recovery_mnemonic result = %d\n", result);
    if(result == 1){
        ioc_spi_read();
        if(message.header.is_ready == 0x55 && message.header.id == AT_S2M_RECOVER_WALLET_RSP){
            result = 1;
        }else{
            result = 0;
        }
    }
    return result;
}

int ioc_start_derive(char *passphrase, char *path,
                        UINT16 deriveAlgoId, UINT16 signAlgoId,
                        UINT16 number, char *pubkey){
    int result = 0;
    UINT16 derive_path_len;
    UINT16 pubkey_len;
    char st[UINT_LEN] = {0};
    derive_path_len = strlen(path);
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x00;
    message.header.id = AT_M2S_GET_PUBKEY;
    strncpy(message.para, passphrase, strlen(passphrase));
    sprintf(st, "%d", number);
    strncat(message.para, st, sizeof(st));
    sprintf(st, "%d", deriveAlgoId);
    strncat(message.para, st, sizeof(st));
    sprintf(st, "%d", signAlgoId);
    strncat(message.para, st, sizeof(st));
    sprintf(st, "%d", derive_path_len);
    strncat(message.para, st, sizeof(st));
    strncat(message.para, path, strlen(path));
    message.header.len = strlen(message.para);
    result = ioc_spi_write();
    LOGD("ioc_start_derive result = %d\n", result);
    if(result == 1){
        ioc_spi_read();
        if(message.header.is_ready == 0x55 && message.header.id == AT_S2M_GET_PUBKEY_RSP){
            memset(st, 0, sizeof(st));
			strncpy(st, message.para, sizeof(st));
			pubkey_len = atoi(st);
            strncpy(pubkey, message.para+sizeof(st), pubkey_len);
            result = 1;
        }else{
            result = 0;
        }
    }
    return result;
}

int ioc_start_sign(char *passphrase, char *path,
                    UINT16 deriveAlgoId, UINT16 signAlgoId,
                    UINT16 number, char *transhash,
                    char *pubkey, char *signhash){
    int result = 0;
    UINT16 derive_path_len;
    UINT16 trans_hash_len;
    UINT16 pubkey_len;
    UINT16 signhash_len;
    char st[UINT_LEN] = {0};
    derive_path_len = strlen(path);
    trans_hash_len = strlen(transhash);
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x00;
    message.header.id = AT_M2S_SIGN_TRANX;
    strncpy(message.para, passphrase, strlen(passphrase));
    sprintf(st, "%d", number);
    strncat(message.para, st, sizeof(st));
    sprintf(st, "%d", deriveAlgoId);
    strncat(message.para, st, sizeof(st));
    sprintf(st, "%d", signAlgoId);
    strncat(message.para, st, sizeof(st));
    sprintf(st, "%d", trans_hash_len);
    strncat(message.para, st, sizeof(st));
    sprintf(st, "%d", derive_path_len);
    strncat(message.para, st, sizeof(st));
    strncat(message.para, transhash, strlen(transhash));
    strncat(message.para, path, strlen(path));
    message.header.len = strlen(message.para);
    result = ioc_spi_write();
    LOGD("ioc_start_sign result = %d\n", result);
    if(result == 1){
        ioc_spi_read();
        if(message.header.is_ready == 0x55 && message.header.id == AT_S2M_SIGN_TRANX_RSP){
            memset(st, 0, sizeof(st));
			strncpy(st, message.para, sizeof(st));
			pubkey_len = atoi(st);
            memset(st, 0, sizeof(st));
			strncpy(st, message.para+sizeof(st), sizeof(st));
			signhash_len = atoi(st);
            strncpy(pubkey, message.para+sizeof(st)*2, pubkey_len);
            strncpy(signhash, message.para+sizeof(st)*2+pubkey_len, signhash_len);
            result = 1;
        }else{
            result = 0;
        }
    }
    return result;
}

int ioc_delete_wallet(char *passphrase){
    int result = 0;
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x00;
    message.header.id = AT_M2S_DEL_WALLET;
    message.header.len = strlen(passphrase);
    strncpy(message.para, passphrase, strlen(passphrase));
    result = ioc_spi_write();
    LOGD("ioc_delete_wallet result = %d\n", result);
    if(result == 1){
        ioc_spi_read();
        if(message.header.is_ready == 0x55 && message.header.id == AT_S2M_DEL_WALLET_RSP){
            result = 1;
        }else{
            result = 0;
        }
    }
    return result;
}