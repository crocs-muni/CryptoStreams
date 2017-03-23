#include <stdio.h>
#include <stdlib.h>

#include "MeshHash_sha3.h"

#define MESHHASH_ROT(w,b) ((((w) & 0xffffffffffffffffULL) >> (b) ^ (w) << (64 - (b))) & 0xffffffffffffffffULL)

unsigned long long MeshHash::meshhash_sbox(unsigned long long w)
{
	w = w * 0x9e3779b97f4a7bb9ULL + 0x5e2d58d8b3bcdef7ULL;
	w = MESHHASH_ROT(w,37);
	w = w * 0x9e3779b97f4a7bb9ULL + 0x5e2d58d8b3bcdef7ULL;
	w = MESHHASH_ROT(w,37);
	return w;
}

void MeshHash::meshhash_add_to_counter(unsigned long long counter[], DataLength to_add)
{
	unsigned long long temp;
	int i, carry;
	
	carry = 0;
	for (i = 0; i < MESHHASH_COUNTER_LENGTH; i++)
	{
		temp = (to_add & 0xffffffffffffffffULL) + carry & 0xffffffffffffffffULL;
		counter[i] += temp;
		counter[i] &= 0xffffffffffffffffULL;
		if (carry == 1 && temp == 0)
			carry = 1;
		else if (counter[i] < temp)
			carry = 1;
		else
			carry = 0;
		to_add >>= 32; /* should be ">>= 64", but is undefined for 64-bit long long */
		to_add >>= 32; /* is necessary for long long with more than 64 bits */
	}
	return;
}

void MeshHash::normal_round(hashState *state, unsigned long long data)
{
	int i;
	
	/* for simplicity */
	unsigned long long *pipe;

	pipe = state->pipe;

	/* save current state of pipe[0] */
	pipe[state->number_of_pipes] = pipe[0];

	/* process every pipe */
	for (i = 0; i < state->number_of_pipes; i++)
	{
		pipe[i] = pipe[i] ^ i*0x0101010101010101ULL ^ data;
		pipe[i]	= MESHHASH_ROT(pipe[i], i*37 & 63);
		pipe[i]	= MeshHash::meshhash_sbox(pipe[i]);
		pipe[i] = pipe[i] + pipe[i+1] & 0xffffffffffffffffULL;
	}
	
	/* update block_round_counter */
	state->block_round_counter++;
	return;
}

void MeshHash::final_block_round(hashState *state)
{
	int i,j;
	
	/* for simplicity */
	int number_of_pipes;
	unsigned long long *pipe;

	pipe = state->pipe;
	number_of_pipes = state->number_of_pipes;

	/* reset block_round_counter for next block */
	state->block_round_counter = 0;
	
	/* process block_counter */
	for (i = 0; i < number_of_pipes; i++)
		pipe[i] = MeshHash::meshhash_sbox(pipe[i] ^ state->block_counter[i % MESHHASH_COUNTER_LENGTH]);
	MeshHash::meshhash_add_to_counter(state->block_counter, 1);

	/* process key */
	if (state->key_length > 0)
	{
		for (i = state->key_counter; i < state->key_length + state->key_counter; i += number_of_pipes)
			for (j = 0; j < number_of_pipes; j++)
				pipe[j] = MeshHash::meshhash_sbox(pipe[j] ^ state->key[(i + j) % state->key_length]);
		
		state->key_counter = (state->key_counter + 1) % state->key_length;
		
		/* process key_length */
		for (i = 0; i < number_of_pipes; i++)
			state->pipe[i] = MeshHash::meshhash_sbox(state->pipe[i] ^ (unsigned long long)state->key_length ^ i*0x0101010101010101ULL);
	}

	return;
}

