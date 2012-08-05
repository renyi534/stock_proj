#include "stdafx.h"
#include "MdSpi.h"
#include <iostream>
#include <stdio.h>
#include "DbAccessorPool.h"
#include "DbAccessor.h"
#include "DbConn.h"
#include "MessageRouter.h"
#include "utility.h"
using namespace std;
#define __FUNCTION__  "function name"
#pragma warning(disable : 4996)
#include "TradeHandlingThread.h"
#include "resource.h"
#include "TradeSystemDoc.h"
#include "TradeSystemView.h"
// USER_API参数
//extern CThostFtdcMdApi* m_pUserApi;

// 配置参数
extern char FRONT_ADDR[];		
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char *ppInstrumentID[];	
extern int iInstrumentID;

// 请求编号

extern DbAccessorPool dbAccessPool;
CMdSpi::~CMdSpi()
{
	m_log.close();

	::DeleteCriticalSection(&m_data_critsec);
	//::DeleteCriticalSection(&m_minute_critsec);
}


CMdSpi::CMdSpi(CThostFtdcMdApi* api):m_pUserApi(api),m_requestID(0),m_log("c:\\marketdata.log",ios::app)
{
	::InitializeCriticalSection(&m_data_critsec);
	// stl library can be buggy with empty maps. Insert some rubbish data here.
	m_tick_data_map.insert(CTickDataPair("", CThostFtdcDepthMarketDataField() ));
	m_one_minute_data_map.insert(CMinuteDataPair("", CMinuteData() ));
	m_prev_one_minute_data_map.insert(CMinuteDataPair("", CMinuteData() ));
	m_half_minute_data_map.insert(CHalfMinuteDataPair("", CHalfMinuteData() ));
	m_prev_half_minute_data_map.insert(CHalfMinuteDataPair("", CHalfMinuteData() ));
}

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	m_log << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	m_log << "--->>> " << __FUNCTION__ << endl;
	m_log << "--->>> Reason = " << nReason << endl;
	CString str;
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL)
	{
	str.Format("Data Server: %s", "OFF");
	view->m_MDStatus.SetWindowText(str);
	}
}
		
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	m_log << "--->>> " << __FUNCTION__ << endl;
	m_log << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
	m_log << "--->>> " << __FUNCTION__ << endl;
	///用户登录请求
	ReqUserLogin();
	CString str;
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL)
	{
	str.Format("Data Server: %s", "ON");
	view->m_MDStatus.SetWindowText(str);
	}
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = m_pUserApi->ReqUserLogin(&req, ++m_requestID);
	m_log << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << __FUNCTION__ << endl;

	if ( pRspUserLogin== NULL || pRspInfo==NULL)
		return;

	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///获取当前交易日
		m_log << "--->>> 获取当前交易日 = " << m_pUserApi->GetTradingDay() << endl;
		// 请求订阅行情
		SubscribeMarketData();	
		/*m_db.OpenEx("DSN=PostgreSQL35W;UID=postgres;PWD=810102");
		if( ! m_db.IsOpen() )
		{
			m_log<<"Database connection error"<<endl;
		}
		else
		{
			/*
			m_db.ExecuteSQL("truncate t");
			m_db.ExecuteSQL("insert into t values(222)");
			m_log<<"Database connection OK"<<endl;
		}*/
	}
}

