#include <string.h>
#include "Mcssha_sha3.h"

//# Logarithm Substitution for ro = 1, teta = 3, r = 0

static unsigned char MSCSHA_S[256]={
	0x30, 0x60, 0x67, 0xB5, 0x43, 0xEA, 0x93, 0x25,
	0x48, 0x0D, 0x18, 0x6F, 0x28, 0x7A, 0xFE, 0xB6,
	0xD5, 0x9C, 0x23, 0x86, 0x52, 0x42, 0xF7, 0xFD,
	0xF6, 0x9B, 0xEE, 0x99, 0x91, 0xBC, 0x2A, 0x63,
	0xA1, 0xA0, 0x57, 0x3C, 0x39, 0xD2, 0xEC, 0x71,
	0x45, 0xCB, 0x41, 0xDC, 0x0B, 0x5B, 0xC2, 0x36,
	0x01, 0x55, 0x7D, 0xFB, 0xED, 0x83, 0x8F, 0x31,
	0xC0, 0x4C, 0x08, 0xE3, 0x9D, 0xC1, 0xD3, 0xE9,
	0xB8, 0xBD, 0xAE, 0x0F, 0xE7, 0x70, 0x5A, 0xEB,
	0x4D, 0x29, 0xF9, 0xA9, 0x3D, 0x26, 0x46, 0x06,
	0xD0, 0x50, 0xA5, 0xBE, 0x66, 0x90, 0xF4, 0x20,
	0xE4, 0x33, 0x27, 0xE2, 0xAB, 0xEF, 0x68, 0x54,
	0x37, 0x6A, 0xDB, 0xBB, 0xD8, 0x7B, 0x69, 0xC4,
	0xF2, 0xBF, 0x85, 0xC7, 0xA6, 0xB4, 0x9A, 0xDD,
	0x72, 0x34, 0xE8, 0xFC, 0xD6, 0x21, 0x98, 0x96,
	0x32, 0xCA, 0x49, 0xB3, 0xF3, 0x97, 0x8E, 0x2F,
	0x00, 0xB0, 0x10, 0x1A, 0x77, 0x38, 0xCF, 0x51,
	0xBA, 0x1F, 0x22, 0xAC, 0x62, 0x89, 0x76, 0xC3,
	0x02, 0x6E, 0x2C, 0x47, 0x3A, 0x5C, 0x1B, 0x56,
	0x8A, 0x5D, 0x03, 0x16, 0x74, 0x58, 0x79, 0x09,
	0xD7, 0xF5, 0x0A, 0x92, 0x4F, 0x87, 0xCD, 0xDA,
	0x8C, 0xC9, 0x9E, 0x3B, 0x12, 0x6B, 0x53, 0xFF,
	0x80, 0xB7, 0xF8, 0xD9, 0xF1, 0x5E, 0xAF, 0xE0,
	0x05, 0xA4, 0x14, 0x2B, 0xA3, 0xCC, 0x6C, 0x7C,
	0x78, 0xAA, 0x95, 0x84, 0x61, 0xA8, 0xCE, 0x13,
	0x88, 0xFA, 0x59, 0x4E, 0xB9, 0xC8, 0x4B, 0x24,
	0xD1, 0x07, 0x94, 0x2E, 0xDF, 0xB1, 0x17, 0xA2,
	0x1D, 0x4A, 0xC6, 0xAD, 0x15, 0x19, 0x35, 0x7F,
	0x81, 0x44, 0x0C, 0x9F, 0x75, 0x7E, 0xD4, 0x82,
	0xDE, 0xE6, 0xE1, 0x2D, 0x3E, 0x73, 0x11, 0x8B,
	0xC5, 0xA7, 0xF0, 0x6D, 0x1C, 0x64, 0x0E, 0x04,
	0x40, 0x1E, 0x8D, 0xE5, 0x3F, 0xB2, 0x65, 0x5F,
	};

//--------------------------------------------------------------------------------------
// Hash Init function
// In this version parameter Delay absent
int Mscsha::Init(int hashbitlen)
	{
		BitSequence n,i;
//Check legal hash length in bits
		if(hashbitlen != 224 && hashbitlen != 256 && hashbitlen != 384 && hashbitlen != 512)return(BAD_HASHBITLEN);
// For set SR Points
		n = (hashbitlen>>3) - 1;
// Save hash length in hash structure for future steps
		mscShaState.hashbitlen = hashbitlen;
// Start positions for SR State points
		mscShaState.x[0] = 0;
		mscShaState.x[1] = 1;
		mscShaState.x[2] = n-3;
		mscShaState.x[3] = n;
// Storage for last byte bits, if DataLengs%8 != 0 
		mscShaState.x[4] = 0;
// Number of bits in mscShaState.x[5]
		mscShaState.x[5] = 0;
// Start SR State = (0,1,...,n)
		for(i=0;i<n+1;i++)mscShaState.data[i] = i;
		return(SUCCESS);
	}

