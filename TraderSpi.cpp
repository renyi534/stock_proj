
#include "stdafx.h"
#include <iostream>
using namespace std;
#include ".\\ThostTraderApi\\ThostFtdcTraderApi.h"
#include "TraderSpi.h"
#include "Algorithm.h"
#include <math.h>
#include "DbConn.h"
#include "MessageRouter.h"
#pragma warning(disable : 4996)
#include "TradeHandlingThread.h"
// 配置参数
extern char FRONT_ADDR[];		// 前置地址
extern char BROKER_ID[];		// 经纪公司代码
extern char INVESTOR_ID[];		// 投资者代码
extern char PASSWORD[];			// 用户密码
extern char INSTRUMENT_ID[];	// 合约代码
extern char* ppInstrumentID[];	
extern int iInstrumentID;

extern DbAccessorPool dbAccessPool;
#include "resource.h"
#include "TradeSystemDoc.h"
#include "TradeSystemView.h"
// 会话参数
TThostFtdcFrontIDType	FRONT_ID;	//前置编号
TThostFtdcSessionIDType	SESSION_ID;	//会话编号
TThostFtdcOrderRefType	ORDER_REF;	//报单引用

void CTraderSpi::OnFrontConnected()
{
	m_log << "--->>> " << "OnFrontConnected" << endl;
	///用户登录请求
	ReqUserLogin();
	CString str;
	CTradeSystemView* view = (CTradeSystemView*)((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	if( view != NULL)
	{
		str.Format("Trade Server: %s", "ON");
		view->m_TradeStatus.SetWindowText(str);
	}
}

CTraderSpi::~CTraderSpi()
{
	m_log.close();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = m_pTradeApi->ReqUserLogin(&req, ++m_requestID);
	m_log << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
								CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// 保存会话参数
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		///获取当前交易日
		m_log << "--->>> 获取当前交易日 = " << m_pTradeApi->GetTradingDay() << endl;
		///投资者结算结果确认
		ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = m_pTradeApi->ReqSettlementInfoConfirm(&req, ++m_requestID);
	m_log << "--->>> 投资者结算结果确认: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求账户
		ReqQryTradingAccount();
	}
}

void CTraderSpi::ReqQryInstrument()
{
	for ( int i=0; i< iInstrumentID; i++)
	{
		ReqQryInstrument(*(ppInstrumentID+i));
	}
}

void CTraderSpi::ReqQryInstrument(string instrument)
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, instrument.c_str());
	int iResult = m_pTradeApi->ReqQryInstrument(&req, ++m_requestID);
	m_log << "--->>> 请求查询合约: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrument == NULL)
	{
		m_log << "--->>> " << "WARNING OnRspQryInstrument NULL response" << endl;
		return;
	}

	m_log << "--->>> " << "OnRspQryInstrument" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求查询投资者持仓
		if( pInstrument!=NULL && pInstrument->IsTrading )
			ReqQryInvestorPosition(pInstrument->InstrumentID);
	}
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = m_pTradeApi->ReqQryTradingAccount(&req, ++m_requestID);
	m_log << "--->>> 请求查询资金账户: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspQryTradingAccount" << endl;
	
	if (pTradingAccount == NULL)
	{
		m_log << "--->>> " << "WARNING OnRspQryTradingAccount NULL response" << endl;
		return;
	}


    m_account = *pTradingAccount;
	MessageRouter::Router.sendData(*pTradingAccount);

	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		
		//请求查询合约
		ReqQryInstrument();
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	for ( int i=0; i< iInstrumentID; i++)
	{
		ReqQryInvestorPosition(*(ppInstrumentID+i));
	}
}

