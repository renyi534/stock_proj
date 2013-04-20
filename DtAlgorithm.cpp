// DtAlgorithm.cpp: implementation of the DtAlgorithm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tradesystem.h"
#include "DtAlgorithm.h"
#include <math.h>
#include "DbConn.h"
#include "matlab\\libMethod_4.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern DbAccessorPool dbAccessPool;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DtAlgorithm::DtAlgorithm(string instrument_id):
m_InstrumentID(instrument_id), m_Amount(0), m_log(".\\dt_algo_data.log", ios::app),
m_state_log(".\\dt_algo_state.log", ios::app)
{
	lastVol=0;
	totalAmount=0;
	totalMoney=0;
	m_BidPrice=0;
	m_AskPrice=0;
	isIni=0;
	ininow=0;
}

DtAlgorithm::~DtAlgorithm()
{
	m_log.close();
	m_state_log.close();
}

void DtAlgorithm::OnMinuteData(const CMinuteData& data)
{

	double val = 0;
	mwArray newopen(1,1, mxDOUBLE_CLASS);
	mwArray newm(1,1, mxDOUBLE_CLASS);
	mwArray newe(1,1, mxDOUBLE_CLASS);
	mwArray newrm(1,1, mxDOUBLE_CLASS);
	mwArray newre(1,1, mxDOUBLE_CLASS);
	mwArray bidprice(1,1, mxDOUBLE_CLASS);
	mwArray to(1,1, mxDOUBLE_CLASS);
	mwArray th(1,1, mxDOUBLE_CLASS);
	mwArray tl(1,1, mxDOUBLE_CLASS);
	mwArray tc(1,1, mxDOUBLE_CLASS);
	mwArray tv(1,1, mxDOUBLE_CLASS);
	mwArray trm(1,1, mxDOUBLE_CLASS);
	mwArray tre(1,1, mxDOUBLE_CLASS);
	mwArray time(1,100,mxCHAR_CLASS);

	val = data.m_OpenPrice;
	to.SetData(&val,1);
	val = data.m_HighPrice;
	th.SetData(&val,1);
	val = data.m_LowPrice;
	tl.SetData(&val,1);
	val = data.m_ClosePrice;
	tc.SetData(&val,1);
	val = data.m_Volume;
	tv.SetData(&val,1);




	MinKsymbProcess4(2, newopen, bidprice, to, th, tl, tc);

	OrderInfoShort res;

	bidprice.GetData(&(res.price), 1);
	
	newopen.GetData(&(res.amount), 1);

	if ( totalAmount >=2 && res.amount > 0)
		res.amount =0;
	else if ( totalAmount <=-2 && res.amount < 0 )
		res.amount =0;

	if (data.m_Time > "15:10")
	{
		//isIni=0;
		res.amount = -totalAmount;
	}

	if(res.amount>0)
	{
		res.price=m_AskPrice;
	}
	
	if(res.amount<0)
	{
		res.price=m_BidPrice;
	}

	
	res.day= data.m_Day;
	res.time = data.m_Time;
	res.milliSec =0;
	res.m_instrumentID = data.m_InstrumentID;

	totalAmount += res.amount;
	res.totalAmount = totalAmount;


	SendStrategy(res);

	
	mwArray s_m(1,1, mxDOUBLE_CLASS);
	mwArray s_e(1,1, mxDOUBLE_CLASS);
	mwArray s_ud(1,1, mxDOUBLE_CLASS);
	mwArray s_pacounter(1,1, mxDOUBLE_CLASS);
	mwArray s_nacounter(1,1, mxDOUBLE_CLASS);
	mwArray s_pisstart(1,1, mxDOUBLE_CLASS);
	mwArray s_nisstart(1,1, mxDOUBLE_CLASS);


	GetInnerState4(7, s_m, s_e, s_ud, s_pacounter, s_nacounter, s_pisstart, s_nisstart);

	double im=0;
	s_m.GetData(&im,1);
	
	double ie=0;
	s_e.GetData(&ie,1);

	double iud=0;
	s_ud.GetData(&iud,1);

	double ipa=0;
	s_pacounter.GetData(&ipa,1);

	double ina=0;
	s_nacounter.GetData(&ina,1);

	double ips=0;
	s_pisstart.GetData(&ips,1);

	double ins=0;
	s_nisstart.GetData(&ins,1);

	m_state_log<<res.m_instrumentID+", "+res.day+" "+res.time<<", price:"<<res.price<<", m:"<<im<<", e:"
		<<ie<<", ud:"<<iud<<", P_action_con:"<<ipa<<",  N_action_con:"<<ina<<", P_is_start:"<<ips<<", N_is_start:"<<ins<<endl;

}



/*int mkk=1;*/

void DtAlgorithm::OnHalfMinuteData(const CHalfMinuteData& data)
{

	
}




int	DtAlgorithm::SendStrategy( OrderInfoShort & res)
{

	//第一行就是真实的发送指令，第二行是本地模拟写log
	if( res.amount != 0 )
	{
		Algorithm::SendStrategy(res);
		m_log<<res.m_instrumentID+",  "+res.day+" "+res.time<<",  Amount, "<< res.amount <<", Price, "<<res.price<<endl;

	}
	
	return 0;
}