//-------------------------------------------------------------------

// This is Fast version.
// In this version we doesn't shift SR State,
// but shift only SR Points.
int Mscsha::Update(const BitSequence *data, DataLength databitlen)
	{
		DataLength i,len = databitlen>>3,len1;
		register BitSequence x1,x2,x3,x4,k,n = (mscShaState.hashbitlen>>3) - 8,N = n+8;
		register BitSequence empty,bits,last;


		if(mscShaState.hashbitlen != 224 && mscShaState.hashbitlen != 256 && mscShaState.hashbitlen != 384 && mscShaState.hashbitlen != 512)return(BAD_HASHBITLEN);

// We use fast registry memory
		x1 = mscShaState.x[0];
		x2 = mscShaState.x[1];
		x3 = mscShaState.x[2];
		x4 = mscShaState.x[3];
		bits = mscShaState.x[4];
		last = mscShaState.x[5];

// data bytes number
		i = 0;
// If we try to Update short message with length < 8 bits,
// in this case all transformations we make in Final
// len1 - in bytes
		   len1 = (databitlen + bits)>>3;

// For cases with remain bits
		   if(bits != 0)
		   {
// In this cases we increase SR points using two steps
		       if(mscShaState.hashbitlen == 224 || mscShaState.hashbitlen == 384)
			   {
		           while(i < len1)
		           {
// Prepare input byte using remain bits
			          empty = last ^ (data[i]>>bits);
			          last = data[i]<<(8-bits);
			          mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + empty;
// Increase SR points using two steps
// Step 1
			          x1++;
			          x2++;
			          x3++;
			          x4++;
// Step 2
			          if(x1 == N)x1=0;
			          if(x2 == N)x2=0;
			          if(x3 == N)x3=0;
			          if(x4 == N)x4=0;
// Delay
				      for(k=0;k<3;k++)
				      {
//Input byte absent
						  mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])];
// Increase SR points using two steps
					      x1++;
					      x2++;
					      x3++;
					      x4++;
					      if(x1 == N)x1=0;
					      if(x2 == N)x2=0;
					      if(x3 == N)x3=0;
					      if(x4 == N)x4=0;
				       } // delay is over
// Next byte
			           i++;

		           }//while(i < len1)
			   
			   }// 224 || 384

// In this cases we increase SR points using one steps
// All same like previous except increasing SR points
		       if(mscShaState.hashbitlen == 256)
		       {
		           while(i < len1)
		           {
				       empty = last ^ (data[i]>>bits);
				       last = data[i]<<(8-bits);
				       mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + empty;
// Increase SR points using one step
// It's more fast, but not so much...
			           x1 = (x1 + 1) & (unsigned char)0x1F;
			           x2 = (x2 + 1) & (unsigned char)0x1F;
			           x3 = (x3 + 1) & (unsigned char)0x1F;
			           x4 = (x4 + 1) & (unsigned char)0x1F;
// Delay
				       for(k=0;k<3;k++)
				       {
					       mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])];
			               x1 = (x1 + 1) & (unsigned char)0x1F;
			               x2 = (x2 + 1) & (unsigned char)0x1F;
			               x3 = (x3 + 1) & (unsigned char)0x1F;
			               x4 = (x4 + 1) & (unsigned char)0x1F;
				       }
			           i++;

		            }//while(i < len1)
		       }// 256

		       if(mscShaState.hashbitlen == 512)
		       {
		           while(i < len1)
		           {
				       empty = last ^ (data[i]>>bits);
				       last = data[i]<<(8-bits);
				       mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + empty;
// Increase SR points using one step
// It's more fast, but not so much...
			           x1 = (x1 + 1) & (unsigned char)0x3F;
			           x2 = (x2 + 1) & (unsigned char)0x3F;
			           x3 = (x3 + 1) & (unsigned char)0x3F;
			           x4 = (x4 + 1) & (unsigned char)0x3F;
// Delay
				       for(k=0;k<3;k++)
				       {
					       mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])];
			               x1 = (x1 + 1) & (unsigned char)0x3F;
			               x2 = (x2 + 1) & (unsigned char)0x3F;
			               x3 = (x3 + 1) & (unsigned char)0x3F;
			               x4 = (x4 + 1) & (unsigned char)0x3F;
				       }
			           i++;

		            }//while(i < len1)
		       }// 512

