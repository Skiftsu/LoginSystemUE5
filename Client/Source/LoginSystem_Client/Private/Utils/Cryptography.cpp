#include "Utils/Cryptography.h"
#include "Misc/Base64.h"

#include <iomanip>
#include <sstream>
#include <string>

#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

string Cryptography::SHA256(const FString Message)
{
	const string Str = TCHAR_TO_UTF8(*Message);
	unsigned char Hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX SHA256; SHA256_Init(&SHA256);
	SHA256_Update(&SHA256, Str.c_str(), Str.size());
	SHA256_Final(Hash, &SHA256);
	
	stringstream MessageHash;
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) MessageHash << hex << setw(2) << setfill('0') << static_cast<int>(Hash[i]);

	return MessageHash.str();
}

void Cryptography::Encrypt(const TArray<uint8> InMessage, string& OutMessage, string& OutIv)
{
	TArray<uint8> Iv;
	Iv.AddUninitialized(16);
	RAND_bytes(Iv.GetData(), 16);
	
	EVP_CIPHER_CTX* Ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(Ctx, EVP_aes_256_cbc(), nullptr, SharedSecretKey.GetData(), Iv.GetData());

	TArray<uint8> EncryptedMsg;
	EncryptedMsg.AddUninitialized(InMessage.Num() + EVP_CIPHER_CTX_block_size(Ctx));
	int32 Len;
	EVP_EncryptUpdate(Ctx, EncryptedMsg.GetData(), &Len,InMessage.GetData(), InMessage.Num());
	int32 CiphertextLen = Len;
	EVP_EncryptFinal_ex(Ctx, EncryptedMsg.GetData() + Len, &Len);
	CiphertextLen += Len;
	EncryptedMsg.SetNum(CiphertextLen);
	
	OutIv = TCHAR_TO_ANSI(*FBase64::Encode(Iv));
	OutMessage = TCHAR_TO_ANSI(*FBase64::Encode(EncryptedMsg));
	EVP_CIPHER_CTX_free(Ctx);
}

string Cryptography::Decrypt(const string InMessage, const string InIv)
{
	TArray<uint8> Message;
	FBase64::Decode(FString(InMessage.c_str()), Message);
	TArray<uint8> Iv;
	FBase64::Decode(FString(InIv.c_str()), Iv);
	
	EVP_CIPHER_CTX* Ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(Ctx, EVP_aes_256_cbc(), nullptr, SharedSecretKey.GetData(), Iv.GetData());
	
	TArray<uint8> OutBytes;
	OutBytes.AddUninitialized(Message.Num() + EVP_CIPHER_CTX_block_size(Ctx));

	int32 Len;
	EVP_DecryptUpdate(Ctx, OutBytes.GetData(), &Len, Message.GetData(), Message.Num());
	EVP_DecryptFinal_ex(Ctx, OutBytes.GetData() + OutBytes.Num(), &Len);

	EVP_CIPHER_CTX_free(Ctx);
	return string(reinterpret_cast<char*>(OutBytes.GetData()), OutBytes.Num());
}

/* ========================== Diffie Hellman ========================== */

string Cryptography::GenerateKeyPair_DH()
{
	if(!DhKeyExchange)
	{
		DhKeyExchange = DH_new();
		constexpr unsigned char dh_p[] = {
			0xEB, 0x61, 0x99, 0x83, 0x8E, 0x4E, 0x30, 0xFD, 0x0F, 0xC0,
			0x4E, 0x1B, 0x69, 0xDB, 0x3D, 0x85, 0x59, 0x4D, 0xB8, 0x55,
			0x08, 0xAF, 0x9D, 0x5D, 0x83, 0xB7, 0x60, 0x20, 0xA0, 0xCE,
			0x22, 0x23, 0x8D, 0x00, 0x72, 0x06, 0xA9, 0xC0, 0x55, 0x01,
			0x31, 0xB9, 0xFB, 0x73, 0xD2, 0x1F, 0xF0, 0x0C, 0xFF, 0xA2,
			0xCE, 0x8F, 0xB1, 0xCA, 0x64, 0xA3, 0xBF, 0x46, 0x61, 0x45,
			0xB6, 0xD1, 0xD7, 0x2B};
		
		constexpr unsigned char dh_g[] = {0x02};
		DH_set0_pqg(DhKeyExchange, BN_bin2bn(dh_p, sizeof dh_p, nullptr), nullptr, BN_bin2bn(dh_g, sizeof dh_g, nullptr));
	}

	DH_generate_key(DhKeyExchange);
	return BN_bn2dec(DH_get0_pub_key(DhKeyExchange));
}

void Cryptography::ComputeSharedSecretKey_DH(const string PublicKey)
{
	BIGNUM* PubKey = BN_new();
	BN_dec2bn(&PubKey, PublicKey.c_str());
	
	SharedSecretKey.AddUninitialized(64);
	DH_compute_key(SharedSecretKey.GetData(), PubKey, DhKeyExchange);
	
	DH_free(DhKeyExchange);
	DhKeyExchange = nullptr;
	BN_free(PubKey);
	KeyExchangeHasBeenCompleted = true;
	GLog->Log("Successful secret key generation");
}