// Algorithm.cpp: implementation of the Algorithm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Algorithm.h"
#include "TradeHandlingThread.h"
#include "DbConn.h"
#include <stdlib.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern DbAccessorPool dbAccessPool;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Algorithm::Algorithm(string configFile)
{
    m_configFile = configFile;
}

Algorithm::Algorithm()
{
}

Algorithm::~Algorithm()
{

}
/*
int	Algorithm::ExecAlgo(OrderInfo & res)
{
	int num = rand();
	if ( num > RAND_MAX/2 )
		res.amount=1;
	else
		res.amount=-1;

	
	if( m_algoType == TickData)
	{
		res.time = m_marketData.UpdateTime;
		res.day= m_marketData.TradingDay;
		res.price = m_marketData.LastPrice;
		res.milliSec = m_marketData.UpdateMillisec;
		res.m_instrumentID = m_marketData.InstrumentID;
	}
	else if( m_algoType == MinuteData )
	{
		res.day= m_oneMinuteData.m_Day;
		res.time = m_oneMinuteData.m_Time;
		res.price = m_oneMinuteData.m_ClosePrice;
		res.milliSec =0;
		res.m_instrumentID = m_oneMinuteData.m_InstrumentID;
	}
	return 0;
}
*/
int	Algorithm::SendStrategy(const OrderInfo & res)
{
	OrderInfo* data=new OrderInfo(res);
	TradeHandlingThread->PostThreadMessage(WM_ACTION_ITEM, (WPARAM)data,  1);
	return 0;
}

int	Algorithm::SendStrategy(const OrderInfoShort & res)
{
	OrderInfoShort* data=new OrderInfoShort(res);
	TradeHandlingThread->PostThreadMessage(WM_ACTION_ITEM, (WPARAM)data,  2);
	return 0;
}

int Algorithm::Run()
{
	//InitializeCriticalSection(&crit_sec);  
    //    Here we will wait for the messages
    while ( true )
    {
        MSG    msg;
        //BOOL    MsgReturn  =  PeekMessage ( &msg , NULL , 
        //    THRD_MESSAGE_SOMEWORK , THRD_MESSAGE_EXIT , PM_REMOVE );
        BOOL    MsgReturn    =    GetMessage ( &msg , NULL , 
            WM_MARKET_DATA , WM_ACCOUNT_INFO );
            
        if ( MsgReturn )
        {
            switch ( msg.message )
            {
            case WM_MARKET_DATA:
				HandleNewMarketData(msg);
                break;
            case WM_TRADE_INFO:
                UpdateTradeInfo(msg);
                break;                
            case WM_POSITION_INFO:
                UpdatePositionInfo(msg);
                break;
            case WM_ACCOUNT_INFO:
                UpdateAccountInfo(msg);
                break;
            }
        }
    }    
    return 0;
}

BOOL Algorithm::InitInstance()
{
	return TRUE;
}

void Algorithm::HandleNewMarketData(MSG& msg)
{
	int data_type = msg.lParam;

	if (data_type ==1)//depth market data; tick data
	{
		CThostFtdcDepthMarketDataField* data= (CThostFtdcDepthMarketDataField*)msg.wParam;
        OnTickData(*data);
		delete data;
	}
	else if(data_type==2)//one minute K series
	{
		CMinuteData* data = (CMinuteData*)msg.wParam;
		OnMinuteData(*data);
        delete data;
	}
	else if(data_type==3)//half minute K series
	{
		CHalfMinuteData* data = (CHalfMinuteData*)msg.wParam;
		OnHalfMinuteData(*data);
        delete data;
	}
}

void Algorithm::UpdateTradeInfo(MSG& msg)
{
    CThostFtdcTradeField* data= (CThostFtdcTradeField*)msg.wParam;
    OnTradeData(*data);
    delete data;
}

void Algorithm::UpdatePositionInfo(MSG& msg)
{
    CThostFtdcInvestorPositionField* data= (CThostFtdcInvestorPositionField*)msg.wParam;
    OnPositionData(*data);
    delete data;
}

void Algorithm::UpdateAccountInfo(MSG& msg)
{
    CThostFtdcTradingAccountField* data= (CThostFtdcTradingAccountField*)msg.wParam;
    OnAccountData(*data);
    delete data;
}

void Algorithm::RegisterInstrument(string instrument)
{
	this->m_Instruments.insert(instrument);
}

bool Algorithm::IsInterestingInstrument(string instrument)
{
	if( m_Instruments.find( instrument ) != m_Instruments.end() )
		return true;
	else
		return false;
}