// Constantes liees a l'algorithme

#define CRUNCH512_K 128 // number of subdivisions of input
// Constantes pour l'entrée et la sortie
#define CRUNCH512_BLOCKSIZE 1024 //in bits
#define CRUNCH512_HASHLEN 512

//NASTAVENIE RUND:
#define CRUNCH512_NBROUND CRUNCH512_HASHLEN  //nombre de tours pour chacune des schemas de Feistel non symetriques

extern CrunchHashReturn Crunch_Init_512();
extern CrunchHashReturn Crunch_Update_512(const CrunchBitSequence * data,CrunchDataLength databitlen, const int rounds);
extern CrunchHashReturn Crunch_Final_512(CrunchBitSequence *hash, const int rounds);
extern CrunchHashReturn Crunch_Hash_512(const CrunchBitSequence *data,CrunchDataLength databitlen,CrunchBitSequence *hashval, const int rounds);