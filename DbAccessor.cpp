// DbAccessor.cpp: implementation of the DbAccessor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TradeSystem.h"
#include "DbAccessor.h"
#include "Algorithm.h"
#include "OneMinuteData.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern char DB_CONN[];
DbAccessor::DbAccessor():
	m_db()
{
	try{
		m_db.OpenEx(DB_CONN);
	}
	catch(CDBException* pe)
	{
		// The error code is in pe->m_nRetCode
		pe->ReportError();
		pe->Delete();
	}
}

DbAccessor::~DbAccessor()
{
	try{
		m_db.Close();
	}
	catch(CDBException* pe)
	{
		// The error code is in pe->m_nRetCode
		pe->ReportError();
		pe->Delete();
	}
}

void DbAccessor::getData(string day_s, string time_s,
		string day_e, string time_e, vector<CThostFtdcDepthMarketDataField>& result)
{
	string sql="select * from stock_data.\"DepthMarketData\" where "
		"(\"TradingDay\"||' '||\"UpdateTime\")::timestamp >='";
	sql+=day_s+" "+time_s+"'::timestamp and (\"TradingDay\"||' '||\"UpdateTime\")::timestamp<='";
	sql+=day_e+" "+time_e+"'::timestamp order by (\"TradingDay\"||' '||\"UpdateTime\")::timestamp,\"UpdateMillisec\"";

	getData(sql, result);
}

void DbAccessor::getData(string instrument_ID, string day_s, string time_s,
		string day_e, string time_e, vector<CThostFtdcDepthMarketDataField>& result)
{
	string sql="select * from stock_data.\"DepthMarketData\" where "
		"(\"TradingDay\"||' '||\"UpdateTime\")::timestamp >='";
	sql+=day_s+" "+time_s+"'::timestamp and (\"TradingDay\"||' '||\"UpdateTime\")::timestamp<='";
	sql+=day_e+" "+time_e+"'::timestamp and \"InstrumentID\"='"+instrument_ID+
		"' order by (\"TradingDay\"||' '||\"UpdateTime\")::timestamp,\"UpdateMillisec\"";

	getData(sql, result);
}

void DbAccessor::getData(string sql, CRecordset* rsCustSet)
{
	try{
		rsCustSet= new CRecordset(&this->m_db);
		rsCustSet->Open(CRecordset::snapshot, sql.c_str());
	}
	catch(CDBException* pe)
	{
		// The error code is in pe->m_nRetCode
		pe->ReportError();
		pe->Delete();
	}
	
}
void DbAccessor::execSql(string sql)
{
	try{
		m_db.ExecuteSQL(sql.c_str());
	}
	catch(CDBException* pe)
	{
		// The error code is in pe->m_nRetCode
		pe->ReportError();
		pe->Delete();
	}
}
void DbAccessor::getData(string sql, vector<CThostFtdcDepthMarketDataField>& result)
{
	result.clear();
	try{
		CRecordset rsCustSet(&this->m_db);
		rsCustSet.Open(CRecordset::snapshot, sql.c_str());


		if( rsCustSet.IsBOF( ) )
		{
			// The recordset is empty
			return;
		}
		rsCustSet.MoveFirst();

		// Scroll to the end of the recordset, past
		// the last record, so no record is current
		while ( !rsCustSet.IsEOF( ) )
		{
			CThostFtdcDepthMarketDataField data;
			getMarketDataFromRecord(rsCustSet,data);
			result.push_back(data);
			rsCustSet.MoveNext( );
		}
		rsCustSet.Close();
	}
	catch(CDBException* pe)
	{
		// The error code is in pe->m_nRetCode
		pe->ReportError();
		pe->Delete();
	}
	
}

