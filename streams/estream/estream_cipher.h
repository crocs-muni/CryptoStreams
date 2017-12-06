#pragma once

#include <array>
#include <eacirc-core/optional.h>
#include <eacirc-core/random.h>
#include <eacirc-core/json.h>
#include <memory>
#include <stream.h>

struct estream_interface;

std::unique_ptr<estream_interface> create_estream_cipher(const std::string& name, core::optional<unsigned> round);

struct estream_cipher {
  estream_cipher(const std::string& name, core::optional<unsigned> round, std::size_t iv_size, std::size_t key_size);

  estream_cipher(estream_cipher&&);
  ~estream_cipher();

  void setup_iv(std::unique_ptr<stream>& stream);
  void setup_key(std::unique_ptr<stream>& stream,  std::size_t iv_size);

  void encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext, std::size_t size);
  void decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext, std::size_t size);

private:
  std::vector<std::uint8_t> _iv;
  std::vector<std::uint8_t> _key;

  std::unique_ptr<estream_interface> _encryptor;
  std::unique_ptr<estream_interface> _decryptor;
};
