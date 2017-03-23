#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __GNUC_
#include <stdint.h>
#endif

#include "Twister_sha3.h"
#include "twister_tables.h"

#define TWISTER_DATA_BLOCKSIZE       512 
#define TWISTER_DATA_BLOCKSIZE_BYTES  64 

#define TWISTER_DATA_OUTPUT_BLOCKSIZE 32

/* Message and output block size in bytes for input/output message round */
#define TWISTER_BLOCKSIZE      64 
#define TWISTER_BLOCKSIZE_BITS  6 

/* Length of the state in bytes */
#define TWISTER_STATE_BYTELEN 64

/*  Initiate Twister */
int Twister::Init(int hashbitlen) {
  if ((hashbitlen>TWISTER_DATA_BLOCKSIZE || hashbitlen <TWISTER_DATA_OUTPUT_BLOCKSIZE) 
      || (hashbitlen%TWISTER_DATA_OUTPUT_BLOCKSIZE))
    return BAD_HASHBITLEN; 
  else {
    /* Set ever state attribut to zero */
	  memset(&twisterState, 0x0 ,sizeof(Twister::hashState));
    
    twisterState.hs_counter=0xffffffffffffffffll;
    twisterState.hs_s[7]=(uint64_t)hashbitlen<<48;
    twisterState.hs_hashbitlen=hashbitlen;
    return SUCCESS;
  }
}

/***************************************************************************/

int Twister::Update(const BitSequence *data,
                  DataLength databitlen)
{

  
  /* Are there any unprocessed byte left */
  if(twisterState.hs_databitlen)
    {
      if(twisterState.hs_databitlen&7) return FAIL; 
      else 
	{
	  unsigned long int t = (unsigned long int) TWISTER_MIN(TWISTER_DATA_BLOCKSIZE-twisterState.hs_databitlen, databitlen);
	  unsigned long int p = (t&7) ? 1:0;
	  
	  p+=t>>3;
	  /* Intermediate data block with length that are not divisible by 8 */
	  memcpy(twisterState.hs_data+(twisterState.hs_databitlen>>3),data,p);

#ifdef TWISTER_DEBUG
	  {
	    unsigned int i;
	    printf("\np= %d\n",p);
	    printf("hs_databitlen(%d)+t(%d) : %d\n",twisterState.hs_databitlen,t,
		   twisterState.hs_databitlen+t);
	    for(i=0;i<(p+(twisterState.hs_databitlen>>3));i++)
	      printf("0x%02x ",twisterState.hs_data[i]);
	    puts("");
	  } 
#endif
	  
	  twisterState.hs_databitlen+=t;
	  if(twisterState.hs_databitlen==TWISTER_DATA_BLOCKSIZE) 
	    {
	      twister_twist((uint64_t *) twisterState.hs_data);
	      twisterState.hs_databitlen=0;
	      databitlen-=t;	
	    }
	  else {
	    return SUCCESS;
	  }
	}
    }

  while(databitlen>=TWISTER_DATA_BLOCKSIZE)
    {
      twister_twist((uint64_t *) data);
      data+=TWISTER_DATA_BLOCKSIZE_BYTES;
      databitlen-=TWISTER_DATA_BLOCKSIZE;
    }

  
  if(databitlen)
    {
      unsigned long int p = (databitlen&7) ? 1:0;
      p+=(unsigned long int) databitlen>>3;
      memcpy(twisterState.hs_data,data,p);
      twisterState.hs_databitlen+=databitlen;
    }    

  
  
  return SUCCESS;
}


/***************************************************************************/

