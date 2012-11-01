// TenMinuteSeriesGenerator.h: interface for the TenMinuteSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TENMINUTESERIESGENERATOR_H__097650AB_0584_401D_A11E_2CD9C4E2BFAB__INCLUDED_)
#define AFX_TENMINUTESERIESGENERATOR_H__097650AB_0584_401D_A11E_2CD9C4E2BFAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KSeriesGenerator.h"

class TenMinuteSeriesGenerator : public KSeriesGenerator  
{
public:
	TenMinuteSeriesGenerator();
	virtual ~TenMinuteSeriesGenerator();
	virtual void InputTickData(const CThostFtdcDepthMarketDataField& tick);
private:
	void resetTenMinuteData(CTenMinuteData& m_ten_minute_data, const string& instrument_id);
	typedef map<string,CTenMinuteData> CTenMinuteDataMap;	
	typedef pair<string,CTenMinuteData> CTenMinuteDataPair;	
	CTenMinuteDataMap m_ten_minute_data_map;
	CTenMinuteDataMap m_prev_ten_minute_data_map;
};

#endif // !defined(AFX_TENMINUTESERIESGENERATOR_H__097650AB_0584_401D_A11E_2CD9C4E2BFAB__INCLUDED_)
