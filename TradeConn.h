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
#include <vector>
#include "MessageRouter.h"

struct TradeConn
{
	CThostFtdcTraderApi*	m_TradeApi;
	CTraderSpi*				m_TradeSpi;
	//CCriticalSection		m_critSec;
	CThostFtdcMdApi*		m_UserApi;
	CMdSpi*		m_UserSpi;

	string m_TradeServerAddr;
	string m_MdServerAddr;
	string m_BrokerId;
	string m_InvestorId;
	string m_Passwd;

	MessageRouter m_Router;
	TradeConn(string FRONT_ADDR_MD, string FRONT_ADDR_TRADE,TE_RESUME_TYPE nResumeType,
		string broker_id, string investor_id, string passwd):
		m_TradeServerAddr(FRONT_ADDR_TRADE), m_MdServerAddr(FRONT_ADDR_MD),
		m_BrokerId(broker_id), m_InvestorId(investor_id), m_Passwd(passwd),
		m_Router(broker_id, investor_id)
	{
		string userDir =string(".\\UserApi")+m_BrokerId+m_InvestorId;
		int ret=_mkdir(userDir.c_str());
		ASSERT( errno  == EEXIST || ret == 0);
		m_TradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi(userDir.c_str());			// ����UserApi
		m_TradeSpi = new CTraderSpi(m_TradeApi, m_BrokerId, m_InvestorId, m_Passwd, this);
		m_TradeApi->RegisterSpi((CThostFtdcTraderSpi*)m_TradeSpi);			// ע���¼���
		m_TradeApi->SubscribePublicTopic(nResumeType);					// ע�ṫ����
		m_TradeApi->SubscribePrivateTopic(nResumeType);					// ע��˽����
		m_TradeApi->RegisterFront((char*)FRONT_ADDR_TRADE.c_str());			// connect
		m_TradeApi->Init();

		string mdDir =string(".\\MdApi")+m_BrokerId+m_InvestorId;
		ret = _mkdir(mdDir.c_str());
		ASSERT( errno  == EEXIST || ret == 0);
		m_UserApi = CThostFtdcMdApi::CreateFtdcMdApi(mdDir.c_str());			// ����UserApi
		m_UserSpi = new CMdSpi(m_UserApi, m_BrokerId, m_InvestorId, m_Passwd, this);
		m_UserApi->RegisterSpi(m_UserSpi);						// ע���¼���
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