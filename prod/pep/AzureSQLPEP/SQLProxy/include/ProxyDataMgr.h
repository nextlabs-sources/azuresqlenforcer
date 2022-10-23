#ifndef PROXY_DATA_MANAGER_H
#define PROXY_DATA_MANAGER_H

#include <windows.h>
#include <map>
#include "MemoryCache.h"
#include "ProxyTask.h"
#include "ProxyChannel.h"

class ProxyDataMgr
{
public:
    ProxyDataMgr(void);
	~ProxyDataMgr(void);

public:
	static ProxyDataMgr* GetInstance()
	{
		static ProxyDataMgr* theProxyDataMgr = NULL;
		if (NULL==theProxyDataMgr)
		{
			theProxyDataMgr = new ProxyDataMgr();
		}
		return theProxyDataMgr;
	}

public:
	void Init(HANDLE hTdsDataReadyEvent, HANDLE hSocketDataReadyEvent);
    uint32_t TdsGetCurrentPacketLength(uint8_t* pBuff, uint32_t len);
    void ReceiveDataEvent(boost::shared_ptr<TcpSocket> tcpSocket, BYTE* data, int length);
	void* GetTdsDataSocket();
	boost::shared_ptr<ProxyTask> GetTdsTask(void* tcpSocket, ProxyChannelPtr channel, uint8_t* data, uint32_t len);
	void CleanSocket(void* tcpSocket);

private:
	std::map<void*, MemoryCache*>  m_tdsDatas;
	CRITICAL_SECTION  m_csTdsData;

	std::list<void*> m_lstDataSockets;
	CRITICAL_SECTION m_csDataSocket;

	HANDLE m_hTdsDataReadyEvent;
	HANDLE m_hSocketDataReadyEvent;

	HANDLE m_hThreadDecryptData;
    HANDLE m_hThreadCacheSockData;
};

extern ProxyDataMgr* theProxyDataMgr;

#endif 