// For next iteration
		       mscShaState.x[4] = databitlen + bits - (i<<3);
		       if(i != 0)mscShaState.x[5] = (last>>(8-mscShaState.x[4]))<<(8-mscShaState.x[4]);
		       else mscShaState.x[5] = ((last ^ (data[i]>>bits))>>(8-mscShaState.x[4]))<<(8-mscShaState.x[4]);

		   }// for cases with remain bits (bits != 0)


// If remain bits absent
           else
		   {

// In this cases we increase SR points using two steps
		       if(mscShaState.hashbitlen == 224 || mscShaState.hashbitlen == 384)
			   {
		           while(i < len1)
		           {
// Update SR state using input byte
			          mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + data[i];
// Increase SR points using two steps
// Step 1
			          x1++;
			          x2++;
			          x3++;
			          x4++;
// Step 2
			          if(x1 == N)x1=0;
			          if(x2 == N)x2=0;
			          if(x3 == N)x3=0;
			          if(x4 == N)x4=0;
// Delay
				      for(k=0;k<3;k++)
				      {
//Input byte absent
						  mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])];
// Increase SR points using two steps
					      x1++;
					      x2++;
					      x3++;
					      x4++;
					      if(x1 == N)x1=0;
					      if(x2 == N)x2=0;
					      if(x3 == N)x3=0;
					      if(x4 == N)x4=0;
				       } // delay is over
// Next byte
			           i++;

		           }//while(i < len1)
			   
			   }// 224 || 384

// In this cases we increase SR points using one steps
// All same like previous except increasing SR points
		       if(mscShaState.hashbitlen == 256)
		       {
		           while(i < len1)
		           {
// Update SR state using input byte
			          mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + data[i];
// Increase SR points using one step
// It's more fast, but not so much...
			           x1 = (x1 + 1) & (unsigned char)0x1F;
			           x2 = (x2 + 1) & (unsigned char)0x1F;
			           x3 = (x3 + 1) & (unsigned char)0x1F;
			           x4 = (x4 + 1) & (unsigned char)0x1F;
// Delay
				       for(k=0;k<3;k++)
				       {
					       mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])];
			               x1 = (x1 + 1) & (unsigned char)0x1F;
			               x2 = (x2 + 1) & (unsigned char)0x1F;
			               x3 = (x3 + 1) & (unsigned char)0x1F;
			               x4 = (x4 + 1) & (unsigned char)0x1F;
				       }
			           i++;

		            }//while(i < len1)
		       }// 256

		       if(mscShaState.hashbitlen == 512)
		       {
		           while(i < len1)
		           {
// Update SR state using input byte
			          mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + data[i];
// Increase SR points using one step
// It's more fast, but not so much...
			           x1 = (x1 + 1) & (unsigned char)0x3F;
			           x2 = (x2 + 1) & (unsigned char)0x3F;
			           x3 = (x3 + 1) & (unsigned char)0x3F;
			           x4 = (x4 + 1) & (unsigned char)0x3F;
// Delay
				       for(k=0;k<3;k++)
				       {
					       mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])];
			               x1 = (x1 + 1) & (unsigned char)0x3F;
			               x2 = (x2 + 1) & (unsigned char)0x3F;
			               x3 = (x3 + 1) & (unsigned char)0x3F;
			               x4 = (x4 + 1) & (unsigned char)0x3F;
				       }
			           i++;

		            }//while(i < len1)
		       }// 512

// For next iteration
		   mscShaState.x[4] = databitlen  - (len<<3);
		   mscShaState.x[5] = (data[i]>>(8-mscShaState.x[4]))<<(8-mscShaState.x[4]);
		   }// for cases without remain bits (bits == 0)


// Write register variables to hash structure
		mscShaState.x[0]=x1;
		mscShaState.x[1]=x2;
		mscShaState.x[2]=x3;
		mscShaState.x[3]=x4;

		return(SUCCESS);
	}

