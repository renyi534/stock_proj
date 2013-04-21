// OneMinuteSeriesGenerator.h: interface for the OneMinuteSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONEMINUTESERIESGENERATOR_H__510C949D_9193_478E_B787_06FC4AB35FDD__INCLUDED_)
#define AFX_ONEMINUTESERIESGENERATOR_H__510C949D_9193_478E_B787_06FC4AB35FDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KSeriesGenerator.h"
class MessageRouter;
class OneMinuteSeriesGenerator : public KSeriesGenerator  
{
public:
	OneMinuteSeriesGenerator();
	OneMinuteSeriesGenerator(MessageRouter* router, bool StoreMarketData=false,string name="OneMinuteSeriesGenerator");
	virtual ~OneMinuteSeriesGenerator();
	virtual void InputTickData(const CThostFtdcDepthMarketDataField& tick);
protected:
	void resetOneMinuteData(CMinuteData& m_one_minute_data, const string& instrument_id);
	typedef map<string,CMinuteData> CMinuteDataMap;	
	typedef pair<string,CMinuteData> CMinuteDataPair;	
	CMinuteDataMap m_one_minute_data_map;
	CMinuteDataMap m_prev_one_minute_data_map;
};

#endif // !defined(AFX_ONEMINUTESERIESGENERATOR_H__510C949D_9193_478E_B787_06FC4AB35FDD__INCLUDED_)
