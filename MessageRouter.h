// MessageRouter.h: interface for the MessageRouter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGEROUTER_H__E15C451A_729C_41C4_90A2_4E14A8C12B2B__INCLUDED_)
#define AFX_MESSAGEROUTER_H__E15C451A_729C_41C4_90A2_4E14A8C12B2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Algorithm.h"

class MessageRouter  
{
public:
	virtual ~MessageRouter();
	void sendData(const CMinuteData& data);
	void sendData(const CHalfMinuteData& data);
	void sendData(const CThostFtdcDepthMarketDataField& data);
	void sendData(const CThostFtdcTradeField& data);
	void sendData(const CThostFtdcTradingAccountField& data);
	void sendData(const CThostFtdcInvestorPositionField& data);
	static MessageRouter Router;
	void InitAlgorithm();
private:
	MessageRouter();
	vector<Algorithm*> m_algorithms;
};

#endif // !defined(AFX_MESSAGEROUTER_H__E15C451A_729C_41C4_90A2_4E14A8C12B2B__INCLUDED_)
