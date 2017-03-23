#include <assert.h>
#include <string.h>

#include "Chi_sha3.h"

/*
 * Byte i of 64 bit x.  i==0 returns top byte.
 */
#define CHI_BYTE(x, i) ((BitSequence)(((x) >> (8 * (7 - i))) & 0xFF))

#define CHI_WORD2BYTE(w, b)         \
    do {                        \
        (b)[7] = CHI_BYTE(w, 7);    \
        (b)[6] = CHI_BYTE(w, 6);    \
        (b)[5] = CHI_BYTE(w, 5);    \
        (b)[4] = CHI_BYTE(w, 4);    \
        (b)[3] = CHI_BYTE(w, 3);    \
        (b)[2] = CHI_BYTE(w, 2);    \
        (b)[1] = CHI_BYTE(w, 1);    \
        (b)[0] = CHI_BYTE(w, 0);    \
    } while(0)

/*
 * Hashes the bytes in hs_DataBuffer.  Either calls te 256 or 512 hash.
 *
 * state    The current state.
 * final    1 for final block in hash, 0 for intermediate blocks.
 */
void
Chi::hash(int final)
{
    switch (chiState.hs_HashBitLen)
    {
    case 224:
    case 256:
		_Chi_256_update(&chiState, final, chiNumRounds256);
        break;

    case 384:
    case 512:
        _Chi_512_update(&chiState, final, chiNumRounds512);
        break;
    }
}

/*
 * Small routine for incrementing total input data length by hs_DataLen.
 *
 * Parameters:
 *      state       A structure that holds the chiHashState information
 *
 * Returns:
 *      CHI_SUCCESS         - on success.
 *      CHI_TOO_MUCH_DATA   - on overflow.
 */
INLINE int
Chi::inc_total_len ()
{
    unsigned long long    old_len;

    old_len = chiState.hs_TotalLenLow;
    chiState.hs_TotalLenLow += chiState.hs_DataLen;
    if (old_len > chiState.hs_TotalLenLow)
    {
        switch (chiState.hs_HashBitLen)
        {
        case 224:
        case 256:
            return CHI_TOO_MUCH_DATA;

        case 384:
        case 512:
            if (++chiState.hs_TotalLenHigh == 0)
                return CHI_TOO_MUCH_DATA;
        }
    }

    return CHI_SUCCESS;
}

/*
 * Initializes a chiHashState with the intended hash length of this particular
 * instantiation.
 *
 * Additionally, any data independent setup is performed.
 *
 * Parameters:
 *     state        A structure that holds the chiHashState information
 *     hashbitlen   An integer value that indicates the length of the hash
 *                  output in bits.
 *
 * Returns:
 *     CHI_SUCCESS        - on success
 *     BAD_HASHBITLEN - hashbitlen is invalid (e.g., unknown value)
 *
 */
int
Chi::Init(int hashbitlen)
{
    if (&chiState == NULL)
       return CHI_BAD_STATE;

    switch (hashbitlen)
    {
    default:
        return CHI_BAD_HASHLEN;

    case 224:
        memcpy(chiState.hs_State.small, Chi_224_init, _CHI_256_STATE_N * sizeof(unsigned long long));
        chiState.hs_MessageLen = _CHI_256_MSG_BLK_LEN;
        break;

    case 256:
        memcpy(chiState.hs_State.small, Chi_256_init, _CHI_256_STATE_N * sizeof(unsigned long long));
        chiState.hs_MessageLen = _CHI_256_MSG_BLK_LEN;
        break;

    case 384:
        memcpy(chiState.hs_State.large, Chi_384_init, _CHI_512_STATE_N * sizeof(unsigned long long));
        chiState.hs_MessageLen = _CHI_512_MSG_BLK_LEN;
        break;

    case 512:
        memcpy(chiState.hs_State.large, Chi_512_init, _CHI_512_STATE_N * sizeof(unsigned long long));
        chiState.hs_MessageLen = _CHI_512_MSG_BLK_LEN;
        break;
    }

    chiState.hs_HashBitLen = (ChiHashBitLen)hashbitlen;
    chiState.hs_DataLen      = 0;
    chiState.hs_TotalLenLow  = 0;
    chiState.hs_TotalLenHigh = 0;
   
    return CHI_SUCCESS;
}

