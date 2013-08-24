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

Algorithm::Algorithm(string instrument)
{
    RegisterInstrument(instrument);

}

Algorithm::Algorithm(string instrument, string config_file)
{
	RegisterInstrument(instrument);
    m_configFile = config_file;

}

Algorithm::Algorithm()
{

}

Algorithm::~Algorithm()
{

}

int	Algorithm::SendStrategy(OrderInfo & res)
{
	if( res.amount == 0 )
		return 0;
	OrderInfo* data=new OrderInfo(res);
	data->broker_id = m_BrokerId;
	data->investor_id = m_InvestorId;
	TradeHandlingThread->PostThreadMessage(WM_ACTION_ITEM, (WPARAM)data,  1);
	return 0;
}

int	Algorithm::SendStrategy(OrderInfoShort & res)
{
	if( res.amount == 0 )
		return 0;

	OrderInfoShort* data=new OrderInfoShort(res);
	data->broker_id = m_BrokerId;
	data->investor_id = m_InvestorId;
	TradeHandlingThread->PostThreadMessage(WM_ACTION_ITEM, (WPARAM)data,  2);
	return 0;
}

void Algorithm::SetSlot(int slot)
{
	m_Slot = slot;
}

void Algorithm::SetAccountInfo(string broker, string investor)
{
	m_BrokerId = broker;
	m_InvestorId = investor;
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
	else if(data_type==4)//ten minute K series
	{
		CTenMinuteData* data = (CTenMinuteData*)msg.wParam;
		OnTenMinuteData(*data);
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