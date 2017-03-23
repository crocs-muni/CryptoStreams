#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
#include "khichidi_core.h"
}
#include "Khichidi_sha3.h"

int Khichidi::Init(int hashbitlen)
{
	int ret = KHICHIDI_FAIL;
	int bytelen = hashbitlen/8;
	int i=0;

	if(&khichidiState != NULL)
	{
		switch (hashbitlen)
		{
			case KHICHIDI_224:
			case KHICHIDI_256:
			case KHICHIDI_384:
			case KHICHIDI_512:
				
				khichidiState.bitlength = hashbitlen;
				
				for(i=0;i<bytelen;i++)
				{
					khichidiState.M[i]=0;
					khichidiState.Mdash[i]=0;
					khichidiState.residue[i]=0;
				}
				khichidiState.residuelen = 0;
				for(i=0;i<9;i++)
					khichidiState.count[i]=0;
#ifdef EIGHT_BIT
				for(i=0;i<bytelen;i++)
#else
				for(i=0;i<bytelen/KHICHIDI_WORDLENGTH;i++)
#endif
				{
					khichidiState.Z[i] = 0;
					khichidiState.H[i] = 0;
				}
				khichidiState.H[i] = 0;

				ret = KHICHIDI_SUCCESS;
				break;
			default: ret = KHICHIDI_BAD_HASHBITLEN;
		}
		
	}
	return ret;

}


#ifdef EIGHT_BIT
void Khichidi::digest()
{
	unsigned short j = 0,k=0,i=0;
	unsigned short numword = (khichidiState.bitlength/8)/KHICHIDI_WORDLENGTH;
	BitSequence param[KHICHIDI_WORDLENGTH];
	for(j=0,k=0;j<numword;j++,k+=KHICHIDI_WORDLENGTH)
	{
		param[0] = khichidiState.H[k+0] ^ khichidiState.M[k];
		param[1] = khichidiState.H[k+1] ^ khichidiState.M[k+1];
		param[2] = khichidiState.H[k+2] ^ khichidiState.M[k+2];
		param[3] = khichidiState.H[k+3] ^ khichidiState.M[k+3];

		message_pre_process(param,khichidiState.Z,k);

		for(i=0;i<KHICHIDI_WORDLENGTH;i++)
			khichidiState.H[k+i+KHICHIDI_WORDLENGTH] = khichidiState.Z[k+i];
	}
	for(i=0;i<KHICHIDI_WORDLENGTH;i++)
		khichidiState.H[0+i] = khichidiState.H[k+i];
}
#else
void Khichidi::digest()
{
	int j = 0,k=0;
	int numword = (khichidiState.bitlength/8)/KHICHIDI_WORDLENGTH;
	KHICHIDI_WORD Mj =0;

	for(j=0, k=0; j<numword; j++, k+=KHICHIDI_WORDLENGTH)
	{
		// Get the bitsequence to message word
		Mj = khichidiState.M[k];
		Mj = Mj<<8 | khichidiState.M[k+1];
		Mj = Mj<<8 | khichidiState.M[k+2];
		Mj = Mj<<8 | khichidiState.M[k+3];
		
		// message_pre_process will do shuffling, t-function and lfsr
		khichidiState.Z[j] = message_pre_process(Mj ^ khichidiState.H[j]);
		khichidiState.H[j+1] = khichidiState.Z[j];

	}
	khichidiState.H[0] = khichidiState.H[j];
}
#endif