void CTraderSpi::ReqQryInvestorPosition(string instrument)
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, instrument.c_str());
	int iResult = m_pTradeApi->ReqQryInvestorPosition(&req, ++m_requestID);
	m_log << "--->>> 请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败") << endl;
	
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspQryInvestorPosition" << endl;
	
	if ( pInvestorPosition != NULL )
	{
		string key = pInvestorPosition->InstrumentID;
		
		InvestorPosition pos;
		
		memset(&pos, 0, sizeof(pos));
		
		if ( m_inv_pos.find(key) != m_inv_pos.end() )
		{
			pos = m_inv_pos[key];
		}		
		static bool is_of_today_state = false;

		if (pInvestorPosition->PositionDate == THOST_FTDC_PSD_Today)
		{
			if( is_of_today_state == false )
			{
				is_of_today_state = true;
				pos.YdLong = pos.YdShort =0;
			}

			if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Net)
			{
				pos.Net = pInvestorPosition->TodayPosition;
				pos.YdNet = pInvestorPosition->Position-pInvestorPosition->TodayPosition;
				
			}
			else if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Long)
			{
				pos.Long = pInvestorPosition->TodayPosition;
				pos.YdLong = pInvestorPosition->Position-pInvestorPosition->TodayPosition;
			}
			else
			{
				pos.Short = pInvestorPosition->TodayPosition;
				pos.YdShort = pInvestorPosition->Position-pInvestorPosition->TodayPosition;
			}
		}
		else if( !is_of_today_state )
		{
			if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Net)
			{
					pos.YdNet = pInvestorPosition->YdPosition;
			}
			else if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Long)
			{
					pos.YdLong = pInvestorPosition->YdPosition;
			}
			else
			{
					pos.YdShort = pInvestorPosition->YdPosition;
			}
		}

		if ( m_inv_pos.find(key) != m_inv_pos.end() )
		{
			m_inv_pos[key] = pos;
		}
		else
		{
			m_inv_pos.insert(inv_pos_pair(key,pos));   
		}
		MessageRouter::Router.sendData(*pInvestorPosition);
		//m_inv_pos.insert(inv_pos_pair(key,*pInvestorPosition));
	}
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///报单录入请求
		//ReqOrderInsert();
	}
}

void CTraderSpi::ReqOrderInsert(CThostFtdcInputOrderField& req)
{
/*CThostFtdcInputOrderField req;
memset(&req, 0, sizeof(req));
///经纪公司代码
strcpy(req.BrokerID, BROKER_ID);
///投资者代码
strcpy(req.InvestorID, INVESTOR_ID);
///合约代码
strcpy(req.InstrumentID, INSTRUMENT_ID);
///报单引用
strcpy(req.OrderRef, ORDER_REF);
///用户代码
//	TThostFtdcUserIDType	UserID;
///报单价格条件: 限价
req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
///买卖方向: 
req.Direction = DIRECTION;
///组合开平标志: 开仓
req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
///组合投机套保标志
req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
///价格
req.LimitPrice = LIMIT_PRICE;
///数量: 1
req.VolumeTotalOriginal = 1;
///有效期类型: 当日有效
req.TimeCondition = THOST_FTDC_TC_GFD;
///GTD日期
//	TThostFtdcDateType	GTDDate;
///成交量类型: 任何数量
req.VolumeCondition = THOST_FTDC_VC_AV;
///最小成交量: 1
req.MinVolume = 1;
///触发条件: 立即
req.ContingentCondition = THOST_FTDC_CC_Immediately;
///止损价
//	TThostFtdcPriceType	StopPrice;
///强平原因: 非强平
req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
///自动挂起标志: 否
req.IsAutoSuspend = 0;
///业务单元
//	TThostFtdcBusinessUnitType	BusinessUnit;
///请求编号
//	TThostFtdcRequestIDType	RequestID;
///用户强评标志: 否
	req.UserForceClose = 0;*/
	
	int iResult = m_pTradeApi->ReqOrderInsert(&req, ++m_requestID);
	m_log << "--->>> 报单录入请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
    int iNextOrderRef = atoi(ORDER_REF);
    iNextOrderRef++;
    sprintf(ORDER_REF, "%d", iNextOrderRef);    
}

