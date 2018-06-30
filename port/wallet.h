#ifndef WALLET_H_
#define WALLET_H_

#include <stdint.h>
#include "typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

int create_wallet(uint16_t mne_number);
int import_wallet(uint16_t mne_number, char *mnemonic);

int change_pwd(void);

int get_root_pubkey(char *derive_path, 
	uint16_t deriveAlgoId, uint16_t signAlgoId, 
	uint16_t mne_number, 
	unsigned char *pubkey, int *pubkey_length);

int sign_transaction(char *derive_path,
    uint16_t deriveAlgoId, uint16_t signAlgoId, uint16_t mne_number,
    unsigned char *hash, unsigned char *pubkey, unsigned char *signature);

int remove_wallet(void);

#ifdef __cplusplus
}
#endif

#endif
