// src: https://github.com/LowMC/lowmc/blob/master/LowMC.h , commit e847fb160ad8ca1f373efd91a55b6d67f7deb425

#ifndef CRYPTO_STREAMS_LOWMC_REF_H
#define CRYPTO_STREAMS_LOWMC_REF_H

#include <bitset>
#include <vector>
#include <string>
#include <iostream>
#include <climits>

namespace block {
namespace lowmc {

//const unsigned blocksize = 256; // Block size in bits
//const unsigned keysize = 80;    // Key size in bits
//const unsigned identitysize = blocksize - 3 * numofboxes;
// Size of the identity part in the Sbox layer

class LowMCBase {
protected:
    unsigned numofboxes = 49; // Number of Sboxes
    unsigned rounds = 12;     // Number of rounds
    unsigned orig_keysize = 80;
    unsigned orig_blocksize = 128;
    unsigned keysize = 80;
    unsigned blocksize = 128;

public:
    virtual void keysetup(const std::uint8_t *key, const std::uint64_t keysize) = 0;
    virtual void encrypt(const std::uint8_t *plaintext, std::uint8_t *ciphertext) = 0;
    virtual void decrypt(const std::uint8_t *ciphertext, std::uint8_t *plaintext) = 0;
    virtual ~LowMCBase() = default;

public:
    unsigned int getNumofboxes() const;
    unsigned int getRounds() const;
    unsigned int getOrigKeysize() const;
    unsigned int getOrigBlocksize() const;
    unsigned int getKeysize() const;
    unsigned int getBlocksize() const;
};

// ----------------------------------------------------------------------------------- bytesToBitset
template<size_t nOfBits, int I> struct LoopOnBITb {
    static inline void bytesToBitset_bit(const uint8_t* arr, size_t arr_size, std::bitset<nOfBits>& result, ssize_t offset = 0, ssize_t dir = 1) {
        if ((I>>3) < arr_size) {
            const ssize_t idx = offset + (dir * I);
            if (idx < 0 || idx >= (ssize_t)nOfBits)
                throw std::runtime_error("Underflow/overflow");
            result[offset + (dir * I)] = ((arr[I>>3] >> (I & 0x7)) & 1);
        }
    }
};

template<size_t nOfBits, int I> struct LoopOnIb {
    static inline void bytesToBitset_b(const uint8_t* arr, size_t arr_size, std::bitset<nOfBits>& result, ssize_t offset = 0, ssize_t dir = 1) {
        LoopOnBITb<nOfBits, I>::bytesToBitset_bit(arr, arr_size, result, offset, dir);
        LoopOnIb<nOfBits, I-1>::bytesToBitset_b(arr, arr_size, result, offset, dir);
    }
};

template<size_t nOfBits> struct LoopOnIb<nOfBits, -1> {  // stop case for LoopOnI
    static inline void bytesToBitset_b(const uint8_t* arr, size_t arr_size, std::bitset<nOfBits>& result, ssize_t offset = 0, ssize_t dir = 1) {
        (void)arr; (void)arr_size; (void)result; (void)offset; (void)dir;
    }
};

template <size_t nOfBits>
void bytesToBitset_b(const uint8_t* arr, size_t arr_size, std::bitset<nOfBits>& result, ssize_t offset = 0, ssize_t dir = 1) {
    LoopOnIb<nOfBits, nOfBits - 1>::bytesToBitset_b(arr, arr_size, result, offset, dir);
}
// ----------------------------------------------------------------------------------- bytesToBitset
template<size_t nOfBytes, int I, int BIT> struct LoopOnBIT {
    static inline void bytesToBitset(const uint8_t* arr, size_t arr_size, std::bitset<nOfBytes * CHAR_BIT>& result, ssize_t offset = 0, ssize_t dir = 1) {
        if (I < arr_size) {
            const ssize_t idx = (offset + dir * ((ssize_t)(I * CHAR_BIT))) + BIT;
            if (idx < 0 || idx >= (ssize_t)nOfBytes * CHAR_BIT)
                throw std::runtime_error("Underflow/overflow");

            result[idx] = ((arr[I] >> BIT) & 1);
        }
        LoopOnBIT<nOfBytes, I, BIT+1>::bytesToBitset(arr, arr_size, result, offset, dir);
    }
};

template<size_t nOfBytes, int I> struct LoopOnBIT<nOfBytes, I, CHAR_BIT> { // stop case for LoopOnBIT
    static inline void bytesToBitset(const uint8_t* arr, size_t arr_size, std::bitset<nOfBytes * CHAR_BIT>& result, ssize_t offset = 0, ssize_t dir = 1) {
        (void)arr; (void)arr_size; (void)result; (void)offset; (void)dir;
    }
};

template<size_t nOfBytes, int I> struct LoopOnI {
    static inline void bytesToBitset(const uint8_t* arr, size_t arr_size, std::bitset<nOfBytes * CHAR_BIT>& result, ssize_t offset = 0, ssize_t dir = 1) {
        LoopOnBIT<nOfBytes, I, 0>::bytesToBitset(arr, arr_size, result, offset, dir);
        LoopOnI<nOfBytes, I-1>::bytesToBitset(arr, arr_size, result, offset, dir);
    }
};

template<size_t nOfBytes> struct LoopOnI<nOfBytes, -1> { // stop case for LoopOnI
    static inline void bytesToBitset(const uint8_t* arr, size_t arr_size, std::bitset<nOfBytes * CHAR_BIT>& result, ssize_t offset = 0, ssize_t dir = 1) {
        (void)arr; (void)arr_size; (void)result; (void)offset; (void)dir;
    }
};

template <size_t nOfBytes>
void bytesToBitset(const uint8_t* arr, size_t arr_size, std::bitset<nOfBytes * CHAR_BIT>& result, ssize_t offset = 0, ssize_t dir = 1) {
    LoopOnI<nOfBytes, nOfBytes - 1>::bytesToBitset(arr, arr_size, result, offset, dir);
}
// -------------------------------------------------------------------------------------------------

template<size_t nOfBits, bool I> struct BitsetConversion {
    static inline void bytesToBitsetMain(const uint8_t* arr, size_t arr_size, std::bitset<nOfBits>& result, ssize_t offset = 0, ssize_t dir = 1) {
        (void)arr; (void)arr_size; (void)result; (void)offset; (void)dir;
    }
};

template<size_t nOfBits> struct BitsetConversion<nOfBits, true> {
    static inline void bytesToBitsetMain(const uint8_t* arr, size_t arr_size, std::bitset<nOfBits>& result, ssize_t offset = 0, ssize_t dir = 1) {
        bytesToBitset_b<nOfBits>(arr, arr_size, result, offset, dir);
    }
};

template<size_t nOfBits> struct BitsetConversion<nOfBits, false> {
    static inline void bytesToBitsetMain(const uint8_t* arr, size_t arr_size, std::bitset<nOfBits>& result, ssize_t offset = 0, ssize_t dir = 1) {
        ssize_t noffset = offset;
        if (offset > 0)
            noffset -= CHAR_BIT - 1;
        else if (noffset < 0)
            noffset += CHAR_BIT + 1;
        bytesToBitset<(nOfBits>>3)>(arr, arr_size, result, noffset, dir);
    }
};
// -------------------------------------------------------------------------------------------------

template <size_t nbits>
void bytesToBitsetUni(const uint8_t* arr, size_t arr_size, std::bitset<nbits>& result, ssize_t offset = 0, ssize_t dir = 1){
#ifdef BUILD_testsuite
#pragma message "LowMC bit_bound=128 for tests"
    constexpr size_t bit_bound = 128;  // testing bit bound to cover both bit conversions
#else
    constexpr size_t bit_bound = 600;
#endif

    if (nbits > bit_bound && (nbits & 0x7) != 0)
        throw std::runtime_error("Unaligned nbits size");
    BitsetConversion<nbits, (nbits < bit_bound)>::bytesToBitsetMain(arr, arr_size, result, offset, dir);
}

template <unsigned tpl_blocksize, unsigned tpl_keysize>
class LowMC : public LowMCBase {
    typedef std::bitset<tpl_blocksize> block; // Store messages and states
    typedef std::bitset<tpl_keysize> keyblock;

public:
    LowMC(unsigned rounds = 0, unsigned blocksize = 0, unsigned keysize = 0, unsigned numofboxes = 0) {
        orig_blocksize = tpl_blocksize;
        orig_keysize = tpl_keysize;
        blocksize = blocksize != 0 ? blocksize : tpl_blocksize;
        keysize = keysize != 0 ? keysize : tpl_keysize;

        if (blocksize > tpl_blocksize)
            throw std::runtime_error("Configured block size is larger than template maximum");
        if (keysize > tpl_keysize)
            throw std::runtime_error("Configured key size is larger than template maximum");

        if (rounds > 0)
            this->rounds = rounds;
        if (blocksize > 0)
            this->blocksize = blocksize;
        if (keysize > 0)
            this->keysize = keysize;
        if (numofboxes > 0)
            this->numofboxes = numofboxes;
        instantiate_LowMC();
    };