void CTraderSpi::ReqOrderInsert(OrderInfo& order_req)
{
	if ( order_req.amount ==0 )
		return;
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, BROKER_ID);
	///投资者代码
	strcpy(req.InvestorID, INVESTOR_ID);
	///合约代码
	strcpy(req.InstrumentID, order_req.m_instrumentID.c_str());
	///报单引用
	strcpy(req.OrderRef, ORDER_REF);
	///用户代码
	//	TThostFtdcUserIDType	UserID;
	strcpy(req.UserID, INVESTOR_ID);
	///报单价格条件: 限价
	
	if (order_req.price<0) {
		req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	}
	else
	{
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	}
	
	///买卖方向: 
	if( order_req.is_buy == 1 )
	{
		req.Direction = THOST_FTDC_D_Buy;
	}
	else
	{
		req.Direction = THOST_FTDC_D_Sell;
	}
	
	///组合开平标志: 开仓
	if (order_req.is_open==1)
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	else
		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	req.LimitPrice = order_req.price;
	///数量: 1
	req.VolumeTotalOriginal = fabs(order_req.amount);
	///有效期类型: 当日有效
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD日期
	//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 0;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
	//	TThostFtdcPriceType	StopPrice;
	req.StopPrice =0;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; //THOST_FTDC_FCC_ClientOverPositionLimit; //
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;
	///业务单元
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	//	TThostFtdcRequestIDType	RequestID;
	req.RequestID = m_requestID;
	///用户强评标志: 否
	req.UserForceClose = 0;
	
	int iResult = m_pTradeApi->ReqOrderInsert(&req, ++m_requestID);
	m_log <<"New Order!   Time:"<<order_req.day<<" "<<order_req.time<<"  Price:"<<order_req.price<< " Amount: "
		<<order_req.amount<<"  is_buy:"<<order_req.is_buy<<"  is_open:"<<order_req.is_open
		<<"--->>> 报单录入请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
    int iNextOrderRef = atoi(ORDER_REF);
    iNextOrderRef++;
    sprintf(ORDER_REF, "%d", iNextOrderRef);    		
}

