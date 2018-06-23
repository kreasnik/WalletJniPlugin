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

int big_endian_input(char *data, int index, UINT16 input){
    data[index] = input & 0x00ff;
    data[index+1] = (input & 0xff00) >> 8;
    return 2;
}

int big_endian_output(char *data, int index, UINT16 *output){
    *output = data[index] | data[index+1] << 8;
    return 2;
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
    message.header.is_ready = 0x55;
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
/**
*   参数顺序
*   1,UINT16 password_len
*   2,char* passphrase
*/
int ioc_set_passphrase(char *passphrase){
    int result = 0;
    UINT16 password_len;
    password_len = strlen(passphrase);
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x55;
    message.header.id = AT_M2S_SET_PWD;

    i += big_endian_input(message.para, i, password_len);

    strncpy(message.para, passphrase, password_len);
    message.header.len = i + password_len;
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
/**
*   参数顺序
*   1,UINT16 number
*   2,UINT16 password_len
*   3,UINT16 mne_len
*   5,char* passphrase
*   6,char* mnemonic
*/
int ioc_save_mnemonic(char *passphrase, char *language, UINT16 number, char *mnemonic){
    int result = 0;
    int i = 0;
    UINT16 password_len, mne_len, language_len;
    password_len = strlen(passphrase);
    mne_len = strlen(mnemonic);

    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x55;
    message.header.id = AT_M2S_SAVE_MNEMONIC;

    i += big_endian_input(message.para, i, number);
    i += big_endian_input(message.para, i, password_len);
    i += big_endian_input(message.para, i, mne_len);

    strncpy(message.para+i, passphrase, password_len);
    strncpy(message.para+i+password_len, mnemonic, mne_len);
    message.header.len = i + password_len + mne_len;

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
/**
*   参数顺序
*   1,UINT16 number
*   2,UINT16 password_len
*   3,UINT16 mne_len
*   4,char* passphrase
*   5,char* mnemonic
*/
int ioc_recovery_mnemonic(char *passphrase, char *language, UINT16 number, char *mnemonic){
    int result = 0;
    int i = 0;
    UINT16 password_len, mne_len, language_len;
    password_len = strlen(passphrase);
    mne_len = strlen(mnemonic);

    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x55;
    message.header.id = AT_M2S_RECOVER_WALLET;

    i += big_endian_input(message.para, i, number);
    i += big_endian_input(message.para, i, password_len);
    i += big_endian_input(message.para, i, mne_len);

    strncpy(message.para+i, passphrase, password_len);
    strncpy(message.para+i+password_len, mnemonic, mne_len);
    message.header.len = i + password_len + mne_len;

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
/**
*   输入参数顺序
*   1,UINT16 number
*   2,UINT16 password_len
*   3,UINT16 deriveAlgoId
*   4,UINT16 signAlgoId
*   5,UINT16 path_len
*   6,char* passphrase
*   7,char* path
*   输出参数顺序
*   1,UINT16 pubkey_len
*   2,char* pubkey
*/
int ioc_start_derive(char *passphrase, char *path,
                        UINT16 deriveAlgoId, UINT16 signAlgoId,
                        UINT16 number, char *pubkey){
    int result = 0;
    int i = 0;
    UINT16 password_len, path_len;
    UINT16 pubkey_len;
    password_len = strlen(passphrase)
    path_len = strlen(path);
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x55;
    message.header.id = AT_M2S_GET_PUBKEY;

    i += big_endian_input(message.para, i, number);
    i += big_endian_input(message.para, i, password_len);
    i += big_endian_input(message.para, i, deriveAlgoId);
    i += big_endian_input(message.para, i, signAlgoId);
    i += big_endian_input(message.para, i, path_len);

    strncpy(message.para+i, passphrase, password_len);
    strncpy(message.para+i+password_len, path, path_len);
    message.header.len = i + password_len + path_len;
    result = ioc_spi_write();
    LOGD("ioc_start_derive result = %d\n", result);
    if(result == 1){
        ioc_spi_read();
        if(message.header.is_ready == 0x55 && message.header.id == AT_S2M_GET_PUBKEY_RSP){
            i = 0;
            i += big_endian_output(message.para, i, &pubkey_len);
            strncpy(pubkey, message.para+i, pubkey_len);
            result = 1;
        }else{
            result = 0;
        }
    }
    return result;
}
/**
*   输入参数顺序
*   1,UINT16 number
*   2,UINT16 password_len
*   3,UINT16 deriveAlgoId
*   4,UINT16 signAlgoId
*   5,UINT16 path_len
*   6,UINT16 transhash_len
*   7,char* passphrase
*   8,char* path
*   9,char* transhash
*   输出参数顺序
*   1,UINT16 pubkey_len
*   2,UINT16 signhash_len
*   3,char* pubkey
*   4,char* signhash
*/
int ioc_start_sign(char *passphrase, char *path,
                    UINT16 deriveAlgoId, UINT16 signAlgoId,
                    UINT16 number, char *transhash,
                    char *pubkey, char *signhash){
    int result = 0;
    int i = 0;
    UINT16 password_len, path_len, transhash_len;
    UINT16 pubkey_len, signhash_len;
    password_len = strlen(passphrase)
    path_len = strlen(path);
    transhash_len = strlen(signhash);
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x55;
    message.header.id = AT_M2S_GET_PUBKEY;

    i += big_endian_input(message.para, i, number);
    i += big_endian_input(message.para, i, password_len);
    i += big_endian_input(message.para, i, deriveAlgoId);
    i += big_endian_input(message.para, i, signAlgoId);
    i += big_endian_input(message.para, i, path_len);
    i += big_endian_input(message.para, i, transhash_len);

    strncpy(message.para+i, passphrase, password_len);
    strncpy(message.para+i+password_len, path, path_len);
    strncpy(message.para+i+password_len+path_len, transhash, transhash_len);
    message.header.len = i + password_len + path_len + transhash_len;
    result = ioc_spi_write();
    LOGD("ioc_start_sign result = %d\n", result);
    if(result == 1){
        ioc_spi_read();
        if(message.header.is_ready == 0x55 && message.header.id == AT_S2M_SIGN_TRANX_RSP){
            i = 0;
            i += big_endian_output(message.para, i, &pubkey_len);
            i += big_endian_output(message.para, i, &signhash_len);
            strncpy(pubkey, message.para+i, pubkey_len);
            strncpy(signhash, message.para+i+pubkey_len, signhash_len);
            result = 1;
        }else{
            result = 0;
        }
    }
    return result;
}
/**
*   参数顺序
*   1,UINT16 password_len
*   2,char* passphrase
*/
int ioc_delete_wallet(char *passphrase){
    int result = 0;
    UINT16 password_len;
    password_len = strlen(passphrase);
    memset(&message, 0, sizeof(message));
    message.header.is_ready = 0x55;
    message.header.id = AT_M2S_DEL_WALLET;

    i += big_endian_input(message.para, i, password_len);

    strncpy(message.para, passphrase, password_len);
    message.header.len = i + password_len;
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