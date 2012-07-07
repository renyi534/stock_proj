#pragma once
#include "stdafx.h"
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include <map>
using namespace std;
struct OrderInfo;
struct OrderInfoShort;
#include <iostream>
#include <fstream>
using namespace std;

	struct InvestorPosition
	{
		int  Net;
		int	 Long;
		int  Short;
		int  YdNet;
		int  YdLong;
		int  YdShort;
	};
class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	CTraderSpi(CThostFtdcTraderApi* api):m_pTradeApi(api),m_requestID(0),m_log("c:\\trade.log", ios::app)
	{
		m_TradeCount =0;
		m_OrderCount =0;
		m_AlgoPos =0;
		memset(&m_account, 0 ,sizeof(m_account));
	}
	CTraderSpi::~CTraderSpi();
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������������Ӧ
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	virtual void OnHeartBeatWarning(int nTimeLapse);
	
	///����֪ͨ
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

public:
	void CancelAllOrders(string instrument);
	void ClearShortPos(string instrument, double price);
	void ClearLongPos(string instrument, double price);
	///�û���¼����
	void ReqUserLogin();
	///Ͷ���߽�����ȷ��
	void ReqSettlementInfoConfirm();
	///�����ѯ��Լ
	void ReqQryInstrument();
	void ReqQryInstrument(string instrument);
	///�����ѯ�ʽ��˻�
	void ReqQryTradingAccount();
	///�����ѯͶ���ֲ߳�
	void ReqQryInvestorPosition();
	void ReqQryInvestorPosition(string instrument);
	///����¼������
	void ReqOrderInsert(CThostFtdcInputOrderField& req);
	void ReqOrderInsert(OrderInfo& req);
	void ReqOrderInsert(OrderInfoShort& order_req, bool isCorrection=false)	;
	///������������
	void ReqOrderAction(CThostFtdcOrderField *pOrder);

	// �Ƿ��յ��ɹ�����Ӧ
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// �Ƿ��ҵı����ر�
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// �Ƿ����ڽ��׵ı���
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);
private:

	void StoreOrder(const OrderInfo& initialData, bool isRej=false);
	CThostFtdcTraderApi* m_pTradeApi;
	int  m_requestID;
    typedef map<string,InvestorPosition> inv_pos_map;	
    typedef pair<string,InvestorPosition> inv_pos_pair;			    
    inv_pos_map m_inv_pos;

    CThostFtdcTradingAccountField  m_account;

	ofstream m_log;
	int m_TradeCount;
	int m_OrderCount;
	int m_AlgoPos;

    typedef map<string,CThostFtdcOrderField> order_state_map;	
    typedef pair<string,CThostFtdcOrderField> order_state_pair;			    
    order_state_map m_order_state;
	friend class CTradeSystemView;
};
