#ifndef SPECTRAL_STRUCTS
#define SPECTRAL_STRUCTS

typedef struct spectral_s_prism {
    unsigned char cell[4][4][8];
} spectral_s_prism;

typedef struct spectral_h_prism {
    unsigned char cell[4][4][8];
} spectral_h_prism;

typedef struct spectral_p_prism {
    unsigned char cell[4][4][8];
} spectral_p_prism;

typedef struct spectral_chunk {
    unsigned char word[128];
} spectral_chunk;

typedef struct spectral_s_column {
    unsigned char cell[8];
} spectral_s_column;

typedef struct spectral_s_row {
    unsigned char cell[4];
} spectral_s_row;

typedef struct spectral_p_column {
    unsigned char cell[8];
} spectral_p_column;

typedef struct spectral_p_row {
    unsigned char cell[4];
} spectral_p_row;

typedef struct spectral_digest {
    unsigned char byte[64];
    int length;
} spectral_digest;

#endif