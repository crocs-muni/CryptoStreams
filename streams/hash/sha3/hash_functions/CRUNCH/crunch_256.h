// Constantes liees a l'algorithme

#define CRUNCH256_K 128 // number of subdivisions of input
// Constantes pour l'entree et la sortie
#define CRUNCH256_BLOCKSIZE 1024 //in bits
#define CRUNCH256_HASHLEN 256

//NASTAVENIE RUND:
#define CRUNCH256_NBROUND CRUNCH256_HASHLEN  //nombre de tours pour chacune des schemas de Feistel non symetriques

extern CrunchHashReturn Crunch_Init_256();
extern CrunchHashReturn Crunch_Update_256(const CrunchBitSequence * data,CrunchDataLength databitlen, const int rounds);
extern CrunchHashReturn Crunch_Final_256(CrunchBitSequence *hash, const int rounds);
extern CrunchHashReturn Crunch_Hash_256(const CrunchBitSequence *data,CrunchDataLength databitlen,CrunchBitSequence *hashval, const int rounds);