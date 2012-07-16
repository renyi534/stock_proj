
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
// ���ò���
extern char FRONT_ADDR[];		// ǰ�õ�ַ
extern char BROKER_ID[];		// ���͹�˾����
extern char INVESTOR_ID[];		// Ͷ���ߴ���
extern char PASSWORD[];			// �û�����
extern char INSTRUMENT_ID[];	// ��Լ����
extern char* ppInstrumentID[];	
extern int iInstrumentID;

extern DbAccessorPool dbAccessPool;
#include "resource.h"
#include "TradeSystemDoc.h"
#include "TradeSystemView.h"
// �Ự����
TThostFtdcFrontIDType	FRONT_ID;	//ǰ�ñ��
TThostFtdcSessionIDType	SESSION_ID;	//�Ự���
TThostFtdcOrderRefType	ORDER_REF;	//��������

void CTraderSpi::OnFrontConnected()
{
	m_log << "--->>> " << "OnFrontConnected" << endl;
	///�û���¼����
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
	m_log << "--->>> �����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
								CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		///��ȡ��ǰ������
		m_log << "--->>> ��ȡ��ǰ������ = " << m_pTradeApi->GetTradingDay() << endl;
		///Ͷ���߽�����ȷ��
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
	m_log << "--->>> Ͷ���߽�����ȷ��: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///�����˻�
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
	m_log << "--->>> �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
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
		///�����ѯͶ���ֲ߳�
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
	m_log << "--->>> �����ѯ�ʽ��˻�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
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
		
		//�����ѯ��Լ
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
	m_log << "--->>> �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
	
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
		///����¼������
		//ReqOrderInsert();
	}
}

void CTraderSpi::ReqOrderInsert(CThostFtdcInputOrderField& req)
{
/*CThostFtdcInputOrderField req;
memset(&req, 0, sizeof(req));
///���͹�˾����
strcpy(req.BrokerID, BROKER_ID);
///Ͷ���ߴ���
strcpy(req.InvestorID, INVESTOR_ID);
///��Լ����
strcpy(req.InstrumentID, INSTRUMENT_ID);
///��������
strcpy(req.OrderRef, ORDER_REF);
///�û�����
//	TThostFtdcUserIDType	UserID;
///�����۸�����: �޼�
req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
///��������: 
req.Direction = DIRECTION;
///��Ͽ�ƽ��־: ����
req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
///���Ͷ���ױ���־
req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
///�۸�
req.LimitPrice = LIMIT_PRICE;
///����: 1
req.VolumeTotalOriginal = 1;
///��Ч������: ������Ч
req.TimeCondition = THOST_FTDC_TC_GFD;
///GTD����
//	TThostFtdcDateType	GTDDate;
///�ɽ�������: �κ�����
req.VolumeCondition = THOST_FTDC_VC_AV;
///��С�ɽ���: 1
req.MinVolume = 1;
///��������: ����
req.ContingentCondition = THOST_FTDC_CC_Immediately;
///ֹ���
//	TThostFtdcPriceType	StopPrice;
///ǿƽԭ��: ��ǿƽ
req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
///�Զ������־: ��
req.IsAutoSuspend = 0;
///ҵ��Ԫ
//	TThostFtdcBusinessUnitType	BusinessUnit;
///������
//	TThostFtdcRequestIDType	RequestID;
///�û�ǿ����־: ��
	req.UserForceClose = 0;*/
	
	int iResult = m_pTradeApi->ReqOrderInsert(&req, ++m_requestID);
	m_log << "--->>> ����¼������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
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
	///���͹�˾����
	strcpy(req.BrokerID, BROKER_ID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, INVESTOR_ID);
	///��Լ����
	strcpy(req.InstrumentID, order_req.m_instrumentID.c_str());
	///��������
	strcpy(req.OrderRef, ORDER_REF);
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	strcpy(req.UserID, INVESTOR_ID);
	///�����۸�����: �޼�
	
	if (order_req.price<0) {
		req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	}
	else
	{
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	}
	
	///��������: 
	if( order_req.is_buy == 1 )
	{
		req.Direction = THOST_FTDC_D_Buy;
	}
	else
	{
		req.Direction = THOST_FTDC_D_Sell;
	}
	
	///��Ͽ�ƽ��־: ����
	if (order_req.is_open==1)
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	else
		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	
	///���Ͷ���ױ���־
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	req.LimitPrice = order_req.price;
	///����: 1
	req.VolumeTotalOriginal = fabs(order_req.amount);
	///��Ч������: ������Ч
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
	//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	req.MinVolume = 0;
	///��������: ����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ֹ���
	//	TThostFtdcPriceType	StopPrice;
	req.StopPrice =0;
	///ǿƽԭ��: ��ǿƽ
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; //THOST_FTDC_FCC_ClientOverPositionLimit; //
	///�Զ������־: ��
	req.IsAutoSuspend = 0;
	///ҵ��Ԫ
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	//	TThostFtdcRequestIDType	RequestID;
	req.RequestID = m_requestID;
	///�û�ǿ����־: ��
	req.UserForceClose = 0;
	
	int iResult = m_pTradeApi->ReqOrderInsert(&req, ++m_requestID);
	m_log <<"New Order!   Time:"<<order_req.day<<" "<<order_req.time<<"  Price:"<<order_req.price<< " Amount: "
		<<order_req.amount<<"  is_buy:"<<order_req.is_buy<<"  is_open:"<<order_req.is_open
		<<"--->>> ����¼������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
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
	///���͹�˾����
	strcpy(req.BrokerID, pOrder->BrokerID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, pOrder->InvestorID);
	///������������
	//	TThostFtdcOrderActionRefType	OrderActionRef;
	///��������
	strcpy(req.OrderRef, pOrder->OrderRef);
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///ǰ�ñ��
	req.FrontID = FRONT_ID;
	///�Ự���
	req.SessionID = SESSION_ID;
	///����������
	//	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	//	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///�۸�
	//	TThostFtdcPriceType	LimitPrice;
	///�����仯
	//	TThostFtdcVolumeType	VolumeChange;
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	///��Լ����
	strcpy(req.InstrumentID, pOrder->InstrumentID);
	
	int iResult = m_pTradeApi->ReqOrderAction(&req, ++m_requestID);
	m_log << "--->>> ������������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_log << "--->>> " << "OnRspOrderAction" << endl;
	IsErrorRspInfo(pRspInfo);
}

