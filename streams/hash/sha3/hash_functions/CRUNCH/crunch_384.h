// Constantes liees a l'algorithme

#define CRUNCH384_K 128 // number of subdivisions of input
// Constantes pour l'entree et la sortie
#define CRUNCH384_BLOCKSIZE 1024 //in bits
#define CRUNCH384_HASHLEN 384

//NASTAVENIE RUND:
#define CRUNCH384_NBROUND CRUNCH384_HASHLEN  //nombre de tours pour chacune des schemas de Feistel non symetriques

extern CrunchHashReturn Crunch_Init_384();
extern CrunchHashReturn Crunch_Update_384(const CrunchBitSequence * data,CrunchDataLength databitlen, const int rounds);
extern CrunchHashReturn Crunch_Final_384(CrunchBitSequence *hash, const int rounds);
extern CrunchHashReturn Crunch_Hash_384(const CrunchBitSequence *data,CrunchDataLength databitlen,CrunchBitSequence *hashval, const int rounds);