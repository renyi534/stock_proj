// DataSimulator.cpp: implementation of the DataSimulator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TradeSystem.h"
#include "DataSimulator.h"
#include "DbAccessor.h"
#include "DbAccessorPool.h"
#include "DbConn.h"
#include <math.h>
#include "MessageRouter.h"
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



extern DbAccessorPool dbAccessPool;

DataSimulator::DataSimulator(string instrument):
	m_Instrument(instrument)
{

}

DataSimulator::~DataSimulator()
{

}

void DataSimulator::Start()
{
	DbConn conn(dbAccessPool);
	conn.m_db->getData(m_Instrument, m_StartDate, "09:14:59", 
		m_EndDate, "15:15:01", m_MinuteData);
	conn.m_db->getData(m_Instrument, m_StartDate, "09:14:59", 
		m_EndDate, "15:15:01", m_HalfMinuteData);
	
	int j=0, i=0;
	for( j=0, i =0; i<m_MinuteData.size(); i++)
	{	
		if( j< m_HalfMinuteData.size()-1 )
		{
			//MessageRouter::Router.sendData(m_HalfMinuteData[j++]);
			//MessageRouter::Router.sendData(m_HalfMinuteData[j++]);
		}

		//MessageRouter::Router.sendData(m_MinuteData[i]);
		//Sleep(500);
	}

	for (; j< m_HalfMinuteData.size(); j++)
	{
		//MessageRouter::Router.sendData(m_HalfMinuteData[j]);
	}
	AfxMessageBox("Simulation Ends");
}

void DataSimulator::SetRange(string StartDate, string EndDate)
{
	this->m_EndDate = EndDate;
	this->m_StartDate = StartDate;
}
