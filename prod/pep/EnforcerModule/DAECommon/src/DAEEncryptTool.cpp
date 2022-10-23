//
// Created by jeff on 2021/10/19.
//

#pragma warning(push,0)
#pragma warning(disable: 5045)


#include "DAEEncryptTool.h"
#include "string.h"


AES::AES(unsigned char *key)
{
    unsigned char sBox[] =
            {
                    /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
                    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, /*0*/
                    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, /*1*/
                    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, /*2*/
                    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, /*3*/
                    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, /*4*/
                    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, /*5*/
                    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, /*6*/
                    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, /*7*/
                    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, /*8*/
                    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, /*9*/
                    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, /*a*/
                    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, /*b*/
                    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, /*c*/
                    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, /*d*/
                    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, /*e*/
                    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16	/*f*/
            };
    unsigned char invsBox[256] =
            {
                    /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  */
                    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, /*0*/
                    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, /*1*/
                    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, /*2*/
                    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, /*3*/
                    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, /*4*/
                    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, /*5*/
                    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, /*6*/
                    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, /*7*/
                    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, /*8*/
                    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, /*9*/
                    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, /*a*/
                    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, /*b*/
                    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, /*c*/
                    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, /*d*/
                    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, /*e*/
                    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d	/*f*/
            };
    memcpy(Sbox, sBox, 256);
    memcpy(InvSbox, invsBox, 256);
    KeyExpansion(key, _w);
}

AES::~AES()
{
}

unsigned char *AES::Cipher(unsigned char *input)
{
    unsigned char state[4][4];
    int i, r, c;

    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            state[r][c] = input[c * 4 + r];
        }
    }

    AddRoundKey(state, _w[0]);

    for (i = 1; i <= 10; i++)
    {
        SubBytes(state);
        ShiftRows(state);
        if (i != 10)
            MixColumns(state);
        AddRoundKey(state, _w[i]);
    }

    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            input[c * 4 + r] = state[r][c];
        }
    }

    return input;
}

unsigned char *AES::InvCipher(unsigned char *input)
{
    unsigned char state[4][4];
    int i, r, c;

    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            state[r][c] = input[c * 4 + r];
        }
    }

    AddRoundKey(state, _w[10]);
    for (i = 9; i >= 0; i--)
    {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, _w[i]);
        if (i)
        {
            InvMixColumns(state);
        }
    }

    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            input[c * 4 + r] = state[r][c];
        }
    }

    return input;
}

void *AES::Cipher(void *input, int length)
{
    unsigned char *in = (unsigned char *)input;
    int i;
    if (!length)
    {
        while (*(in + length++))
            ;
        in = (unsigned char *)input;
    }
    for (i = 0; i < length; i += 16)
    {
        Cipher(in + i);
    }
    return input;
}

void *AES::InvCipher(void *input, int length)
{
    unsigned char *in = (unsigned char *)input;
    int i;
    for (i = 0; i < length; i += 16)
    {
        InvCipher(in + i);
    }
    return input;
}

void AES::KeyExpansion(unsigned char *key, unsigned char wCh[][4][4])
{
    int i, j, r, c;
    unsigned char rc[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            wCh[0][r][c] = key[r + c * 4];
        }
    }
    for (i = 1; i <= 10; i++)
    {
        for (j = 0; j < 4; j++)
        {
            unsigned char t[4];
            for (r = 0; r < 4; r++)
            {
                t[r] = j ? wCh[i][r][j - 1] : wCh[i - 1][r][3];
            }
            if (j == 0)
            {
                unsigned char temp = t[0];
                for (r = 0; r < 3; r++)
                {
                    t[r] = Sbox[t[(r + 1) % 4]];
                }
                t[3] = Sbox[temp];
                t[0] ^= rc[i - 1];
            }
            for (r = 0; r < 4; r++)
            {
                wCh[i][r][j] = wCh[i - 1][r][j] ^ t[r];
            }
        }
    }
}

