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
#include "HalfMinuteSeriesGenerator.h"
#include "OneMinuteSeriesGenerator.h"
#include "TenMinuteSeriesGenerator.h"
#include "VarOneMinuteSeriesGenerator.h"
#include "VarHalfMinuteSeriesGenerator.h"
#include "TradeConn.h"
// USER_API参数
//extern CThostFtdcMdApi* m_pUserApi;

// 配置参数	

extern char *ppInstrumentID[];	
extern int iInstrumentID;

// 请求编号

extern DbAccessorPool dbAccessPool;

bool CMdSpi::StoreMarketData = true;
CMdSpi::~CMdSpi()
{
	m_log.close();

	int size = m_series_generator.size();
	
	for( int i =0; i<size; i++)
	{
		delete m_series_generator[i];
	}
	::DeleteCriticalSection(&m_data_critsec);
	//::DeleteCriticalSection(&m_minute_critsec);
}

CMdSpi::CMdSpi(CThostFtdcMdApi* api, string broker_id, string investor_id, 
			   string passwd, TradeConn* conn)
	:m_pUserApi(api),m_requestID(0), m_BrokerId(broker_id), m_InvestorId(investor_id),
		m_Passwd(passwd), 
		m_log((string(".\\")+broker_id+investor_id+"market.log").c_str(),ios::app), 
		m_Conn(conn), 
		m_ConnStatus(false)
{
	::InitializeCriticalSection(&m_data_critsec);
	ASSERT(m_Conn != NULL);
	// stl library can be buggy with empty maps. Insert some rubbish data here.
	m_tick_data_map.insert(CTickDataPair("", CThostFtdcDepthMarketDataField() ));
	m_StoreMarketData = StoreMarketData;
	StoreMarketData = false;
	//m_series_generator.push_back(new OneMinuteSeriesGenerator(&(m_Conn->m_Router),m_StoreMarketData));
	//m_series_generator.push_back(new HalfMinuteSeriesGenerator(&(m_Conn->m_Router),m_StoreMarketData));
	m_series_generator.push_back(new TenMinuteSeriesGenerator(&(m_Conn->m_Router),m_StoreMarketData));
	m_series_generator.push_back(new VarOneMinuteSeriesGenerator(&(m_Conn->m_Router),m_StoreMarketData,5));
	//	m_series_generator.push_back(new VarOneMinuteSeriesGenerator(0));
	//	m_series_generator.push_back(new VarOneMinuteSeriesGenerator(59));
	m_series_generator.push_back(new VarHalfMinuteSeriesGenerator(&(m_Conn->m_Router),m_StoreMarketData,5));

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
	m_ConnStatus = false;
	CString str;
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL && view->GetCurrConn() == m_Conn)
	{
		str.Format("数据服务器: %s", "OFF");
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
	m_ConnStatus = true;
	CString str;
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL && view->GetCurrConn() == m_Conn)
	{
		str.Format("数据服务器: %s", "ON");
		view->m_MDStatus.SetWindowText(str);
	}
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_BrokerId.c_str());
	strcpy(req.UserID, m_InvestorId.c_str());
	strcpy(req.Password, m_Passwd.c_str());
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
	else if (bIsLast && IsErrorRspInfo(pRspInfo))
	{
		Sleep(1000);
		ReqUserLogin();
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
    
	m_Conn->m_Router.sendData(* pDepthMarketData);
	
	string time = pDepthMarketData->UpdateTime;

	if ( time > "09:13:59" && time < "23:40" )
	{
		int size = m_series_generator.size();
		
		for( int i =0; i<size; i++)
		{
			m_series_generator[i]->InputTickData(* pDepthMarketData);
		}
		//genOneMinuteData(*pDepthMarketData);
		//genHalfMinuteData(*pDepthMarketData);
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

	if( m_StoreMarketData )
	{
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
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		m_log << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}