int MeshHash::Update(const BitSequence *data, DataLength databitlen)
{
	const BitSequence *end;

	/* data has to be an array of bytes */

	/* update bit_counter */
	MeshHash::meshhash_add_to_counter(meshhashState.bit_counter, databitlen);

	/* process given data stream */
	while (databitlen > 0)
	{
		/* process as many words as possible */
		if (meshhashState.data_counter == 0)
		{
			/* compute the address of the last whole word */
			end = data + (databitlen / 8 & ~(DataLength)7);
			databitlen -= databitlen & ~(DataLength)63;
			while(data != end)
			{
				MeshHash::normal_round(&meshhashState, (unsigned long long)*data << 56 
							^ (unsigned long long)*(data+1) << 48
							^ (unsigned long long)*(data+2) << 40
							^ (unsigned long long)*(data+3) << 32
							^ (unsigned long long)*(data+4) << 24
							^ (unsigned long long)*(data+5) << 16
							^ (unsigned long long)*(data+6) << 8
							^ (unsigned long long)*(data+7));
				if (meshhashState.block_round_counter == meshhashState.number_of_pipes)
					MeshHash::final_block_round(&meshhashState);
				data += 8;
			}
		}
		
		/* process bits and bytes that can not be processed as part of a word above */
		if (databitlen >= 8)
		{
			/* store a byte in data_buffer */
			meshhashState.data_buffer <<= 8;
			meshhashState.data_buffer |= *data;
			data++;
			databitlen -= 8;
			meshhashState.data_counter++;
		}
		else if (databitlen > 0)
		{
			/* process the last bits of a message */
			meshhashState.data_buffer <<= 8;
			meshhashState.data_buffer |= *data >> 8-databitlen << 8-databitlen & 0xff;
			databitlen = 0;
			meshhashState.data_counter++;
		}
		
		/* if a whole word is in data_buffer, process it */
		if (meshhashState.data_counter == 8)
		{
			MeshHash::normal_round(&meshhashState, meshhashState.data_buffer);
			if (meshhashState.block_round_counter == meshhashState.number_of_pipes)
				MeshHash::final_block_round(&meshhashState);
			meshhashState.data_counter = 0;
			meshhashState.data_buffer = 0;
		}
	}

	return SUCCESS;
}

void MeshHash::final_rounds(hashState *state)
{
	int i,j;
	
	/* empty data_buffer */
	if (state->data_counter > 0)
	{
		state->data_buffer <<= 8 * (8 - state->data_counter);
		MeshHash::normal_round(state, state->data_buffer);
		state->data_counter = 0;
		state->data_buffer = 0;
	}

	/* finish actual block */
	if (state->block_round_counter != 0)
	{
		while (state->block_round_counter < state->number_of_pipes)
			MeshHash::normal_round(state, 0);
		MeshHash::final_block_round(state);
	}

	/* process bit_counter */
	for (i = 0; i < MESHHASH_COUNTER_LENGTH; i++)
		for (j = 0; j < state->number_of_pipes; j++)
			state->pipe[j] = MeshHash::meshhash_sbox(state->pipe[j] ^ state->bit_counter[i] ^ j*0x0101010101010101ULL);
	
	/* process hashbitlen */
	for (i = 0; i < state->number_of_pipes; i++)
		state->pipe[i] = MeshHash::meshhash_sbox(state->pipe[i] ^ (unsigned long long)state->hashbitlen ^ i*0x0101010101010101ULL);
	
	/* set sponge-mode to squeezing */
	state->squeezing = 1;
}

int MeshHash::SqueezeNBytes(BitSequence *hashval, int rounds)
{
	int i,j;
	unsigned long long temp;

	if (!meshhashState.squeezing)
		MeshHash::final_rounds(&meshhashState);

	for (i = 0; i < rounds; i++)
	{
		MeshHash::normal_round(&meshhashState, 0);
		
		temp = 0;
		for (j = 0; j < meshhashState.number_of_pipes; j += 2)
			temp ^= meshhashState.pipe[j];
		hashval[i] = temp & 0xff;

		if (meshhashState.block_round_counter == meshhashState.number_of_pipes)
			MeshHash::final_block_round(&meshhashState);
	}

	return SUCCESS;
}

void MeshHash::Deinit()
{
	free(meshhashState.pipe);
	if (meshhashState.key_length > 0)
		free(meshhashState.key);
}

int MeshHash::Final(BitSequence *hashval)
{
	int error;

	error = MeshHash::SqueezeNBytes(hashval, meshhashState.hashbitlen / 8);
	MeshHash::Deinit();
	return error;
}

int MeshHash::Hash2(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval, int keybytelen, unsigned char *key)
{
	int error;
	//hashState state;

	error = MeshHash::Init2(hashbitlen, keybytelen, key);
	if (error != SUCCESS)
		return error;

	error = MeshHash::Update(data, databitlen);
	if (error != SUCCESS)
		return error;

	error = MeshHash::Final(hashval);
	if (error != SUCCESS)
		return error;

	return SUCCESS;
}

