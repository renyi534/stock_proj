// MessageRouter.cpp: implementation of the MessageRouter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TradeSystem.h"
#include "MessageRouter.h"
#include "RandomAlgorithm.h"
#include "TradeHandlingThread.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern TThostFtdcInstrumentIDType INSTRUMENT_ID;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MessageRouter MessageRouter::Router;

MessageRouter::MessageRouter()
{

}

MessageRouter::~MessageRouter()
{
	int size = m_algorithms.size();

	for( int i =0; i<size; i++)
	{
		delete m_algorithms[i];
	}
}

void MessageRouter::InitAlgorithm()
{
	Algorithm * algo ;
	algo= new RandomAlgorithm(INSTRUMENT_ID);
	algo->CreateThread(CREATE_SUSPENDED);
	algo->ResumeThread();
	m_algorithms.push_back(algo);
}

void MessageRouter::sendData(const CMinuteData& data)
{
	int size = m_algorithms.size();

	for( int i =0; i<size; i++)
	{
		CMinuteData* dataToSend=new CMinuteData(data);
		m_algorithms[i]->PostThreadMessage(WM_MARKET_DATA, (WPARAM)dataToSend,  2);
	}
}

void MessageRouter::sendData(const CHalfMinuteData& data)
{
	int size = m_algorithms.size();

	for( int i =0; i<size; i++)
	{
		CHalfMinuteData* dataToSend=new CHalfMinuteData(data);
		m_algorithms[i]->PostThreadMessage(WM_MARKET_DATA, (WPARAM)dataToSend,  3);
	}
}

void MessageRouter::sendData(const CThostFtdcDepthMarketDataField& data)
{
	int size = m_algorithms.size();

	for( int i =0; i<size; i++)
	{
		CThostFtdcDepthMarketDataField* dataToSend=new CThostFtdcDepthMarketDataField(data);
		m_algorithms[i]->PostThreadMessage(WM_MARKET_DATA, (WPARAM)dataToSend,  1);
	}
}

void MessageRouter::sendData(const CThostFtdcTradeField& data)
{
	int size = m_algorithms.size();

	for( int i =0; i<size; i++)
	{
		CThostFtdcTradeField* dataToSend=new CThostFtdcTradeField(data);
		m_algorithms[i]->PostThreadMessage(WM_TRADE_INFO, (WPARAM)dataToSend,  NULL);
	}
}

void MessageRouter::sendData(const CThostFtdcTradingAccountField& data)
{
	int size = m_algorithms.size();

	for( int i =0; i<size; i++)
	{
		CThostFtdcTradingAccountField* dataToSend=new CThostFtdcTradingAccountField(data);
		m_algorithms[i]->PostThreadMessage(WM_ACCOUNT_INFO, (WPARAM)dataToSend,  NULL);
	}
}

void MessageRouter::sendData(const CThostFtdcInvestorPositionField& data)
{
	int size = m_algorithms.size();

	for( int i =0; i<size; i++)
	{
		CThostFtdcInvestorPositionField* dataToSend=new CThostFtdcInvestorPositionField(data);
		m_algorithms[i]->PostThreadMessage(WM_POSITION_INFO, (WPARAM)dataToSend,  NULL);
	}
}