// DbAccessor.h: interface for the DbAccessor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBACCESSOR_H__0736BBB8_6280_4883_80DA_C8887B25DD49__INCLUDED_)
#define AFX_DBACCESSOR_H__0736BBB8_6280_4883_80DA_C8887B25DD49__INCLUDED_
#include ".\\ThostTraderApi\\ThostFtdcUserApiStruct.h"
#include <string>
#include <vector>

using namespace std;
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CMinuteData;
class CHalfMinuteData;
struct OrderInfo;
class DbAccessor  
{
public:
	virtual ~DbAccessor();
	void getData(string day_s, string time_s,
		string day_e, string time_e, vector<CThostFtdcDepthMarketDataField>& result); 
	void getData(string instrument_ID, string day_s, string time_s,
		string day_e, string time_e, vector<CThostFtdcDepthMarketDataField>& result); 
	void getData(string sql, vector<CThostFtdcDepthMarketDataField>& result); 
	void getData(string instrument_ID, string day_s, string time_s,
		string day_e, string time_e, vector<OrderInfo>& result);
	void getData(string sql, CRecordset* result); 
	void execSql(string sql);
	void getData(string sql, vector<OrderInfo>& result);
	void getData(string instrument_ID, string day_s, string time_s,
		string day_e, string time_e, vector<CMinuteData>& result);
	void getData(string instrument_ID, int limitNum, vector<CMinuteData>& result);
	void getData(string sql, vector<CMinuteData>& result);
	void getData(string instrument_ID, string day_s, string time_s,
		string day_e, string time_e, vector<CHalfMinuteData>& result);
	void getData(string instrument_ID, int limitNum, vector<CHalfMinuteData>& result);
	void getData(string sql, vector<CHalfMinuteData>& result);
	DbAccessor();
private:
	void getMarketDataFromRecord(CRecordset& record,CThostFtdcDepthMarketDataField& data);
	void getOrderInfoFromRecord(CRecordset& rs,OrderInfo& data);

	void getOneMinuteDataFromRecord(CRecordset& rs,CMinuteData& data);
	void getHalfMinuteDataFromRecord(CRecordset& rs,CHalfMinuteData& data);
	CDatabase m_db;
};

#endif // !defined(AFX_DBACCESSOR_H__0736BBB8_6280_4883_80DA_C8887B25DD49__INCLUDED_)
