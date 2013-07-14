// CompositeAlgorithm.h: interface for the CompositeAlgorithm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPOSITEALGORITHM_H__274107D6_4B23_435F_92BE_D82967399E32__INCLUDED_)
#define AFX_COMPOSITEALGORITHM_H__274107D6_4B23_435F_92BE_D82967399E32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Algorithm.h"
#include <vector>
#include <string>
using namespace std;

class CompositeAlgorithm : public Algorithm  
{
public:
	CompositeAlgorithm(string name, string instrument);
	virtual ~CompositeAlgorithm();
	virtual BOOL InitInstance();
    virtual int OnMinuteData(const CMinuteData& data);
    virtual int OnTenMinuteData(const CTenMinuteData& data);
	virtual int OnHalfMinuteData(const CHalfMinuteData& data);
    virtual int OnTickData(const CThostFtdcDepthMarketDataField&);
    virtual void OnTradeData(const CThostFtdcTradeField&);
    virtual void OnAccountData(const CThostFtdcTradingAccountField&);
    virtual void OnPositionData(const CThostFtdcInvestorPositionField&);
	virtual bool AddAlgorithm(Algorithm * algo);
	virtual void SetSlot(int slot);
	virtual void SetAccountInfo(string broker, string investor);
private:
	vector<Algorithm*> m_AlgoList;
	string m_Instrument;
	string m_Name;
};

#endif // !defined(AFX_COMPOSITEALGORITHM_H__274107D6_4B23_435F_92BE_D82967399E32__INCLUDED_)
