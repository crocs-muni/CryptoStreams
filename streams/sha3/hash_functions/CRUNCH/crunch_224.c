#include <stdio.h>
#include <stdlib.h>

#include "crunch_type.h"
#include "crunch_224.h"

#ifdef M64
#define LONGSIZE 64
#else
#define LONGSIZE 32
#endif

#define CRUNCH224_NE (CRUNCH224_BLOCKSIZE/LONGSIZE)	// Number of long in input
#define CRUNCH224_NS (CRUNCH224_HASHLEN/LONGSIZE)	// Number of long in outpout
#define CRUNCH224_NPM ((LONGSIZE*CRUNCH224_K)/CRUNCH224_BLOCKSIZE)	//Longs (32 or 64 bits) are divided in CRUNCH224_NPM parts

#define CRUNCH224_NA 256			// = 2^CRUNCH224_NBE number of elements in start ensemble of random function
#define CRUNCH224_NBE (CRUNCH224_BLOCKSIZE/CRUNCH224_K)		//  number of bits in elements in start ensemble of random function

//macro used to send final results in same order in 32 and 64  bits version (due to integers representation on X86)
#define crunch224_idx(a)  ( (LONGSIZE/8) * ((a)/(LONGSIZE/8)) + ((LONGSIZE/8)-1) - (a)%(LONGSIZE/8))

//#define CRUNCH224_SHIFT	((CRUNCH224_NBROUND%128)*8)/LONGSIZE

#define CRUNCH224_MAXDATAPERUPDATE 1024ULL*1024ULL*1024ULL

extern unsigned long int crunch_nbalea[];

//Left data after a call of Update function
unsigned long int Crunch224_LeftData[(CRUNCH224_BLOCKSIZE-CRUNCH224_HASHLEN)/LONGSIZE];
unsigned int Crunch224_SizeLeft; //size of left data in bytes
unsigned long int Crunch224_PrevHash[CRUNCH224_HASHLEN/LONGSIZE];
CrunchDataLength Crunch224TotalInputSizeInBit; //Total size of message in bits


unsigned long int *padding_224(int *nb_final_block)
{
	int new_nb_bit,i,no_block;
	unsigned long int *mes;
	char *mes_char,*DL_char;
	int temp;
	new_nb_bit=Crunch224_SizeLeft*8+1+2*LONGSIZE;
	i=new_nb_bit % (CRUNCH224_BLOCKSIZE-CRUNCH224_HASHLEN);
	new_nb_bit+=(CRUNCH224_BLOCKSIZE-CRUNCH224_HASHLEN)-i;
	*nb_final_block=new_nb_bit/LONGSIZE;

	mes=(unsigned long  int *)calloc((*nb_final_block),sizeof(unsigned long int));
	mes_char=(char*)mes;
	DL_char=(char*)Crunch224_LeftData;
	/*copy left data in beginning of the block*/
	for(i=0;i<Crunch224_SizeLeft;i++)
	{
 		temp =crunch224_idx(i);
		mes_char[temp]=DL_char[temp];
	}
	no_block=((Crunch224_SizeLeft)*8+1)/LONGSIZE;
	mes[no_block]|=1UL<<( (LONGSIZE-1)-( ((Crunch224_SizeLeft)*8 )%LONGSIZE));
	/*complete block with 0 and stock message size in the last 64bits*/
	#if LONGSIZE==32
	for(i=no_block+1;i<*nb_final_block-2;i++)
	{
		mes[i]=0;
	}
	mes[*nb_final_block-2]=(unsigned long int)(Crunch224TotalInputSizeInBit>>32);
	mes[*nb_final_block-1]=(unsigned long int)(Crunch224TotalInputSizeInBit);
	#endif
	#if LONGSIZE==64
	for(i=no_block+1;i<*nb_final_block-1;i++)
	{
		mes[i]=0;
	}
	mes[*nb_final_block-1]=Crunch224TotalInputSizeInBit; 
	#endif
	return(mes);
}