int Twister::Final(BitSequence *hashval)
{
  DataLength ml= twisterState.hs_cmesglen+twisterState.hs_databitlen;
 
  if(twisterState.hs_databitlen)
    {
      unsigned long int i = twisterState.hs_databitlen>>3;
      unsigned long int r = twisterState.hs_databitlen&7;

#ifdef TWISTER_DEBUG
      {
	int j;
	puts("Unpadded  Message:");
	for(j=0;j<=i;j++) printf("0x%02x ",twisterState.hs_data[j]);
	puts("");
      }
#endif     

      twisterState.hs_data[i]&=0xFF<<(8-r);
      twisterState.hs_data[i]|=1<<(7-r);
      i+=1;
      memset(twisterState.hs_data+i,0,TWISTER_DATA_BLOCKSIZE_BYTES-i);

#ifdef TWISTER_DEBUG
      puts("Padded  Message:");
      for(r=0;r<i;r++) printf("0x%02x ",twisterState.hs_data[r]);
      puts("");
#endif
    }
  else
    {
      memset(twisterState.hs_data,0,TWISTER_DATA_BLOCKSIZE_BYTES);
      twisterState.hs_data[0]=0x80;
    }

  twister_twist((uint64_t *) twisterState.hs_data);
  

  /* message length */
  twisterState.hs_s[0]^=(uint8_t) (ml>>56);
  twisterState.hs_s[1]^=(uint8_t) (ml>>48);
  twisterState.hs_s[2]^=(uint8_t) (ml>>40);
  twisterState.hs_s[3]^=(uint8_t) (ml>>32);
  twisterState.hs_s[4]^=(uint8_t) (ml>>24);
  twisterState.hs_s[5]^=(uint8_t) (ml>>16);
  twisterState.hs_s[6]^=(uint8_t) (ml>>8);  
  twisterState.hs_s[7]^=(uint8_t) (ml);
  twister_mini_round();

    if(twisterState.hs_hashbitlen<=256)
    {
     twister_mini_round();
    }
  else
    {
      uint64_t ff[TWISTER_ROWS];

      ff[7]=twisterState.hs_s[7];
      ff[6]=twisterState.hs_s[6];
      ff[5]=twisterState.hs_s[5];
      ff[4]=twisterState.hs_s[4];
      ff[3]=twisterState.hs_s[3];
      ff[2]=twisterState.hs_s[2];
      ff[1]=twisterState.hs_s[1];
      ff[0]=twisterState.hs_s[0];
      twisterState.hs_s[0]^=(uint8_t) (twisterState.hs_ck0>>56);
      twisterState.hs_s[1]^=(uint8_t) (twisterState.hs_ck0>>48);
      twisterState.hs_s[2]^=(uint8_t) (twisterState.hs_ck0>>40);
      twisterState.hs_s[3]^=(uint8_t) (twisterState.hs_ck0>>32);
      twisterState.hs_s[4]^=(uint8_t) (twisterState.hs_ck0>>24);
      twisterState.hs_s[5]^=(uint8_t) (twisterState.hs_ck0>>16);
      twisterState.hs_s[6]^=(uint8_t) (twisterState.hs_ck0>>8);
      twisterState.hs_s[7]^=(uint8_t) (twisterState.hs_ck0);
      twister_mini_round();

      twisterState.hs_s[0]^=(uint8_t) (twisterState.hs_ck1>>56);
      twisterState.hs_s[1]^=(uint8_t) (twisterState.hs_ck1>>48);
      twisterState.hs_s[2]^=(uint8_t) (twisterState.hs_ck1>>40);
      twisterState.hs_s[3]^=(uint8_t) (twisterState.hs_ck1>>32);
      twisterState.hs_s[4]^=(uint8_t) (twisterState.hs_ck1>>24);
      twisterState.hs_s[5]^=(uint8_t) (twisterState.hs_ck1>>16);
      twisterState.hs_s[6]^=(uint8_t) (twisterState.hs_ck1>>8);
      twisterState.hs_s[7]^=(uint8_t) (twisterState.hs_ck1);
      twister_mini_round();

      twisterState.hs_s[0]^=(uint8_t) (twisterState.hs_ck2>>56);
      twisterState.hs_s[1]^=(uint8_t) (twisterState.hs_ck2>>48);
      twisterState.hs_s[2]^=(uint8_t) (twisterState.hs_ck2>>40);
      twisterState.hs_s[3]^=(uint8_t) (twisterState.hs_ck2>>32);
      twisterState.hs_s[4]^=(uint8_t) (twisterState.hs_ck2>>24);
      twisterState.hs_s[5]^=(uint8_t) (twisterState.hs_ck2>>16);
      twisterState.hs_s[6]^=(uint8_t) (twisterState.hs_ck2>>8);
      twisterState.hs_s[7]^=(uint8_t) (twisterState.hs_ck2);
      twister_mini_round();
      twisterState.hs_s[7]^=ff[7];   
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[0]^=ff[0];


      ff[7]=twisterState.hs_s[7];
      ff[6]=twisterState.hs_s[6];
      ff[5]=twisterState.hs_s[5];
      ff[4]=twisterState.hs_s[4];
      ff[3]=twisterState.hs_s[3];
      ff[2]=twisterState.hs_s[2];
      ff[1]=twisterState.hs_s[1];
      ff[0]=twisterState.hs_s[0];
      twisterState.hs_s[0]^=(uint8_t) (twisterState.hs_ck3>>56);
      twisterState.hs_s[1]^=(uint8_t) (twisterState.hs_ck3>>48);
      twisterState.hs_s[2]^=(uint8_t) (twisterState.hs_ck3>>40);
      twisterState.hs_s[3]^=(uint8_t) (twisterState.hs_ck3>>32);
      twisterState.hs_s[4]^=(uint8_t) (twisterState.hs_ck3>>24);
      twisterState.hs_s[5]^=(uint8_t) (twisterState.hs_ck3>>16);
      twisterState.hs_s[6]^=(uint8_t) (twisterState.hs_ck3>>8);
      twisterState.hs_s[7]^=(uint8_t) (twisterState.hs_ck3);
      twister_mini_round();

      twisterState.hs_s[0]^=(uint8_t) (twisterState.hs_ck4>>56);
      twisterState.hs_s[1]^=(uint8_t) (twisterState.hs_ck4>>48);
      twisterState.hs_s[2]^=(uint8_t) (twisterState.hs_ck4>>40);
      twisterState.hs_s[3]^=(uint8_t) (twisterState.hs_ck4>>32);
      twisterState.hs_s[4]^=(uint8_t) (twisterState.hs_ck4>>24);
      twisterState.hs_s[5]^=(uint8_t) (twisterState.hs_ck4>>16);
      twisterState.hs_s[6]^=(uint8_t) (twisterState.hs_ck4>>8);
      twisterState.hs_s[7]^=(uint8_t) (twisterState.hs_ck4);
      twister_mini_round();

      twisterState.hs_s[0]^=(uint8_t) (twisterState.hs_ck5>>56);
      twisterState.hs_s[1]^=(uint8_t) (twisterState.hs_ck5>>48);
      twisterState.hs_s[2]^=(uint8_t) (twisterState.hs_ck5>>40);
      twisterState.hs_s[3]^=(uint8_t) (twisterState.hs_ck5>>32);
      twisterState.hs_s[4]^=(uint8_t) (twisterState.hs_ck5>>24);
      twisterState.hs_s[5]^=(uint8_t) (twisterState.hs_ck5>>16);
      twisterState.hs_s[6]^=(uint8_t) (twisterState.hs_ck5>>8);
      twisterState.hs_s[7]^=(uint8_t) (twisterState.hs_ck5);
      twister_mini_round();
      twisterState.hs_s[7]^=ff[7];
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[0]^=ff[0];


      ff[7]=twisterState.hs_s[7];
      ff[6]=twisterState.hs_s[6];
      ff[5]=twisterState.hs_s[5];
      ff[4]=twisterState.hs_s[4];
      ff[3]=twisterState.hs_s[3];
      ff[2]=twisterState.hs_s[2];
      ff[1]=twisterState.hs_s[1];
      ff[0]=twisterState.hs_s[0];
      twisterState.hs_s[0]^=(uint8_t) (twisterState.hs_ck6>>56);
      twisterState.hs_s[1]^=(uint8_t) (twisterState.hs_ck6>>48);
      twisterState.hs_s[2]^=(uint8_t) (twisterState.hs_ck6>>40);
      twisterState.hs_s[3]^=(uint8_t) (twisterState.hs_ck6>>32);
      twisterState.hs_s[4]^=(uint8_t) (twisterState.hs_ck6>>24);
      twisterState.hs_s[5]^=(uint8_t) (twisterState.hs_ck6>>16);
      twisterState.hs_s[6]^=(uint8_t) (twisterState.hs_ck6>>8);
      twisterState.hs_s[7]^=(uint8_t) (twisterState.hs_ck6);
      twister_mini_round();


      twisterState.hs_s[0]^=(uint8_t) (twisterState.hs_ck7>>56);
      twisterState.hs_s[1]^=(uint8_t) (twisterState.hs_ck7>>48);
      twisterState.hs_s[2]^=(uint8_t) (twisterState.hs_ck7>>40);
      twisterState.hs_s[3]^=(uint8_t) (twisterState.hs_ck7>>32);
      twisterState.hs_s[4]^=(uint8_t) (twisterState.hs_ck7>>24);
      twisterState.hs_s[5]^=(uint8_t) (twisterState.hs_ck7>>16);
      twisterState.hs_s[6]^=(uint8_t) (twisterState.hs_ck7>>8);
      twisterState.hs_s[7]^=(uint8_t) (twisterState.hs_ck7);
      twister_mini_round();

      twister_mini_round();
      twisterState.hs_s[7]^=ff[7];   
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[0]^=ff[0];
    }

  /* Output */
  twister_output((uint64_t *) hashval);

  return SUCCESS;
}

