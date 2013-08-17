// CompositeAlgorithm.cpp: implementation of the CompositeAlgorithm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tradesystem.h"
#include "CompositeAlgorithm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CompositeAlgorithm::CompositeAlgorithm(string name, string instrument, double max_loss):
	Algorithm(instrument),
	m_Instrument(instrument), m_Name(name)
{
	m_Position = 0;
	m_CurrProfit = 0;
	m_MaxLoss = max_loss;
	m_ClosePrice = 0;
	m_AlreadyForceClear = false;
}

CompositeAlgorithm::~CompositeAlgorithm()
{
	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		delete *iter;
	}
}

BOOL CompositeAlgorithm::InitInstance()
{
	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		BOOL result = (*iter)->InitInstance();
		
		if(result != TRUE)
			return result;
	}
	return TRUE;
}

bool CompositeAlgorithm::AddAlgorithm(Algorithm* algo)
{
	if (algo == NULL)
		return false;

	m_AlgoList.push_back(algo);
	return true;
}

int CompositeAlgorithm::OnMinuteData(const CMinuteData& data)
{
	OrderInfoShort res;
	
	res.day= data.m_Day;
	res.time = data.m_Time;
	res.milliSec =0;
	res.m_instrumentID = data.m_InstrumentID;
	res.amount = 0;
	res.price = -1;

	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		res.amount += (*iter)->OnMinuteData(data); 
	}
	SendStrategy(res);
	return res.amount;
}

int CompositeAlgorithm::OnHalfMinuteData(const CHalfMinuteData& data)
{
	OrderInfoShort res;
	
	res.day= data.m_Day;
	res.time = data.m_Time;
	res.milliSec =0;
	res.m_instrumentID = data.m_InstrumentID;
	res.amount = 0;
	res.price = -1;

	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		res.amount += (*iter)->OnHalfMinuteData(data); 
	}
	SendStrategy(res);
	return res.amount;
}

int CompositeAlgorithm::OnTenMinuteData(const CTenMinuteData& data)
{
	OrderInfoShort res;
	
	res.day= data.m_Day;
	res.time = data.m_Time;
	res.milliSec =0;
	res.m_instrumentID = data.m_InstrumentID;
	res.amount = 0;
	res.price = -1;

	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		res.amount += (*iter)->OnTenMinuteData(data); 
	}

	SendStrategy(res);
	return res.amount;
}

int	CompositeAlgorithm::SendStrategy(OrderInfoShort & res)
{
	if (m_AlreadyForceClear)
		return 0;

	if(res.amount>0)
	{
		res.price=m_AskPrice;
	}
	else if(res.amount<0)
	{
		res.price=m_BidPrice;
	}

	Algorithm::SendStrategy(res);
	m_Position += res.amount;
	return 0;
}

int CompositeAlgorithm::OnTickData(const CThostFtdcDepthMarketDataField& data)
{
	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		(*iter)->OnTickData(data); 
	}
	m_AskPrice = data.AskPrice1;
	m_BidPrice = data.BidPrice1;
	
	if (m_ClosePrice >0)
		m_PrevPrice = m_ClosePrice;
	else
		m_PrevPrice = data.ClosePrice;

	m_ClosePrice = data.ClosePrice;


	m_CurrProfit += m_Position * (m_ClosePrice - m_PrevPrice);

	if (m_MaxLoss<0 && m_CurrProfit<m_MaxLoss)
	{
		OrderInfoShort res;
		
		res.day= data.TradingDay;
		res.time = data.UpdateTime;
		res.milliSec = data.UpdateMillisec;
		res.m_instrumentID = data.InstrumentID;
		res.amount = -m_Position;
		res.price = -1;
		SendStrategy(res);
		m_AlreadyForceClear = true;
		return res.amount;
	}
	return 0;
}

void CompositeAlgorithm::OnTradeData(const CThostFtdcTradeField& data)
{
	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		(*iter)->OnTradeData(data); 
	}

	return;
}

void CompositeAlgorithm::OnAccountData(const CThostFtdcTradingAccountField& data)
{
	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		(*iter)->OnAccountData(data); 
	}

	return;
}

void CompositeAlgorithm::OnPositionData(const CThostFtdcInvestorPositionField& data)
{
	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		(*iter)->OnPositionData(data); 
	}

	return;
}

void CompositeAlgorithm::SetSlot(int slot)
{
	Algorithm::SetSlot(slot);

	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		(*iter)->SetSlot(slot); 
	}

	return;
}

void CompositeAlgorithm::SetAccountInfo(string broker, string investor)
{
	Algorithm::SetAccountInfo(broker, investor);
	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		(*iter)->SetAccountInfo(broker, investor); 
	}

	return;
}