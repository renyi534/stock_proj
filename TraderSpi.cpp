
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

extern char* ppInstrumentID[];	
extern int iInstrumentID;

extern DbAccessorPool dbAccessPool;
#include "resource.h"
#include "TradeSystemDoc.h"
#include "TradeSystemView.h"
// �Ự����


void CTraderSpi::OnFrontConnected()
{
	m_log << "--->>> " << "OnFrontConnected" << endl;
	///�û���¼����
	ReqUserLogin();
	m_ConnStatus = true;
	CString str;
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL && view->GetCurrConn() == m_Conn)
	{
		str.Format("���׷�����: %s", "ON");
		view->m_TradeStatus.SetWindowText(str);
	}
}

CTraderSpi::~CTraderSpi()
{
	m_log.close();
}

CTraderSpi::CTraderSpi(CThostFtdcTraderApi* api, string broker_id, 
					   string investor_id, string passwd, TradeConn* conn)
	:m_pTradeApi(api),m_requestID(0),m_log(".\\trade.log", ios::app),
	  m_BrokerId(broker_id), m_InvestorId(investor_id), m_Passwd(passwd),
	  m_Conn(conn)
{
	ASSERT(m_Conn!=NULL);
	m_TradeCount =0;
	m_OrderCount =0;

	memset(&m_account, 0 ,sizeof(m_account));
	// stl library can be buggy with empty maps. Insert some rubbish data here.
	m_inv_pos.insert(inv_pos_pair("", InvestorPosition() ));
	m_order_state.insert(order_state_pair("", CThostFtdcOrderField() ));
	m_algo_instrument_pos.insert(instrument_pos_pair("",0));
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_BrokerId.c_str());
	strcpy(req.UserID, m_InvestorId.c_str());
	strcpy(req.Password, m_Passwd.c_str());
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
		m_FrontId = pRspUserLogin->FrontID;
		m_SessionId = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(m_OrderRef, "%d", iNextOrderRef);
		///��ȡ��ǰ������
		m_log << "--->>> ��ȡ��ǰ������ = " << m_pTradeApi->GetTradingDay() << endl;
		///Ͷ���߽�����ȷ��
		ReqSettlementInfoConfirm();
	}
	else if (bIsLast && IsErrorRspInfo(pRspInfo))
	{
		Sleep(1000);
		ReqUserLogin();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_BrokerId.c_str());
	strcpy(req.InvestorID, m_InvestorId.c_str());
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
	strcpy(req.BrokerID, m_BrokerId.c_str());
	strcpy(req.InvestorID, m_InvestorId.c_str());
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
	m_Conn->m_Router.sendData(*pTradingAccount);

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
	strcpy(req.BrokerID, m_BrokerId.c_str());
	strcpy(req.InvestorID, m_InvestorId.c_str());
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
		pos.Instrument = pInvestorPosition->InstrumentID;
		if ( m_inv_pos.find(key) != m_inv_pos.end() )
		{
			pos = m_inv_pos[key];
		}		
		//static bool is_of_today_state = false;

		if (pInvestorPosition->PositionDate == THOST_FTDC_PSD_Today)
		{
			/*if( is_of_today_state == false )
			{
				is_of_today_state = true;
				pos.YdLong = pos.YdShort =0;
			}*/

			if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Net)
			{
				pos.Net = pInvestorPosition->TodayPosition;
				pos.YdNet = pInvestorPosition->Position-pInvestorPosition->TodayPosition;
				
			}
			else if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Long)
			{
				pos.Long = pInvestorPosition->TodayPosition;
				//pos.YdLong = pInvestorPosition->Position-pInvestorPosition->TodayPosition;
				pos.LongPositionCost = pInvestorPosition->PositionCost;
				pos.LongPositionProfit = pInvestorPosition->PositionProfit;
				pos.LongUseMargin = pInvestorPosition->UseMargin;
			}
			else
			{
				pos.Short = pInvestorPosition->TodayPosition;
				//pos.YdShort = pInvestorPosition->Position-pInvestorPosition->TodayPosition;
				pos.ShortPositionCost = pInvestorPosition->PositionCost;
				pos.ShortPositionProfit = pInvestorPosition->PositionProfit;
				pos.ShortUseMargin = pInvestorPosition->UseMargin;
			}
		}
		else //if( !is_of_today_state )
		{
			if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Net)
			{
				pos.YdNet = pInvestorPosition->YdPosition;
			}
			else if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Long)
			{
				pos.YdLong = pInvestorPosition->YdPosition-pInvestorPosition->CloseVolume;
				pos.YdLongPositionCost = pInvestorPosition->PositionCost;
				pos.YdLongPositionProfit = pInvestorPosition->PositionProfit;
				pos.YdLongUseMargin = pInvestorPosition->UseMargin;
			}
			else
			{
				pos.YdShort = pInvestorPosition->YdPosition-pInvestorPosition->CloseVolume;
				pos.YdShortPositionCost = pInvestorPosition->PositionCost;
				pos.YdShortPositionProfit = pInvestorPosition->PositionProfit;
				pos.YdShortUseMargin = pInvestorPosition->UseMargin;
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
		m_Conn->m_Router.sendData(*pInvestorPosition);
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
strcpy(req.BrokerID, m_BrokerId.c_str());
///Ͷ���ߴ���
strcpy(req.InvestorID, m_InvestorId.c_str());
///��Լ����
strcpy(req.InstrumentID, INSTRUMENT_ID);
///��������
strcpy(req.OrderRef, m_OrderRef);
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
    int iNextOrderRef = atoi(m_OrderRef);
    iNextOrderRef++;
    sprintf(m_OrderRef, "%d", iNextOrderRef);    
}

