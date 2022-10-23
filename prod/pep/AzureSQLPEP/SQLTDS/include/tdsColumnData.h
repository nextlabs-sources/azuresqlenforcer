#ifndef TDS_COLUMN_DATA_H
#define TDS_COLUMN_DATA_H

#include <windows.h>
#include "tdsPacket.h"
#include "tdsCountVarValue.h"
#include "tdsColumnDataInfo.h"
#include "tdsTypeHelper.h"
#include "MyBuffer.h"

class tdsColumnDataBase
{
public:
	virtual void Parse(PBYTE pBuf)=0;
	virtual void Parse(ReadBuffer&) = 0;
	virtual DWORD CBSize()=0;
    virtual ~tdsColumnDataBase() {}

	// by jie 2018.05.11
	virtual bool SetData(PBYTE pBuf) = 0;
	virtual int Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData) = 0;
	virtual int Serialize(WriteBuffer& wBuf) = 0;
	virtual bool IsNull() = 0;
};

template<typename LenType,typename ValueType>
class tdsColumnData : public tdsColumnDataBase
{
public:
	tdsColumnData(tdsColumnDataInfo* ColInfo):pColumnInfo(ColInfo)
	{
		bHaveTextPoint = FALSE;
		m_nullFlagByteCount = 0;
		GNULL = 1;
	}
	~tdsColumnData(void){}

public:
	void Parse(PBYTE pBuf)
	{
		PBYTE p = pBuf;

		if (pColumnInfo->IsFixedLenType())
		{
			FixedLen = tdsTypeHelper::FixedTypeLen(pColumnInfo->GetDataType() );
			assert(FixedLen<=8);
			memcpy(FixedLenData, p, FixedLen);
		}
		else
		{
			bHaveTextPoint = tdsTypeHelper::HaveTextPoint(pColumnInfo->GetDataType());
			if (bHaveTextPoint)
			{
				TextPointer.Parse(p);
				p += TextPointer.CBSize();

				memcpy(Timestamp, p, sizeof(Timestamp) );
				p += sizeof(Timestamp);
			}

			Data.Parse(p);
		}
	
	}

	void Parse(ReadBuffer& rBuf)
	{
		if (pColumnInfo->IsFixedLenType())
		{
			FixedLen = tdsTypeHelper::FixedTypeLen(pColumnInfo->GetDataType());
			assert(FixedLen <= 8);
			rBuf.ReadData(FixedLenData, FixedLen);
		}
		else
		{
			bHaveTextPoint = tdsTypeHelper::HaveTextPoint(pColumnInfo->GetDataType());
			if (bHaveTextPoint)
			{
				rBuf.PeekAny(GNULL);
				if (0x00 == GNULL)
				{
					rBuf.ReadAny(GNULL);
					return;
				}
					
				TextPointer.Parse(rBuf);
				rBuf.ReadData(Timestamp, sizeof(Timestamp));
			}
			if (PeekCheckNull(rBuf))
			{
				TDS_NULL_TYPE sz = GetNullType();
				m_nullFlagByteCount = sz;
				rBuf.ReadData(m_nullFlag, m_nullFlagByteCount);
			}
			else
			{
				Data.Parse(rBuf);
			}
		}
	}

