// StockRetriever.h: interface for the StockRetriever class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCKRETRIEVER_H__6A4B2CFC_D9B0_49B6_BC47_D5E555C60E00__INCLUDED_)
#define AFX_STOCKRETRIEVER_H__6A4B2CFC_D9B0_49B6_BC47_D5E555C60E00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxinet.h>
#include "THOSTTRADERAPI\ThostFtdcUserApiStruct.h"	// Added by ClassView


class StockRetriever  
{
public:
	void GetStockData(CString url, CThostFtdcDepthMarketDataField& data);
	StockRetriever();
	virtual ~StockRetriever();
	CString  GetStockInfo(CString url);
private:
	CString   convertTime(CString str);

};

#endif // !defined(AFX_STOCKRETRIEVER_H__6A4B2CFC_D9B0_49B6_BC47_D5E555C60E00__INCLUDED_)
