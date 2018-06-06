//
// Created by xuejin on 18-6-2.
//

#ifndef WALLETJNIPLUGIN_IOCTL_WALLET_H
#define WALLETJNIPLUGIN_IOCTL_WALLET_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <android/log.h>

typedef signed           char INT8;
typedef signed short     int  INT16;
typedef signed           int  INT32;

typedef unsigned           char UINT8;
typedef unsigned short     int  UINT16;
typedef unsigned           int  UINT32;

typedef unsigned           char BYTE;
typedef unsigned short     int  WORD;
typedef unsigned           int  DWORD;
typedef unsigned           char * PBYTE;
typedef unsigned short     int  * PWORD;
typedef unsigned           int  * PDWORD;

#define TRUE  1
#define FALSE 0

#define UINT_LEN             4
#define PRIV_KEY_BIT         32
#define BIP32_SERIALIZED_LEN 78
#define DATA_LEN		     256
#define MNEMO_LEN            24
#define ENTROPY_128         128/8
#define ENTROPY_160         160/8
#define ENTROPY_192         192/8
#define ENTROPY_224         224/8
#define ENTROPY_256         256/8

#define NODE "dev/spi_node"
#define IOC_MAGIC           'i'
#define IOC_SPI_READ        _IOR(IOC_MAGIC, 1, message_t)
#define IOC_SPI_WRITE       _IOW(IOC_MAGIC, 2, message_t)

typedef struct
{
	UINT8   is_ready;     //数据是否就绪， 用于AP模块读取它来判断SE是否已经处理完成
	UINT8	id;			  //参考命令ID
	UINT16	len;		  //para data len
}message_header;

typedef struct
{
	message_header	header;
	char 			para[DATA_LEN];	// pointer to the actual data in the buffer
}message_t;

typedef enum
{
	AT_M2S_GEN_WALLET = 0,  /*创建钱包*/
	AT_M2S_SET_PWD,			/*用户更改交易密码*/
	AT_M2S_SAVE_MNEMONIC,   /*保存助记词*/
	AT_M2S_RECOVER_WALLET,  /*恢复钱包*/
	AT_M2S_GET_PUBKEY,      /*获取账户公钥*/
	AT_M2S_SIGN_TRANX,		/*签名交易*/
	AT_M2S_DEL_WALLET,      /*删除钱包*/
	AT_M2S_END,

	AT_S2M_GEN_WALLET_RSP,
	AT_S2M_SET_PWD_RSP,
	AT_S2M_SAVE_MNEMONIC_RSP,
	AT_S2M_RECOVER_WALLET_RSP,
	AT_S2M_GET_PUBKEY_RSP,
	AT_S2M_SIGN_TRANX_RSP,
	AT_S2M_DEL_WALLET_RSP,
	AT_MAX,
}AT_CMD_ID;

static int fd = 0;
message_t message;

extern int open_dev();
extern int close_dev();
extern int ioc_create_mnemonic(char *entropy);
extern int ioc_set_passphrase(char *psssphrase);
extern int ioc_save_mnemonic(char *passphrase, char *language, UINT16 number, char *mnemonic);
extern int ioc_recovery_mnemonic(char *passphrase, char *language, UINT16 number, char *mnemonic);
extern int ioc_start_derive(char *passphrase, char *path, UINT16 deriveAlgoId, UINT16 signAlgoId, UINT16 number, char *pubkey);
extern int ioc_start_sign(char *passphrase, char *path, UINT16 deriveAlgoId, UINT16 signAlgoId, UINT16 number, char *transhash, char *pubkey, char *signhash);
extern int ioc_delete_wallet(char *passphrase);

#endif //WALLETJNIPLUGIN_IOCTL_WALLET_H