void DbAccessor::getMarketDataFromRecord(CRecordset& rs,CThostFtdcDepthMarketDataField& data)
{
	// Loop through the recordset,
	// using GetFieldValue and
	// GetODBCFieldCount to retrieve
	// data in all columns
	short nFields = rs.GetODBCFieldCount();
	CString* strValue=new CString[nFields];

	for( short index = 0; index < nFields; index++ )
	{
		rs.GetFieldValue(index, strValue[index] );
	}
	sprintf(data.TradingDay,"%s",(LPCTSTR)strValue[0]);
	sprintf(data.InstrumentID,"%s",(LPCTSTR)strValue[1]);
	sprintf(data.ExchangeID,"%s",(LPCTSTR)strValue[2]);
	sprintf(data.ExchangeInstID,"%s",(LPCTSTR)strValue[3]);
	data.LastPrice = atof(strValue[4]);
	data.PreSettlementPrice = atof(strValue[5]);
	data.PreClosePrice = atof(strValue[6]);
	data.PreOpenInterest = atof(strValue[7]);
	data.OpenPrice = atof(strValue[8]);
	data.HighestPrice = atof(strValue[9]);

	data.LowestPrice = atof(strValue[10]);
	data.Volume = atof(strValue[11]);
	data.Turnover = atof(strValue[12]);
	data.OpenInterest = atof(strValue[13]);
	data.ClosePrice = atof(strValue[14]);
	data.SettlementPrice = atof(strValue[15]);

	data.UpperLimitPrice = atof(strValue[16]);
	data.LowerLimitPrice = atof(strValue[17]);
	data.PreDelta = atof(strValue[18]);
	data.CurrDelta = atof(strValue[19]);
	sprintf(data.UpdateTime,"%s",(LPCTSTR)strValue[20]);
	data.UpdateMillisec = atoi(strValue[21]);

	data.BidPrice1 = atof(strValue[22]);
	data.BidVolume1 = atof(strValue[23]);
	data.AskPrice1 = atof(strValue[24]);
	data.AskVolume1 = atof(strValue[25]);

	data.BidPrice2 = atof(strValue[26]);
	data.BidVolume2 = atof(strValue[27]);
	data.AskPrice2 = atof(strValue[28]);
	data.AskVolume2 = atof(strValue[29]);

	data.BidPrice3 = atof(strValue[30]);
	data.BidVolume3 = atof(strValue[31]);
	data.AskPrice3 = atof(strValue[32]);
	data.AskVolume3 = atof(strValue[33]);

	data.BidPrice4 = atof(strValue[34]);
	data.BidVolume4 = atof(strValue[35]);
	data.AskPrice4 = atof(strValue[36]);
	data.AskVolume4 = atof(strValue[37]);

	data.BidPrice5 = atof(strValue[38]);
	data.BidVolume5 = atof(strValue[39]);
	data.AskPrice5 = atof(strValue[40]);
	data.AskVolume5 = atof(strValue[41]);

	data.AveragePrice = atof(strValue[42]);
	delete[] strValue;
}

void DbAccessor::getData(string instrument_ID, string day_s, string time_s,
		string day_e, string time_e, vector<OrderInfo>& result)
{
	string sql="select * from stock_data.trade_details where "
		"(trans_day||' '||trans_time)::timestamp >='";
	sql+=day_s+" "+time_s+"'::timestamp and (trans_day||' '||trans_time)::timestamp<='";
	sql+=day_e+" "+time_e+"'::timestamp and instrument_id='"+instrument_ID+
		"' order by (trans_day||' '||trans_time)::timestamp";

	getData(sql, result);
}

void DbAccessor::getData(string sql, vector<OrderInfo>& result)
{
	result.clear();
	try{
		CRecordset rsCustSet(&this->m_db);
		rsCustSet.Open(CRecordset::snapshot, sql.c_str());


		if( rsCustSet.IsBOF( ) )
		{
			// The recordset is empty
			return;
		}
		rsCustSet.MoveFirst();

		// Scroll to the end of the recordset, past
		// the last record, so no record is current
		while ( !rsCustSet.IsEOF( ) )
		{
			OrderInfo data;
			getOrderInfoFromRecord(rsCustSet,data);
			result.push_back(data);
			rsCustSet.MoveNext( );
		}
		rsCustSet.Close();
	}
	catch(CDBException* pe)
	{
		// The error code is in pe->m_nRetCode
		pe->ReportError();
		pe->Delete();
	}
	
}

void DbAccessor::getOrderInfoFromRecord(CRecordset& rs,OrderInfo& data)
{
	// Loop through the recordset,
	// using GetFieldValue and
	// GetODBCFieldCount to retrieve
	// data in all columns
	short nFields = rs.GetODBCFieldCount();
	CString* strValue=new CString[nFields];

	for( short index = 0; index < nFields; index++ )
	{
		rs.GetFieldValue(index, strValue[index] );
	}
	data.amount = atof(strValue[0]);
	data.price = atof(strValue[1]);
	data.day = (LPCTSTR)strValue[2];
	data.time = (LPCTSTR)strValue[3];
	data.m_instrumentID = (LPCTSTR)strValue[4];
	data.is_buy = atoi(strValue[5]);
	data.is_open = atoi(strValue[6]);
	data.milliSec =0;
	delete[] strValue;
}

void DbAccessor::getData(string instrument_ID, string day_s, string time_s,
		string day_e, string time_e, vector<CMinuteData>& result)
{
	string sql="select * from stock_data.\"OneMinuteData\" where "
		"(day||' '||\"time\")::timestamp >='";
	sql+=day_s+" "+time_s+"'::timestamp and (day||' '||\"time\")::timestamp<='";
	sql+=day_e+" "+time_e+"'::timestamp and instrument_id='"+instrument_ID+
		"' order by (day||' '||\"time\")::timestamp";
   
	getData(sql, result);
}

void DbAccessor::getData(string instrument_ID, int limitNum, vector<CMinuteData>& result)
{
	char numstr[20];
	sprintf(numstr, "%d", limitNum);
	string sql="select * from stock_data.\"OneMinuteData\" where instrument_id='"+instrument_ID+
		"' order by (day||' '||\"time\")::timestamp desc limit "+numstr;

	sql ="select * from ("+sql+") s order by (day||' '||\"time\")::timestamp";
   
	getData(sql, result);
}

