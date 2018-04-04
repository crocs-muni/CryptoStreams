/*
 * Copyright (c) Innovation Labs, Tata Consultancy Services (TCS), Hyderabad. All Rights
 * reserved.
 *
 * This software is the confidential and proprietary information of TCS.
 * ("Confidential Information").  You shall not disclose such
 * Confidential Information and shall use it only in accordance with
 * the terms of the license agreement you entered into with TCS.
 *
 * TCS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF
 * THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  TCS SHALL NOT BE LIABLE FOR
 * ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
 * DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
 */

#include "khichidi_core.h"



#ifdef EIGHT_BIT

/* Shuffling operation */
void shuffling(const KhichidiBitSequence *in, KhichidiBitSequence *out)
{
	//given input - a0,a1,a2,a3,a4,a5,a6,a7- b0,b1,b2,b3,b4,b5,b6,b7- c0,c1,c2,c3,c4,c5,c6,c7- d0,d1,d2,d3,d4,d5,d6,d7
	//output required - a0,c0,a1,c1,a2,c2,a3,c3 - a4,c4,a5,c5,a6,c6,a7,c7 - b0,d0,b1,d1,b2,d2,b3,d3 - b4,d4,b5,d6,b6,d6,b7,d7

	if(in != 0 && out != 0)
	{
		//a0,a1,a2,a3,0,0,0,0  - 0,0,0,0,c0,c1,c2,c3
		//a0,a1,a2,a3,c0,c1,c2,c3
		out[0] = (in[0] & 0xF0) | (in[2] & 0xF0)>>4;

		//0,0,c0,c1,0,0,0,0 - 0,0,0,0,a2,a3,0,0 - a0,a1,0,0,0,0,c2,c3
		//a0,a1,c0,c1,a2,a3,c2,c3
		out[0] = (out[0] & 0x0C) << 2 | (out[0] >> 2) & 0x0C | out[0] & 0xC3 ;
		//0,c0,0,0,0,c2,0,0 - 0,0,a1,0,0,0,a3,0 - a0,0,0,c1,a2,0,0,c3
		//a0,c0,a1,c1,a2,c2,a3,c3
		out[0] = (out[0] & 0x22) << 1 | (out[0] >> 1) & 0x22 | out[0] & 0x99 ;

		//a4,a5,a6,a7,0,0,0,0  - 0,0,0,0,c4,c4,c6,c7
		//a4,a5,a6,a7,c4,c5,c6,c7
		out[1] = ((in[0] & 0x0F) << 4)  | (in[2] & 0x0F);
		//0,0,c4,c5,0,0,0,0 - 0,0,0,0,a6,a7,0,0 - a4,a5,0,0,0,0,c6,c7
		//a4,a5,c4,c5,a6,a7,c6,c7
		out[1] = ((out[1] & 0x0C) << 2) | ((out[1] >> 2) & 0x0C) | (out[1] & 0xC3) ;
		//0,c4,0,0,0,c6,0,0 - 0,0,a5,0,0,0,a7,0 - a4,0,0,c5,a6,0,0,c7
		//a4,c4,a5,c5,a6,c6,a7,c7
		out[1] = ((out[1] & 0x22) << 1) | ((out[1] >> 1) & 0x22) | (out[1] & 0x99) ;


		//b0,b1,b2,b3,0,0,0,0  - 0,0,0,0,d0,d1,d2,d3
		//b0,b1,b2,b3,d0,d1,d2,d3
		out[2] = ((in[1] & 0xF0)) | ((in[3] & 0xF0) >> 4);
		//0,0,d0,d1,0,0,0,0 - 0,0,0,0,b2,b3,0,0 - b0,b1,0,0,0,0,d2,d3
		//b0,b1,d0,d1,b2,b3,d2,d3
		out[2] = ((out[2] & 0x0C) << 2) | ((out[2] >> 2) & 0x0C) | (out[2] & 0xC3) ;
		//0,d0,0,0,0,d2,0,0 - 0,0,b1,0,0,0,b3,0 - b0,0,0,d1,b2,0,0,d3
		//b0,d0,b1,d1,b2,d2,b3,d3
		out[2] = ((out[2] & 0x22) << 1) | ((out[2] >> 1) & 0x22) | (out[2] & 0x99) ;


		//b4,b5,b6,b7,0,0,0,0  - 0,0,0,0,d4,d4,d6,d7
		//b4,b5,b6,b7,d4,d5,d6,d7
		out[3] = ((in[1] & 0x0F) << 4) | (in[3] &0x0F);
		//0,0,d4,d5,0,0,0,0 - 0,0,0,0,b6,b7,0,0 - b4,b5,0,0,0,0,d6,d7
		//b4,b5,d4,d5,b6,b7,d6,d7
		out[3] = ((out[3] & 0x0C) << 2) | ((out[3] >> 2) & 0x0C) | (out[3] & 0xC3) ;
		//0,d4,0,0,0,d6,0,0 - 0,0,b5,0,0,0,b7,0 - b4,0,0,d5,b6,0,0,d7
		//b4,d4,b5,d5,b6,d6,b7,d7
		out[3] = ((out[3] & 0x22) << 1) | ((out[3] >> 1) & 0x22) | (out[3] & 0x99) ;
	}
}
#else
/* Shuffling operation */
KHICHIDI_WORD shuffling(KHICHIDI_WORD x)
{
	// KHICHIDI_WORD - 4 bytes - 32 bits 
	//given input - a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15- b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15
	//output required - a0,b0,a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7 - a8,b8,a9,b9,a10,b10,a11,b11,a12,b12,a13,b13,a14,b14,a15,b15

	x = (x & 0X0000FF00) << 8 | (x >> 8) & 0X0000FF00 | x & 0XFF0000FF;
	x = (x & 0X00F000F0) << 4 | (x >> 4) & 0X00F000F0 | x & 0XF00FF00F;
	x = (x & 0X0C0C0C0C) << 2 | (x >> 2) & 0X0C0C0C0C | x & 0XC3C3C3C3;
	x = (x & 0X22222222) << 1 | (x >> 1) & 0X22222222 | x & 0X99999999;
	return x;
}
#endif