unsigned char AES::FFmul(unsigned char a, unsigned char b)
{
    unsigned char bw[4];
    unsigned char res = 0;
    int i;
    bw[0] = b;
    for (i = 1; i < 4; i++)
    {
        bw[i] = bw[i - 1] << 1;
        if (bw[i - 1] & 0x80)
        {
            bw[i] ^= 0x1b;
        }
    }
    for (i = 0; i < 4; i++)
    {
        if ((a >> i) & 0x01)
        {
            res ^= bw[i];
        }
    }
    return res;
}

void AES::SubBytes(unsigned char state[][4])
{
    int r, c;
    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            state[r][c] = Sbox[state[r][c]];
        }
    }
}

void AES::ShiftRows(unsigned char state[][4])
{
    unsigned char t[4];
    int r, c;
    for (r = 1; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            t[c] = state[r][(c + r) % 4];
        }
        for (c = 0; c < 4; c++)
        {
            state[r][c] = t[c];
        }
    }
}

void AES::MixColumns(unsigned char state[][4])
{
    unsigned char t[4];
    int r, c;
    for (c = 0; c < 4; c++)
    {
        for (r = 0; r < 4; r++)
        {
            t[r] = state[r][c];
        }
        for (r = 0; r < 4; r++)
        {
            state[r][c] = FFmul(0x02, t[r]) ^ FFmul(0x03, t[(r + 1) % 4]) ^ FFmul(0x01, t[(r + 2) % 4]) ^ FFmul(0x01, t[(r + 3) % 4]);
        }
    }
}

void AES::AddRoundKey(unsigned char state[][4], unsigned char k[][4])
{
    int r, c;
    for (c = 0; c < 4; c++)
    {
        for (r = 0; r < 4; r++)
        {
            state[r][c] ^= k[r][c];
        }
    }
}

void AES::InvSubBytes(unsigned char state[][4])
{
    int r, c;
    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            state[r][c] = InvSbox[state[r][c]];
        }
    }
}

void AES::InvShiftRows(unsigned char state[][4])
{
    unsigned char t[4];
    int r, c;
    for (r = 1; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            t[c] = state[r][(c - r + 4) % 4];
        }
        for (c = 0; c < 4; c++)
        {
            state[r][c] = t[c];
        }
    }
}

void AES::InvMixColumns(unsigned char state[][4])
{
    unsigned char t[4];
    int r, c;
    for (c = 0; c < 4; c++)
    {
        for (r = 0; r < 4; r++)
        {
            t[r] = state[r][c];
        }
        for (r = 0; r < 4; r++)
        {
            state[r][c] = FFmul(0x0e, t[r]) ^ FFmul(0x0b, t[(r + 1) % 4]) ^ FFmul(0x0d, t[(r + 2) % 4]) ^ FFmul(0x09, t[(r + 3) % 4]);
        }
    }
}


#include <fstream>
using namespace std;

#ifdef _WIN32
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#else
#include <string.h>
inline void *memcpy_s(void *_Destination, size_t _DestinationSize, const void *_Source, size_t _SourceSize)
{
    return memcpy(_Destination, _Source, _SourceSize);
}
#endif



AesEncryptor::AesEncryptor(unsigned char *key)
{
    m_pEncryptor = new AES(key);
}

AesEncryptor::~AesEncryptor(void)
{
    delete m_pEncryptor;
}

void AesEncryptor::Byte2Hex(const unsigned char *src, int len, char *dest)
{
    for (int i = 0; i < len; ++i)
    {
#ifdef _WIN32
        sprintf_s(dest + i * 2, 3, "%02X", src[i]);
#else
        sprintf(dest + i * 2, "%02X", src[i]);
#endif
    }
}

void AesEncryptor::Hex2Byte(const char *src, int len, unsigned char *dest)
{
    int length = len / 2;
    for (int i = 0; i < length; ++i)
    {
#pragma warning(push)
#pragma warning(disable: 4244)	// '=': conversion from 'int' to 'unsigned char', possible loss of data
        dest[i] = Char2Int((src[i * 2])) * 16 + Char2Int(src[i * 2 + 1]);
#pragma warning(pop)
    }
}

