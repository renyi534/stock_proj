// VarOneMinuteSeriesGenerator.cpp: implementation of the VarOneMinuteSeriesGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tradesystem.h"
#include "VarOneMinuteSeriesGenerator.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VarOneMinuteSeriesGenerator::VarOneMinuteSeriesGenerator(int delimiter)
	:OneMinuteSeriesGenerator("VarOneMinuteKSeries"),
	 m_delimiter(delimiter)
{

}

VarOneMinuteSeriesGenerator::~VarOneMinuteSeriesGenerator()
{

}

void VarOneMinuteSeriesGenerator::InputTickData(const CThostFtdcDepthMarketDataField& tick_data)
{
	string tradingDay=tick_data.TradingDay;
	string updateTime=tick_data.UpdateTime;
	string second = updateTime.substr(6, 2);
	int i_second = atoi(second.c_str());
	updateTime=updateTime.substr(0,5);
	string instrument_id = tick_data.InstrumentID;
	
	CMinuteData m_one_minute_data;

	int curr_minute = 0;
	int tick_minute = atoi(updateTime.substr(3,2).c_str());
	CMinuteDataMap::iterator iter = m_one_minute_data_map.find(instrument_id);
	if (iter == m_one_minute_data_map.end())
	{
		//add map data for this instrument
		this->resetOneMinuteData(m_one_minute_data, instrument_id);
		m_one_minute_data_map.insert(CMinuteDataPair(instrument_id, m_one_minute_data));
	}
	else
	{
		m_one_minute_data = m_one_minute_data_map[instrument_id];
		curr_minute = atoi(m_one_minute_data.m_Time.substr(3,2).c_str());
	}

	if(tradingDay==m_one_minute_data.m_Day &&
		(updateTime==m_one_minute_data.m_Time || 
		 (i_second < m_delimiter && 
		  (tick_minute<curr_minute+2 && curr_minute-tick_minute<58)
		 )
		) 
	  )
	{
		m_one_minute_data.m_ClosePrice= tick_data.LastPrice;

		if(m_one_minute_data.m_HighPrice<tick_data.LastPrice)
		{
			m_one_minute_data.m_HighPrice=tick_data.LastPrice;
		}

		if(m_one_minute_data.m_LowPrice>tick_data.LastPrice)
		{
			m_one_minute_data.m_LowPrice=tick_data.LastPrice;
		}
		m_one_minute_data.m_Volume=tick_data.Volume;
		m_one_minute_data.m_OpenInterest=tick_data.OpenInterest;
	}
	else
	{
		if( m_one_minute_data.m_Day!="" && m_one_minute_data.m_Time!="")
		{
			char buffer[10];
			sprintf(buffer, ":%02d", m_delimiter);
			
			m_one_minute_data.m_Time =updateTime+buffer;

			//We have gotten one minute statistics data
			CMinuteData prev_data;
			//memset(&prev_data, 0 ,sizeof(prev_data));
			iter = m_prev_one_minute_data_map.find(instrument_id);
			bool send = false;
			if (iter == m_prev_one_minute_data_map.end())
			{
				m_prev_one_minute_data_map.insert(CMinuteDataPair(instrument_id, m_one_minute_data));
			}
			else
			{
				prev_data = iter->second;
				m_prev_one_minute_data_map[instrument_id] = m_one_minute_data;
				send = true;
			}
			if (send)
			{
				m_one_minute_data.m_OpenInterest -= prev_data.m_OpenInterest;
				m_one_minute_data.m_Volume -= prev_data.m_Volume;
				MessageRouter::Router.sendData(m_one_minute_data);
				
				char* buffer = new char[8196];
				int index=0;
				
				const char* format_str="insert into stock_data.\"OneMinuteData\" values('%s','%s',%lf,%lf,%lf,%lf,%lf,%lf,'%s')";
				

				sprintf(buffer,format_str,
					m_one_minute_data.m_Day.c_str(),
					(m_one_minute_data.m_Time).c_str(),
					m_one_minute_data.m_OpenPrice,
					m_one_minute_data.m_ClosePrice,
					m_one_minute_data.m_HighPrice,
					m_one_minute_data.m_LowPrice,
					m_one_minute_data.m_Volume,
					m_one_minute_data.m_OpenInterest,
					instrument_id.c_str()
					);
				m_log<< buffer<<endl;
				//DbConn conn(dbAccessPool);
				//conn.m_db->execSql(buffer);
				gThreadPool.Run(ExecSQL, (void*) buffer);
			}
		}
		resetOneMinuteData(m_one_minute_data, instrument_id);
		m_one_minute_data.m_Day=tradingDay;
		m_one_minute_data.m_Time=updateTime;
		m_one_minute_data.m_OpenPrice=tick_data.LastPrice;
		m_one_minute_data.m_ClosePrice=tick_data.LastPrice;
		m_one_minute_data.m_HighPrice=tick_data.LastPrice;
		m_one_minute_data.m_LowPrice=tick_data.LastPrice;
		m_one_minute_data.m_Volume=tick_data.Volume;
		m_one_minute_data.m_OpenInterest=tick_data.OpenInterest;
	}

	m_one_minute_data_map[instrument_id]=m_one_minute_data ;
}

