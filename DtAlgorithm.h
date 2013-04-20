// DtAlgorithm.h: interface for the DtAlgorithm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DTALGORITHM_H__92A427DD_79CF_425A_9AE0_8D95FAEB09AC__INCLUDED_)
#define AFX_DTALGORITHM_H__92A427DD_79CF_425A_9AE0_8D95FAEB09AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Algorithm.h"


class DtAlgorithm : public Algorithm  
{
public:
	DtAlgorithm(string instrument_id);
	virtual ~DtAlgorithm();
    virtual void OnMinuteData(const CMinuteData& data);
	virtual void OnHalfMinuteData(const CHalfMinuteData& data);
    virtual void OnTickData(const CThostFtdcDepthMarketDataField&);
    virtual void OnTradeData(const CThostFtdcTradeField&);
    virtual void OnAccountData(const CThostFtdcTradingAccountField&);
    virtual void OnPositionData(const CThostFtdcInvestorPositionField&);
	virtual BOOL InitInstance();
	virtual int	SendStrategy(OrderInfoShort & res);
private:
	double totalAmount;
	double totalMoney;
	double lastVol;
	double m_BidPrice;
	double m_AskPrice;
	vector<CMinuteData> m_historyData;
	string m_InstrumentID;
	int m_Amount;
	ofstream m_log;
	ofstream m_state_log;
	int isIni;
	int ininow;
};

#endif // !defined(AFX_DTALGORITHM_H__92A427DD_79CF_425A_9AE0_8D95FAEB09AC__INCLUDED_)