int AesEncryptor::Char2Int(char c)
{
    if ('0' <= c && c <= '9')
    {
        return (c - '0');
    }
    else if ('a' <= c && c <= 'f')
    {
        return (c - 'a' + 10);
    }
    else if ('A' <= c && c <= 'F')
    {
        return (c - 'A' + 10);
    }
    return -1;
}

string AesEncryptor::EncryptString(string strInfor)
{
    size_t nLength = strInfor.length();
    int spaceLength = 16 - (nLength % 16);
    unsigned char *pBuffer = new unsigned char[nLength + spaceLength];
    memset(pBuffer, '\0', nLength + spaceLength);
    memcpy_s(pBuffer, nLength + spaceLength, strInfor.c_str(), nLength);
    m_pEncryptor->Cipher(pBuffer, (int)nLength + spaceLength);

    // change to hex
    char *pOut = new char[2 * (nLength + spaceLength) + 2];
    memset(pOut, '\0', 2 * (nLength + spaceLength) + 2);
    Byte2Hex(pBuffer, (int)nLength + spaceLength, pOut);

    string retValue(pOut);
    delete[] pBuffer;
    delete[] pOut;
    return retValue;
}

string AesEncryptor::DecryptString(string strMessage)
{
    size_t nLength = strMessage.length() / 2;
    if(nLength%16!=0) return "";
    unsigned char *pBuffer = new unsigned char[nLength];
    memset(pBuffer, '\0', nLength);
    Hex2Byte(strMessage.c_str(), (int)strMessage.length(), pBuffer);

    m_pEncryptor->InvCipher(pBuffer, (int)nLength);
    string retValue((char *)pBuffer);
    delete[] pBuffer;
    return retValue;
}

void AesEncryptor::EncryptTxtFile(const char *inputFileName, const char *outputFileName)
{
    ifstream ifs;

    // Open file:
    ifs.open(inputFileName);
    if (!ifs)
    {
        //UNILOGW("AesEncryptor::EncryptTxtFile() - Open input file failed!");
        return;
    }

    // Read config data:
    string strInfor;
    string strLine;
    while (!ifs.eof())
    {
        char temp[1024];
        memset(temp, '\0', 1024);
        ifs.read(temp, 1000);
        strInfor += temp;
    }
    ifs.close();

    // Encrypt
    strLine = EncryptString(strInfor);

    // Writefile
    ofstream ofs;
    ofs.open(outputFileName);
    if (!ofs)
    {
        //UNILOGW("AesEncryptor::EncryptTxtFile() - Open output file failed!");
        return;
    }
    ofs << strLine;
    ofs.close();
}

void AesEncryptor::DecryptTxtFile(const char *inputFile, const char *outputFile)
{
    ifstream ifs;

    // Open file:
    ifs.open(inputFile);
    if (!ifs)
    {
        //UNILOGW("AesEncryptor::DecryptTxtFile() - Open input file failed!");
        return;
    }

    // Read config data:
    string strInfor;
    string strLine;
    while (!ifs.eof())
    {
        char temp[1024];
        memset(temp, '\0', 1024);
        ifs.read(temp, 1000);
        strInfor += temp;
    }
    ifs.close();

    // Encrypt
    strLine = DecryptString(strInfor);

    // Writefile
    ofstream ofs;
    ofs.open(outputFile);
    if (!ofs)
    {
        //UNILOGW("AesEncryptor::DecryptTxtFile() - Open output file failed!");
        return;
    }
    ofs << strLine;
    ofs.close();
}

#if 0
int main(int argc, char **argv)
{
	if (argc < 2)
		return 0;
	// TODO change the code to make the szKey can't be crack easier directly.
	CommonFun::AesEncryptor theAes();
	std::string strEPwd = theAes.EncryptString(argv[1]);
	std::string strDPwd = theAes.DecryptString(strEPwd);
	return 0;
}
#endif

#pragma warning(pop)