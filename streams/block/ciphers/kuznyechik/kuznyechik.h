/**
 * Source : https://github.com/dzruyk/kuznyechik_cipherx
 */

#ifndef __KUZN_H__
#define __KUZN_H__

#define _KUZ_DEBUG

#define KUZN_BLOCK_LEN	16

#ifdef __cplusplus
extern "C" {
#endif

struct kuzn_ctx {
	uint8_t polynomial;
	uint8_t keys[10 * 16];
};

void kuzn_context_init(struct kuzn_ctx *ctx);

void kuzn_set_key(struct kuzn_ctx *ctx, const uint8_t *key);

void kuzn_encrypt(struct kuzn_ctx *ctx, const uint8_t *in, uint8_t *out, unsigned int rounds);
void kuzn_decrypt(struct kuzn_ctx *ctx, const uint8_t *in, uint8_t *out, unsigned int rounds);

#ifdef _KUZ_DEBUG
int test_kuzn();
#endif

#ifdef __cplusplus
}
#endif

#endif
