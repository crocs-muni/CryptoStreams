#ifndef SPECTRALHASH_SHA3_H
#define SPECTRALHASH_SHA3_H

#include "../../sha3_interface.h"

#include "spectral_structs.h"

class SpectralHash : public sha3_interface {

typedef enum {SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2} HashReturn;

typedef struct hashState {
    int hashbitlen;
    spectral_s_prism sPrism;
    spectral_p_prism pPrism;
    spectral_h_prism hPrism;    
    unsigned char remainder[64];
    int remainderbitlen;
    unsigned long long mesagelen;
    int started;
} hashState;

private:
hashState spectralState;

public:
int Init(int hbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
unsigned char get_last_word(unsigned char curChar);
unsigned char get_first_word(unsigned char curChar);
int fill_s_prism(spectral_s_prism *prism, spectral_chunk *spectral_chunk);
void init_h_prism(spectral_h_prism *prism);	
void init_p_prism(spectral_p_prism *prism);
void add_length(spectral_chunk *curChunk, long int length);
int s_set_ij_column(spectral_s_prism *prism, int i, int j, spectral_s_column column);
int s_set_ik_row(spectral_s_prism *prism, int i, int k, spectral_s_row row);
int s_set_jk_row(spectral_s_prism *prism, int j, int k, spectral_s_row row);
spectral_s_column s_get_ij_column(spectral_s_prism *prism, int i, int j);
spectral_s_row s_get_ik_row(spectral_s_prism *prism, int i, int k);
spectral_s_row s_get_jk_row(spectral_s_prism *prism, int j, int k);
spectral_s_row four_point_DFT(spectral_s_row row);
spectral_s_column eight_point_DFT(spectral_s_column column);
char get_word_sl(unsigned char word);
char get_word_sh(unsigned char word);
int initial_swap_control(spectral_s_prism *sPrism, spectral_p_prism *pPrism);
void affine_transform(spectral_s_prism *prism);
int k1_swap(spectral_s_prism *sPrism, spectral_p_prism *pPrism);
int k2_swap(spectral_s_prism *sPrism, spectral_p_prism *pPrism);
int j_swap(spectral_s_prism *sPrism, spectral_p_prism *pPrism);
int i_swap(spectral_s_prism *sPrism, spectral_p_prism *pPrism);
void NLST(spectral_s_prism *sPrism, spectral_p_prism *pPrism, spectral_h_prism *hPrism);
void rubic_rot(spectral_p_prism *prism);
int compress(spectral_s_prism *sPrism, spectral_p_prism *pPrism, spectral_h_prism *hPrism);
void mark_bits(spectral_p_prism *pPrism, spectral_s_prism *markingPrism, int digestSize);
spectral_digest* extract_bits(spectral_h_prism *hPrism, spectral_s_prism *markingPrism, spectral_digest *hashDigest);
void make_digest(spectral_digest *hashDigest, spectral_p_prism *pPrism, spectral_h_prism *hPrism);

};

#endif