/***************************************************************************/

int Twister::Hash(int hashbitlen, const BitSequence *data, 
		DataLength databitlen, BitSequence *hashval)
{
  //hashstate state;
  int i;

  if((i=Twister::Init(hashbitlen))) return i;
  if((i=Twister::Update(data,databitlen))) return i;
  Twister::Final(hashval);
 
  return SUCCESS;
}


/***************************************************************************/

void Twister::twister_twist(uint64_t *data)
{
uint64_t ff[TWISTER_ROWS];
  if(twisterState.hs_hashbitlen<=256)
    {
      /* Twister<=256 */

      /* 1st maxi round */ 
      ff[7]=twisterState.hs_s[7];
      ff[6]=twisterState.hs_s[6];
      ff[5]=twisterState.hs_s[5];
      ff[4]=twisterState.hs_s[4];
      ff[3]=twisterState.hs_s[3];
      ff[2]=twisterState.hs_s[2];
      ff[1]=twisterState.hs_s[1];
      ff[0]=twisterState.hs_s[0];

	  if (twisterNumRounds256 >= 1) {
	  twisterState.hs_s[0]^= (uint8_t) (*data>>56); 
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds256 >= 2) {
      twisterState.hs_s[0]^= (uint8_t) (*data>>56); 
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds256 >= 3) {
      twisterState.hs_s[0]^= (uint8_t) (*data>>56);
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

      twisterState.hs_s[7]^=ff[7];   
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[0]^=ff[0];


      /* 2nd maxi round */ 
      ff[7]=twisterState.hs_s[7];
      ff[6]=twisterState.hs_s[6];
      ff[5]=twisterState.hs_s[5];
      ff[4]=twisterState.hs_s[4];
      ff[3]=twisterState.hs_s[3];
      ff[2]=twisterState.hs_s[2];
      ff[1]=twisterState.hs_s[1];
      ff[0]=twisterState.hs_s[0];

	  if (twisterNumRounds256 >= 4) {
      twisterState.hs_s[0]^= (uint8_t) (*data>>56); 
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds256 >= 5) {
      twisterState.hs_s[0]^= (uint8_t) (*data>>56);
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds256 >= 6) {
      twisterState.hs_s[0]^= (uint8_t) (*data>>56); 
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

      twisterState.hs_s[7]^=ff[7];
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[0]^=ff[0];


      /* 3rd maxi round */ 
      ff[7]=twisterState.hs_s[7];
      ff[6]=twisterState.hs_s[6];
      ff[5]=twisterState.hs_s[5];
      ff[4]=twisterState.hs_s[4];
      ff[3]=twisterState.hs_s[3];
      ff[2]=twisterState.hs_s[2];
      ff[1]=twisterState.hs_s[1];
      ff[0]=twisterState.hs_s[0];

	  if (twisterNumRounds256 >= 7) {
      twisterState.hs_s[0]^= (uint8_t) (*data>>56); 
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds256 >= 8) {
      twisterState.hs_s[0]^= (uint8_t) (*data>>56);
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds256 >= 9) {
      twister_mini_round();
	  }

      twisterState.hs_s[7]^=ff[7];
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[0]^=ff[0];

    }
  /* Twister>256 */
  else
    {
      /* 1st maxi round */
      ff[7]=twisterState.hs_s[7];
      ff[6]=twisterState.hs_s[6];
      ff[5]=twisterState.hs_s[5];
      ff[4]=twisterState.hs_s[4];
      ff[3]=twisterState.hs_s[3];
      ff[2]=twisterState.hs_s[2];
      ff[1]=twisterState.hs_s[1];
      ff[0]=twisterState.hs_s[0];

	  if (twisterNumRounds384 >= 1) {
      twisterState.hs_ck0^=twisterState.hs_s[0]+twisterState.hs_ck1;
      twisterState.hs_s[0]^= (uint8_t) (*data>>56); 
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds384 >= 2) {
      twisterState.hs_ck1^=twisterState.hs_s[0]+twisterState.hs_ck2;
      twisterState.hs_s[0]^= (uint8_t) (*data>>56);
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds384 >= 3) {
      twisterState.hs_ck2^=twisterState.hs_s[0]+twisterState.hs_ck3;
      twisterState.hs_s[0]^= (uint8_t) (*data>>56);
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

      twisterState.hs_s[7]^=ff[7];
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[0]^=ff[0];

      /* 2nd maxi round */
      ff[7]=twisterState.hs_s[7];
      ff[6]=twisterState.hs_s[6];
      ff[5]=twisterState.hs_s[5];
      ff[4]=twisterState.hs_s[4];
      ff[3]=twisterState.hs_s[3];
      ff[2]=twisterState.hs_s[2];
      ff[1]=twisterState.hs_s[1];
      ff[0]=twisterState.hs_s[0];

	  if (twisterNumRounds384 >= 4) {
      twisterState.hs_ck3^=twisterState.hs_s[0]+twisterState.hs_ck4;
      twisterState.hs_s[0]^= (uint8_t) (*data>>56);
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds384 >= 5) {
      /* Blank round */
      twister_mini_round();
	  }

	  if (twisterNumRounds384 >= 6) {
      twisterState.hs_ck4^=twisterState.hs_s[0]+twisterState.hs_ck5;
      twisterState.hs_s[0]^= (uint8_t) (*data>>56);
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

      twisterState.hs_s[7]^=ff[7];
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[0]^=ff[0];

      /* 3rd maxi round */
      ff[7]=twisterState.hs_s[7];
      ff[6]=twisterState.hs_s[6];
      ff[5]=twisterState.hs_s[5];
      ff[4]=twisterState.hs_s[4];
      ff[3]=twisterState.hs_s[3];
      ff[2]=twisterState.hs_s[2];
      ff[1]=twisterState.hs_s[1];
      ff[0]=twisterState.hs_s[0];

	  if (twisterNumRounds384 >= 7) {
      twisterState.hs_ck5^=twisterState.hs_s[0]+twisterState.hs_ck6;
      twisterState.hs_s[0]^= (uint8_t) (*data>>56);
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds384 >= 8) {
      twisterState.hs_ck6^=twisterState.hs_s[0]+twisterState.hs_ck7;
      twisterState.hs_s[0]^= (uint8_t) (*data>>56); 
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds384 >= 9) {
      twisterState.hs_ck7^=twisterState.hs_s[0]+twisterState.hs_ck0;
      twisterState.hs_s[0]^= (uint8_t) (*data>>56);
      twisterState.hs_s[1]^= (uint8_t) (*data>>48);
      twisterState.hs_s[2]^= (uint8_t) (*data>>40);
      twisterState.hs_s[3]^= (uint8_t) (*data>>32);
      twisterState.hs_s[4]^= (uint8_t) (*data>>24);
      twisterState.hs_s[5]^= (uint8_t) (*data>>16);
      twisterState.hs_s[6]^= (uint8_t) (*data>>8);
      twisterState.hs_s[7]^= (uint8_t) (*data++);
      twister_mini_round();
	  }

	  if (twisterNumRounds384 >= 10) {
      /* Blank round */
      twister_mini_round();
	  }

      twisterState.hs_s[7]^=ff[7];
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[0]^=ff[0];
    }
  twisterState.hs_cmesglen+=512;
}

/***************************************************************************/

void Twister::twister_mini_round()
{
  uint64_t p0; 
  uint64_t p1;
  uint64_t p2;
  uint64_t p3;
  uint64_t p4;
  uint64_t p5;
  uint64_t p6;
  uint64_t p7;


  /* Add inverted twist counter */
  twisterState.hs_s[1]^= twisterState.hs_counter--;

  p0= twister_t0[(uint8_t) (twisterState.hs_s[0]>>56)];
  p7= twister_t1[(uint8_t) (twisterState.hs_s[0]>>48)];
  p6= twister_t2[(uint8_t) (twisterState.hs_s[0]>>40)];
  p5= twister_t3[(uint8_t) (twisterState.hs_s[0]>>32)]; 
  p4= twister_t4[(uint8_t) (twisterState.hs_s[0]>>24)];
  p3= twister_t5[(uint8_t) (twisterState.hs_s[0]>>16)];
  p2= twister_t6[(uint8_t) (twisterState.hs_s[0]>>8)];
  p1= twister_t7[(uint8_t) (twisterState.hs_s[0])];

  p1^= twister_t0[(uint8_t) (twisterState.hs_s[1]>>56)];
  p0^= twister_t1[(uint8_t) (twisterState.hs_s[1]>>48)];
  p7^= twister_t2[(uint8_t) (twisterState.hs_s[1]>>40)];
  p6^= twister_t3[(uint8_t) (twisterState.hs_s[1]>>32)]; 
  p5^= twister_t4[(uint8_t) (twisterState.hs_s[1]>>24)];
  p4^= twister_t5[(uint8_t) (twisterState.hs_s[1]>>16)];
  p3^= twister_t6[(uint8_t) (twisterState.hs_s[1]>>8)];
  p2^= twister_t7[(uint8_t) (twisterState.hs_s[1])];

  p2^= twister_t0[(uint8_t) (twisterState.hs_s[2]>>56)];
  p1^= twister_t1[(uint8_t) (twisterState.hs_s[2]>>48)];
  p0^= twister_t2[(uint8_t) (twisterState.hs_s[2]>>40)];
  p7^= twister_t3[(uint8_t) (twisterState.hs_s[2]>>32)]; 
  p6^= twister_t4[(uint8_t) (twisterState.hs_s[2]>>24)];
  p5^= twister_t5[(uint8_t) (twisterState.hs_s[2]>>16)]; 
  p4^= twister_t6[(uint8_t) (twisterState.hs_s[2]>>8)];
  p3^= twister_t7[(uint8_t) (twisterState.hs_s[2])];

  p3^= twister_t0[(uint8_t) (twisterState.hs_s[3]>>56)];
  p2^= twister_t1[(uint8_t) (twisterState.hs_s[3]>>48)];
  p1^= twister_t2[(uint8_t) (twisterState.hs_s[3]>>40)];
  p0^= twister_t3[(uint8_t) (twisterState.hs_s[3]>>32)]; 
  p7^= twister_t4[(uint8_t) (twisterState.hs_s[3]>>24)];
  p6^= twister_t5[(uint8_t) (twisterState.hs_s[3]>>16)]; 
  p5^= twister_t6[(uint8_t) (twisterState.hs_s[3]>>8)];
  p4^= twister_t7[(uint8_t) (twisterState.hs_s[3])];

  p4^= twister_t0[(uint8_t) (twisterState.hs_s[4]>>56)];
  p3^= twister_t1[(uint8_t) (twisterState.hs_s[4]>>48)];
  p2^= twister_t2[(uint8_t) (twisterState.hs_s[4]>>40)];
  p1^= twister_t3[(uint8_t) (twisterState.hs_s[4]>>32)];
  p0^= twister_t4[(uint8_t) (twisterState.hs_s[4]>>24)];
  p7^= twister_t5[(uint8_t) (twisterState.hs_s[4]>>16)];
  p6^= twister_t6[(uint8_t) (twisterState.hs_s[4]>>8)];
  p5^= twister_t7[(uint8_t) (twisterState.hs_s[4])];

  p5^= twister_t0[(uint8_t) (twisterState.hs_s[5]>>56)];
  p4^= twister_t1[(uint8_t) (twisterState.hs_s[5]>>48)];
  p3^= twister_t2[(uint8_t) (twisterState.hs_s[5]>>40)];
  p2^= twister_t3[(uint8_t) (twisterState.hs_s[5]>>32)];
  p1^= twister_t4[(uint8_t) (twisterState.hs_s[5]>>24)];
  p0^= twister_t5[(uint8_t) (twisterState.hs_s[5]>>16)];
  p7^= twister_t6[(uint8_t) (twisterState.hs_s[5]>>8)];
  p6^= twister_t7[(uint8_t) (twisterState.hs_s[5])];

  p6^= twister_t0[(uint8_t) (twisterState.hs_s[6]>>56)];
  p5^= twister_t1[(uint8_t) (twisterState.hs_s[6]>>48)];
  p4^= twister_t2[(uint8_t) (twisterState.hs_s[6]>>40)];
  p3^= twister_t3[(uint8_t) (twisterState.hs_s[6]>>32)]; 
  p2^= twister_t4[(uint8_t) (twisterState.hs_s[6]>>24)];
  p1^= twister_t5[(uint8_t) (twisterState.hs_s[6]>>16)];
  p0^= twister_t6[(uint8_t) (twisterState.hs_s[6]>>8)];
  p7^= twister_t7[(uint8_t) (twisterState.hs_s[6])];

  p7^= twister_t0[(uint8_t) (twisterState.hs_s[7]>>56)];
  p6^= twister_t1[(uint8_t) (twisterState.hs_s[7]>>48)];
  p5^= twister_t2[(uint8_t) (twisterState.hs_s[7]>>40)];
  p4^= twister_t3[(uint8_t) (twisterState.hs_s[7]>>32)];
  p3^= twister_t4[(uint8_t) (twisterState.hs_s[7]>>24)];
  p2^= twister_t5[(uint8_t) (twisterState.hs_s[7]>>16)];
  p1^= twister_t6[(uint8_t) (twisterState.hs_s[7]>>8)];
  p0^= twister_t7[(uint8_t) (twisterState.hs_s[7])];

  twisterState.hs_s[0]=p0;
  twisterState.hs_s[1]=p1;
  twisterState.hs_s[2]=p2;
  twisterState.hs_s[3]=p3;
  twisterState.hs_s[4]=p4;
  twisterState.hs_s[5]=p5;
  twisterState.hs_s[6]=p6;
  twisterState.hs_s[7]=p7;
}

/***************************************************************************/

void Twister::twister_output(uint64_t *hashval)
{
  int i;
  int oblocks=twisterState.hs_hashbitlen>>6; /* Number of 64-bit outbut blocks */
  uint64_t ff[TWISTER_ROWS];

  if(twisterState.hs_hashbitlen%TWISTER_BLOCKSIZE) oblocks+=1;

  for(i=0;i<oblocks;i++)
    {
      ff[0]=twisterState.hs_s[0];
      ff[1]=twisterState.hs_s[1];
      ff[2]=twisterState.hs_s[2];
      ff[3]=twisterState.hs_s[3];
      ff[4]=twisterState.hs_s[4];
      ff[5]=twisterState.hs_s[5];
      ff[6]=twisterState.hs_s[6];
      ff[7]=twisterState.hs_s[7];
      twister_mini_round();
      twisterState.hs_s[0]^=ff[0];
      twisterState.hs_s[1]^=ff[1];
      twisterState.hs_s[2]^=ff[2];
      twisterState.hs_s[3]^=ff[3];
      twisterState.hs_s[4]^=ff[4];
      twisterState.hs_s[5]^=ff[5];
      twisterState.hs_s[6]^=ff[6];
      twisterState.hs_s[7]^=ff[7]; 
      twister_mini_round();
      hashval[i]=twisterState.hs_s[0]^ff[0];
    }

}


/***************************************************************************/

void Twister::twister_print_state()
{
  printf("%02llx %02llx %02llx %02llx %02llx %02llx %02llx %02llx\n",
	 twisterState.hs_s[0]>>56 & 0xFF, twisterState.hs_s[1]>>56 & 0xFF, 
	 twisterState.hs_s[2]>>56 & 0xFF, twisterState.hs_s[3]>>56 & 0xFF,
	 twisterState.hs_s[4]>>56 & 0xFF, twisterState.hs_s[5]>>56 & 0xFF, 
	 twisterState.hs_s[6]>>56 & 0xFF, twisterState.hs_s[7]>>56 & 0xFF);

  printf("%02llx %02llx %02llx %02llx %02llx %02llx %02llx %02llx\n",
	 twisterState.hs_s[0]>>48 & 0xFF, twisterState.hs_s[1]>>48 & 0xFF, 
	 twisterState.hs_s[2]>>48 & 0xFF, twisterState.hs_s[3]>>48 & 0xFF,
	 twisterState.hs_s[4]>>48 & 0xFF, twisterState.hs_s[5]>>48 & 0xFF, 
	 twisterState.hs_s[6]>>48 & 0xFF, twisterState.hs_s[7]>>48 & 0xFF);

  printf("%02llx %02llx %02llx %02llx %02llx %02llx %02llx %02llx\n",
	 twisterState.hs_s[0]>>40 & 0xFF, twisterState.hs_s[1]>>40 & 0xFF, 
	 twisterState.hs_s[2]>>40 & 0xFF, twisterState.hs_s[3]>>40 & 0xFF,
	 twisterState.hs_s[4]>>40 & 0xFF, twisterState.hs_s[5]>>40 & 0xFF, 
	 twisterState.hs_s[6]>>40 & 0xFF, twisterState.hs_s[7]>>40 & 0xFF);

  printf("%02llx %02llx %02llx %02llx %02llx %02llx %02llx %02llx\n",
	 twisterState.hs_s[0]>>32 & 0xFF, twisterState.hs_s[1]>>32 & 0xFF,
	 twisterState.hs_s[2]>>32 & 0xFF, twisterState.hs_s[3]>>32 & 0xFF,
	 twisterState.hs_s[4]>>32 & 0xFF, twisterState.hs_s[5]>>32 & 0xFF, 
	 twisterState.hs_s[6]>>32 & 0xFF, twisterState.hs_s[7]>>32 & 0xFF);

  printf("%02llx %02llx %02llx %02llx %02llx %02llx %02llx %02llx\n",
	 twisterState.hs_s[0]>>24 & 0xFF, twisterState.hs_s[1]>>24 & 0xFF,
	 twisterState.hs_s[2]>>24 & 0xFF, twisterState.hs_s[3]>>24 & 0xFF,
	 twisterState.hs_s[4]>>24 & 0xFF, twisterState.hs_s[5]>>24 & 0xFF,
	 twisterState.hs_s[6]>>24 & 0xFF, twisterState.hs_s[7]>>24 & 0xFF);

  printf("%02llx %02llx %02llx %02llx %02llx %02llx %02llx %02llx\n",
	 twisterState.hs_s[0]>>16 & 0xFF, twisterState.hs_s[1]>>16 & 0xFF, 
	 twisterState.hs_s[2]>>16 & 0xFF, twisterState.hs_s[3]>>16 & 0xFF,
	 twisterState.hs_s[4]>>16 & 0xFF, twisterState.hs_s[5]>>16 & 0xFF, 
	 twisterState.hs_s[6]>>16 & 0xFF, twisterState.hs_s[7]>>16 & 0xFF);

  printf("%02llx %02llx %02llx %02llx %02llx %02llx %02llx %02llx\n",
	 twisterState.hs_s[0]>>8 & 0xFF, twisterState.hs_s[1]>>8 & 0xFF, 
	 twisterState.hs_s[2]>>8 & 0xFF, twisterState.hs_s[3]>>8 & 0xFF,
	 twisterState.hs_s[4]>>8 & 0xFF, twisterState.hs_s[5]>>8 & 0xFF, 
	 twisterState.hs_s[6]>>8 & 0xFF, twisterState.hs_s[7]>>8 & 0xFF);

  printf("%02llx %02llx %02llx %02llx %02llx %02llx %02llx %02llx\n\n",
	 twisterState.hs_s[0] & 0xFF, twisterState.hs_s[1] & 0xFF,
	 twisterState.hs_s[2] & 0xFF, twisterState.hs_s[3] & 0xFF,
	 twisterState.hs_s[4] & 0xFF, twisterState.hs_s[5] & 0xFF,
	 twisterState.hs_s[6] & 0xFF, twisterState.hs_s[7] & 0xFF);
}

Twister::Twister(const int numRounds) {
	if (numRounds == -1) {
		twisterNumRounds256 = TWISTER_DEFAULT_MINI_ROUNDS_256;
		twisterNumRounds384 = TWISTER_DEFAULT_MINI_ROUNDS_384;
	} else {
		twisterNumRounds256 = numRounds;
		twisterNumRounds384 = numRounds;
	}
}