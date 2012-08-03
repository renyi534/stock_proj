// OneMinuteData.h: interface for the CMinuteData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONEMINUTEDATA_H__A9E8A124_E74F_40CE_A335_CA7F9011B934__INCLUDED_)
#define AFX_ONEMINUTEDATA_H__A9E8A124_E74F_40CE_A335_CA7F9011B934__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

class CMinuteData  
{
public:
	string m_Day;
	string m_Time;
	string m_InstrumentID;
	double m_OpenPrice;
	double m_ClosePrice;
	double m_HighPrice;
	double m_LowPrice;
	double m_Volume;
	double m_OpenInterest;
};


class CHalfMinuteData  
{
public:
	string m_Day;
	string m_Time;
	string m_Sec;
	string m_InstrumentID;
	double m_OpenPrice;
	double m_ClosePrice;
	double m_HighPrice;
	double m_LowPrice;
	double m_Volume;
	double m_OpenInterest;
};

#endif // !defined(AFX_ONEMINUTEDATA_H__A9E8A124_E74F_40CE_A335_CA7F9011B934__INCLUDED_)