int MeshHash::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
	int error;
	//hashState state;

	error = MeshHash::Init(hashbitlen);
	if (error != SUCCESS)
		return error;

	error = MeshHash::Update(data, databitlen);
	if (error != SUCCESS)
		return error;

	error = MeshHash::Final(hashval);
	if (error != SUCCESS)
		return error;

	return SUCCESS;
}

int MeshHash::Init3(int hashbitlen, int number_of_pipes, int keybytelen, unsigned char *key)
{
	int i;
	int error;

	if (hashbitlen % 8 != 0 || hashbitlen < 0 || hashbitlen > 0x7fff)
		return BAD_HASHBITLEN;
	
	meshhashState.hashbitlen = hashbitlen;
	meshhashState.number_of_pipes = number_of_pipes;

	if (keybytelen % 8 != 0 || keybytelen < 0 || keybytelen > 0x7fff)
		return BAD_KEY_LENGTH;
	meshhashState.key_length = keybytelen/8;
	if (meshhashState.key_length > 0)
	{
		meshhashState.key = (unsigned long long*) calloc(meshhashState.key_length, sizeof(unsigned long long));
		if (meshhashState.key == NULL)
			return MEMORY_ALLOCATION_FAILED;
		for (i = 0; i < meshhashState.key_length; i++)
			meshhashState.key[i] = (unsigned long long)(key[8*i] & 0xff) << 56
					^ (unsigned long long)(key[8*i+1] & 0xff) << 48
					^ (unsigned long long)(key[8*i+2] & 0xff) << 40
					^ (unsigned long long)(key[8*i+3] & 0xff) << 32
					^ (unsigned long long)(key[8*i+4] & 0xff) << 24
					^ (unsigned long long)(key[8*i+5] & 0xff) << 16
					^ (unsigned long long)(key[8*i+6] & 0xff) << 8
					^ (unsigned long long)(key[8*i+7] & 0xff);
	}
	else
		meshhashState.key = NULL;

	meshhashState.block_round_counter = 0;
	meshhashState.key_counter = 0;
	meshhashState.data_counter = 0;
	meshhashState.data_buffer = 0;
	meshhashState.squeezing = 0;

	meshhashState.pipe = (unsigned long long*) calloc(number_of_pipes + 1, sizeof(unsigned long long));
	if (meshhashState.pipe == NULL)
		return MEMORY_ALLOCATION_FAILED;

	for (i = 0; i < MESHHASH_COUNTER_LENGTH; i++)
	{
		meshhashState.bit_counter[i] = 0;
		meshhashState.block_counter[i] = 0;
	}

	error = SUCCESS;
	if (meshhashState.key_length > 0)
		error = MeshHash::Update(key, 8*keybytelen);
	
	for (i = 0; i < MESHHASH_COUNTER_LENGTH; i++)
	{
		meshhashState.bit_counter[i] = 0;
	}

	return error;
}

int MeshHash::ComputeNumberOfPipes(int hashbitlen)
{
	int number_of_pipes;

	number_of_pipes = (hashbitlen + 63) / 64;
	number_of_pipes += (MESHHASH_NUMBER_OF_EXTRA_PIPES);
	if (number_of_pipes < MESHHASH_MIN_NUMBER_OF_PIPES)
		number_of_pipes = MESHHASH_MIN_NUMBER_OF_PIPES;
	if (number_of_pipes > MESHHASH_MAX_NUMBER_OF_PIPES)
		number_of_pipes = MESHHASH_MAX_NUMBER_OF_PIPES;
	return number_of_pipes;
}

int MeshHash::Init2(int hashbitlen, int keybytelen, unsigned char *key)
{
	return MeshHash::Init3(hashbitlen, ComputeNumberOfPipes(hashbitlen), keybytelen, key);
}

int MeshHash::Init(int hashbitlen)
{
	return MeshHash::Init3(hashbitlen, ComputeNumberOfPipes(hashbitlen), 0, NULL);
}

int MeshHash::InitSponge2(int number_of_pipes, int keybytelen, unsigned char *key)
{
	return MeshHash::Init3(0, number_of_pipes, keybytelen, key);
}

int MeshHash::InitSponge(int number_of_pipes)
{
	return MeshHash::Init3(0, number_of_pipes, 0, NULL);
}

MeshHash::MeshHash(const int numRounds) {
	if (numRounds == -1) {
		meshHashNumRounds = MESHHASH_DEFAULT_ROUNDS;
	} else {
		meshHashNumRounds = numRounds * 8;
	}
}