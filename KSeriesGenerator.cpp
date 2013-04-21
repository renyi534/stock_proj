// KSeriesGenerator.cpp: implementation of the KSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tradesystem.h"
#include "KSeriesGenerator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KSeriesGenerator::KSeriesGenerator(string name, MessageRouter* router,
								   bool StoreMarketData):
	m_log((string(".\\")+name+string(".txt")).c_str(),ios::app), 
	m_Router(router), m_name(name), m_StoreMarketData(StoreMarketData)
{

}

KSeriesGenerator::~KSeriesGenerator()
{
	m_log.close();
}



