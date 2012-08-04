// StockRetriever.cpp: implementation of the StockRetriever class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tradesystem.h"
#include "StockRetriever.h"
#include "string"
using namespace std;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StockRetriever::StockRetriever()
{

}

StockRetriever::~StockRetriever()
{

}

CString  StockRetriever::GetStockInfo(CString url)
{
	CInternetSession inter_session;
	CString strHtml;
	CHttpFile* file =NULL;
	try{  
		file = (CHttpFile*)inter_session.OpenURL(url);  
	}catch(CInternetException * m_pException){  
		file = NULL;  
		m_pException->m_dwError;  
		m_pException->Delete();  
		inter_session.Close();
		return "";
		//AfxMessageBox("CInternetException");  
	}  
	CString strLine;  
	if(file != NULL){  
		while(file->ReadString(strLine) != NULL){  
			strHtml += strLine;  
		}  
	}else{  
		return "";
		//AfxMessageBox("fail");  
	}  
	inter_session.Close();  
	file->Close();  
	delete file;  
	file = NULL;  

	int start = strHtml.Find('"');

	int end = strHtml.Find('"', start+1);

	ASSERT( end > start+1 );

	CString ret = strHtml.Mid(start+1, end-start-1);
	return ret;
}

void StockRetriever::GetStockData(CString url, CThostFtdcDepthMarketDataField& data)
{
	CString str = GetStockInfo(url);

	if( str.GetLength() == 0)
		return;

	string  stl_str = (LPCSTR)str;

	char* delimiters = ",";
	char* instrument = strtok ((char*)stl_str.c_str(), delimiters);
	strcpy(data.InstrumentID, instrument);

	char* open_price = strtok(NULL, delimiters);
	data.OpenPrice = atof(open_price);

	char* pre_close_price = strtok(NULL, delimiters);
	data.PreClosePrice = atof(pre_close_price);

	char* last_price = strtok(NULL, delimiters);
	data.LastPrice = atof(last_price);

	char* high_price = strtok(NULL, delimiters);
	data.HighestPrice = atof(high_price);

	char* low_price = strtok(NULL, delimiters);
	data.LowestPrice = atof(low_price);

	// jump over buy1 and sell1, We have the detailed data later
	strtok(NULL, delimiters);
	strtok(NULL, delimiters);

	char* volume = strtok(NULL, delimiters);
	data.Volume =  atoi(volume);

	char* turnover = strtok(NULL, delimiters);
	data.Turnover =  atof(turnover);

	char* bid_volume = strtok(NULL, delimiters);
	data.BidVolume1 =  atoi(bid_volume);

	char* bid_price =  strtok(NULL, delimiters);
	data.BidPrice1 = atof(bid_price);

	bid_volume = strtok(NULL, delimiters);
	data.BidVolume2 =  atoi(bid_volume);

	bid_price =  strtok(NULL, delimiters);
	data.BidPrice2 = atof(bid_price);

	bid_volume = strtok(NULL, delimiters);
	data.BidVolume3 =  atoi(bid_volume);

	bid_price =  strtok(NULL, delimiters);
	data.BidPrice3 = atof(bid_price);

	bid_volume = strtok(NULL, delimiters);
	data.BidVolume4 =  atoi(bid_volume);

	bid_price =  strtok(NULL, delimiters);
	data.BidPrice4 = atof(bid_price);

	bid_volume = strtok(NULL, delimiters);
	data.BidVolume5 =  atoi(bid_volume);

	bid_price =  strtok(NULL, delimiters);
	data.BidPrice5 = atof(bid_price);

	char* ask_volume = strtok(NULL, delimiters);
	data.AskVolume1 =  atoi(ask_volume);

	char* ask_price =  strtok(NULL, delimiters);
	data.AskPrice1 = atof(ask_price);

	ask_volume = strtok(NULL, delimiters);
	data.AskVolume2 =  atoi(ask_volume);

	ask_price =  strtok(NULL, delimiters);
	data.AskPrice2 = atof(ask_price);

	ask_volume = strtok(NULL, delimiters);
	data.AskVolume3 =  atoi(ask_volume);

	ask_price =  strtok(NULL, delimiters);
	data.AskPrice3 = atof(ask_price);

	ask_volume = strtok(NULL, delimiters);
	data.AskVolume4 =  atoi(ask_volume);

	ask_price =  strtok(NULL, delimiters);
	data.AskPrice4 = atof(ask_price);

	ask_volume = strtok(NULL, delimiters);
	data.AskVolume5 =  atoi(ask_volume);

	ask_price =  strtok(NULL, delimiters);
	data.AskPrice5 = atof(ask_price);

	char * date = strtok(NULL, delimiters);
	strcpy( data.TradingDay, (LPCSTR)convertTime(date));

	char * time = strtok(NULL, delimiters);
	strcpy( data.UpdateTime, (LPCSTR)convertTime(time));
}

CString   StockRetriever::convertTime(CString str)
{
	CString ret;
	int i = str.GetLength();

	for (int j=0; j< i; j++)
	{
		if( str[j] != '-' )
			ret+=str[j];
	}
	return ret;
}