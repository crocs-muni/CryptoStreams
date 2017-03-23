// Constantes liees a l'algorithme

#define CRUNCH224_K 128 // number of subdivisions of input
// Constantes pour l'entree et la sortie
#define CRUNCH224_BLOCKSIZE 1024 //in bits
#define CRUNCH224_HASHLEN 224

//NASTAVENIE RUND:
#define CRUNCH224_NBROUND CRUNCH224_HASHLEN  //nombre de tours pour chacune des schemas de Feistel non symetriques

extern CrunchHashReturn Crunch_Init_224();
extern CrunchHashReturn Crunch_Update_224(const CrunchBitSequence * data,CrunchDataLength databitlen, const int rounds);
extern CrunchHashReturn Crunch_Final_224(CrunchBitSequence *hash, const int rounds);
extern CrunchHashReturn Crunch_Hash_224(const CrunchBitSequence *data,CrunchDataLength databitlen,CrunchBitSequence *hashval, const int rounds);
