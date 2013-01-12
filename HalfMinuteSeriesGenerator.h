// HalfMinuteSeriesGenerator.h: interface for the HalfMinuteSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HALFMINUTESERIESGENERATOR_H__1A2ECD5E_67D7_416D_AD8C_C00CD4C07AE0__INCLUDED_)
#define AFX_HALFMINUTESERIESGENERATOR_H__1A2ECD5E_67D7_416D_AD8C_C00CD4C07AE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KSeriesGenerator.h"

class HalfMinuteSeriesGenerator : public KSeriesGenerator  
{
public:
	HalfMinuteSeriesGenerator(string name="HalfMinuteSeriesGenerator");
	virtual ~HalfMinuteSeriesGenerator();
	virtual void InputTickData(const CThostFtdcDepthMarketDataField& tick);

protected:
	typedef map<string,CHalfMinuteData> CHalfMinuteDataMap;	
	typedef pair<string,CHalfMinuteData> CHalfMinuteDataPair;	
	CHalfMinuteDataMap m_half_minute_data_map;
	CHalfMinuteDataMap m_prev_half_minute_data_map;
};

#endif // !defined(AFX_HALFMINUTESERIESGENERATOR_H__1A2ECD5E_67D7_416D_AD8C_C00CD4C07AE0__INCLUDED_)