void CTraderSpi::ReqOrderInsert(OrderInfoShort& order_req, bool isCorrection)
{
	InvestorPosition pos;
	string key = order_req.m_instrumentID;

	if( !isCorrection )
		m_AlgoPos += order_req.amount;
	
	memset(&pos, 0, sizeof(pos));
	if ( m_inv_pos.find(key) != m_inv_pos.end() )
	{
		pos = m_inv_pos[key];
	}
	
	if ( order_req.amount ==0 )
		return;

	OrderInfo order;
	order.amount = fabs(order_req.amount);
	order.day = order_req.day;
	order.m_instrumentID = order_req.m_instrumentID;
	order.milliSec = order_req.milliSec;
	order.price = order_req.price;
	order.time = order_req.time;
	
	if(  order_req.amount < 0)
	{
		if(pos.Long >= fabs(order_req.amount))
		{
			order.is_open = 0;
			order.is_buy = 0;
			ReqOrderInsert(order);
		}
		else if( pos.Long > 0)
		{
			order.is_open =0;
			order.is_buy =0;
			order.amount = pos.Long;
			ReqOrderInsert(order);
			
			order.amount = fabs(order_req.amount)-pos.Long; 
			order.is_open =1;
			order.is_buy =0;
			ReqOrderInsert(order);
		}
		else
		{
			order.amount = fabs(order_req.amount); 
			order.is_open =1;
			order.is_buy =0;
			ReqOrderInsert(order);
		}
	}
	else
	{
		if(pos.Short >= fabs(order_req.amount))
		{
			order.is_open = 0;
			order.is_buy = 1;
			ReqOrderInsert(order);
		}
		else if( pos.Short > 0)
		{
			order.is_open =0;
			order.is_buy =1;
			order.amount = pos.Short;
			ReqOrderInsert(order);
			
			order.amount = fabs(order_req.amount)-pos.Short; 
			order.is_open =1;
			order.is_buy =1;
			ReqOrderInsert(order);
		}
		else
		{
			order.amount = fabs(order_req.amount); 
			order.is_open =1;
			order.is_buy =1;
			ReqOrderInsert(order);
		}
	}
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspOrderInsert" << endl;
	
	if (pInputOrder == NULL)
	{
		return;
	}

	if (IsErrorRspInfo(pRspInfo))
    {
        if( bIsLast )
        {
            // Error order, shuffle open/close flag
            CThostFtdcInputOrderField req= *pInputOrder;

            strcpy(req.OrderRef, ORDER_REF);
            
            // if cash not enough, try to release cash here
	        if (req.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
            {
		        req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	            
                //else
		        //req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

                req.RequestID = m_requestID;
                ReqOrderInsert(req);
            }
            ReqQryInstrument(req.InstrumentID);
        }
    }
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{	
	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, pOrder->BrokerID);
	///投资者代码
	strcpy(req.InvestorID, pOrder->InvestorID);
	///报单操作引用
	//	TThostFtdcOrderActionRefType	OrderActionRef;
	///报单引用
	strcpy(req.OrderRef, pOrder->OrderRef);
	///请求编号
	//	TThostFtdcRequestIDType	RequestID;
	///前置编号
	req.FrontID = FRONT_ID;
	///会话编号
	req.SessionID = SESSION_ID;
	///交易所代码
	//	TThostFtdcExchangeIDType	ExchangeID;
	///报单编号
	//	TThostFtdcOrderSysIDType	OrderSysID;
	///操作标志
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///价格
	//	TThostFtdcPriceType	LimitPrice;
	///数量变化
	//	TThostFtdcVolumeType	VolumeChange;
	///用户代码
	//	TThostFtdcUserIDType	UserID;
	///合约代码
	strcpy(req.InstrumentID, pOrder->InstrumentID);
	
	int iResult = m_pTradeApi->ReqOrderAction(&req, ++m_requestID);
	m_log << "--->>> 报单操作请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspOrderAction" << endl;
	IsErrorRspInfo(pRspInfo);
}

///报单通知
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	m_log << "--->>> " << "OnRtnOrder"  << endl;
	if (pOrder == NULL)
	{
		m_log << "--->>> " << "WARNING OnRtnOrder fail"  << endl;
		return;
	}
	OrderInfo order_req;

	string key = pOrder->OrderSysID;

	if ( m_order_state.find(key) != m_order_state.end() )
	{
		m_order_state[key]= *pOrder;
		m_log << "--->>> " << "OnRtnOrder order already registered"  << endl;
		return;
	}
	else
	{
		m_order_state.insert(order_state_pair(key,*pOrder));
		m_log << "--->>> " << "OnRtnOrder new order found"  << endl;
        /*if ( pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted ||
            pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected )
		    m_order_state.insert(order_state_pair(key,*pOrder));  
        else
            return;*/
	}
    
    ReqQryInstrument(pOrder->InstrumentID);

	order_req.amount = pOrder->VolumeTotalOriginal;
	order_req.is_buy = (pOrder->Direction==THOST_FTDC_D_Buy)?1:0;
	order_req.is_open = (pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Open)? 1: 0;
	order_req.price = pOrder->LimitPrice;
	order_req.day = pOrder->InsertDate;
	order_req.time = pOrder->InsertTime;
	order_req.m_instrumentID = pOrder->InstrumentID;
	order_req.milliSec =0;
	StoreOrder(order_req, false);
	/*if ( pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted )
	{
		order_req.amount = pOrder->VolumeTotalOriginal;
		order_req.is_buy = (pOrder->Direction==THOST_FTDC_D_Buy)?1:0;
		order_req.is_open = (pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Open)? 1: 0;
		order_req.price = pOrder->LimitPrice;
		order_req.day = pOrder->InsertDate;
		order_req.time = pOrder->InsertTime;
		order_req.m_instrumentID = pOrder->InstrumentID;
		order_req.milliSec =0;
		StoreOrder(order_req, false);
	}
	else if( pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected )
	{
		order_req.amount = pOrder->VolumeTotalOriginal;
		order_req.is_buy = (pOrder->Direction==THOST_FTDC_D_Buy)?1:0;
		order_req.is_open = (pOrder->CombOffsetFlag[0] == THOST_FTDC_OF_Open)? 1: 0;
		order_req.price = pOrder->LimitPrice;
		order_req.day = pOrder->InsertDate;
		order_req.time = pOrder->InsertTime;
		order_req.m_instrumentID = pOrder->InstrumentID;
		order_req.milliSec =0;
		StoreOrder(order_req, true);		
	}*/
}

