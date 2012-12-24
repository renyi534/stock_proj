// VarOneMinuteSeriesGenerator.h: interface for the VarOneMinuteSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARONEMINUTESERIESGENERATOR_H__F9CF189B_9D93_4105_AC18_327A1236546E__INCLUDED_)
#define AFX_VARONEMINUTESERIESGENERATOR_H__F9CF189B_9D93_4105_AC18_327A1236546E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OneMinuteSeriesGenerator.h"

class VarOneMinuteSeriesGenerator : public OneMinuteSeriesGenerator  
{
public:
	VarOneMinuteSeriesGenerator(int delimiter);
	virtual ~VarOneMinuteSeriesGenerator();
	virtual void InputTickData(const CThostFtdcDepthMarketDataField& tick);
protected:
	const int m_delimiter;
};

#endif // !defined(AFX_VARONEMINUTESERIESGENERATOR_H__F9CF189B_9D93_4105_AC18_327A1236546E__INCLUDED_)
