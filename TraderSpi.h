#pragma once
#include "stdafx.h"
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include <map>
#include <vector>
using namespace std;
struct OrderInfo;
struct OrderInfoShort;
#include <iostream>
#include <fstream>
using namespace std;
struct TradeConn;

	struct InvestorPosition
	{
		string Instrument;
		int  Net;
		int  Long;
		int  Short;
		int  YdNet;
		int  YdLong;
		int  YdShort;
		double LongPositionCost;
		double ShortPositionCost;
		double LongPositionProfit;
		double ShortPositionProfit;
		double LongUseMargin;
		double ShortUseMargin;
		double YdLongPositionCost;
		double YdShortPositionCost;
		double YdLongPositionProfit;
		double YdShortPositionProfit;
		double YdLongUseMargin;
		double YdShortUseMargin;
	};
class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	CTraderSpi(CThostFtdcTraderApi* api, string broker_id, string investor_id, 
		string passwd, TradeConn* conn);
	CTraderSpi::~CTraderSpi();
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason);
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	virtual void OnHeartBeatWarning(int nTimeLapse);
	
	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

public:
	void ShowTradeDetail();
	void CancelAllOrders(string instrument);
	void ClearShortPos(string instrument, double price);
	void ClearLongPos(string instrument, double price);
	///用户登录请求
	void ReqUserLogin();
	///投资者结算结果确认
	void ReqSettlementInfoConfirm();
	///请求查询合约
	void ReqQryInstrument();
	void ReqQryInstrument(string instrument);
	///请求查询资金账户
	void ReqQryTradingAccount();
	///请求查询投资者持仓
	void ReqQryInvestorPosition();
	void ReqQryInvestorPosition(string instrument);
	///报单录入请求
	void ReqOrderInsert(CThostFtdcInputOrderField& req);
	void ReqOrderInsert(OrderInfo& req);
	void ReqOrderInsert(OrderInfoShort& order_req, bool isCorrection=false)	;
	///报单操作请求
	void ReqOrderAction(CThostFtdcOrderField *pOrder);

	// 是否收到成功的响应
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// 是否我的报单回报
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// 是否正在交易的报单
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);

	inline bool GetConnStatus()
	{
		return m_ConnStatus;
	}
private:
	CString GetOrderPriceType(TThostFtdcOrderPriceTypeType type);
	CString GetTradeType(TThostFtdcTradeTypeType type);
	CString GetTradeFlag(TThostFtdcOffsetFlagType flag);

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
    typedef map<string,int> instrument_pos_map;	
    typedef pair<string,int> instrument_pos_pair;			    
    instrument_pos_map m_algo_instrument_pos;

    typedef map<string,CThostFtdcOrderField> order_state_map;	
    typedef pair<string,CThostFtdcOrderField> order_state_pair;			    
    order_state_map m_order_state;

	vector<CThostFtdcTradeField> m_TradeList;

	string m_BrokerId;
	string m_InvestorId;
	string m_Passwd;
	TThostFtdcFrontIDType	m_FrontId;	//前置编号
	TThostFtdcSessionIDType	m_SessionId;	//会话编号
	TThostFtdcOrderRefType	m_OrderRef;	//报单引用

	TradeConn* m_Conn;
	bool m_ConnStatus;
	friend class CTradeSystemView;
};