//------------------------------------------------------------------
// Final transformation
// 4*N times update SR state without delay
// Same like Update function, exept delay part.
int Mscsha::Final(BitSequence *md)
{
		BitSequence data[256];
		DataLength i,databitlen = mscShaState.hashbitlen>>1;
		register BitSequence x1,x2,x3,x4,n = (mscShaState.hashbitlen>>3) - 8,N = n+8;
		register BitSequence empty,bits,last;

		if(mscShaState.hashbitlen != 224 && mscShaState.hashbitlen != 256 && mscShaState.hashbitlen != 384 && mscShaState.hashbitlen != 512)return(BAD_HASHBITLEN);


// We use fast registry memory
		x1 = mscShaState.x[0];
		x2 = mscShaState.x[1];
		x3 = mscShaState.x[2];
		x4 = mscShaState.x[3];
		bits = mscShaState.x[4];
		last = mscShaState.x[5];
// Prepare input sequence for Final (4 times repeated SR State)
		memcpy(data,mscShaState.data,N);
		i = N;
		while (i!=databitlen)
		{
		   memcpy(data+i,data,N);
		   i+=N;
		}
// Same cycle like in Update, but without delay
		i = 0;
		if(bits != 0)
		{
		   if(mscShaState.hashbitlen == 224 || mscShaState.hashbitlen == 384)
		   {
		     while(i < databitlen)
		     {
				empty = last ^ (data[i]>>bits);
				last = data[i]<<(8-bits);
				mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + empty;
			    x1++;
			    x2++;
			    x3++;
			    x4++;
			    if(x1 == N)x1=0;
			    if(x2 == N)x2=0;
			    if(x3 == N)x3=0;
			    if(x4 == N)x4=0;
			    i++;
			 }

		   }//224 or 384

		   if(mscShaState.hashbitlen == 256)
		   {
		     while(i < databitlen)
		     {
				empty = last ^ (data[i]>>bits);
				last = data[i]<<(8-bits);
				mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + empty;
			    x1 = (x1 + 1) & (unsigned char)0x1F;
			    x2 = (x2 + 1) & (unsigned char)0x1F;
			    x3 = (x3 + 1) & (unsigned char)0x1F;
			    x4 = (x4 + 1) & (unsigned char)0x1F;
			    i++;
			 }

		   }//256

		   if(mscShaState.hashbitlen == 512)
		   {
		     while(i < databitlen)
		     {
				empty = last ^ (data[i]>>bits);
				last = data[i]<<(8-bits);
				mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + empty;
			    x1 = (x1 + 1) & (unsigned char)0x3F;
			    x2 = (x2 + 1) & (unsigned char)0x3F;
			    x3 = (x3 + 1) & (unsigned char)0x3F;
			    x4 = (x4 + 1) & (unsigned char)0x3F;
			    i++;
			 }
		   }//512
		}// bits != 0

		// bits == 0
		else
		{
		   if(mscShaState.hashbitlen == 224 || mscShaState.hashbitlen == 384)
		   {
		     while(i < databitlen)
		     {
			    mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + data[i];

				x1++;
			    x2++;
			    x3++;
			    x4++;
			    if(x1 == N)x1=0;
			    if(x2 == N)x2=0;
			    if(x3 == N)x3=0;
			    if(x4 == N)x4=0;
			    i++;
			 }

		   }//224 or 384

		   if(mscShaState.hashbitlen == 256)
		   {
		     while(i < databitlen)
		     {
			    mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + data[i];

				x1 = (x1 + 1) & (unsigned char)0x1F;
			    x2 = (x2 + 1) & (unsigned char)0x1F;
			    x3 = (x3 + 1) & (unsigned char)0x1F;
			    x4 = (x4 + 1) & (unsigned char)0x1F;
			    i++;
			 }

		   }//256

		   if(mscShaState.hashbitlen == 512)
		   {
		     while(i < databitlen)
		     {
			    mscShaState.data[x1] = MSCSHA_S[(unsigned char)(mscShaState.data[x1] - mscShaState.data[x2] - mscShaState.data[x3] + mscShaState.data[x4])] + data[i];
				x1 = (x1 + 1) & (unsigned char)0x3F;
			    x2 = (x2 + 1) & (unsigned char)0x3F;
			    x3 = (x3 + 1) & (unsigned char)0x3F;
			    x4 = (x4 + 1) & (unsigned char)0x3F;
			    i++;
			 }
		   }//512
		}// bits == 0

// Hash ready
		memcpy(md,mscShaState.data,N);

return (SUCCESS);
}


//------------------------------------------------------------------
// Total Hash.
// No comments.
int Mscsha::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
	  int ret;
	  //MCSSHA_CTX c;
	  ret = Mscsha::Init(hashbitlen);
	  if(ret != SUCCESS)return(ret);
	  ret = Mscsha::Update(data,databitlen);
	  if(ret != SUCCESS)return(ret);
	  return(Mscsha::Final(hashval));
}