void CMdSpi::SubscribeMarketData()
{
	int iResult = m_pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	m_log << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << __FUNCTION__ << endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << __FUNCTION__ << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	CLock lock(&m_data_critsec);

	{
	DWORD thread_id = GetCurrentThreadId();
	if (pDepthMarketData == NULL)
		return;

	m_log<<" OnRtnDepthMarketData  "<<"Time:"<<pDepthMarketData->TradingDay<<" "
		<<pDepthMarketData->UpdateTime<<" "<<pDepthMarketData->UpdateMillisec<<" "<<pDepthMarketData->InstrumentID<<" "
		<<"  Price:"<<pDepthMarketData->LastPrice<<" threadid: "<<thread_id<<endl;
    
	MessageRouter::Router.sendData(* pDepthMarketData);
	
	string time = pDepthMarketData->UpdateTime;

	if ( time > "09:14:59" && time < "23:17" )
	{
		genOneMinuteData(*pDepthMarketData);
		genHalfMinuteData(*pDepthMarketData);
	}
	//CTradeSystemApp* app = (CTradeSystemApp*) AfxGetApp();

	CTickDataMap::iterator iter = m_tick_data_map.find(pDepthMarketData->InstrumentID);
	if (iter == m_tick_data_map.end())
	{
		//add map data for this instrument
		m_tick_data_map.insert(CTickDataPair(pDepthMarketData->InstrumentID, *pDepthMarketData));
	}
	else
	{
		m_tick_data_map[pDepthMarketData->InstrumentID] = *pDepthMarketData;
	}

	CString str;
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL && strcmp("沪深300",pDepthMarketData->InstrumentID) != 0)
	{
		view->m_AskPrice = pDepthMarketData->AskPrice1;
		view->m_BidPrice = pDepthMarketData->BidPrice1;
	}
	char* buffer = new char[8196];
	int index=0;
	/*const char* format_str="insert into stock_data.depthmarketdata values('%s','%s','%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,'%s',"
		"%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf)";*/
	const char* format_str="insert into stock_data.\"DepthMarketData\" values('%s','%s','%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,'%s',";


	index= sprintf(buffer,format_str,
		pDepthMarketData->TradingDay,
		pDepthMarketData->InstrumentID,
		pDepthMarketData->ExchangeID,
		pDepthMarketData->ExchangeInstID,
		pDepthMarketData->LastPrice,
		pDepthMarketData->PreSettlementPrice,
		pDepthMarketData->PreClosePrice,
		pDepthMarketData->PreOpenInterest,
		pDepthMarketData->OpenPrice,
		pDepthMarketData->HighestPrice,
		pDepthMarketData->LowestPrice,
		pDepthMarketData->Volume,
		pDepthMarketData->Turnover,
		pDepthMarketData->OpenInterest,
		pDepthMarketData->ClosePrice,
		pDepthMarketData->SettlementPrice,
		pDepthMarketData->UpperLimitPrice,
		pDepthMarketData->LowerLimitPrice,
		pDepthMarketData->PreDelta,
		pDepthMarketData->CurrDelta,
		pDepthMarketData->UpdateTime
	);

	format_str= "%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf,%d,%lf)";
	index+= sprintf(buffer+index,format_str,
		pDepthMarketData->UpdateMillisec,
		pDepthMarketData->BidPrice1,
		pDepthMarketData->BidVolume1,
		pDepthMarketData->AskPrice1,
		pDepthMarketData->AskVolume1,
		pDepthMarketData->BidPrice2,
		pDepthMarketData->BidVolume2,
		pDepthMarketData->AskPrice2,
		pDepthMarketData->AskVolume2,
		pDepthMarketData->BidPrice3,
		pDepthMarketData->BidVolume3,
		pDepthMarketData->AskPrice3,
		pDepthMarketData->AskVolume3,
		pDepthMarketData->BidPrice4,
		pDepthMarketData->BidVolume4,
		pDepthMarketData->AskPrice4,
		pDepthMarketData->AskVolume4,
		pDepthMarketData->BidPrice5,
		pDepthMarketData->BidVolume5,
		pDepthMarketData->AskPrice5,
		pDepthMarketData->AskVolume5,
		pDepthMarketData->AveragePrice
	);
/*
	sprintf(buffer, format_str, 
		pDepthMarketData->TradingDay,
		pDepthMarketData->InstrumentID,
		pDepthMarketData->ExchangeID,
		pDepthMarketData->ExchangeInstID,
		pDepthMarketData->LastPrice,
		pDepthMarketData->PreSettlementPrice,
		pDepthMarketData->PreClosePrice,
		pDepthMarketData->PreOpenInterest,
		pDepthMarketData->OpenPrice,
		pDepthMarketData->HighestPrice,
		pDepthMarketData->LowestPrice,
		pDepthMarketData->Volume,
		pDepthMarketData->Turnover,
		pDepthMarketData->OpenInterest,
		pDepthMarketData->ClosePrice,
		pDepthMarketData->SettlementPrice,
		pDepthMarketData->UpperLimitPrice,
		pDepthMarketData->LowerLimitPrice,
		pDepthMarketData->PreDelta,
		pDepthMarketData->CurrDelta,
		pDepthMarketData->UpdateTime,
		pDepthMarketData->UpdateMillisec,
		pDepthMarketData->BidPrice1,
		pDepthMarketData->BidVolume1,
		pDepthMarketData->AskPrice1,
		pDepthMarketData->AskVolume1,
		pDepthMarketData->BidPrice2,
		pDepthMarketData->BidVolume2,
		pDepthMarketData->AskPrice2,
		pDepthMarketData->AskVolume2,
		pDepthMarketData->BidPrice3,
		pDepthMarketData->BidVolume3,
		pDepthMarketData->AskPrice3,
		pDepthMarketData->AskVolume3,
		pDepthMarketData->BidPrice4,
		pDepthMarketData->BidVolume4,
		pDepthMarketData->AskPrice4,
		pDepthMarketData->AskVolume4,
		pDepthMarketData->BidPrice5,
		pDepthMarketData->BidVolume5,
		pDepthMarketData->AskPrice5,
		pDepthMarketData->AskVolume5,
		pDepthMarketData->AveragePrice
	);*/
	//m_log<<buffer<<endl;

	gThreadPool.Run(ExecSQL, (void*) buffer);
	}
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		m_log << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

