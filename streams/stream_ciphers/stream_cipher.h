#pragma once

#include <array>
#include <eacirc-core/optional.h>
#include <eacirc-core/random.h>
#include <eacirc-core/json.h>
#include <memory>
#include <stream.h>

#include "stream_interface.h"

namespace stream_ciphers {

std::unique_ptr<stream_interface> create_stream_cipher(const std::string& name, const unsigned round);

struct stream_cipher {
  stream_cipher(const std::string& name, const unsigned round, const std::size_t iv_size, const std::size_t key_size);

  stream_cipher(stream_cipher&&);
  ~stream_cipher();

  void setup_key_iv(std::unique_ptr<stream>& key, std::unique_ptr<stream> &iv);

  void encrypt(const std::uint8_t* plaintext, std::uint8_t* ciphertext, const std::size_t size);
  void decrypt(const std::uint8_t* ciphertext, std::uint8_t* plaintext, const std::size_t size);

protected:
  std::vector<value_type> _iv;
  std::vector<value_type> _key;

  std::unique_ptr<stream_interface> _encryptor;
  std::unique_ptr<stream_interface> _decryptor;
};

} // namespace stream_ciphers
