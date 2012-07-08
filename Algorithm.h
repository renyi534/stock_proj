// Algorithm.h: interface for the Algorithm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALGORITHM_H__F1FFFDD3_5236_4857_B14B_1F689122F5FD__INCLUDED_)
#define AFX_ALGORITHM_H__F1FFFDD3_5236_4857_B14B_1F689122F5FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include "DbAccessorPool.h"
#include "DbAccessor.h"
#include "TradeConn.h"
#include "RunObject.h"
#include "stdafx.h"
#include "OneMinuteData.h"
#include <set>
using namespace std;
struct OrderInfo 
{
	string m_instrumentID;//股票代码
	int	   amount;
	int    is_buy;
	int    is_open;
	double price;
	string day;
	string time;
	int	   milliSec;
};

struct OrderInfoShort 
{
	string m_instrumentID;//股票代码
	int	   amount;
	int    totalAmount;
	double price;
	string day;
	string time;
	int	   milliSec;
};

class Algorithm  : public CWinThread
{
public:	
	Algorithm();
    Algorithm(string configFile);
    virtual BOOL InitInstance();
    virtual void OnMinuteData(const CMinuteData& data) =0;
	virtual void OnHalfMinuteData(const CHalfMinuteData& data) =0;
    virtual void OnTickData(const CThostFtdcDepthMarketDataField&) =0;
    virtual void OnTradeData(const CThostFtdcTradeField&) =0;
    virtual void OnAccountData(const CThostFtdcTradingAccountField&) =0;
    virtual void OnPositionData(const CThostFtdcInvestorPositionField&) =0;
	virtual int	 SendStrategy(const OrderInfo &);
	virtual int	 SendStrategy(const OrderInfoShort &);
	virtual int  Run();
	virtual ~Algorithm();
	void RegisterInstrument(string instrument);
	bool IsInterestingInstrument(string instrument);
protected:
    string  m_configFile;
	set<string>  m_Instruments;
private:
    void HandleNewMarketData(MSG& msg);
    void UpdateTradeInfo(MSG&);
    void UpdatePositionInfo(MSG&);
    void UpdateAccountInfo(MSG&);
};

#endif // !defined(AFX_ALGORITHM_H__F1FFFDD3_5236_4857_B14B_1F689122F5FD__INCLUDED_)
