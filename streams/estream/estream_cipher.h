#pragma once

#include <array>
#include <eacirc-core/optional.h>
#include <eacirc-core/random.h>
#include <eacirc-core/json.h>
#include <memory>
#include <stream.h>

struct estream_interface;

std::unique_ptr<estream_interface> create_estream_cipher(const std::string& name, const unsigned round);

struct estream_cipher {
  estream_cipher(const std::string& name, const unsigned round, const std::size_t iv_size, const std::size_t key_size);

  estream_cipher(estream_cipher&&);
  ~estream_cipher();

  void setup_key_iv(std::unique_ptr<stream>& key, std::unique_ptr<stream> &iv);

  void encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext, const std::size_t size);
  void decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext, const std::size_t size);

private:
  std::vector<value_type> _iv;
  std::vector<value_type> _key;

  std::unique_ptr<estream_interface> _encryptor;
  std::unique_ptr<estream_interface> _decryptor;
};
