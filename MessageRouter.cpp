// MessageRouter.cpp: implementation of the MessageRouter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TradeSystem.h"
#include "MessageRouter.h"
#include "RandomAlgorithm.h"
#include "HsAlgorithm.h"
#include "DtAlgorithm.h"
#include "WeightedAlgorithm.h"
#include "TradeHandlingThread.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern char * ppInstrumentID[30];
extern set<string> activeAlgorithm;
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
	Algorithm * algo=NULL;
	
	vector<AlgoInstrumentPair>::iterator iter;
	for(iter = m_algoInstrument.begin(); iter != m_algoInstrument.end(); iter++)
	{
		algo = createAlgorithm(*iter);
		if (algo != NULL)
		{
			algo->CreateThread(CREATE_SUSPENDED);
			algo->ResumeThread();
			m_algorithms.push_back(algo);
		}
	}
}

void MessageRouter::sendData(const CMinuteData& data)
{
	int size = m_algorithms.size();
	
	for( int i =0; i<size; i++)
	{
		if( m_algorithms[i]->IsInterestingInstrument(data.m_InstrumentID) )
		{
			CMinuteData* dataToSend=new CMinuteData(data);
			m_algorithms[i]->PostThreadMessage(WM_MARKET_DATA, (WPARAM)dataToSend,  2);
		}
	}
}

void MessageRouter::sendData(const CTenMinuteData& data)
{
	int size = m_algorithms.size();
	
	for( int i =0; i<size; i++)
	{
		if( m_algorithms[i]->IsInterestingInstrument(data.m_InstrumentID) )
		{
			CTenMinuteData* dataToSend=new CTenMinuteData(data);
			m_algorithms[i]->PostThreadMessage(WM_MARKET_DATA, (WPARAM)dataToSend,  4);
		}
	}
}

void MessageRouter::sendData(const CHalfMinuteData& data)
{
	int size = m_algorithms.size();
	
	for( int i =0; i<size; i++)
	{
		if( m_algorithms[i]->IsInterestingInstrument(data.m_InstrumentID) )
		{
			CHalfMinuteData* dataToSend=new CHalfMinuteData(data);
			
			m_algorithms[i]->PostThreadMessage(WM_MARKET_DATA, (WPARAM)dataToSend,  3);
		}
	}
}

void MessageRouter::sendData(const CThostFtdcDepthMarketDataField& data)
{
	int size = m_algorithms.size();
	
	for( int i =0; i<size; i++)
	{
		if( m_algorithms[i]->IsInterestingInstrument(data.InstrumentID) )
		{
			CThostFtdcDepthMarketDataField* dataToSend=new CThostFtdcDepthMarketDataField(data);
			m_algorithms[i]->PostThreadMessage(WM_MARKET_DATA, (WPARAM)dataToSend,  1);
		}
	}
}

void MessageRouter::sendData(const CThostFtdcTradeField& data)
{
	int size = m_algorithms.size();
	
	for( int i =0; i<size; i++)
	{
		if( m_algorithms[i]->IsInterestingInstrument(data.InstrumentID) )
		{
			CThostFtdcTradeField* dataToSend=new CThostFtdcTradeField(data);
			m_algorithms[i]->PostThreadMessage(WM_TRADE_INFO, (WPARAM)dataToSend,  NULL);
		}
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
		if( m_algorithms[i]->IsInterestingInstrument(data.InstrumentID) )
		{
			CThostFtdcInvestorPositionField* dataToSend=new CThostFtdcInvestorPositionField(data);
			m_algorithms[i]->PostThreadMessage(WM_POSITION_INFO, (WPARAM)dataToSend,  NULL);
		}
	}
}

void MessageRouter::AddAlgorithm(string algo_name, string instrument)
{
	AlgoInstrumentPair pair;
	pair.AlgoName=algo_name;
	pair.Instrument = instrument;
	m_algoInstrument.push_back(pair);
}

Algorithm* MessageRouter::createAlgorithm(AlgoInstrumentPair algoInstrument)
{
	Algorithm* algo = NULL;
	if (activeAlgorithm.find(algoInstrument.AlgoName) == activeAlgorithm.end() )
		return NULL;
	
	if (algoInstrument.AlgoName == "RandomAlgorithm")
	{
		algo= new RandomAlgorithm(algoInstrument.Instrument);
	}
	else if (algoInstrument.AlgoName == "HsAlgorithm")
	{
		algo= new HsAlgorithm(algoInstrument.Instrument);
	}
	else if (algoInstrument.AlgoName == "WeightedAlgorithm" )
	{
		algo= new WeightedAlgorithm(algoInstrument.Instrument);
	}	
	else if (algoInstrument.AlgoName == "DtAlgorithm" )
	{
		algo= new DtAlgorithm(algoInstrument.Instrument);
	}
	
	return algo;
}