void CTraderSpi::ReqOrderInsert(OrderInfo& order_req)
{
	if ( order_req.amount ==0 )
		return;
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, m_BrokerId.c_str());
	///Ͷ���ߴ���
	strcpy(req.InvestorID, m_InvestorId.c_str());
	///��Լ����
	strcpy(req.InstrumentID, order_req.m_instrumentID.c_str());
	///��������
	strcpy(req.OrderRef, m_OrderRef);
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	strcpy(req.UserID, m_InvestorId.c_str());
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
	if (order_req.price<0) {
		req.LimitPrice = 0;
	}
	else
	{
		req.LimitPrice = order_req.price;
	}

	///����: 1
	req.VolumeTotalOriginal = fabs(order_req.amount);
	///��Ч������: ������Ч
	if (order_req.price<0) {
		req.TimeCondition = THOST_FTDC_TC_IOC;
	}
	else
	{
		req.TimeCondition = THOST_FTDC_TC_GFD;
	}
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
    int iNextOrderRef = atoi(m_OrderRef);
    iNextOrderRef++;
    sprintf(m_OrderRef, "%d", iNextOrderRef);    		
}

void CTraderSpi::ReqOrderInsert(OrderInfoShort& order_req, bool isCorrection)
{
	InvestorPosition pos;
	string key = order_req.m_instrumentID;

	if( !isCorrection )
	{
		if (m_algo_instrument_pos.find(key) != m_algo_instrument_pos.end())
		{
			int new_pos = m_algo_instrument_pos[key]+order_req.amount;
			m_algo_instrument_pos[key] = new_pos;
		}
		else
		{
			m_algo_instrument_pos.insert(instrument_pos_pair(key,order_req.amount));
		}
	}

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

            strcpy(req.OrderRef, m_OrderRef);
            
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
	req.FrontID = m_FrontId;
	///�Ự���
	req.SessionID = m_SessionId;
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
	else if (strlen(pOrder->OrderSysID) == 0)
	{
		return;
	}
	CString str;
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL && view->GetCurrConn()==this->m_Conn)
	{
		LVFINDINFO info;
		int n;
		
		info.flags = LVFI_PARTIAL|LVFI_STRING;
		info.psz = pOrder->OrderSysID;
				
		n = view->m_OrderTradeDlg.m_OrderList.FindItem(&info);
		if( n == -1)
		{
			n = view->m_OrderTradeDlg.m_OrderList.GetItemCount();
			view->m_OrderTradeDlg.m_OrderList.InsertItem(n, pOrder->OrderSysID);
		}

		view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 1, pOrder->InstrumentID);

		view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 2, 
			pOrder->Direction == THOST_FTDC_D_Sell? "    ��" : "��");

		str = GetTradeFlag(pOrder->CombOffsetFlag[0]);
		view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 3, str);	

		str.Format("%u", pOrder->VolumeTotalOriginal);
		view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 4, str);	

		str.Format("%u", pOrder->VolumeTotal);
		view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 5, str);	

		str.Format("%u", pOrder->VolumeTraded);
		view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 6, str);

		str.Format("%.2f", pOrder->LimitPrice);
		view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 7, str);	

		view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 8, pOrder->InsertTime);	

		str = GetOrderPriceType(pOrder->OrderPriceType);
		view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 9, str);
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
	order_req.broker_id = m_BrokerId;
	order_req.investor_id = m_InvestorId;
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

	m_Conn->m_Router.sendData(*pTrade);
	//DbConn conn(dbAccessPool);
	char* buffer= new char[2048];
	string insert_sql = "insert into stock_data.trade_details(broker_id, "
		"investor_id, instrument_id, "
		"trans_day, trans_time, price, amount, is_buy, is_open) "
		"values('%s','%s','%s',%.2f,%d,%d,%d)";
	
    int amount = pTrade->Volume;
	
	int is_buy = 1;
    if ( pTrade->Direction == THOST_FTDC_D_Sell )
        is_buy =0 ;
	
	int is_open = 1;
	
	if (pTrade->OffsetFlag != THOST_FTDC_OF_Open )
		is_open =0;
	
	sprintf(buffer, insert_sql.c_str(), 
		m_BrokerId.c_str(),
		m_InvestorId.c_str(),
        pTrade->InstrumentID,
        pTrade->TradeDate,
        pTrade->TradeTime,
        pTrade->Price,
        amount,
		is_buy,
		is_open);

	CString str;
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL && view->GetCurrConn() == m_Conn)
	{
		int n = view->m_OrderTradeDlg.m_TradeList.GetItemCount();
		view->m_OrderTradeDlg.m_TradeList.InsertItem(n, pTrade->TradeID);
		view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 1, pTrade->OrderSysID);
		view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 2, pTrade->InstrumentID);	
		view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 3, 
			pTrade->Direction == THOST_FTDC_D_Sell? "    ��" : "��");
		str = GetTradeFlag(pTrade->OffsetFlag);
		view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 4, str);	
		str.Format("%.2f", pTrade->Price);
		view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 5, str);	
		str.Format("%u", pTrade->Volume);
		view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 6, str);	
		view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 7, pTrade->TradeTime);	
		str = GetTradeType( pTrade->TradeType );
		view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 8, str);	
	}
	
	gThreadPool.Run(ExecSQL, (void*) buffer);
	//conn.m_db->execSql(buffer);
	
    ReqQryTradingAccount();
}