void DbAccessor::getData(string sql, vector<CMinuteData>& result)
{
	result.clear();
	try{
		CRecordset rsCustSet(&this->m_db);
		rsCustSet.Open(CRecordset::snapshot, sql.c_str());


		if( rsCustSet.IsBOF( ) )
		{
			// The recordset is empty
			return;
		}
		rsCustSet.MoveFirst();

		// Scroll to the end of the recordset, past
		// the last record, so no record is current
		while ( !rsCustSet.IsEOF( ) )
		{
			CMinuteData data;
			getOneMinuteDataFromRecord(rsCustSet,data);
			result.push_back(data);
			rsCustSet.MoveNext( );
		}
		rsCustSet.Close();
	}
	catch(CDBException* pe)
	{
		// The error code is in pe->m_nRetCode
		pe->ReportError();
		pe->Delete();
	}
	
}

void DbAccessor::getOneMinuteDataFromRecord(CRecordset& rs,CMinuteData& data)
{
	// Loop through the recordset,
	// using GetFieldValue and
	// GetODBCFieldCount to retrieve
	// data in all columns
	short nFields = rs.GetODBCFieldCount();
	CString* strValue=new CString[nFields];

	for( short index = 0; index < nFields; index++ )
	{
		rs.GetFieldValue(index, strValue[index] );
	}
	data.m_Day = (LPCTSTR)strValue[0];
	data.m_Time = (LPCTSTR)strValue[1];
	data.m_OpenPrice = atof(strValue[2]);
	data.m_ClosePrice = atof(strValue[3]);
	data.m_HighPrice = atof(strValue[4]);
	data.m_LowPrice = atof(strValue[5]);
	data.m_Volume = atof(strValue[6]);
	data.m_OpenInterest = atof(strValue[7]);
	data.m_InstrumentID = (LPCTSTR)strValue[8];

	delete[] strValue;
}

void DbAccessor::getData(string instrument_ID, string day_s, string time_s,
		string day_e, string time_e, vector<CHalfMinuteData>& result)
{
	string sql="select * from stock_data.\"HalfMinuteData\" where "
		"(day||' '||\"time\")::timestamp >='";
	sql+=day_s+" "+time_s+"'::timestamp and (day||' '||\"time\")::timestamp<='";
	sql+=day_e+" "+time_e+"'::timestamp and instrument_id='"+instrument_ID+
		"' order by (day||' '||\"time\")::timestamp";
   
	getData(sql, result);
}

void DbAccessor::getData(string instrument_ID, int limitNum, vector<CHalfMinuteData>& result)
{
	char numstr[20];
	sprintf(numstr, "%d", limitNum);
	string sql="select * from stock_data.\"HalfMinuteData\" where instrument_id='"+instrument_ID+
		"' order by (day||' '||\"time\")::timestamp desc limit "+numstr;

	sql ="select * from ("+sql+") s order by (day||' '||\"time\")::timestamp asc";
   
	getData(sql, result);
}

void DbAccessor::getData(string sql, vector<CHalfMinuteData>& result)
{
	result.clear();
	try{
		CRecordset rsCustSet(&this->m_db);
		rsCustSet.Open(CRecordset::snapshot, sql.c_str());


		if( rsCustSet.IsBOF( ) )
		{
			// The recordset is empty
			return;
		}
		rsCustSet.MoveFirst();

		// Scroll to the end of the recordset, past
		// the last record, so no record is current
		while ( !rsCustSet.IsEOF( ) )
		{
			CHalfMinuteData data;
			getHalfMinuteDataFromRecord(rsCustSet,data);
			result.push_back(data);
			rsCustSet.MoveNext( );
		}
		rsCustSet.Close();
	}
	catch(CDBException* pe)
	{
		// The error code is in pe->m_nRetCode
		pe->ReportError();
		pe->Delete();
	}
	
}

void DbAccessor::getHalfMinuteDataFromRecord(CRecordset& rs,CHalfMinuteData& data)
{
	// Loop through the recordset,
	// using GetFieldValue and
	// GetODBCFieldCount to retrieve
	// data in all columns
	short nFields = rs.GetODBCFieldCount();
	CString* strValue=new CString[nFields];

	for( short index = 0; index < nFields; index++ )
	{
		rs.GetFieldValue(index, strValue[index] );
	}
	data.m_Day = (LPCTSTR)strValue[0];
	data.m_Time = (LPCTSTR)strValue[1];
	data.m_OpenPrice = atof(strValue[2]);
	data.m_ClosePrice = atof(strValue[3]);
	data.m_HighPrice = atof(strValue[4]);
	data.m_LowPrice = atof(strValue[5]);
	data.m_Volume = atof(strValue[6]);
	data.m_OpenInterest = atof(strValue[7]);
	data.m_InstrumentID = (LPCTSTR)strValue[8];

	delete[] strValue;
}