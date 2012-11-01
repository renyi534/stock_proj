// DataSimulator.h: interface for the DataSimulator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASIMULATOR_H__9AC9A654_E0AC_4679_BEC9_6D77355CC72C__INCLUDED_)
#define AFX_DATASIMULATOR_H__9AC9A654_E0AC_4679_BEC9_6D77355CC72C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include "OneMinuteData.h"
using namespace std;


class DataSimulator  
{
public:
	void SetRange(string StartDate, string EndDate);
	void Start();
	DataSimulator(string);
	virtual ~DataSimulator();

private:
	string m_Instrument;
	string m_StartDate;
	string m_EndDate;
	vector<CMinuteData> m_MinuteData;
	vector<CHalfMinuteData> m_HalfMinuteData;
};

#endif // !defined(AFX_DATASIMULATOR_H__9AC9A654_E0AC_4679_BEC9_6D77355CC72C__INCLUDED_)