#ifdef EIGHT_BIT
/* T- function - 2x^2+x mod 2^32 */
void t_function(const KhichidiBitSequence *in,KhichidiBitSequence *out)
{
	//input 4 bytes
	//squaring of 4 bytes number require 8 bytes to store the result value
	//adding of two 4 bytes require 5 bytes to store the result value

	KhichidiBitSequence square_result[8];
	KhichidiBitSequence temp[4];
	KhichidiBitSequence add_result[5];
	char i=0;

	if(in != 0 && out != 0)
	{
		//t_result = ((2*n*n)+n) &0xFFFFFFFF;
		// above statement is same as
		// var     = 2*((n*n)&0xFFFFFFFF) + n
		// t_result =  var & 0xFFFFFFFF;

		// x= n*n & 0xFFFFFFFF
		square(square_result,in);
		for (i=4;i<8;i++)
			temp[i-4] = square_result[i];


		// y=2x = x+x & 0xFFFFFFFF
		uadd(add_result,temp,temp,4);
		for (i=1;i<5;i++)
			temp[i-1] = add_result[i];

		// y+n & 0xFFFFFFFF
		uadd(add_result,temp,in,4);
		for (i=1;i<5;i++)
			out[i-1] = add_result[i];
	}
}

#else
/* T- function */
/* T- function - 2x^2+x mod 2^32 */
KHICHIDI_WORD t_function(KHICHIDI_WORD n)
{
	KHICHIDI_WORD t_result=0;
	KHICHIDI_WORD64 var = 2*((n*n)& 0xFFFFFFFF)+n;   // (n*n mod FFFFFFFF) becomes a 32-bit word
	t_result = (KHICHIDI_WORD) ((var)& 0xFFFFFFFF); 
	return t_result;
}

#endif



/* Unsigned addition
* Aval - big endian - example number 0x12345678 - Aval[0]=0x12,Aval[1]=0x34 ....
* Bval - 
* Cval - result 5 bytes
* limit - number of bytes of A and B
* Reference : Algorithm A in "The Art of Computer
*            Programming Vol 2 - 4.3.1" By D. Knuth
* Used in t_function
*/
void uadd(unsigned char *Cval,const unsigned char *Aval, const unsigned char *Bval,unsigned char limit)
{
	char j =0;
	unsigned char carry = 0;
	unsigned char  maxlimit;
	unsigned short sum;

	if(Cval !=0 && Aval !=0 && Bval !=0)
	{
		maxlimit = limit; 

		for(j=maxlimit-1;j>=0;j--)
		{
			sum = (unsigned short)Aval[j] + Bval[j] + carry;
			Cval[j+1] = (unsigned char)(sum & 0xFF);
			carry = (unsigned char)( sum >> 8);
		}
		Cval[j+1] =carry; //  carry 
	}
}

#ifdef EIGHT_BIT
/*
* C = A^2
* Reference :14.16 Algorithm Multi-precision Squaring in "Handbook
* of Applied Cryptography" by A.Menezes, P. van Oorschot and S. Vanstone
* Used in t_function_8
*/

