// KSeriesGenerator.h: interface for the KSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KSERIESGENERATOR_H__57B090C5_A72C_4C8A_A6A7_6FBD7062EF42__INCLUDED_)
#define AFX_KSERIESGENERATOR_H__57B090C5_A72C_4C8A_A6A7_6FBD7062EF42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "stdafx.h"
#include "OneMinuteData.h"
#include "MessageRouter.h"
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include "TradeHandlingThread.h"

extern CThreadPool gThreadPool;
class KSeriesGenerator  
{
public:
	virtual void InputTickData(const CThostFtdcDepthMarketDataField& tick)=0;
	KSeriesGenerator(string name);
	virtual ~KSeriesGenerator();

private:

protected:
	ofstream m_log;
	string m_name;
};

#endif // !defined(AFX_KSERIESGENERATOR_H__57B090C5_A72C_4C8A_A6A7_6FBD7062EF42__INCLUDED_)
