#ifndef WALLET_H_
#define WALLET_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int create_wallet(void);

int import_wallet(void);

int change_pwd(void);

int get_root_pubkey(char *derive_path,
    uint16_t deriveAlgoId, uint16_t signAlgoId,
    uint16_t mne_number,
    unsigned char *pubkey, uint32_t *pubkey_length);

int sign_transaction(char *derive_path,
    uint16_t deriveAlgoId, uint16_t signAlgoId, uint16_t mne_number,
    char *hash, uint16_t hash_len,
    unsigned char *pubkey, uint32_t *pubkey_length,
    unsigned char *signature, uint32_t *signature_length);

int remove_wallet(void);

int return_to_boot(void);

#ifdef __cplusplus
}
#endif

#endif