void CTraderSpi::StoreOrder(const OrderInfo& initialData, bool isRej)
{
	//DbConn conn(dbAccessPool);
	char* buffer= new char[2048];
	string insert_sql = "insert into stock_data.order_details(broker_id, "
		"investor_id, instrument_id, trans_day, trans_time, price, amount,is_buy,is_open) "
		"values('%s','%s','%s','%s','%s',%.2f,%d,%d,%d)";

	if (isRej)
	{
		insert_sql+= " Rejected Order";
	}


	sprintf(buffer, insert_sql.c_str(), 
		m_BrokerId.c_str(),
		m_InvestorId.c_str(),
		initialData.m_instrumentID.c_str(),
		initialData.day.c_str(),
		initialData.time.c_str(),
		initialData.price,
		initialData.amount,
		initialData.is_buy,
		initialData.is_open
        );
	
	//conn.m_db->execSql(buffer);
	
	if (!isRej)
		gThreadPool.Run(ExecSQL, (void*) buffer);
	
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
	m_log << "--->>> " << "OnFrontDisconnected" << endl;
	m_log << "--->>> Reason = " << nReason << endl;
	
	m_ConnStatus = false;
	CString str;
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL && view->GetCurrConn()==m_Conn)
	{
		str.Format("���׷�����: %s", "OFF");
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
	if (pOrder==NULL)
		return false;

	return ((pOrder->FrontID == m_FrontId) &&
		(pOrder->SessionID == m_SessionId) );
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	if (pOrder==NULL)
		return false;

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
	strcpy(req.BrokerID, m_BrokerId.c_str());
	///Ͷ���ߴ���
	strcpy(req.InvestorID, m_InvestorId.c_str());
	///��Լ����
	strcpy(req.InstrumentID, instrument.c_str());

	///�û�����
	//	TThostFtdcUserIDType	UserID;
	strcpy(req.UserID, m_InvestorId.c_str());
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
	if (price<0) {
		req.LimitPrice = 0;
	}
	else
	{
		req.LimitPrice = price;
	}


	///��Ч������: ������Ч
	if (price<0) {
		req.TimeCondition = THOST_FTDC_TC_IOC;
	}
	else
	{
		req.TimeCondition = THOST_FTDC_TC_GFD;
	}
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
		strcpy(req.OrderRef, m_OrderRef);
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
		if (m_algo_instrument_pos.find(instrument) != m_algo_instrument_pos.end())
		{
			int new_pos = m_algo_instrument_pos[instrument]-pos.Long;
			m_algo_instrument_pos[instrument] = new_pos;
		}
		else
		{
			m_algo_instrument_pos.insert(instrument_pos_pair(instrument,-pos.Long));
		}
		///��������
		strcpy(req.OrderRef, m_OrderRef);
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
	strcpy(req.BrokerID, m_BrokerId.c_str());
	///Ͷ���ߴ���
	strcpy(req.InvestorID, m_InvestorId.c_str());
	///��Լ����
	strcpy(req.InstrumentID, instrument.c_str());

	///�û�����
	//	TThostFtdcUserIDType	UserID;
	strcpy(req.UserID, m_InvestorId.c_str());
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
	if (price<0) {
		req.LimitPrice = 0;
	}
	else
	{
		req.LimitPrice = price;
	}


	///��Ч������: ������Ч
	if (price<0) {
		req.TimeCondition = THOST_FTDC_TC_IOC;
	}
	else
	{
		req.TimeCondition = THOST_FTDC_TC_GFD;
	}
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
		strcpy(req.OrderRef, m_OrderRef);
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
		if (m_algo_instrument_pos.find(instrument) != m_algo_instrument_pos.end())
		{
			int new_pos = m_algo_instrument_pos[instrument]+pos.Short;
			m_algo_instrument_pos[instrument] = new_pos;
		}
		else
		{
			m_algo_instrument_pos.insert(instrument_pos_pair(instrument,pos.Short));
		}
		///��������
		strcpy(req.OrderRef, m_OrderRef);
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

CString CTraderSpi::GetTradeFlag(TThostFtdcOffsetFlagType flag)
{
/*
///����
#define THOST_FTDC_OF_Open '0'
///ƽ��
#define THOST_FTDC_OF_Close '1'
///ǿƽ
#define THOST_FTDC_OF_ForceClose '2'
///ƽ��
#define THOST_FTDC_OF_CloseToday '3'
///ƽ��
#define THOST_FTDC_OF_CloseYesterday '4'
*/
	CString ret="";
	switch(flag)
	{
	case THOST_FTDC_OF_Open:
		ret = "����";
		break;
	case THOST_FTDC_OF_Close:
		ret = "ƽ��";
		break;
	case THOST_FTDC_OF_ForceClose:
		ret = "ǿƽ";
		break;
	case THOST_FTDC_OF_CloseToday:
		ret = "ƽ��";
		break;
	case THOST_FTDC_OF_CloseYesterday:
		ret = "ƽ��";
		break;
	default:
		ASSERT(FALSE);
	}
	return ret;
}

CString CTraderSpi::GetTradeType(TThostFtdcTradeTypeType type)
{
/*
///��ͨ�ɽ�
#define THOST_FTDC_TRDT_Common '0'
///��Ȩִ��
#define THOST_FTDC_TRDT_OptionsExecution '1'
///OTC�ɽ�
#define THOST_FTDC_TRDT_OTC '2'
///��ת�������ɽ�
#define THOST_FTDC_TRDT_EFPDerived '3'
///��������ɽ�
#define THOST_FTDC_TRDT_CombinationDerived '4'

typedef char TThostFtdcTradeTypeType;
*/
	CString ret="";
	switch(type)
	{
	case THOST_FTDC_TRDT_Common:
		ret = "��ͨ�ɽ�";
		break;
	case THOST_FTDC_TRDT_OptionsExecution:
		ret = "��Ȩִ��";
		break;
	case THOST_FTDC_TRDT_OTC:
		ret = "OTC�ɽ�";
		break;
	case THOST_FTDC_TRDT_EFPDerived:
		ret = "��ת�������ɽ�";
		break;
	case THOST_FTDC_TRDT_CombinationDerived:
		ret = "��������ɽ�";
		break;
	default:
		ret = "��ͨ�ɽ�";
	}
	return ret;
}

CString CTraderSpi::GetOrderPriceType(TThostFtdcOrderPriceTypeType type)
{
/*
/////////////////////////////////////////////////////////////////////////
///TFtdcOrderPriceTypeType��һ�������۸���������
/////////////////////////////////////////////////////////////////////////
///�����
#define THOST_FTDC_OPT_AnyPrice '1'
///�޼�
#define THOST_FTDC_OPT_LimitPrice '2'
///���ż�
#define THOST_FTDC_OPT_BestPrice '3'
///���¼�
#define THOST_FTDC_OPT_LastPrice '4'
///���¼۸����ϸ�1��ticks
#define THOST_FTDC_OPT_LastPricePlusOneTicks '5'
///���¼۸����ϸ�2��ticks
#define THOST_FTDC_OPT_LastPricePlusTwoTicks '6'
///���¼۸����ϸ�3��ticks
#define THOST_FTDC_OPT_LastPricePlusThreeTicks '7'
///��һ��
#define THOST_FTDC_OPT_AskPrice1 '8'
///��һ�۸����ϸ�1��ticks
#define THOST_FTDC_OPT_AskPrice1PlusOneTicks '9'
///��һ�۸����ϸ�2��ticks
#define THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A'
///��һ�۸����ϸ�3��ticks
#define THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B'
///��һ��
#define THOST_FTDC_OPT_BidPrice1 'C'
///��һ�۸����ϸ�1��ticks
#define THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D'
///��һ�۸����ϸ�2��ticks
#define THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E'
///��һ�۸����ϸ�3��ticks
#define THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F'
*/
	CString ret="";
	switch(type)
	{
	case THOST_FTDC_OPT_AnyPrice:
		ret = "�����";
		break;
	case THOST_FTDC_OPT_LimitPrice:
		ret = "�޼�";
		break;
	case THOST_FTDC_OPT_BestPrice:
		ret = "���ż�";
		break;
	case THOST_FTDC_OPT_LastPrice:
		ret = "���¼�";
		break;
	default:
		ret = "��������";
	}
	return ret;
}

void CTraderSpi::ShowTradeDetail()
{
	CTradeSystemView* view = CTradeSystemView::GetCurrView();
	if( view != NULL && view->GetCurrConn() == m_Conn)
	{
		view->m_OrderTradeDlg.m_TradeList.DeleteAllItems();

		int item = m_TradeList.size();
		int n=0;
		for(n=0; n<item; n++)
		{
			CThostFtdcTradeField tradeInfo= m_TradeList[n];
			CThostFtdcTradeField *pTrade = &tradeInfo;
			view->m_OrderTradeDlg.m_TradeList.InsertItem(n, pTrade->TradeID);
			view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 1, pTrade->OrderSysID);
			view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 2, pTrade->InstrumentID);	
			view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 3, 
				pTrade->Direction == THOST_FTDC_D_Sell? "    ��" : "��");
			CString str = GetTradeFlag(pTrade->OffsetFlag);
			view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 4, str);	
			str.Format("%.2f", pTrade->Price);
			view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 5, str);	
			str.Format("%u", pTrade->Volume);
			view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 6, str);	
			view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 7, pTrade->TradeTime);	
			str = GetTradeType( pTrade->TradeType );
			view->m_OrderTradeDlg.m_TradeList.SetItemText(n, 8, str);
		}		

		n =0;
		view->m_OrderTradeDlg.m_OrderList.DeleteAllItems();
		for( order_state_map::iterator iter = m_order_state.begin();
			iter != m_order_state.end() ; iter++, n++)
		{
			//omit the padding one, which is used to avoid a empty map
			if (iter->first.length() == 0)
				continue;
			CThostFtdcOrderField* pOrder = &(iter->second);
			view->m_OrderTradeDlg.m_OrderList.InsertItem(n, pOrder->OrderSysID);
			
			view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 1, pOrder->InstrumentID);
			
			view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 2, 
				pOrder->Direction == THOST_FTDC_D_Sell? "    ��" : "��");
			
			CString str = GetTradeFlag(pOrder->CombOffsetFlag[0]);
			view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 3, str);	
			
			str.Format("%u", pOrder->VolumeTotalOriginal);
			view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 4, str);	
			
			str.Format("%u", pOrder->VolumeTotal);
			view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 5, str);	
			
			str.Format("%u", pOrder->VolumeTraded);
			view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 6, str);
			
			str.Format("%.2f", pOrder->LimitPrice);
			view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 7, str);	
			
			view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 8, pOrder->InsertTime);	
			
			str = GetOrderPriceType(pOrder->OrderPriceType);
			view->m_OrderTradeDlg.m_OrderList.SetItemText(n, 9, str);
		}
	}
}
