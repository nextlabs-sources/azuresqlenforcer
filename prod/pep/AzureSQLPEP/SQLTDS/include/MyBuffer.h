#pragma once

#ifndef __MY_BUFFER_H__
#define __MY_BUFFER_H__

#include <vector>
using namespace std;
//#define BYTE unsigned char
//#define PBYTE BYTE*
typedef unsigned char BYTE;
typedef BYTE* PBYTE;

class WriteBuffer
{
public:
	WriteBuffer() :m_curBufIndex(0), m_curBufPos(0) {}
	~WriteBuffer() {}
	void attach(PBYTE begin, size_t length)
	{
		m_vecBegins.push_back(begin);
		m_vecLengths.push_back(length);
	}
	size_t WriteData(void* buf, size_t n)
	{
		size_t ret = n;
		while (Avail() && n > 0)
		{
			PBYTE cur = m_vecBegins[m_curBufIndex] + m_curBufPos;
			size_t canCpy = next(n);
			memcpy(cur, buf, canCpy);
			buf = (BYTE*)buf + canCpy;
			n = n - canCpy;
		}
		return ret - n;
	}
	template<class T>
	bool WriteAny(T t)
	{
		return WriteData(&t, sizeof(T)) == sizeof(T);
	}
	size_t GetCapacity() const
	{
		size_t ret = 0;
		for (auto it = m_vecLengths.begin(); it != m_vecLengths.end(); ++it)
			ret += *it;
		return ret;
	}
	bool Avail() const
	{
		if (m_curBufIndex == m_vecBegins.size() - 1 &&
			m_curBufPos == m_vecLengths.back())
		{
			return false;
		}
		return true;
	}
	
private:
	size_t next(size_t n)
	{
		size_t curEnd = m_vecLengths[m_curBufIndex];
		if (n + m_curBufPos >= curEnd)
		{
			int ret = curEnd - m_curBufPos;
			if (m_curBufIndex == m_vecBegins.size() - 1)
			{
				m_curBufPos = m_vecLengths.back();  // at end
			}
			else
			{
				m_curBufIndex++;
				m_curBufPos = 0;				
			}
			return ret;
		}
		else
		{
			m_curBufPos += n;
			return n;
		}
	}
private:
	std::vector<PBYTE> m_vecBegins;
	std::vector<size_t> m_vecLengths;
	size_t m_curBufIndex;
	size_t m_curBufPos;
};

class ReadBuffer
{
public:
	ReadBuffer() :m_curBufIndex(0), m_curBufPos(0) {}
	~ReadBuffer() {}
	void attach(PBYTE begin, size_t length)
	{
		m_vecBegins.push_back(begin);
		m_vecLengths.push_back(length);
	}
	size_t ReadData(void* buf, size_t n)
	{
		size_t ret = n;
		while (Avail() && n > 0)
		{
			PBYTE cur = m_vecBegins[m_curBufIndex] + m_curBufPos;
			size_t canCpy = next(n);
			memcpy(buf, cur, canCpy);
			buf = (BYTE*)buf + canCpy;
			n = n - canCpy;
		}
		return ret - n;
	}
	size_t PeekData(void* buf, size_t n, int from = -1)
	{
		size_t m_curBufIndex_ = m_curBufIndex;
		size_t m_curBufPos_ = m_curBufPos;
		size_t ret = 0;
		if (from >= 0)
		{
			if (SetPos(from))
				ret = ReadData(buf, n);
		}
		else
		{
			ret = ReadData(buf, n);
		}
		m_curBufIndex = m_curBufIndex_;
		m_curBufPos = m_curBufPos_;
		return ret;
	}
	template <class T>
	size_t ReadAny(T& t)
	{
		return ReadData(&t, sizeof(T));
	}
	template <class T>
	size_t PeekAny(T& t, int from = -1)
	{
		return PeekData(&t, sizeof(T), from);
	}
	size_t GetCapacity() const
	{
		size_t ret = 0;
		for (auto it = m_vecLengths.begin(); it != m_vecLengths.end(); ++it)
			ret += *it;
		return ret;
	}
	bool Avail() const
	{
		if (m_curBufIndex == m_vecBegins.size() - 1 &&
			m_curBufPos == m_vecLengths.back())
		{
			return false;
		}
		return true;
	}

private:
	size_t next(size_t n)
	{
		size_t curEnd = m_vecLengths[m_curBufIndex];
		if (n + m_curBufPos >= curEnd)
		{
			int ret = curEnd - m_curBufPos;
			if (m_curBufIndex == m_vecBegins.size() - 1)
			{
				m_curBufPos = m_vecLengths.back();  // at end
			}
			else
			{
				m_curBufIndex++;
				m_curBufPos = 0;
			}
			return ret;
		}
		else
		{
			m_curBufPos += n;
			return n;
		}
	}
	bool SetPos(size_t pos)
	{
		if (pos >= GetCapacity())
			return false;
		size_t idx = 0;
		for (auto it = m_vecLengths.begin(); it != m_vecLengths.end(); ++it, ++idx)
		{
			if (pos > *it)
			{
				pos -= *it;
			}
			else
			{
				break;
			}
		}
		m_curBufIndex = idx;
		m_curBufPos = pos;
		return true;
	}
private:
	std::vector<PBYTE> m_vecBegins;
	std::vector<size_t> m_vecLengths;
	size_t m_curBufIndex;
	size_t m_curBufPos;
};

#endif