/*
 * Process the supplied data.
 *
 * Parameters:
 *    state         A structure that holds the chiHashState information
 *    data          The data to be hashed
 *    databitlen    The length, in bits, of the data to be hashed
 *
 * Returns:
 *    CHI_SUCCESS  - on success
 *    CHI_BAD_DATA - data is NULL, or call made to update and previous
 *               call passed in a partial byte.
 *
 */
int Chi::Update(const BitSequence *data, DataLength databitlen)
{
    //HashReturn          ret;
	int ret;
    unsigned int            cs;
    /*
     * cs       Amount of data copied out of data into hash block.
     */

    if (&chiState == NULL)
       return CHI_BAD_STATE;
    if (data == NULL)
        return CHI_BAD_DATA;

    /*
     * Handle partial bytes only if last call to update
     */
    if ((chiState.hs_DataLen & 0x7) != 0)
        return CHI_BAD_DATA;

    while (databitlen > 0)
    {
        cs = CHI_MIN((unsigned int)databitlen, chiState.hs_MessageLen - chiState.hs_DataLen);

        memcpy(chiState.hs_DataBuffer+chiState.hs_DataLen / 8, data, (cs + 7) / 8);
        data += cs / 8;
        databitlen -= cs;
        chiState.hs_DataLen += cs;

        if (chiState.hs_DataLen >= (unsigned int)chiState.hs_MessageLen)
        {
            /*
             * Hash hs_DataBuffer
             */
            hash(0);
            ret = Chi::inc_total_len();
            if (ret != CHI_SUCCESS)
                return ret;
            chiState.hs_DataLen = 0;
        }
    }

    return CHI_SUCCESS;
}

/*
 * Perform any post processing and output filtering required and return
 * the final hash value.
 *
 * Parameters:
 *     state    A structure that holds the chiHashState information
 *     hashval  The storage for the final hash value to be returned
 *
 * Returns:
 *     CHI_SUCCESS - on success
 *
 */
