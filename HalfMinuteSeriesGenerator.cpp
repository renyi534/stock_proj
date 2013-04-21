// HalfMinuteSeriesGenerator.cpp: implementation of the HalfMinuteSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tradesystem.h"
#include "HalfMinuteSeriesGenerator.h"
#include "MessageRouter.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HalfMinuteSeriesGenerator::HalfMinuteSeriesGenerator(MessageRouter* router, bool StoreMarketData, string name):
	KSeriesGenerator(name, router, StoreMarketData)
{
	// stl library can be buggy with empty maps. Insert some rubbish data here.
	m_half_minute_data_map.insert(CHalfMinuteDataPair("", CHalfMinuteData() ));
	m_prev_half_minute_data_map.insert(CHalfMinuteDataPair("", CHalfMinuteData() ));
}

HalfMinuteSeriesGenerator::~HalfMinuteSeriesGenerator()
{

}

void HalfMinuteSeriesGenerator::InputTickData(const CThostFtdcDepthMarketDataField& tick_data)
{
	string tradingDay=tick_data.TradingDay;
	string updateTime=tick_data.UpdateTime;
	string second = updateTime.substr(6, 2);
	updateTime=updateTime.substr(0,5);
	string instrument_id = tick_data.InstrumentID;
	
	CHalfMinuteData half_minute_data;

	CHalfMinuteDataMap::iterator iter = m_half_minute_data_map.find(instrument_id);
	if (iter == m_half_minute_data_map.end())
	{
		//add map data for this instrument
		half_minute_data.m_Day= "";
		half_minute_data.m_Time= "";
		half_minute_data.m_ClosePrice=0;
		half_minute_data.m_HighPrice=0;
		half_minute_data.m_LowPrice=1e10;
		half_minute_data.m_OpenInterest=0;
		half_minute_data.m_OpenPrice=0;
		half_minute_data.m_Volume=0;
		half_minute_data.m_Sec = "";
		half_minute_data.m_InstrumentID= instrument_id;
		m_half_minute_data_map.insert(CHalfMinuteDataPair(instrument_id, half_minute_data));
	}
	else
	{
		half_minute_data = m_half_minute_data_map[instrument_id];
	}

	if(tradingDay==half_minute_data.m_Day &&
		( (second<"30" && half_minute_data.m_Sec< "30")
		  || (second>="30" && half_minute_data.m_Sec>= "30") ) )
	{
		half_minute_data.m_ClosePrice= tick_data.LastPrice;

		if(half_minute_data.m_HighPrice<tick_data.LastPrice)
		{
			half_minute_data.m_HighPrice=tick_data.LastPrice;
		}

		if(half_minute_data.m_LowPrice>tick_data.LastPrice)
		{
			half_minute_data.m_LowPrice=tick_data.LastPrice;
		}
		half_minute_data.m_Volume=tick_data.Volume;
		half_minute_data.m_OpenInterest=tick_data.OpenInterest;
		
	}
	else
	{
		if( half_minute_data.m_Day!="" && half_minute_data.m_Time!="")
		{
			if ( half_minute_data.m_Sec < "30" )
				half_minute_data.m_Sec = "29";
			else
				half_minute_data.m_Sec = "59";

			half_minute_data.m_Time = half_minute_data.m_Time.substr(0,5)+":"+half_minute_data.m_Sec;
			//We have gotten one minute statistics data
			CHalfMinuteData prev_data;
			//memset(&prev_data, 0 ,sizeof(prev_data));
			iter = m_prev_half_minute_data_map.find(instrument_id);
			bool send = false;
			if (iter == m_prev_half_minute_data_map.end())
			{
				m_prev_half_minute_data_map.insert(CHalfMinuteDataPair(instrument_id, half_minute_data));
			}
			else
			{
				prev_data = iter->second;
				m_prev_half_minute_data_map[instrument_id] = half_minute_data;
				send = true;
			}
			if( send )
			{
				half_minute_data.m_OpenInterest -= prev_data.m_OpenInterest;
				half_minute_data.m_Volume -= prev_data.m_Volume;
				
				m_Router->sendData(half_minute_data);
				
				if( m_StoreMarketData)
				{
					char* buffer = new char[8196];
					int index=0;
					
					const char* format_str="insert into stock_data.\"HalfMinuteData\" values('%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,'%s')";
					
					
					sprintf(buffer,format_str,
						half_minute_data.m_Day.c_str(),
						(half_minute_data.m_Time).c_str(),
						half_minute_data.m_OpenPrice,
						half_minute_data.m_ClosePrice,
						half_minute_data.m_HighPrice,
						half_minute_data.m_LowPrice,
						half_minute_data.m_Volume,
						half_minute_data.m_OpenInterest,
						instrument_id.c_str()
						);
					m_log<< buffer<<endl;
					//DbConn conn(dbAccessPool);
					//conn.m_db->execSql(buffer);
					gThreadPool.Run(ExecSQL, (void*) buffer);
				}
			}
		}
		
		half_minute_data.m_Day=tradingDay;
		half_minute_data.m_Time=updateTime;
		half_minute_data.m_Sec = second;
		half_minute_data.m_OpenPrice=tick_data.LastPrice;
		half_minute_data.m_ClosePrice=tick_data.LastPrice;
		half_minute_data.m_HighPrice=tick_data.LastPrice;
		half_minute_data.m_LowPrice=tick_data.LastPrice;
		half_minute_data.m_Volume=tick_data.Volume;
		half_minute_data.m_OpenInterest=tick_data.OpenInterest;
	}

	m_half_minute_data_map[instrument_id]=half_minute_data ;
}