///����֪ͨ
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

///�ɽ�֪ͨ
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
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
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
	///���͹�˾����
	strcpy(req.BrokerID, BROKER_ID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, INVESTOR_ID);
	///��Լ����
	strcpy(req.InstrumentID, instrument.c_str());

	///�û�����
	//	TThostFtdcUserIDType	UserID;
	strcpy(req.UserID, INVESTOR_ID);
	///�����۸�����: �޼�
	
	if (price<0) {
		req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	}
	else
	{
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	}

	///��������: 
	req.Direction = THOST_FTDC_D_Sell;
	
	///���Ͷ���ױ���־
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	req.LimitPrice = price;

	///��Ч������: ������Ч
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
	//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	req.MinVolume = 0;
	///��������: ����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ֹ���
	//	TThostFtdcPriceType	StopPrice;
	req.StopPrice =0;
	///ǿƽԭ��: ��ǿƽ
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; //THOST_FTDC_FCC_ClientOverPositionLimit; //
	///�Զ������־: ��
	req.IsAutoSuspend = 0;

	///�û�ǿ����־: ��
	req.UserForceClose = 0;

	if( pos.YdLong > 0 )
	{
		///��������
		strcpy(req.OrderRef, ORDER_REF);
		///ҵ��Ԫ
		//	TThostFtdcBusinessUnitType	BusinessUnit;
		///������
		//	TThostFtdcRequestIDType	RequestID;
		req.RequestID = m_requestID;

		req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		///����: 
		req.VolumeTotalOriginal = pos.YdLong;
		this->ReqOrderInsert(req);
	}

	if(pos.Long >0)
	{
		m_AlgoPos -= pos.Long;  
		///��������
		strcpy(req.OrderRef, ORDER_REF);
		///ҵ��Ԫ
		//	TThostFtdcBusinessUnitType	BusinessUnit;
		///������
		//	TThostFtdcRequestIDType	RequestID;
		req.RequestID = m_requestID;

		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		///����: 
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
	///���͹�˾����
	strcpy(req.BrokerID, BROKER_ID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, INVESTOR_ID);
	///��Լ����
	strcpy(req.InstrumentID, instrument.c_str());

	///�û�����
	//	TThostFtdcUserIDType	UserID;
	strcpy(req.UserID, INVESTOR_ID);
	///�����۸�����: �޼�
	
	if (price<0) {
		req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	}
	else
	{
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	}

	///��������: 
	req.Direction = THOST_FTDC_D_Buy;
	
	///���Ͷ���ױ���־
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	req.LimitPrice = price;

	///��Ч������: ������Ч
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
	//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	req.MinVolume = 0;
	///��������: ����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ֹ���
	//	TThostFtdcPriceType	StopPrice;
	req.StopPrice =0;
	///ǿƽԭ��: ��ǿƽ
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose; //THOST_FTDC_FCC_ClientOverPositionLimit; //
	///�Զ������־: ��
	req.IsAutoSuspend = 0;

	///�û�ǿ����־: ��
	req.UserForceClose = 0;

	if( pos.YdShort > 0 )
	{
		///��������
		strcpy(req.OrderRef, ORDER_REF);
		///ҵ��Ԫ
		//	TThostFtdcBusinessUnitType	BusinessUnit;
		///������
		//	TThostFtdcRequestIDType	RequestID;
		req.RequestID = m_requestID;

		req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		///����: 
		req.VolumeTotalOriginal = pos.YdShort;
		this->ReqOrderInsert(req);
	}

	if(pos.Short >0)
	{
		m_AlgoPos += pos.Short;  
		///��������
		strcpy(req.OrderRef, ORDER_REF);
		///ҵ��Ԫ
		//	TThostFtdcBusinessUnitType	BusinessUnit;
		///������
		//	TThostFtdcRequestIDType	RequestID;
		req.RequestID = m_requestID;

		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		///����: 
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
