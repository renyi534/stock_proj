// TenMinuteSeriesGenerator.cpp: implementation of the TenMinuteSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tradesystem.h"
#include "TenMinuteSeriesGenerator.h"
#include "MessageRouter.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TenMinuteSeriesGenerator::TenMinuteSeriesGenerator(MessageRouter* router, bool StoreMarketData):
		KSeriesGenerator("TenMinuteKSeries", router, StoreMarketData)
{
	// stl library can be buggy with empty maps. Insert some rubbish data here.
	m_ten_minute_data_map.insert(CTenMinuteDataPair("", CTenMinuteData() ));
	m_prev_ten_minute_data_map.insert(CTenMinuteDataPair("", CTenMinuteData() ));
}

TenMinuteSeriesGenerator::~TenMinuteSeriesGenerator()
{

}

void TenMinuteSeriesGenerator::resetTenMinuteData(CTenMinuteData& m_ten_minute_data, const string& instrument_id)
{
	m_ten_minute_data.m_Day= "";
	m_ten_minute_data.m_Time= "";
	m_ten_minute_data.m_ClosePrice=0;
	m_ten_minute_data.m_HighPrice=0;
	m_ten_minute_data.m_LowPrice=1e10;
	m_ten_minute_data.m_OpenInterest=0;
	m_ten_minute_data.m_OpenPrice=0;
	m_ten_minute_data.m_Volume=0;
	m_ten_minute_data.m_InstrumentID= instrument_id;
}

void TenMinuteSeriesGenerator::InputTickData(const CThostFtdcDepthMarketDataField& tick_data)
{
	string tradingDay=tick_data.TradingDay;
	string updateTime=tick_data.UpdateTime;
	updateTime=updateTime.substr(0,4);
	string instrument_id = tick_data.InstrumentID;
	
	CTenMinuteData m_ten_minute_data;

	CTenMinuteDataMap::iterator iter = m_ten_minute_data_map.find(instrument_id);
	if (iter == m_ten_minute_data_map.end())
	{
		//add map data for this instrument
		this->resetTenMinuteData(m_ten_minute_data, instrument_id);
		m_ten_minute_data_map.insert(CTenMinuteDataPair(instrument_id, m_ten_minute_data));
	}
	else
	{
		m_ten_minute_data = m_ten_minute_data_map[instrument_id];
	}

	if(tradingDay==m_ten_minute_data.m_Day &&
		updateTime==m_ten_minute_data.m_Time)
	{
		m_ten_minute_data.m_ClosePrice= tick_data.LastPrice;

		if(m_ten_minute_data.m_HighPrice<tick_data.LastPrice)
		{
			m_ten_minute_data.m_HighPrice=tick_data.LastPrice;
		}

		if(m_ten_minute_data.m_LowPrice>tick_data.LastPrice)
		{
			m_ten_minute_data.m_LowPrice=tick_data.LastPrice;
		}
		m_ten_minute_data.m_Volume=tick_data.Volume;
		m_ten_minute_data.m_OpenInterest=tick_data.OpenInterest;
	}
	else
	{
		if( m_ten_minute_data.m_Day!="" && m_ten_minute_data.m_Time!="")
		{
			m_ten_minute_data.m_Time+="9:59";

			//We have gotten one minute statistics data
			CTenMinuteData prev_data;
			//memset(&prev_data, 0 ,sizeof(prev_data));
			iter = m_prev_ten_minute_data_map.find(instrument_id);
			bool send = false;
			if (iter == m_prev_ten_minute_data_map.end())
			{
				m_prev_ten_minute_data_map.insert(CTenMinuteDataPair(instrument_id, m_ten_minute_data));
			}
			else
			{
				prev_data = iter->second;
				m_prev_ten_minute_data_map[instrument_id] = m_ten_minute_data;
				send = true;
			}
			if (send)
			{
				m_ten_minute_data.m_OpenInterest -= prev_data.m_OpenInterest;
				m_ten_minute_data.m_Volume -= prev_data.m_Volume;
				m_Router->sendData(m_ten_minute_data);
				
				if(m_StoreMarketData)
				{
					char* buffer = new char[8196];
					int index=0;
					
					const char* format_str="insert into stock_data.\"TenMinuteData\" values('%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,'%s')";
					
					sprintf(buffer,format_str,
						m_ten_minute_data.m_Day.c_str(),
						(m_ten_minute_data.m_Time).c_str(),
						m_ten_minute_data.m_OpenPrice,
						m_ten_minute_data.m_ClosePrice,
						m_ten_minute_data.m_HighPrice,
						m_ten_minute_data.m_LowPrice,
						m_ten_minute_data.m_Volume,
						m_ten_minute_data.m_OpenInterest,
						instrument_id.c_str()
						);
					m_log<< buffer<<endl;
					//DbConn conn(dbAccessPool);
					//conn.m_db->execSql(buffer);
					gThreadPool.Run(ExecSQL, (void*) buffer);
				}
			}
		}
		resetTenMinuteData(m_ten_minute_data, instrument_id);
		m_ten_minute_data.m_Day=tradingDay;
		m_ten_minute_data.m_Time=updateTime;
		m_ten_minute_data.m_OpenPrice=tick_data.LastPrice;
		m_ten_minute_data.m_ClosePrice=tick_data.LastPrice;
		m_ten_minute_data.m_HighPrice=tick_data.LastPrice;
		m_ten_minute_data.m_LowPrice=tick_data.LastPrice;
		m_ten_minute_data.m_Volume=tick_data.Volume;
		m_ten_minute_data.m_OpenInterest=tick_data.OpenInterest;
	}

	m_ten_minute_data_map[instrument_id]=m_ten_minute_data ;
}