void CMdSpi::genOneMinuteData(const CThostFtdcDepthMarketDataField & tick_data)
{
	string tradingDay=tick_data.TradingDay;
	string updateTime=tick_data.UpdateTime;
	updateTime=updateTime.substr(0,5);
	string instrument_id = tick_data.InstrumentID;
	
	CMinuteData m_one_minute_data;

	CMinuteDataMap::iterator iter = m_one_minute_data_map.find(instrument_id);
	if (iter == m_one_minute_data_map.end())
	{
		//add map data for this instrument
		this->resetOneMinuteData(m_one_minute_data, instrument_id);
		m_one_minute_data_map.insert(CMinuteDataPair(instrument_id, m_one_minute_data));
	}
	else
	{
		m_one_minute_data = m_one_minute_data_map[instrument_id];
	}

	if(tradingDay==m_one_minute_data.m_Day &&
		updateTime==m_one_minute_data.m_Time)
	{
		m_one_minute_data.m_ClosePrice= tick_data.LastPrice;

		if(m_one_minute_data.m_HighPrice<tick_data.LastPrice)
		{
			m_one_minute_data.m_HighPrice=tick_data.LastPrice;
		}

		if(m_one_minute_data.m_LowPrice>tick_data.LastPrice)
		{
			m_one_minute_data.m_LowPrice=tick_data.LastPrice;
		}
		m_one_minute_data.m_Volume=tick_data.Volume;
		m_one_minute_data.m_OpenInterest=tick_data.OpenInterest;
	}
	else
	{
		if( m_one_minute_data.m_Day!="" && m_one_minute_data.m_Time!="")
		{
			m_one_minute_data.m_Time+=":59";

			//We have gotten one minute statistics data
			CMinuteData prev_data;
			//memset(&prev_data, 0 ,sizeof(prev_data));
			iter = m_prev_one_minute_data_map.find(instrument_id);
			bool send = false;
			if (iter == m_prev_one_minute_data_map.end())
			{
				m_prev_one_minute_data_map.insert(CMinuteDataPair(instrument_id, m_one_minute_data));
			}
			else
			{
				prev_data = iter->second;
				m_prev_one_minute_data_map[instrument_id] = m_one_minute_data;
				send = true;
			}
			if (send)
			{
				m_one_minute_data.m_OpenInterest -= prev_data.m_OpenInterest;
				m_one_minute_data.m_Volume -= prev_data.m_Volume;
				MessageRouter::Router.sendData(m_one_minute_data);
				
				char* buffer = new char[8196];
				int index=0;
				
				const char* format_str="insert into stock_data.\"OneMinuteData\" values('%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,'%s')";
				

				sprintf(buffer,format_str,
					m_one_minute_data.m_Day.c_str(),
					(m_one_minute_data.m_Time).c_str(),
					m_one_minute_data.m_OpenPrice,
					m_one_minute_data.m_ClosePrice,
					m_one_minute_data.m_HighPrice,
					m_one_minute_data.m_LowPrice,
					m_one_minute_data.m_Volume,
					m_one_minute_data.m_OpenInterest,
					instrument_id.c_str()
					);
				m_log<< buffer<<endl;
				//DbConn conn(dbAccessPool);
				//conn.m_db->execSql(buffer);
				gThreadPool.Run(ExecSQL, (void*) buffer);
			}
		}
		resetOneMinuteData(m_one_minute_data, instrument_id);
		m_one_minute_data.m_Day=tradingDay;
		m_one_minute_data.m_Time=updateTime;
		m_one_minute_data.m_OpenPrice=tick_data.LastPrice;
		m_one_minute_data.m_ClosePrice=tick_data.LastPrice;
		m_one_minute_data.m_HighPrice=tick_data.LastPrice;
		m_one_minute_data.m_LowPrice=tick_data.LastPrice;
		m_one_minute_data.m_Volume=tick_data.Volume;
		m_one_minute_data.m_OpenInterest=tick_data.OpenInterest;
	}

	m_one_minute_data_map[instrument_id]=m_one_minute_data ;
}

void CMdSpi::resetOneMinuteData(CMinuteData& m_one_minute_data, const string& instrument_id)
{
	m_one_minute_data.m_Day= "";
	m_one_minute_data.m_Time= "";
	m_one_minute_data.m_ClosePrice=0;
	m_one_minute_data.m_HighPrice=0;
	m_one_minute_data.m_LowPrice=1e10;
	m_one_minute_data.m_OpenInterest=0;
	m_one_minute_data.m_OpenPrice=0;
	m_one_minute_data.m_Volume=0;
	m_one_minute_data.m_InstrumentID= instrument_id;
}