int Khichidi::Final(BitSequence *hashval)
{
	int ret = KHICHIDI_FAIL;
	int i = 0, k=0,j=0;
	int outlen = 0;
	int algsize = khichidiState.bitlength/8;
	BitSequence aa[130]="";
	KHICHIDI_WORD flag=0;

	if(hashval != 0 && &khichidiState != 0)
	{
		// Initialize the M and Mdash values
		for(i=0;i<algsize;i++)
		{
			khichidiState.M[i]=0;
			khichidiState.Mdash[i]=0;
		}

		// If there is anything left in the residue, digest that also
		// Or even if no hash has been computed till now(means we are
		// creating hash for empty) then just do padding and compute hash
#ifdef EIGHT_BIT
		if((khichidiState.residuelen > 0) || ((khichidiState.Z[0] == 0) && (khichidiState.Z[1] == 0) && (khichidiState.Z[2] == 0) && (khichidiState.Z[3] == 0)) )
#else
		if((khichidiState.residuelen > 0) || (khichidiState.Z[0] == 0))
#endif
		{
			dopad(khichidiState.residue, khichidiState.residuelen, khichidiState.M, algsize);
			khichidiState.residuelen = 0;
			Khichidi::digest();
		}
		for(i=0;i<algsize-8;i++)
		{
			khichidiState.M[i] = 0;
		}
		for(j=1;j<=8;j++,i++)
			khichidiState.M[i] = khichidiState.count[j];

		Khichidi::digest();
			
#ifdef DEBUG
#ifdef EIGHT_BIT
		printf("Round 1 = ");
		for(i=0;i<algsize;i++)
		{
			printf("%02X",khichidiState.Z[i]);
		}
		printf("\n");
#else
		Khichidi::wordstobytes(khichidiState.Z,algsize/KHICHIDI_WORDLENGTH,khichidiState.M, &outlen);
		Khichidi::hexify(khichidiState.M,algsize, aa);
		Khichidi::hexify(khichidiState.M,algsize, khichidiState.Round[0]);
		printf("Round 1 = %s \n",aa);
#endif
#endif
		// By this time the first outer round is over once. Now we have to do the same for
		// 5 more times. Now M will be constructed from Z's of prior rounds
	

		for(i=0;i<5;i++)
		{
			
#ifdef EIGHT_BIT
			for(j=0;j<algsize;j++)
			{
				khichidiState.M[j]=khichidiState.Z[j];
				khichidiState.Mdash[j]=khichidiState.Z[j];
			}
			Khichidi::digest();
			for(j=0;j<algsize;j++)
			{
				khichidiState.M[j]=khichidiState.Mdash[j];
			}
				//process mdash
			Khichidi::digest();
#else
			Khichidi::wordstobytes(khichidiState.Z, algsize/KHICHIDI_WORDLENGTH, khichidiState.M, &outlen);
			Khichidi::wordstobytes(khichidiState.Z, algsize/KHICHIDI_WORDLENGTH, khichidiState.Mdash, &outlen);

			Khichidi::digest();
			for(j=0;j<algsize;j++)
			{
				khichidiState.M[j]=khichidiState.Mdash[j];
			}
			//process mdash
			Khichidi::digest();
#endif
#ifdef DEBUG
#ifdef EIGHT_BIT
		printf("Round %d = ",i+2);
		for(k=0;k<algsize;k++)
		{
			printf("%02X",khichidiState.Z[k]);
		}
		printf("\n");
#else
		Khichidi::wordstobytes(khichidiState.Z,algsize/KHICHIDI_WORDLENGTH,khichidiState.M, &outlen);
		Khichidi::hexify(khichidiState.M,algsize, aa);
		Khichidi::hexify(khichidiState.M,algsize, khichidiState.Round[i+1]);
		printf("Round %d = %s \n",i+2,aa);
#endif
#endif
		}

#ifdef EIGHT_BIT
		for(k=0;k<algsize;k++)
			hashval[k] = khichidiState.Z[k];
#else
		Khichidi::wordstobytes(khichidiState.Z, algsize/KHICHIDI_WORDLENGTH, hashval, &outlen);
#endif
		ret = KHICHIDI_SUCCESS;
	}

	return ret;
}


int Khichidi::Update(const BitSequence *data, DataLength databitlen)
{
	int ret = KHICHIDI_FAIL;
	int i = 0;
	int numblocks = 0;
	int remlen = 0;
	int totallen = 0;
	int algsize = khichidiState.bitlength/8;
	const BitSequence *buff = data;
	BitSequence temp_length[8],temp_count[8];

	if(&khichidiState !=0 && databitlen >= 0 && data !=0)
	{
		// copy the count for addition
		for(i=1;i<9;i++)
		{
			temp_count[i-1] = khichidiState.count[i];
		}

		for(i=0;i<8;i++)
			temp_length[i]=0;
		// initialize the array and convert KHICHIDI_WORD to bytes
		Khichidi::convertLength((KHICHIDI_WORD*)&databitlen,temp_length);

		// add and store the databit length
		uadd(khichidiState.count,temp_count,temp_length,8);

		// take the lenght in bytes
		totallen = (khichidiState.residuelen+databitlen)/8;
		
		// If the current available data is not enough to start process,
		// then store it.
		if(totallen < algsize)
		{

			ret = Khichidi::bitcpy(khichidiState.residue, khichidiState.residuelen, data, databitlen);
			if(ret == KHICHIDI_SUCCESS)
				khichidiState.residuelen = khichidiState.residuelen+databitlen;
			 
		}
		else
		{
			// Gent the number of 'bitlength' message blocks
			numblocks = totallen / algsize;
			// Also get what will be the residue size (in bits)
			remlen = (khichidiState.residuelen+databitlen) % khichidiState.bitlength;
			
			for(i=0; i<numblocks; i++)
			{
				if(khichidiState.residuelen > 0)
				{
					Khichidi::bitcpy(khichidiState.M, 0, khichidiState.residue, khichidiState.residuelen);
					Khichidi::bitcpy(khichidiState.M, khichidiState.residuelen, buff, khichidiState.bitlength - khichidiState.residuelen);
					buff = buff + ((khichidiState.bitlength - khichidiState.residuelen)/8);
					khichidiState.residuelen = 0;
					
				}
				else
				{
					Khichidi::bitcpy(khichidiState.M,0, buff, khichidiState.bitlength);
					buff = buff + algsize;
				}

				// process M and get value
				Khichidi::digest();
			}


			// Copy the unprocessed data to residue
			if(remlen>0)
			{
				Khichidi::bitcpy(khichidiState.residue,0, buff, remlen);
				khichidiState.residuelen = remlen;
			}

			ret = KHICHIDI_SUCCESS;
		}
	}

	return ret;
}


