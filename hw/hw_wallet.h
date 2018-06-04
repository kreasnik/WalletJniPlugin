#ifndef HW_WALLET_H
#define HW_WALLET_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <hardware/hardware.h>
#include <hardware/hwcomposer_defs.h>

#define WALLET_MODULE_API_VERSION_1_1 HARDWARE_MODULE_API_VERSION(1, 1)
#define WALLET_HARDWARE_MODULE_ID "wallet"

typedef struct wallet_module {
    struct hw_module_t common;
} wallet_module_t;

typedef struct wallet {
    struct hw_device_t common;

    /** 打开设备
     *  Returns: 1 success
     */
    int (*wallet_init)(struct wallet *dev);

    /** 关闭设备
     *  Returns: 1 success
     */
    int (*wallet_deinit)(struct wallet *dev);

    /** 设置密码
     *  Returns: 1 success
     *  In:      passphrase: 交易密码
     */
    int (*wallet_set_passphrase)(struct wallet *dev, char *passphrase);

    /** 创建
     *  Returns: 1 success
     *  In:      passphrase: 交易密码
     *           language: 助记词语言
     *           number: 助记词需要的个数
     */
    int (*wallet_create_mnemonic)(struct wallet *dev, char *passphrase, char *language, UINT16 number);

    /** 恢复
     *  Returns: 1 success
     *  In:      passphrase: 交易密码
     *           language: 助记词语言
     *           number: 助记词需要的个数
     *           mnemonic: 助记词
     */
    int (*wallet_recovery_mnemonic)(struct wallet *dev, char *passphrase, char *language, UINT16 number, char *mnemonic);

    /** 派生
     *  Returns: 1 success
     *  In:      passphrase: 交易密码
     *           path: 派生路径
     *           deriveAlgoId: 派生算法
     *           signAlgoId: 签名算法
     *           number: 助记词需要的个数
     *  Out:     pubkey: 获得派生的序列化公钥
     */
    int (*wallet_start_derive)(struct wallet *dev, char *passphrase, char *path, UINT16 deriveAlgoId, UINT16 signAlgoId, UINT16 number, char *pubkey);

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
    int (*wallet_start_sign)(struct wallet *dev, char *passphrase, char *path, UINT16 deriveAlgoId, UINT16 signAlgoId, UINT16 number, char *transhash, char *pubkey, char *signhash);

    /** 删除钱包
     *  Returns: 1 success
     *  In:      passphrase: 交易密码
     */
    int (*wallet_delete_wallet)(struct wallet *dev, char *passphrase);
    void* reserved[8 - 6];
} wallet_t;

#endif /* HW_WALLET_H */