void CMdSpi::genHalfMinuteData(const CThostFtdcDepthMarketDataField & tick_data)
{

	string tradingDay=tick_data.TradingDay;
	string updateTime=tick_data.UpdateTime;
	string second = updateTime.substr(6, 2);
	updateTime=updateTime.substr(0,5);
	string instrument_id = tick_data.InstrumentID;
	
	CHalfMinuteData half_minute_data;

	CHalfMinuteDataMap::iterator iter = m_half_minute_data_map.find(instrument_id);
	if (iter == m_half_minute_data_map.end())
	{
		//add map data for this instrument
		half_minute_data.m_Day= "";
		half_minute_data.m_Time= "";
		half_minute_data.m_ClosePrice=0;
		half_minute_data.m_HighPrice=0;
		half_minute_data.m_LowPrice=1e10;
		half_minute_data.m_OpenInterest=0;
		half_minute_data.m_OpenPrice=0;
		half_minute_data.m_Volume=0;
		half_minute_data.m_Sec = "";
		half_minute_data.m_InstrumentID= instrument_id;
		m_half_minute_data_map.insert(CHalfMinuteDataPair(instrument_id, half_minute_data));
	}
	else
	{
		half_minute_data = m_half_minute_data_map[instrument_id];
	}

	if(tradingDay==half_minute_data.m_Day &&
		( (second<"30" && half_minute_data.m_Sec< "30")
		  || (second>="30" && half_minute_data.m_Sec>= "30") ) )
	{
		half_minute_data.m_ClosePrice= tick_data.LastPrice;

		if(half_minute_data.m_HighPrice<tick_data.LastPrice)
		{
			half_minute_data.m_HighPrice=tick_data.LastPrice;
		}

		if(half_minute_data.m_LowPrice>tick_data.LastPrice)
		{
			half_minute_data.m_LowPrice=tick_data.LastPrice;
		}
		half_minute_data.m_Volume=tick_data.Volume;
		half_minute_data.m_OpenInterest=tick_data.OpenInterest;
		
	}
	else
	{
		if( half_minute_data.m_Day!="" && half_minute_data.m_Time!="")
		{
			if ( half_minute_data.m_Sec < "30" )
				half_minute_data.m_Sec = "29";
			else
				half_minute_data.m_Sec = "59";

			half_minute_data.m_Time = half_minute_data.m_Time.substr(0,5)+":"+half_minute_data.m_Sec;
			//We have gotten one minute statistics data
			CHalfMinuteData prev_data;
			//memset(&prev_data, 0 ,sizeof(prev_data));
			iter = m_prev_half_minute_data_map.find(instrument_id);
			bool send = false;
			if (iter == m_prev_half_minute_data_map.end())
			{
				m_prev_half_minute_data_map.insert(CHalfMinuteDataPair(instrument_id, half_minute_data));
			}
			else
			{
				prev_data = iter->second;
				m_prev_half_minute_data_map[instrument_id] = half_minute_data;
				send = true;
			}
			if( send )
			{
				half_minute_data.m_OpenInterest -= prev_data.m_OpenInterest;
				half_minute_data.m_Volume -= prev_data.m_Volume;
				
				MessageRouter::Router.sendData(half_minute_data);
				
				char* buffer = new char[8196];
				int index=0;
				
				const char* format_str="insert into stock_data.\"HalfMinuteData\" values('%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,'%s')";
				
				
				sprintf(buffer,format_str,
					half_minute_data.m_Day.c_str(),
					(half_minute_data.m_Time).c_str(),
					half_minute_data.m_OpenPrice,
					half_minute_data.m_ClosePrice,
					half_minute_data.m_HighPrice,
					half_minute_data.m_LowPrice,
					half_minute_data.m_Volume,
					half_minute_data.m_OpenInterest,
					instrument_id.c_str()
					);
				m_log<< buffer<<endl;
				//DbConn conn(dbAccessPool);
				//conn.m_db->execSql(buffer);
				gThreadPool.Run(ExecSQL, (void*) buffer);
			}
		}
		
		half_minute_data.m_Day=tradingDay;
		half_minute_data.m_Time=updateTime;
		half_minute_data.m_Sec = second;
		half_minute_data.m_OpenPrice=tick_data.LastPrice;
		half_minute_data.m_ClosePrice=tick_data.LastPrice;
		half_minute_data.m_HighPrice=tick_data.LastPrice;
		half_minute_data.m_LowPrice=tick_data.LastPrice;
		half_minute_data.m_Volume=tick_data.Volume;
		half_minute_data.m_OpenInterest=tick_data.OpenInterest;
	}

	m_half_minute_data_map[instrument_id]=half_minute_data ;
}