#ifndef EIGHT_BIT
void Khichidi::wordstobytes(KHICHIDI_WORD *in, int inlen, BitSequence *khichidi_out, int *outlen)
{
	int i = 0;
	int k = 0;
	for(i=0;i<inlen;i++)
	{
		khichidi_out[k++] = (BitSequence)((in[i] & 0xFF000000)>>24);
		khichidi_out[k++] = (BitSequence)((in[i] & 0x00FF0000)>>16);
		khichidi_out[k++] = (BitSequence)((in[i] & 0x0000FF00)>>8);
		khichidi_out[k++] = (BitSequence)(in[i] & 0x000000FF);

	}
	*outlen = k;
}
#endif

void Khichidi::convertLength(KHICHIDI_WORD *in, BitSequence *khichidi_out)
{
	int i = 0;
	int k = 0;

#ifdef SIXTYFOUR_BIT
	khichidi_out[k++] = (BitSequence)((in[i] & 0xFF00000000000000)>>56);
	khichidi_out[k++] = (BitSequence)((in[i] & 0x00FF000000000000)>>48);
	khichidi_out[k++] = (BitSequence)((in[i] & 0x0000FF0000000000)>>40);
	khichidi_out[k++] = (BitSequence)((in[i] & 0x000000FF00000000)>>32);
#else
		// fill with zeros
	khichidi_out[k++] = 0;
	khichidi_out[k++] = 0;
	khichidi_out[k++] = 0;
	khichidi_out[k++] = 0;
#endif
	khichidi_out[k++] = (BitSequence)((in[i] & 0xFF000000)>>24);
	khichidi_out[k++] = (BitSequence)((in[i] & 0x00FF0000)>>16);
	khichidi_out[k++] = (BitSequence)((in[i] & 0x0000FF00)>>8);
	khichidi_out[k++] = (BitSequence)(in[i] & 0x000000FF);
}

void Khichidi::hexify(BitSequence *in, int inlen, BitSequence *khichidi_out)
{
	int i=0;
	BitSequence *buff = khichidi_out;
	for(i=0;i<inlen;i++)
	{
		sprintf((char*)buff, "%02X",in[i]);
		buff = buff+2;
	}
	*buff = '\0';
}

int Khichidi::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
	//hashState state;
	int ret = KHICHIDI_FAIL;

	if((data != 0) && (hashval !=0))
	{
		ret = Khichidi::Init(hashbitlen);
		if(ret == KHICHIDI_SUCCESS)
		{
			ret = Khichidi::Update(data, databitlen);
		}
		if(ret == KHICHIDI_SUCCESS)
		{
			ret = Khichidi::Final(hashval);
		}
		
	}
	return ret;

}


int Khichidi::bitcpy(BitSequence *dest, int destoffset, const BitSequence *source, int numbits)
{
	BitSequence MASK[8] = {0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
	int kdest = destoffset/8;
	int ldest = destoffset%8;
	int retval = KHICHIDI_FAIL;
	int balbits = numbits%8;

	if((ldest == 0))
	{
		memcpy(dest+kdest,source, numbits/8);
		if(balbits != 0)
		{
			dest[kdest+numbits/8] = source[numbits/8] & MASK[balbits-1];
		}
		retval = KHICHIDI_SUCCESS;

	}
	
	return retval;
}