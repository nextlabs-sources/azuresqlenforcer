#ifndef TDS_BUFFER_LIST_H
#define TDS_BUFFER_LIST_H

#include <list>
#include <numeric>
#include "TDSHelper.h"

class TdsBufferList
{
public:
	TdsBufferList(std::list<PBYTE> listBuf) : buffer_(nullptr), buffer_len_(0)
	{
		for (auto it = listBuf.begin(); it != listBuf.end(); ++it)
		{
			if (it == listBuf.begin())
			{
				buffer_len_ += TDS::GetTdsPacketLength(*it);
			}
			else
			{
				buffer_len_ += TDS::GetTdsPacketLength(*it) - TDS_HEADER_LEN;
			}
		}

		buffer_ = new BYTE[buffer_len_];
		PBYTE p = buffer_;
		for (auto it = listBuf.begin(); it != listBuf.end(); ++it)
		{
			if (it == listBuf.begin())
			{
				memcpy(p, *it, TDS::GetTdsPacketLength(*it));
				p += TDS::GetTdsPacketLength(*it);
			}
			else
			{
				memcpy(p, (*it) + TDS_HEADER_LEN, TDS::GetTdsPacketLength(*it) - TDS_HEADER_LEN);
				p += TDS::GetTdsPacketLength(*it) - TDS_HEADER_LEN;
			}
		}
	}
	~TdsBufferList() { delete[] buffer_; }
	PBYTE Get() { return buffer_; }
    size_t Len() { return buffer_len_; }
private:
	PBYTE buffer_;
    size_t buffer_len_;
};

#endif // !TDS_BUFFER_LIST_H

