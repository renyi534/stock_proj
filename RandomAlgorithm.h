// RandomAlgorithm.h: interface for the RandomAlgorithm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANDOMALGORITHM_H__2EA7E4BA_C2CD_4FDD_978B_C3B4E42FEF93__INCLUDED_)
#define AFX_RANDOMALGORITHM_H__2EA7E4BA_C2CD_4FDD_978B_C3B4E42FEF93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Algorithm.h"
class COneMinuteData;
class RandomAlgorithm : public Algorithm  
{
public:
	RandomAlgorithm(string instrument_id);
	virtual ~RandomAlgorithm();
    virtual void OnMinuteData(const CMinuteData& data);
	virtual void OnHalfMinuteData(const CHalfMinuteData& data);
    virtual void OnTickData(const CThostFtdcDepthMarketDataField&);
    virtual void OnTradeData(const CThostFtdcTradeField&);
    virtual void OnAccountData(const CThostFtdcTradingAccountField&);
    virtual void OnPositionData(const CThostFtdcInvestorPositionField&);
	virtual BOOL InitInstance();
	virtual int	SendStrategy(const OrderInfoShort & res);
private:
	double totalAmount;
	double totalMoney;
	vector<CHalfMinuteData> m_historyData;
	string m_InstrumentID;
	int m_Amount;
	ofstream m_log;
	double m_AskPrice;
	double m_BidPrice;
};

#endif // !defined(AFX_RANDOMALGORITHM_H__2EA7E4BA_C2CD_4FDD_978B_C3B4E42FEF93__INCLUDED_)