int Chi::Final(BitSequence *hashval)
{
    //HashReturn  ret;
	int ret;
    unsigned int    whole_bytes;
    unsigned int    last_byte_bits;
    unsigned int    left_over_bytes;
    unsigned int    length_bytes;
    /*
     * whole_bytes      Number of whole bytes in last block with data in them.
     * last_byte_bits   Number of bits in last byte in last block with data in
     *                  them.
     * left_over_bytes  The number of bytes in the last message block that
     *                  have padding & the message length in them.
     * length_bytes     The number of bytes to use right at the end of the
     *                  message to store the message length.
     */

    if (&chiState == NULL)
       return CHI_BAD_STATE;
    if (hashval == NULL)
       return CHI_BAD_HASHVALPTR;

    ret = Chi::inc_total_len();
    if (ret != CHI_SUCCESS)
        return ret;

    switch (chiState.hs_HashBitLen)
    {
    case 224:
    case 256:
        length_bytes = sizeof (unsigned long long);
        break;

    default: /* Suppress compiler warning. */
        assert(0);
    case 384:
    case 512:
        length_bytes = sizeof (unsigned long long) * 2;
        break;
    }

    whole_bytes = chiState.hs_DataLen / 8;
    last_byte_bits = chiState.hs_DataLen % 8;
   
    /*
     * Padding
     *
     * Append a 1 to end of buffer and pad byte with zeros.
     */
    chiState.hs_DataBuffer[whole_bytes] &= ~((1 << (7-last_byte_bits)) - 1);
    chiState.hs_DataBuffer[whole_bytes] |= (1 << (7-last_byte_bits));

    /*
     * Skip to end of byte
     */
    chiState.hs_DataLen += (8 - last_byte_bits);

    left_over_bytes = (chiState.hs_MessageLen - chiState.hs_DataLen) / 8;

    if (left_over_bytes < length_bytes)
    {
        /*
         * length doesn't fit in the end of this block.  Pad this block to the
         * end using zeros then write the length in the next block.
         */
        memset(chiState.hs_DataBuffer + chiState.hs_DataLen/8, 0, left_over_bytes);
        hash(0);
        chiState.hs_DataLen = 0;
        left_over_bytes = chiState.hs_MessageLen / 8;
    }

    memset(chiState.hs_DataBuffer + chiState.hs_DataLen / 8, 0, left_over_bytes);

    /*
     * Write 64 bit or 128 bit message length.
     */
    whole_bytes = chiState.hs_MessageLen / 8 - length_bytes;
    if (length_bytes == sizeof (unsigned long long) * 2)
    {
        CHI_WORD2BYTE(chiState.hs_TotalLenHigh, chiState.hs_DataBuffer + whole_bytes);
        whole_bytes += sizeof (unsigned long long);
    }
    else
    {
        assert(chiState.hs_TotalLenHigh == 0);
        if (chiState.hs_TotalLenHigh != 0)
            return CHI_BAD_STATE;
    }

    CHI_WORD2BYTE(chiState.hs_TotalLenLow, chiState.hs_DataBuffer + whole_bytes);
    chiState.hs_DataLen = chiState.hs_MessageLen;

    /*
     * Hash final block
     * */
    hash(1);
    chiState.hs_DataLen = 0;

    /*
     * Write hash output into buffer in network byte order.
     */
    switch (chiState.hs_HashBitLen)
    {
    case 224:
        CHI_WORD2BYTE(chiState.hs_State.large[0],hashval);
        CHI_WORD2BYTE(chiState.hs_State.large[1],hashval + 8);
        CHI_WORD2BYTE(chiState.hs_State.large[3],hashval + 16);
        hashval[24] = CHI_BYTE(chiState.hs_State.large[4], 0);
        hashval[25] = CHI_BYTE(chiState.hs_State.large[4], 1);
        hashval[26] = CHI_BYTE(chiState.hs_State.large[4], 2);
        hashval[27] = CHI_BYTE(chiState.hs_State.large[4], 3);
        break;

    case 256:
        CHI_WORD2BYTE(chiState.hs_State.large[0],hashval);
        CHI_WORD2BYTE(chiState.hs_State.large[1],hashval + 8);
        CHI_WORD2BYTE(chiState.hs_State.large[3],hashval + 16);
        CHI_WORD2BYTE(chiState.hs_State.large[4],hashval + 24);
        break;

    case 384:
        CHI_WORD2BYTE(chiState.hs_State.large[0],hashval);
        CHI_WORD2BYTE(chiState.hs_State.large[1],hashval + 8);
        CHI_WORD2BYTE(chiState.hs_State.large[2],hashval + 16);
        CHI_WORD2BYTE(chiState.hs_State.large[3],hashval + 24);
        CHI_WORD2BYTE(chiState.hs_State.large[4],hashval + 32);
        CHI_WORD2BYTE(chiState.hs_State.large[5],hashval + 40);
        break;

    case 512:
        CHI_WORD2BYTE(chiState.hs_State.large[0],hashval);
        CHI_WORD2BYTE(chiState.hs_State.large[1],hashval + 8);
        CHI_WORD2BYTE(chiState.hs_State.large[2],hashval + 16);
        CHI_WORD2BYTE(chiState.hs_State.large[3],hashval + 24);
        CHI_WORD2BYTE(chiState.hs_State.large[4],hashval + 32);
        CHI_WORD2BYTE(chiState.hs_State.large[5],hashval + 40);
        CHI_WORD2BYTE(chiState.hs_State.large[6],hashval + 48);
        CHI_WORD2BYTE(chiState.hs_State.large[7],hashval + 56);
        break;
    }

    return CHI_SUCCESS;
}

/*
 * Hash the supplied data and provide the resulting hash value. Set return
 * code as appropriate.
 *
 * Parameters:
 *    hashbitlen    The length in bits of the desired hash value
 *    data          The data to be hashed
 *    databitlen    The length, in bits, of the data to be hashed
 *    hashval       The resulting hash value of the provided data
 *
 * Returns:
 *    CHI_SUCCESS - on success
 *    CHI_FAIL – arbitrary failure
 *    CHI_BAD_HASHLEN – unknown hashbitlen requested
 */
int Chi::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
    //chiHashState           hs;
    //HashReturn          ret_code;
	int ret_code;

	if ((ret_code = Chi::Init(hashbitlen)) != CHI_SUCCESS)
        return ret_code;

    if ((ret_code = Chi::Update(data, databitlen)) != CHI_SUCCESS)
        return ret_code;

    ret_code = Chi::Final(hashval);

    return ret_code;
}

Chi::Chi(const int numRounds) {
	if (numRounds == -1) {
		chiNumRounds256 = _CHI_256_STEPS;
		chiNumRounds512 = _CHI_512_STEPS;
	} else {
		chiNumRounds256 = numRounds;
		chiNumRounds512 = numRounds;
	}
}