    ~LowMC() override = default;

    block encrypt(const block message){
        block c = message ^ roundkeys[0];
        for (unsigned r = 1; r <= rounds; ++r) {
            c = Substitution(c);
            c = MultiplyWithGF2Matrix(LinMatrices[r - 1], c);
            c ^= roundconstants[r - 1];
            c ^= roundkeys[r];
        }
        return c;
    }

    block decrypt(const block message){
        block c = message;
        for (unsigned r = rounds; r > 0; --r) {
            c ^= roundkeys[r];
            c ^= roundconstants[r - 1];
            c = MultiplyWithGF2Matrix(invLinMatrices[r - 1], c);
            c = invSubstitution(c);
        }
        c ^= roundkeys[0];
        return c;
    }

    void set_key(keyblock k){
        key = k;
        keyschedule();
    }

    void print_matrices(){
        std::cout << "LowMC matrices and constants" << std::endl;
        std::cout << "============================" << std::endl;
        std::cout << "Block size: " << blocksize << std::endl;
        std::cout << "Key size: " << keysize << std::endl;
        std::cout << "Rounds: " << rounds << std::endl;
        std::cout << std::endl;

        std::cout << "Linear layer matrices" << std::endl;
        std::cout << "---------------------" << std::endl;
        for (unsigned r = 1; r <= rounds; ++r) {
            std::cout << "Linear layer " << r << ":" << std::endl;
            for (auto row : LinMatrices[r - 1]) {
                std::cout << "[";
                for (unsigned i = 0; i < blocksize; ++i) {
                    std::cout << row[i];
                    if (i != blocksize - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << "]" << std::endl;
            }
            std::cout << std::endl;
        }

        std::cout << "Round constants" << std::endl;
        std::cout << "---------------------" << std::endl;
        for (unsigned r = 1; r <= rounds; ++r) {
            std::cout << "Round constant " << r << ":" << std::endl;
            std::cout << "[";
            for (unsigned i = 0; i < blocksize; ++i) {
                std::cout << roundconstants[r - 1][i];
                if (i != blocksize - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;
            std::cout << std::endl;
        }

        std::cout << "Round key matrices" << std::endl;
        std::cout << "---------------------" << std::endl;
        for (unsigned r = 0; r <= rounds; ++r) {
            std::cout << "Round key matrix " << r << ":" << std::endl;
            for (auto row : KeyMatrices[r]) {
                std::cout << "[";
                for (unsigned i = 0; i < keysize; ++i) {
                    std::cout << row[i];
                    if (i != keysize - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << "]" << std::endl;
            }
            if (r != rounds) {
                std::cout << std::endl;
            }
        }
    }

    unsigned identitySize() { return blocksize - 3 * numofboxes; }

    void keysetup(const std::uint8_t *key, const std::uint64_t keysize) override {
        if (keysize > this->keysize)
            throw std::runtime_error("keysetup - keysize is larger than configured one");

        bytesToBitsetUni<tpl_keysize>(key, keysize, this->key);
        keyschedule();
    }

    void encrypt(const std::uint8_t *plaintext, std::uint8_t *ciphertext) override {
        block inp;
        bytesToBitsetUni<tpl_blocksize>(plaintext, blocksize, inp);
        auto cip = encrypt(inp);

        constexpr std::bitset<tpl_blocksize> mask(0xff);
        for (unsigned i = 0; i < blocksize / 8; ++i) {
            ciphertext[i] = static_cast<uint8_t>((cip & mask).to_ulong());
            cip >>= 8;
        }
    }

    void decrypt(const std::uint8_t *ciphertext, std::uint8_t *plaintext) override {
        block inp;
        bytesToBitsetUni<tpl_blocksize>(ciphertext, blocksize, inp);
        auto plain = decrypt(inp);

        constexpr std::bitset<tpl_blocksize> mask(0xff);
        for (unsigned i = 0; i < blocksize / 8; ++i) {
            plaintext[i] = static_cast<uint8_t>((plain & mask).to_ulong());
            plain >>= 8;
        }
    }

protected:
    // LowMC private data members //
    // The Sbox and its inverse
    const std::vector<unsigned> Sbox = {0x00, 0x01, 0x03, 0x06, 0x07, 0x04, 0x05, 0x02};
    const std::vector<unsigned> invSbox = {0x00, 0x01, 0x07, 0x02, 0x05, 0x06, 0x03, 0x04};
    std::vector<std::vector<block>> LinMatrices;
    // Stores the binary matrices for each round
    std::vector<std::vector<block>> invLinMatrices;
    // Stores the inverses of LinMatrices
    std::vector<block> roundconstants;
    // Stores the round constants
    keyblock key = 0;
    // Stores the master key
    std::vector<std::vector<keyblock>> KeyMatrices;
    // Stores the matrices that generate the round keys
    std::vector<block> roundkeys;
    // Stores the round keys

    // RNG state
    std::bitset<80> rng_state; // Keeps the 80 bit LSFR state

    // LowMC private functions //
    block Substitution(const block message){
        block temp = 0;
        // Get the identity part of the message
        temp ^= (message >> 3 * numofboxes);
        // Get the rest through the Sboxes
        for (unsigned i = 1; i <= numofboxes; ++i) {
            temp <<= 3;
            temp ^= Sbox[((message >> 3 * (numofboxes - i)) & block(0x7)).to_ulong()];
        }
        return temp;
    }

    // The substitution layer
    block invSubstitution(const block message) {
        block temp = 0;
        // Get the identity part of the message
        temp ^= (message >> 3 * numofboxes);
        // Get the rest through the invSboxes
        for (unsigned i = 1; i <= numofboxes; ++i) {
            temp <<= 3;
            temp ^= invSbox[((message >> 3 * (numofboxes - i)) & block(0x7)).to_ulong()];
        }
        return temp;
    }
    // The inverse substitution layer

    block MultiplyWithGF2Matrix(const std::vector<block> matrix, const block message){
        block temp = 0;
        for (unsigned i = 0; i < blocksize; ++i) {
            temp[i] = (message & matrix[i]).count() % 2;
        }
        return temp;
    }
    // For the linear layer
    block MultiplyWithGF2Matrix_Key(const std::vector<keyblock> matrix, const keyblock k){
        block temp = 0;
        for (unsigned i = 0; i < blocksize; ++i) {
            temp[i] = (k & matrix[i]).count() % 2;
        }
        return temp;
    }
    // For generating the round keys

    void keyschedule(){
        roundkeys.clear();
        for (unsigned r = 0; r <= rounds; ++r) {
            roundkeys.push_back(MultiplyWithGF2Matrix_Key(KeyMatrices[r], key));
        }
        return;
    }
    // Creates the round keys from the master key

    void instantiate_LowMC(){
        // Create LinMatrices and invLinMatrices
        LinMatrices.clear();
        invLinMatrices.clear();
        for (unsigned r = 0; r < rounds; ++r) {
            // Create matrix
            std::vector<block> mat;
            // Fill matrix with random bits
            do {
                mat.clear();
                for (unsigned i = 0; i < blocksize; ++i) {
                    mat.push_back(getrandblock());
                }
                // Repeat if matrix is not invertible
            } while (rank_of_Matrix(mat, &blocksize) != blocksize);
            LinMatrices.push_back(mat);
            invLinMatrices.push_back(invert_Matrix(LinMatrices.back(), &blocksize));
        }

        // Create roundconstants
        roundconstants.clear();
        for (unsigned r = 0; r < rounds; ++r) {
            roundconstants.push_back(getrandblock());
        }

        // Create KeyMatrices
        KeyMatrices.clear();
        for (unsigned r = 0; r <= rounds; ++r) {
            // Create matrix
            std::vector<keyblock> mat;
            // Fill matrix with random bits
            do {
                mat.clear();
                for (unsigned i = 0; i < blocksize; ++i) {
                    mat.push_back(getrandkeyblock());
                }
                // Repeat if matrix is not of maximal rank
            } while (rank_of_Matrix_Key(mat, &keysize) < std::min(blocksize, keysize));
            KeyMatrices.push_back(mat);
        }

        return;
    }
    // Fills the matrices and roundconstants with pseudorandom bits

    // Binary matrix functions //
    unsigned rank_of_Matrix(const std::vector<block> matrix, const unsigned *m_size=nullptr){
        std::vector<block> mat; // Copy of the matrix
        for (auto u : matrix) {
            mat.push_back(u);
        }

        unsigned size = m_size == nullptr ? mat[0].size() : *m_size;
        // Transform to upper triangular matrix
        unsigned row = 0;
        for (unsigned col = 1; col <= size; ++col) {
            if (!mat[row][size - col]) {
                unsigned r = row;
                while (r < mat.size() && !mat[r][size - col]) {
                    ++r;
                }
                if (r >= mat.size()) {
                    continue;
                } else {
                    auto temp = mat[row];
                    mat[row] = mat[r];
                    mat[r] = temp;
                }
            }
            for (unsigned i = row + 1; i < mat.size(); ++i) {
                if (mat[i][size - col])
                    mat[i] ^= mat[row];
            }
            ++row;
            if (row == size)
                break;
        }
        return row;
    }

    unsigned rank_of_Matrix_Key(const std::vector<keyblock> matrix, const unsigned *m_size=nullptr){
        std::vector<keyblock> mat; // Copy of the matrix
        for (auto u : matrix) {
            mat.push_back(u);
        }
        unsigned size = m_size == nullptr ? mat[0].size() : *m_size;
        // Transform to upper triangular matrix
        unsigned row = 0;
        for (unsigned col = 1; col <= size; ++col) {
            if (!mat[row][size - col]) {
                unsigned r = row;
                while (r < mat.size() && !mat[r][size - col]) {
                    ++r;
                }
                if (r >= mat.size()) {
                    continue;
                } else {
                    auto temp = mat[row];
                    mat[row] = mat[r];
                    mat[r] = temp;
                }
            }
            for (unsigned i = row + 1; i < mat.size(); ++i) {
                if (mat[i][size - col])
                    mat[i] ^= mat[row];
            }
            ++row;
            if (row == size)
                break;
        }
        return row;
    }

    std::vector<block> invert_Matrix(const std::vector<block> matrix, const unsigned *m_size=nullptr){
        std::vector<block> mat; // Copy of the matrix
        for (auto u : matrix) {
            mat.push_back(u);
        }
        std::vector<block> invmat(blocksize, 0); // To hold the inverted matrix
        for (unsigned i = 0; i < blocksize; ++i) {
            invmat[i][i] = 1;
        }

        unsigned size = m_size == nullptr ? mat[0].size() : *m_size;
        // Transform to upper triangular matrix
        unsigned row = 0;
        for (unsigned col = 0; col < size; ++col) {
            if (!mat[row][col]) {
                unsigned r = row + 1;
                while (r < mat.size() && !mat[r][col]) {
                    ++r;
                }
                if (r >= mat.size()) {
                    continue;
                } else {
                    auto temp = mat[row];
                    mat[row] = mat[r];
                    mat[r] = temp;
                    temp = invmat[row];
                    invmat[row] = invmat[r];
                    invmat[r] = temp;
                }
            }
            for (unsigned i = row + 1; i < mat.size(); ++i) {
                if (mat[i][col]) {
                    mat[i] ^= mat[row];
                    invmat[i] ^= invmat[row];
                }
            }
            ++row;
        }

        // Transform to identity matrix
        for (unsigned col = size; col > 0; --col) {
            for (unsigned r = 0; r < col - 1; ++r) {
                if (mat[r][col - 1]) {
                    mat[r] ^= mat[col - 1];
                    invmat[r] ^= invmat[col - 1];
                }
            }
        }

        return invmat;
    }

    // Random bits functions //
    block getrandblock(){
        block tmp = 0;
        for (unsigned i = 0; i < blocksize; ++i)
            tmp[i] = getrandbit();
        return tmp;
    }
    keyblock getrandkeyblock(){
        keyblock tmp = 0;
        for (unsigned i = 0; i < keysize; ++i)
            tmp[i] = getrandbit();
        return tmp;
    }
    bool getrandbit(){
        auto & state = rng_state;
        bool tmp = 0;
        // If state has not been initialized yet
        if (state.none()) {
            state.set(); // Initialize with all bits set
            // Throw the first 160 bits away
            for (unsigned i = 0; i < 160; ++i) {
                // Update the state
                tmp = state[0] ^ state[13] ^ state[23] ^ state[38] ^ state[51] ^ state[62];
                state >>= 1;
                state[79] = tmp;
            }
        }
        // choice records whether the first bit is 1 or 0.
        // The second bit is produced if the first bit is 1.
        bool choice = false;
        do {
            // Update the state
            tmp = state[0] ^ state[13] ^ state[23] ^ state[38] ^ state[51] ^ state[62];
            state >>= 1;
            state[79] = tmp;
            choice = tmp;
            tmp = state[0] ^ state[13] ^ state[23] ^ state[38] ^ state[51] ^ state[62];
            state >>= 1;
            state[79] = tmp;
        } while (!choice);
        return tmp;
    }
};


}}
#endif // CRYPTO_STREAMS_LOWMC_REF_H
