#include "Hash.h"
#include <openssl/evp.h>
#include <string>

template <class T>
Hash<T>::Hash() {};

template <class T>
Hash<T>::Hash(const bool ultraSecure) {
    this->ultraSecure = ultraSecure;
};

template <class T>
int Hash<T>::hash(const T &key, const int size) {
  // Unsigned values are used since at 8-bits they go from 0-255 instead
  // of -128 to 127 Since hash values are usually represented as bytes, we can
  // use unsigned chars to represent bytes correctly as positive values
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int lengthOfHash = 0;

  // Using the OpenSSL EVP (ENVELOPE) library to hash the key with
  // sha256
  EVP_MD_CTX *context = EVP_MD_CTX_new();
  if (context != nullptr) {
    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {

      // If its a string
      if constexpr (std::is_same<T, std::string>::value) {
        EVP_DigestUpdate(context, key.c_str(), key.length());
      }

      // For other values interpret as byte arrays
      else {
        EVP_DigestUpdate(context, &key, sizeof(T));
      }

      if (EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
        EVP_MD_CTX_free(context);
 
        uint64_t high = 0;
        uint64_t low = 0;

        if (lengthOfHash >= 16) {
          for (unsigned int i = 0; i < 8; i++) {
            high = (high << 8) | hash[i];
          }

          for (unsigned int i = 8; i < 16; i++) {
            low = (low << 8) | hash[i];
          }
        }

        uint64_t result = high ^ low; // XOR the high and low parts of the hash
        return result % size;
      }
    }
    EVP_MD_CTX_free(context);
  }

  // If we reach this, the hash failed
  return 0;
}