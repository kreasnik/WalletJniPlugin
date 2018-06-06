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
#include "typedef.h"

#define UINT_LEN             4
#define PRIV_KEY_BIT         32
#define BIP32_SERIALIZED_LEN 78
#define DATA_LEN		     256
#define MNEMO_LEN            24
#define ENTROPY_128         (128/8) //12个助记词
#define ENTROPY_160         (160/8) //15个助记词
#define ENTROPY_192         (192/8) //18个助记词
#define ENTROPY_224         (224/8) //21个助记词
#define ENTROPY_256         (256/8) //24个助记词

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

/** 打开设备
 *  Returns: 1 success
 */
extern int open_dev();

/** 关闭设备
 *  Returns: 1 success
 */
extern int close_dev();

/** 获得熵
 *  Returns: 1 success
 *  Out:     entropy: 256位随机数
 */
extern int ioc_create_mnemonic(char *entropy);

/** 设置密码
 *  Returns: 1 success
 *  In:      passphrase: 交易密码
 */
extern int ioc_set_passphrase(char *psssphrase);

/** 保存助记词
 *  Returns: 1 success
 *  In:      passphrase: 交易密码
 *           language: 助记词语言
 *           number: 助记词需要的个数
 *           mnemonic: 需要保存的助记词
 */
extern int ioc_save_mnemonic(char *passphrase, char *language, UINT16 number, char *mnemonic);

/** 恢复
 *  Returns: 1 success
 *  In:      passphrase: 交易密码
 *           language: 助记词语言
 *           number: 助记词需要的个数
 *           mnemonic: 助记词
 */
extern int ioc_recovery_mnemonic(char *passphrase, char *language, UINT16 number, char *mnemonic);

/** 派生
 *  Returns: 1 success
 *  In:      passphrase: 交易密码
 *           path: 派生路径
 *           deriveAlgoId: 派生算法
 *           signAlgoId: 签名算法
 *           number: 助记词需要的个数
 *  Out:     pubkey: 获得派生的序列化公钥
 */
extern int ioc_start_derive(char *passphrase, char *path,
                        UINT16 deriveAlgoId, UINT16 signAlgoId,
                        UINT16 number, char *pubkey);

/** 签名
 *  Returns: 1 success
 *  In:      passphrase: 交易密码
 *           path: 派生路径
 *           deriveAlgoId: 派生算法
 *           signAlgoId: 签名算法
 *           number: 助记词需要的个数
 *           transhash: 需要签名的摘要
 *  Out:     pubkey: 获得派生的序列化公钥
 *           signhash: 签名结果
 */
extern int ioc_start_sign(char *passphrase, char *path,
                        UINT16 deriveAlgoId, UINT16 signAlgoId,
                        UINT16 number, char *transhash,
                        char *pubkey, char *signhash);

/** 删除钱包
 *  Returns: 1 success
 *  In:      passphrase: 交易密码
 */
extern int ioc_delete_wallet(char *passphrase);

#endif //WALLETJNIPLUGIN_IOCTL_WALLET_H