void DtAlgorithm::OnTickData(const CThostFtdcDepthMarketDataField& data)
{
	m_AskPrice = data.AskPrice1;
	m_BidPrice = data.BidPrice1;
/*
	string strTime(data.UpdateTime, 0, 5);
	if (strTime > "15:10")
	{
	//	return;
	}

	mwArray stopopen(1,1, mxDOUBLE_CLASS);
	mwArray ia(1,1, mxDOUBLE_CLASS);
	mwArray ib(1,1, mxDOUBLE_CLASS);

	double val;
	val = m_AskPrice;
	ia.SetData(&val,1);
	val = m_BidPrice;
	ib.SetData(&val,1);

	TickStopLoss4(1, stopopen, ia, ib);

	stopopen.GetData(&val,1);

	if(val==0)
	{
		return;
	}


	OrderInfoShort res;
	
	//res.price=-1;
	
	res.amount=val;

	if ( totalAmount >=2 && res.amount > 0)
		res.amount =0;
	else if ( totalAmount <=-2 && res.amount < 0 )
		res.amount =0;



	if(res.amount>0)
	{
		res.price=m_AskPrice;
	}
	
	if(res.amount<0)
	{
		res.price=m_BidPrice;
	}

	
	res.day= data.TradingDay;
	res.time = data.UpdateTime;
	res.milliSec =0;
	res.m_instrumentID = data.InstrumentID;

	totalAmount += res.amount;
	res.totalAmount = totalAmount;


	SendStrategy(res);
	return;
*/


                                                  
}
void DtAlgorithm::OnTradeData(const CThostFtdcTradeField& data)
{
}
void DtAlgorithm::OnAccountData(const CThostFtdcTradingAccountField& data)
{
}
void DtAlgorithm::OnPositionData(const CThostFtdcInvestorPositionField& data)
{
}


BOOL DtAlgorithm::InitInstance()
{

	this->RegisterInstrument(m_InstrumentID);

	
	try{
		DbConn conn(dbAccessPool);
		//conn.m_db->getData(m_InstrumentID, startDayBuffer, startTimeBuffer,
		//	currDayBuffer, currTimeBuffer, m_historyData);
		conn.m_db->getData(m_InstrumentID, 100, m_historyData);
	}
	catch(CDBException* pe)
	{
		// The error code is in pe->m_nRetCode
		pe->ReportError();
		pe->Delete();
	}

	int size = m_historyData.size();

	//size=1;/////////////////////////////////////////////////////


	double val =10;

	mwArray mwErrorCode(1,1, mxDOUBLE_CLASS);	// 1，1表示矩阵的大小（所有maltab只有一种变量，就是矩阵，为了和Cpp变量接轨，设置成1*1的矩阵，mxDOUBLE_CLASS表示变量的精度）   
	mwArray mwOpen( size, 1,mxDOUBLE_CLASS); 
	mwArray mwHigh( size, 1,mxDOUBLE_CLASS); 
	mwArray mwLow(size, 1, mxDOUBLE_CLASS); 
	mwArray mwClose( size,1, mxDOUBLE_CLASS); 
	mwArray mwVolume( size,1, mxDOUBLE_CLASS);
	mwArray intime(size,1, mxDOUBLE_CLASS);

	


	mwArray inn1(1,1, mxDOUBLE_CLASS);
	mwArray inkb(1,1, mxDOUBLE_CLASS);
	mwArray inkm(1,1, mxDOUBLE_CLASS);

	mwArray inw(1,1, mxDOUBLE_CLASS);
	mwArray inwl(1,1, mxDOUBLE_CLASS);
	mwArray inks(1,1, mxDOUBLE_CLASS);

	double* openPrice = new double[size];
	double* highPrice = new double[size];
	double* lowPrice = new double[size];
	double* closePrice = new double[size];
	double* vol = new double[size];
	
	
	int * intimeA=new int[size];
	if ( size >0)
		intimeA[size-1]=0;
	int iSameDay=1;

	val=0;

	// set data，不用我解释了吧，很简单的，调用类里面的SetData函数给类赋值   
	for( int i=size-1; i>=0; i-- )
	{
		openPrice[i] = m_historyData[i].m_OpenPrice;
		highPrice[i] = m_historyData[i].m_HighPrice;
		lowPrice[i] = m_historyData[i].m_LowPrice;
		closePrice[i] = m_historyData[i].m_ClosePrice;
		vol[i] = m_historyData[i].m_Volume;
		if(i!=size-1)
		{
			if(m_historyData[i].m_Day!=m_historyData[i+1].m_Day)
			{
				intimeA[i] = intimeA[i+1]-1;
			}
			else
			{
				intimeA[i] = intimeA[i+1];
			}

			

		}
		
	}
	

	mwOpen.SetData(openPrice, size);
	mwHigh.SetData(highPrice, size);
	mwLow.SetData(lowPrice, size);
	mwClose.SetData(closePrice, size);
	intime.SetData(intimeA, size);
	mwVolume.SetData(vol,size);



	val = 5;
	inn1.SetData(&val,1);
	val = 2;
	inkb.SetData(&val,1);
	val = 5;
	inkm.SetData(&val,1);
	val = 40;
	inks.SetData(&val,1);
	val = 30;
	inwl.SetData(&val,1);
	val = 15;
	inw.SetData(&val,1);


	IniMethod4(1, mwErrorCode,
		mwOpen, mwHigh, mwLow, mwClose, inn1, inkb, inkm, inw, inwl, inks);

	mwErrorCode.GetData(&val,1);
                                     
	delete []openPrice;
	delete []highPrice;
	delete []lowPrice;
	delete []closePrice;
	delete []vol;
	delete []intimeA;

  

	return TRUE;
}

