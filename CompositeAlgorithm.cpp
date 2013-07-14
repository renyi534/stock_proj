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

CompositeAlgorithm::CompositeAlgorithm(string name, string instrument):
	m_Instrument(instrument), m_Name(name)
{
	RegisterInstrument(instrument);
}

CompositeAlgorithm::~CompositeAlgorithm()
{

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

int CompositeAlgorithm::OnTickData(const CThostFtdcDepthMarketDataField& data)
{
	vector<Algorithm*>::iterator iter;
	for(iter = m_AlgoList.begin(); iter != m_AlgoList.end(); iter++)
	{
		(*iter)->OnTickData(data); 
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