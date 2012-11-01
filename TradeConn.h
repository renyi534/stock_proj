#if !defined(TRADE_CONN_43241FQDS)
#define TRADE_CONN_43241FQDS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"
#include <direct.h>
struct TradeConn
{
	CThostFtdcTraderApi*	m_TradeApi;
	CTraderSpi*				m_TradeSpi;
	//CCriticalSection		m_critSec;
	CThostFtdcMdApi*		m_UserApi;
	CMdSpi*		m_UserSpi;

	TradeConn(string FRONT_ADDR_MD, string FRONT_ADDR_TRADE,TE_RESUME_TYPE nResumeType)
	{
		int ret=_mkdir(".\\UserApi");
		ASSERT( errno  == EEXIST || ret == 0);
		m_TradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi(".\\UserApi");			// 创建UserApi
		m_TradeSpi = new CTraderSpi(m_TradeApi);
		m_TradeApi->RegisterSpi((CThostFtdcTraderSpi*)m_TradeSpi);			// 注册事件类
		m_TradeApi->SubscribePublicTopic(nResumeType);					// 注册公有流
		m_TradeApi->SubscribePrivateTopic(nResumeType);					// 注册私有流
		m_TradeApi->RegisterFront((char*)FRONT_ADDR_TRADE.c_str());			// connect
		m_TradeApi->Init();

		ret = _mkdir(".\\MdApi");
		ASSERT( errno  == EEXIST || ret == 0);
		m_UserApi = CThostFtdcMdApi::CreateFtdcMdApi(".\\MdApi");			// 创建UserApi
		m_UserSpi = new CMdSpi(m_UserApi);
		m_UserApi->RegisterSpi(m_UserSpi);						// 注册事件类
		m_UserApi->RegisterFront((char*)FRONT_ADDR_MD.c_str());					// connect
		m_UserApi->Init();
	}

	virtual ~TradeConn()
	{
		m_TradeApi->RegisterSpi(NULL);
		m_UserApi->RegisterSpi(NULL);
		m_UserApi->Release();
		m_TradeApi->Release();
		delete m_TradeSpi;
		delete m_UserSpi;
	}
};
#endif // TRADE_CONN_43241FQDS