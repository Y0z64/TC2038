#include <openssl/evp.h>
#include <string>

template <class T>
class Hash
{
public:
    Hash() {}
    int hash(const T& key, const int size);
};

// Include the implementation in the header file
template <class T>
int Hash<T>::hash(const T& key, const int size) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    EVP_MD_CTX *context = EVP_MD_CTX_new();
    if (context != nullptr) {
        if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {
            if constexpr (std::is_same<T, std::string>::value) {
                EVP_DigestUpdate(context, key.c_str(), key.length());
            }
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
                uint64_t result = high ^ low;
                return result % size;
            }
        }
        EVP_MD_CTX_free(context);
    }
    return 0;
}