void Merkle_224(unsigned long int H[],unsigned long int mes[],int nb_merkle, const int rounds) {
	int n,k;
	int j_round,j,a,aa,aa2,p,block;
    	int alpha;
	unsigned long int message[CRUNCH224_NE],res[CRUNCH224_NE];
	unsigned long i,i2;
	for(k=0;k<(CRUNCH224_NS);k++)
	{
		message[k]=H[k];
	}

	for(n=0;n<nb_merkle;n++)
	{
		for(k=0;k<CRUNCH224_NS;k++)
		{
			res[k]=message[k];
		}
		for(k=(CRUNCH224_NS) ;k<CRUNCH224_NE;k++)
		{
			message[k]=mes[k-(CRUNCH224_NS)];
			res[k]=message[k];
		}
		mes=mes+(CRUNCH224_NE-CRUNCH224_NS);


		a=CRUNCH224_NE-(128/LONGSIZE);
 	   	p=0;block=0;
	   	alpha=1;
 	   	for(j_round=0;j_round<rounds;j_round++)
 	   	{
			i2=(res[block]>>((CRUNCH224_NPM-1-p)*CRUNCH224_NBE))&(CRUNCH224_NA-1);
			i2=(i2*alpha)&0xff;
			i=(message[block]>>((CRUNCH224_NPM-1-p)*CRUNCH224_NBE))&(CRUNCH224_NA-1);
			i=(i*alpha)&0xff;
			aa2=a+CRUNCH224_NE*CRUNCH224_NA*16+i2*CRUNCH224_NE;
			aa=a+i*CRUNCH224_NE;
        		for(j=0;j<CRUNCH224_NE;j++)
			{
	   			message[j]^=crunch_nbalea[aa];
				aa++;
            			res[j]^=crunch_nbalea[aa2];
	    			aa2++;
        		}
        		if ((j_round & 15) == 15)
			{
				alpha=alpha+2;
				if ( (j_round&127) == 127)
				{
					a=CRUNCH224_NE-(128/LONGSIZE);
				}
				else
				{
					a-=15*CRUNCH224_NA*CRUNCH224_NE+128/LONGSIZE;
				}
			}
			else
			{
				a+=CRUNCH224_NA*CRUNCH224_NE;
			}
        		p++;
        		if (p==CRUNCH224_NPM)
			{
        	    		block++;
		    		if (block==CRUNCH224_NE) block=0;
        	    		p=0;
        		}
		}
		if ((((rounds%128)*8)/LONGSIZE) == 0) {
			for(j=0;j<CRUNCH224_NS;j++) message[j]^=res[j];
		} else {
			for(j=(((rounds%128)*8)/LONGSIZE);j<(((rounds%128)*8)/LONGSIZE)+CRUNCH224_NS;j++) res[j%CRUNCH224_NE]^=message[j%CRUNCH224_NE];
			for(j=0;j<CRUNCH224_NS;j++) message[j]=res[(j+(((rounds%128)*8)/LONGSIZE))%CRUNCH224_NE];
		}
	}
	for(i=0;i<CRUNCH224_NS;i++) H[i]=message[i];
}

//concatenate the left data of the previous call of update with the new data 
unsigned long int *concatenation_update_224(const CrunchBitSequence *data,int databitlen,int *nb_block)
{
	unsigned long int *mes;
	int nb_bits_restants;
	int nb_block_in_new_mes;
	int nb_bits_in_new_mes;
	int i;
	int bits_traite_dans_data;
	int no_block,pos_block;
	int temp;
	char *mes_char,*DL_char;
	nb_bits_in_new_mes=( (databitlen+8*Crunch224_SizeLeft)/(CRUNCH224_BLOCKSIZE-CRUNCH224_HASHLEN) )*(CRUNCH224_BLOCKSIZE-CRUNCH224_HASHLEN);
	nb_bits_restants=(databitlen+8*Crunch224_SizeLeft)%(CRUNCH224_BLOCKSIZE-CRUNCH224_HASHLEN);
	nb_block_in_new_mes=nb_bits_in_new_mes/(LONGSIZE);
	if(nb_block_in_new_mes>0) 
	{
		/*there is enough of new data to complete a block*/
		mes = (unsigned long int *)calloc( nb_block_in_new_mes,(LONGSIZE/8) );
		if(mes==NULL) return NULL;
		//copy left data to the beginning of the new "message"
		mes_char=(char*)mes;
		DL_char=(char*)Crunch224_LeftData;
		for(i=0;i<Crunch224_SizeLeft;i++)
		{
			temp =(i/(LONGSIZE/8))*(LONGSIZE/8) + ( (LONGSIZE/8-1)-(i%(LONGSIZE/8))) ;
			mes_char[temp]=DL_char[temp];
		}
		/*complete message with new datas*/
		no_block=(i*8)/(LONGSIZE);
		pos_block=(LONGSIZE-8)-( ((i)*8 )%LONGSIZE);
		for(i=0;8*i<nb_bits_in_new_mes-8*Crunch224_SizeLeft;i++) 
		{
			mes[no_block]|=(unsigned long int)data[i] << pos_block;
			pos_block-=8;
			if (pos_block<0) 
			{
				pos_block=LONGSIZE-8;
				no_block++;
			}
		}
		bits_traite_dans_data=i;
		no_block=0;pos_block=LONGSIZE-8;
		Crunch224_SizeLeft=(nb_bits_restants)/8;
		Crunch224_LeftData[0]=0;
	}
	else
	{
		//no new block with new data
		i=0;
		no_block=((Crunch224_SizeLeft)*8+1)/LONGSIZE;
		pos_block= (LONGSIZE-8)-( ((Crunch224_SizeLeft)*8 )%LONGSIZE);
		Crunch224_SizeLeft=Crunch224_SizeLeft+databitlen/8;
	}
	/*save left data of new data for the next call of update/final*/
	for( ;8*i<databitlen;i++) 
	{
		Crunch224_LeftData[no_block]|=(unsigned long int)data[i] << pos_block;
		pos_block-=8;
		if (pos_block<0) 
		{
			pos_block=LONGSIZE-8;
			no_block++;
			Crunch224_LeftData[no_block]=0UL;
		}
	}
	*nb_block=nb_block_in_new_mes;
	return mes;	
}

