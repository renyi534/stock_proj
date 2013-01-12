// VarHalfMinuteSeriesGenerator.h: interface for the VarHalfMinuteSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARHALFMINUTESERIESGENERATOR_H__4CFF53E0_FF22_4519_B1F7_8C911656C78D__INCLUDED_)
#define AFX_VARHALFMINUTESERIESGENERATOR_H__4CFF53E0_FF22_4519_B1F7_8C911656C78D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HalfMinuteSeriesGenerator.h"

class VarHalfMinuteSeriesGenerator : public HalfMinuteSeriesGenerator  
{
public:
	VarHalfMinuteSeriesGenerator(int delimiter);
	virtual ~VarHalfMinuteSeriesGenerator();
	virtual void InputTickData(const CThostFtdcDepthMarketDataField& tick);
protected:
	const int m_delimiter;

};

#endif // !defined(AFX_VARHALFMINUTESERIESGENERATOR_H__4CFF53E0_FF22_4519_B1F7_8C911656C78D__INCLUDED_)
