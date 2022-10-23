//
// Created by jeff on 2021/10/19.
//

#ifndef EMDB_DAEENCRYPTTOOL_H
#define EMDB_DAEENCRYPTTOOL_H

#include <string>

class AES
{
public:
    AES(unsigned char *key);
    virtual ~AES();
    unsigned char *Cipher(unsigned char *input);
    unsigned char *InvCipher(unsigned char *input);
    void *Cipher(void *input, int length = 0);
    void *InvCipher(void *input, int length);

private:
    unsigned char Sbox[256];
    unsigned char InvSbox[256];
    unsigned char _w[11][4][4];

    void KeyExpansion(unsigned char *key, unsigned char w[][4][4]);
    unsigned char FFmul(unsigned char a, unsigned char b);

    void SubBytes(unsigned char state[][4]);
    void ShiftRows(unsigned char state[][4]);
    void MixColumns(unsigned char state[][4]);
    void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);

    void InvSubBytes(unsigned char state[][4]);
    void InvShiftRows(unsigned char state[][4]);
    void InvMixColumns(unsigned char state[][4]);
};



#define AES_DEFAULT_KEY "343949349~!@##$$$+__)(**&^%$%NM<<>>>>>JHGFDZXCBNM<>???PPO(*&^%$$$$&POPPOOIII"

class  AesEncryptor
{
public:
    AesEncryptor(unsigned char *key = (unsigned char *)AES_DEFAULT_KEY);
    ~AesEncryptor(void);

    std::string EncryptString(std::string strInfor);
    std::string DecryptString(std::string strMessage);

    void EncryptTxtFile(const char *inputFileName, const char *outputFileName);
    void DecryptTxtFile(const char *inputFileName, const char *outputFileName);

private:
    void Byte2Hex(const unsigned char *src, int len, char *dest);
    void Hex2Byte(const char *src, int len, unsigned char *dest);
    int Char2Int(char c);

private:
    AES *m_pEncryptor;
};



#endif //EMDB_DAEENCRYPTTOOL_H
