// HsAlgorithm.h: interface for the HsAlgorithm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANDOMALGORITHM_H__2EA7E4BA_C2CD_4FDD_978B_C3B4E42FEF94__INCLUDED_)
#define AFX_RANDOMALGORITHM_H__2EA7E4BA_C2CD_4FDD_978B_C3B4E42FEF94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Algorithm.h"
class COneMinuteData;
class HsAlgorithm : public Algorithm  
{
public:
	HsAlgorithm(string instrument_id);
	virtual ~HsAlgorithm();
    virtual void OnMinuteData(const CMinuteData& data);
	virtual void OnHalfMinuteData(const CHalfMinuteData& data);
    virtual void OnTickData(const CThostFtdcDepthMarketDataField&);
    virtual void OnTradeData(const CThostFtdcTradeField&);
    virtual void OnAccountData(const CThostFtdcTradingAccountField&);
    virtual void OnPositionData(const CThostFtdcInvestorPositionField&);
	virtual BOOL InitInstance();
	virtual int	SendStrategy( OrderInfoShort & res);
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
};

#endif // !defined(AFX_RANDOMALGORITHM_H__2EA7E4BA_C2CD_4FDD_978B_C3B4E42FEF94__INCLUDED_)
