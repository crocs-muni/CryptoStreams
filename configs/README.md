Prepare readme that illustrates what you have to do to run experiment.

# Typical tasks

 * example config file for first run

 * change primitive type
 * change cipher name
 * key size
 * (iv size)
 * data size
 * choose config file with given generation strategy
 * use multiple seeds if you need to run more experiments
 * write experiment's description for backup-ing your configs

# Later

 * prepare script that generates updates configs for tested cryptoprimitive


# Overview of inputs useful for cryptanalysis

## Stream ciphers

* single keystream
* IV reinitialization keystream
 * random IV
 * counter IV
 * LHW
* key reinitialization keystream
* IV and key random, reinitialization -> output (key, IV, keystream)
* SAC
 * (original IV, k, keystream with IV, keystream with SAC IV)
 * (IV, original k, keystream with k, keystream with SAC k)
* LHW key

## PRNG

* single sequence
* reseeding length ?single block, 2 blocks?
 * seeding with counter
 * seeding with LHW, HHW
* reseeding with rnd -> output (seed, stream)
* SAC
 * (original seed, stream with seed, stream with SAC seed)

## Hash functions

* sac
 * rnd position
 * all positions separately
* sac 2D (what with booltest?)
* cube
* counter
* LHW
* HHW
* RPC
* rho stream
 * take just k-th output (Syso's idea, probably not worthy, as it runs as more rounds = discussion)

## Block ciphers

* CTR
* LHW
* HHW
* SAC
* OFB (our rho stream)
 * all with key as well

* RPC
 * (plaintext, ciphertext)
 * (key, ciphertext)
 * (key, plaintext, ciphertext)
