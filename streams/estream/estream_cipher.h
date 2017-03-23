#pragma once

#include <array>
#include <core/optional.h>
#include <core/random.h>
#include <memory>

struct estream_interface;

enum class estream_ivtype { ZEROS, ONES, RANDOM, BIASRANDOM };
enum class estream_keytype { ZEROS, ONES, RANDOM, BIASRANDOM };

struct estream_cipher {
  constexpr static unsigned block_size = 16;

  estream_cipher(const std::string& name, core::optional<unsigned> round, const std::string& ivtype,
                 const std::string& keytype);

  estream_cipher(estream_cipher&&);
  ~estream_cipher();

  void setup_iv(polymorphic_generator& rng);
  void setup_key(polymorphic_generator& rng);

  void encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext, std::size_t size);
  void decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext, std::size_t size);

private:
  estream_ivtype _ivtype;
  estream_keytype _keytype;

  std::array<std::uint8_t, block_size> _iv;
  std::array<std::uint8_t, block_size> _key;

  std::unique_ptr<estream_interface> _encryptor;
  std::unique_ptr<estream_interface> _decryptor;
};
