#pragma once

#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#include <openssl/dh.h>
THIRD_PARTY_INCLUDES_END
#undef UI

class Cryptography 
{
public:
	static std::string SHA256(const FString Message);
	
	static void Encrypt(const TArray<uint8> InMessage, std::string& OutMessage, std::string& OutIv);
	static std::string Decrypt(const std::string InMessage, const std::string InIv);
	
	//Diffie Hellman
	static std::string GenerateKeyPair_DH(); /* Function return generated public key */
	static void ComputeSharedSecretKey_DH(const std::string PublicKey);
	static inline bool KeyExchangeHasBeenCompleted = false;

private:
	static inline DH* DhKeyExchange = nullptr;
	static inline TArray<uint8> SharedSecretKey;
};