///成交通知
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	m_log << "--->>> " << "OnRtnTrade"  << endl;

	if (pTrade == NULL)
	{
		m_log << "--->>> " << "WARNING OnRtnTrade fail"  << endl;
		return;
	}

	MessageRouter::Router.sendData(*pTrade);
	//DbConn conn(dbAccessPool);
	char* buffer= new char[2048];
	string insert_sql = "insert into stock_data.trade_details(instrument_id, trans_day, trans_time, price, amount, is_buy, is_open) "
		"values('%s','%s','%s',%f,%d,%d,%d)";
	
    int amount = pTrade->Volume;
	
	int is_buy = 1;
    if ( pTrade->Direction == THOST_FTDC_D_Sell )
        is_buy =0 ;
	
	int is_open = 1;
	
	if (pTrade->OffsetFlag != THOST_FTDC_OF_Open )
		is_open =0;
	
	sprintf(buffer, insert_sql.c_str(), 
        pTrade->InstrumentID,
        pTrade->TradeDate,
        pTrade->TradeTime,
        pTrade->Price,
        amount,
		is_buy,
		is_open);
	
	
	string display_str = pTrade->InstrumentID;
	display_str+=", ";
	display_str+=pTrade->TradeDate;
	display_str+=" ";
	display_str+=pTrade->TradeTime;
	
	if ( is_buy )
		display_str += ", buy";
	else
		display_str += ", sell";

	if (is_open )
		display_str += ", open, ";
	else
		display_str += ", close, ";

	char temp_buf[30];
	sprintf(temp_buf, "price: %f, ", pTrade->Price);

	display_str += temp_buf;

	sprintf(temp_buf, "amount: %d", amount);

	display_str += temp_buf;

	m_TradeCount++;

	CString str;
	CTradeSystemView* view = (CTradeSystemView*)((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	if( view != NULL)
	{
		view->m_TradeDetail.SetSel(-1,-1);
		view->m_TradeDetail.ReplaceSel(display_str.c_str());
		view->m_TradeDetail.SetSel(-1,-1);
		view->m_TradeDetail.ReplaceSel("\r\n");
		str.Format("Trade Count: %d", m_TradeCount);
		view->m_TradeTotal.SetWindowText(str);
	}
	
	gThreadPool.Run(ExecSQL, (void*) buffer);
	//conn.m_db->execSql(buffer);
	
    ReqQryTradingAccount();
}


void CTraderSpi::StoreOrder(const OrderInfo& initialData, bool isRej)
{
	//DbConn conn(dbAccessPool);
	char* buffer= new char[2048];
	string insert_sql = "insert into stock_data.order_details(instrument_id, trans_day, trans_time, price, amount,is_buy,is_open) "
		"values('%s','%s','%s',%f,%d,%d,%d)";

	if (isRej)
	{
		insert_sql+= " Rejected Order";
	}
	else
		m_OrderCount++;

	sprintf(buffer, insert_sql.c_str(), 
		initialData.m_instrumentID.c_str(),
		initialData.day.c_str(),
		initialData.time.c_str(),
		initialData.price,
		initialData.amount,
		initialData.is_buy,
		initialData.is_open
        );
	
	string display_str = initialData.m_instrumentID;
	display_str+=", "+initialData.day+" "+initialData.time;
	
	if ( initialData.is_buy )
		display_str += ", buy";
	else
		display_str += ", sell";

	if (initialData.is_open )
		display_str += ", open, ";
	else
		display_str += ", close, ";

	char temp_buf[30];
	sprintf(temp_buf, "price: %f, ", initialData.price);

	display_str += temp_buf;

	sprintf(temp_buf, "amount: %d", initialData.amount);

	display_str += temp_buf;
	
	//conn.m_db->execSql(buffer);
	CString str;
	CTradeSystemView* view = (CTradeSystemView*)((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	if( view != NULL)
	{
		view->m_OrderDetail.SetSel(-1,-1);
		view->m_OrderDetail.ReplaceSel(display_str.c_str());
		view->m_OrderDetail.SetSel(-1,-1);
		view->m_OrderDetail.ReplaceSel("\r\n");
		str.Format("Order Count: %d", m_OrderCount);
		view->m_OrderTotal.SetWindowText(str);
	}
	
	if (!isRej)
		gThreadPool.Run(ExecSQL, (void*) buffer);
	
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
	m_log << "--->>> " << "OnFrontDisconnected" << endl;
	m_log << "--->>> Reason = " << nReason << endl;
	CString str;
	CTradeSystemView* view = (CTradeSystemView*)((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	if( view != NULL)
	{
		str.Format("Trade Server: %s", "OFF");
		view->m_TradeStatus.SetWindowText(str);
	}
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	m_log << "--->>> " << "OnHeartBeatWarning" << endl;
	m_log << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	
	if (bResult)
	{
		CString str;
		str.Format("ErrorID: %d; ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		m_log << "Trade Error found. "<<" ErrorID:"<<pRspInfo->ErrorID
            <<"  ErrorMsg: "<<pRspInfo->ErrorMsg<<endl;
	}
	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
		(pOrder->SessionID == SESSION_ID) );
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

void CTraderSpi::ClearLongPos(string instrument, double price)
{
	InvestorPosition pos;
	if( m_inv_pos.find(instrument) == m_inv_pos.end() )
	{
		return;
	}
	else
	{
		pos = m_inv_pos[instrument];
	}


	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, BROKER_ID);
	///投资者代码
	strcpy(req.InvestorID, INVESTOR_ID);
	///合约代码
	strcpy(req.InstrumentID, instrument.c_str());

	///用户代码
	//	TThostFtdcUserIDType	UserID;
	strcpy(req.UserID, INVESTOR_ID);
	///报单价格条件: 限价
	
	if (price<0) {
		req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	}
	else
	{
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	}

	///买卖方向: 
	req.Direction = THOST_FTDC_D_Sell;
	
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	req.LimitPrice = price;

	///有效期类型: 当日有效
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD日期
	//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 0;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
	//	TThostFtdcPriceType	StopPrice;
	req.StopPrice =0;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; //THOST_FTDC_FCC_ClientOverPositionLimit; //
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;

	///用户强评标志: 否
	req.UserForceClose = 0;

	if( pos.YdLong > 0 )
	{
		///报单引用
		strcpy(req.OrderRef, ORDER_REF);
		///业务单元
		//	TThostFtdcBusinessUnitType	BusinessUnit;
		///请求编号
		//	TThostFtdcRequestIDType	RequestID;
		req.RequestID = m_requestID;

		req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		///数量: 
		req.VolumeTotalOriginal = pos.YdLong;
		this->ReqOrderInsert(req);
	}

	if(pos.Long >0)
	{
		m_AlgoPos -= pos.Long;  
		///报单引用
		strcpy(req.OrderRef, ORDER_REF);
		///业务单元
		//	TThostFtdcBusinessUnitType	BusinessUnit;
		///请求编号
		//	TThostFtdcRequestIDType	RequestID;
		req.RequestID = m_requestID;

		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		///数量: 
		req.VolumeTotalOriginal = pos.Long;
		this->ReqOrderInsert(req);
	}
}

void CTraderSpi::ClearShortPos(string instrument, double price)
{
	InvestorPosition pos;
	if( m_inv_pos.find(instrument) == m_inv_pos.end() )
	{
		return;
	}
	else
	{
		pos = m_inv_pos[instrument];
	}


	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, BROKER_ID);
	///投资者代码
	strcpy(req.InvestorID, INVESTOR_ID);
	///合约代码
	strcpy(req.InstrumentID, instrument.c_str());

	///用户代码
	//	TThostFtdcUserIDType	UserID;
	strcpy(req.UserID, INVESTOR_ID);
	///报单价格条件: 限价
	
	if (price<0) {
		req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	}
	else
	{
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	}

	///买卖方向: 
	req.Direction = THOST_FTDC_D_Buy;
	
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	req.LimitPrice = price;

	///有效期类型: 当日有效
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD日期
	//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 0;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
	//	TThostFtdcPriceType	StopPrice;
	req.StopPrice =0;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; //THOST_FTDC_FCC_ClientOverPositionLimit; //
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;

	///用户强评标志: 否
	req.UserForceClose = 0;

	if( pos.YdShort > 0 )
	{
		///报单引用
		strcpy(req.OrderRef, ORDER_REF);
		///业务单元
		//	TThostFtdcBusinessUnitType	BusinessUnit;
		///请求编号
		//	TThostFtdcRequestIDType	RequestID;
		req.RequestID = m_requestID;

		req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		///数量: 
		req.VolumeTotalOriginal = pos.YdShort;
		this->ReqOrderInsert(req);
	}

	if(pos.Short >0)
	{
		m_AlgoPos += pos.Short;  
		///报单引用
		strcpy(req.OrderRef, ORDER_REF);
		///业务单元
		//	TThostFtdcBusinessUnitType	BusinessUnit;
		///请求编号
		//	TThostFtdcRequestIDType	RequestID;
		req.RequestID = m_requestID;

		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		///数量: 
		req.VolumeTotalOriginal = pos.Short;
		this->ReqOrderInsert(req);
	}
}

void CTraderSpi::CancelAllOrders(string instrument)
{
	order_state_map::iterator iter =  m_order_state.begin();

	while ( iter != m_order_state.end() )
	{
		CThostFtdcOrderField order = iter->second;

		if (instrument == order.InstrumentID && 
			IsTradingOrder(&order) && IsMyOrder(&order) )
		{
			ReqOrderAction(&order);
		}
		iter++;
	}
}