void square(unsigned char *Cval,const unsigned char *Aval)
{
	//Aval - number to be squared  - number in big endian notation
	//Cval - result
	char i=0,j=0,m;
	unsigned short W=0,Ains,T1=0,T2=0;
	unsigned short carry = 0,tempcarry;

	if(Cval != 0 && Aval != 0)
	{

		m = 4; // length is 4 bytes

		for(i=0;i<2*m;i++)
		{
			Cval[i] = 0;
		}

	for(i=m-1;i>=0;i--)
	{
		Ains = (unsigned short) Aval[i];
		W = Cval[2*i + 1]+Ains*Ains;
		Cval[2*i + 1] = (unsigned char)(W & 0xFF);
		carry = (unsigned char)(W >> 8);
		for(j=i-1;j>=0;j--)
		{
			T1 = Ains*(unsigned short)Aval[j];
			T2 = T1&0xFF;
			tempcarry = (T1>>8);
			T2 *=2;
			tempcarry *= 2;
			W = Cval[i+j+1]+T2+carry;
			Cval[i+j+1] = (unsigned char)(W & 0xFF);
			carry = (unsigned char)(W >> 8);
			carry += tempcarry;
		}
		Cval[i] += (unsigned char)(carry&0xFF);
		if(i>0)
			Cval[i-1] = (unsigned char)(carry>>8);
		}
	}
}

#endif

#ifdef EIGHT_BIT

void lfsr(const KhichidiBitSequence *in, KhichidiBitSequence *out)
{
	unsigned char flag = 0;

	if(in !=0 && out != 0)
	{
		 // left shift the input value by 1
		// If MSB is 1 then XOR the reult with the primitive polynomial

		flag = in[0] & 0x80;


		// a0a1a2a3a4a5a6a7 b0b1b2b3b4b5b6b7 c0c1c2c3c4c5c6c7 d0d1d2d3d4d5d6d7 -input
		// a1a2a3a4a5a6a7b0 b1b2b3b4b5b6b7c0 c1c2c3c4c5c6c7d0 d1d2d3d4d5d6d70 - after left shift
		out[0] = in[0] << 1 | ((in[1] & 0x80) >> 7);
		out[1] = in[1] << 1 | ((in[2] & 0x80) >> 7);
		out[2] = in[2] << 1 | ((in[3] & 0x80) >> 7);
		out[3] = in[3] << 1 ;

		// primitive polynomial
		// x^32 = x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1
		// 100 11000001 00011101 10110111
		if(flag > 0)
		{
			out[0] = out[0] ^ 0x04;
			out[1] = out[1] ^ 0xC1;
			out[2] = out[2] ^ 0x1D;
			out[3] = out[3] ^ 0xB7;
		}

	}
}


#else

/*LFSR*/
KHICHIDI_WORD lfsr(KHICHIDI_WORD t_result)
{
	KHICHIDI_WORD returnValue = t_result;
	KHICHIDI_WORD flag = 0;
	flag = returnValue & 0x80000000; // Checking if MSB is 1 or 0

	// Left shift the input
	returnValue = returnValue << 1;

	// If MSB is 1 then XOR the reult with the primitive polynomial
	if(flag > 0)
	{
		returnValue = returnValue ^ 0x4C11DB7;
	}
	return returnValue;
}


#endif


#ifdef EIGHT_BIT

/* message Pre-processing */
void message_pre_process(const KhichidiBitSequence *in,KhichidiBitSequence *out,short from)
{
	short i=0;
	unsigned char result[4];

	shuffling(in,result);

	t_function(result,result);

	lfsr(result,result);
	       
	for(i=0;i<4;i++)
		out[from+i] = result[i];
}

#else

/* message Pre-processing */
KHICHIDI_WORD message_pre_process(KHICHIDI_WORD z)
{
	z = shuffling(z);
	z = t_function(z);
	z = lfsr(z);
	return z;
}

#endif


/* Padding */
void dopad(const KhichidiBitSequence *data, int datalen, KhichidiBitSequence *paddeddata, int paddeddatalen)
{
	KhichidiBitSequence PAD[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	int i = 0;
	int kdest = (datalen + 7)/8;
	int rem = datalen % 8;
	int totallen = paddeddatalen * 8;

	
	if(datalen < totallen) 
	{
		//copy the remaining data
		for(i=0;i<kdest;i++)
			paddeddata[i] = data[i];

		// set the first bit to 1 and remaining bits to zero
		if (rem==0) 
		{
			paddeddata[i]= paddeddata[i] | PAD[rem];
			i++;
		}
		else 
			paddeddata[i-1] = paddeddata[i-1]|PAD[rem];
		

		for(;i<paddeddatalen;i++)
			paddeddata[i] = 0;
			
	}
	
}




