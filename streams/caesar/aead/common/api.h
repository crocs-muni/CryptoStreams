#ifndef CAESAR_COMMON_H
#define CAESAR_COMMON_H

namespace CaesarCommon {
extern int numRounds;

int crypto_verify_16(const unsigned char *x,const unsigned char *y);
int crypto_verify_32(const unsigned char *x,const unsigned char *y);
int crypto_core_aes128encrypt(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
);
int crypto_core_aes128decrypt(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
);
int crypto_core_aes256encrypt(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
);

} // namespace CaesarCommon

#endif // CAESAR_COMMON_H