	int Serialize(PBYTE pBuf, DWORD dwBufLen, DWORD& cbData)
	{
		cbData = 0;
		PBYTE p = pBuf;
		if (pColumnInfo->IsFixedLenType())
		{
			memcpy(p, FixedLenData, FixedLen);
			p += FixedLen;
			dwBufLen -= FixedLen;
			cbData += FixedLen;
		}
		else
		{
			if (bHaveTextPoint)
			{
				DWORD a = 0;
				TextPointer.Serialize(p, dwBufLen, a);
				p += TextPointer.CBSize();
				dwBufLen -= TextPointer.CBSize();
				cbData += TextPointer.CBSize();

				memcpy(p, Timestamp, sizeof(Timestamp));
				p += sizeof(Timestamp);
				dwBufLen -= sizeof(Timestamp);
				cbData += sizeof(Timestamp);
			}

			DWORD a = 0;
			Data.Serialize(p, dwBufLen, a);
			p += Data.CBSize();
			dwBufLen -= Data.CBSize();
			cbData += Data.CBSize();
		}
		return 0;
	}
	int Serialize(WriteBuffer& wBuf)
	{
		if (pColumnInfo->IsFixedLenType())
		{
			wBuf.WriteData(FixedLenData, FixedLen);
		}
		else
		{
			if (bHaveTextPoint)
			{
				if (GNULL == 0x00)
				{
					wBuf.WriteAny(GNULL);
					return 0;
				}

				TextPointer.Serialize(wBuf);
				wBuf.WriteData(Timestamp, sizeof(Timestamp));
			}
			if (IsNull())
			{
				wBuf.WriteData(m_nullFlag, m_nullFlagByteCount);
			}
			else
			{
				Data.Serialize(wBuf);
			}
			
		}
		return 0;
	}
	DWORD CBSize()
	{
		if (pColumnInfo->IsFixedLenType())
		{
			return FixedLen;
		}
		else
		{
			DWORD dwSize = 0;
			
			if (bHaveTextPoint)
			{
				if (GNULL == 0x00)
				{
					dwSize += sizeof(BYTE);
					return dwSize;
				}
				dwSize += TextPointer.CBSize();
				dwSize += sizeof(Timestamp);
			}
			if (IsNull())
			{
				dwSize += m_nullFlagByteCount;
			}
			else
			{
				dwSize += Data.CBSize();
			}
			return dwSize;
		}
	}

	// by jie 2018.05.11
	bool SetData(PBYTE pBuf)
	{
		if (IsNull())
			return false;
		Data.SetData(pBuf);
		return true;
	}
	const tdsCountVarValue<LenType, ValueType>& getData() const { return Data; }
	bool IsNull()
	{
		return m_nullFlagByteCount != 0;
	}
	bool PeekCheckNull(ReadBuffer& rbuf)
	{
		TDS_NULL_TYPE sz = GetNullType();
		m_nullFlagByteCount = sz;
		rbuf.PeekData(m_nullFlag, m_nullFlagByteCount);
		bool ret = false;
		switch (sz)
		{
		case GEN_NULL:
			ret = *m_nullFlag == 0x00;
			break;
		case CHARBIN_NULL_2:
		case CHARBIN_NULL_4:
		case PLP_NULL:
			ret = CheckAllFF();
			break;
		default:
			ret = false;
			break;
		}
		if (!ret)
		{
			m_nullFlagByteCount = 0;
		}
		return ret;
	}
	bool CheckAllFF()
	{
		bool ret = true;
		for (size_t i = 0; i < m_nullFlagByteCount; ++i)
		{
			if (m_nullFlag[i] != 0xff)
			{
				ret = false;
				break;
			}
		}
		return ret;
	}
private:
	TDS_NULL_TYPE GetNullType()
	{
		switch (pColumnInfo->GetDataType())
		{
		case BIGCHARTYPE:
		case BIGVARCHRTYPE:
		case NCHARTYPE:
		case NVARCHARTYPE:
		case BIGBINARYTYPE:
		case BIGVARBINTYPE:
			return CHARBIN_NULL_2;
		case TEXTTYPE:
		case NTEXTTYPE:
		case IMAGETYPE:
			return CHARBIN_NULL_4;
		case XMLTYPE:
			return PLP_NULL;
		default:
			return GEN_NULL;
		}
	}
private:
	 tdsColumnDataInfo* pColumnInfo;

private:

	//varlen type
	BOOL bHaveTextPoint;
	tdsCountVarValue<BYTE,BYTE> TextPointer; //option
	BYTE  Timestamp[8]; //option
    tdsCountVarValue<LenType,ValueType> Data;

	BYTE m_nullFlag[8];
	int m_nullFlagByteCount;

	//fixedlen Type
	BYTE FixedLenData[8]; //the max len of the FixedLen type is 8
	BYTE FixedLen;

	BYTE GNULL;
};

#endif 