CrunchHashReturn Crunch_Update_224(const CrunchBitSequence * data,CrunchDataLength databitlen, const int rounds)
{
	unsigned long int *mes;int nb_final_block[1];
	int i;
	if(databitlen>=CRUNCH224_MAXDATAPERUPDATE)
	{
		printf("too many data for an update\n");
		return CRUNCH_FAIL;
	}
	mes= concatenation_update_224(data,databitlen,nb_final_block);
	if(*nb_final_block>0)
	{
		if(mes == NULL)
		{
			printf("fail: can't allocate enough memory\n");
			return CRUNCH_FAIL;
		}
	}
	if(*nb_final_block>0)
	{
		Merkle_224(Crunch224_PrevHash,mes,*nb_final_block/(CRUNCH224_NE-CRUNCH224_NS), rounds);
		free(mes);
	}
	Crunch224TotalInputSizeInBit+=databitlen;
	return CRUNCH_SUCCESS;
}



CrunchHashReturn Crunch_Init_224()
{
	int k;
	for(k=0;k<(CRUNCH224_HASHLEN/LONGSIZE);k++) 
	{
		Crunch224_PrevHash[k]=crunch_nbalea[k];
	}
	for(k=0;k<(CRUNCH224_BLOCKSIZE-CRUNCH224_HASHLEN)/LONGSIZE;k++)
	{
		Crunch224_LeftData[k]=0;
	}
	Crunch224_SizeLeft=0;
	Crunch224TotalInputSizeInBit=0;
	return CRUNCH_SUCCESS;
}


CrunchHashReturn Crunch_Final_224(CrunchBitSequence *hash, const int rounds)
{
	unsigned long int *mes;
	int i;
	int nb_final_block[1];
	char *tocopy;
	mes=padding_224(nb_final_block);
	tocopy=(char*)mes;
	Merkle_224(Crunch224_PrevHash,mes,*nb_final_block/(CRUNCH224_NE-CRUNCH224_NS), rounds);
	
	tocopy=(char*)Crunch224_PrevHash;
	for(i=0;i<CRUNCH224_HASHLEN/8;i++)
	{
		hash[i]=tocopy[crunch224_idx(i)];
	}

	free(mes);
	return CRUNCH_SUCCESS;
}

CrunchHashReturn Crunch_Hash_224(const CrunchBitSequence *data,CrunchDataLength databitlen,CrunchBitSequence *hashval, const int rounds) 
{
	crunchHashState state;
	CrunchHashReturn HR;
	int i;
	state.hashbitlen=224;
	HR=Crunch_Init_224();
	if(HR!=CRUNCH_SUCCESS) return HR;
	i=0;
	while(databitlen>CRUNCH224_MAXDATAPERUPDATE)
	{
		HR=Crunch_Update_224(&(data[i*CRUNCH224_MAXDATAPERUPDATE/8]),CRUNCH224_MAXDATAPERUPDATE, rounds);
		if(HR!=CRUNCH_SUCCESS) return HR;
		databitlen-=(CRUNCH224_MAXDATAPERUPDATE);
		i++;
	}
	HR=Crunch_Update_224(&(data[i*CRUNCH224_MAXDATAPERUPDATE/8]),databitlen, rounds);
	HR=Crunch_Final_224(hashval, rounds);
	return HR;
}



 
