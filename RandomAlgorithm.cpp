// RandomAlgorithm.cpp: implementation of the RandomAlgorithm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TradeSystem.h"
#include "RandomAlgorithm.h"
#include <math.h>
#include "DbConn.h"
#include "matlab\\libMethod_1.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern DbAccessorPool dbAccessPool;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RandomAlgorithm::RandomAlgorithm(string instrument_id):
m_InstrumentID(instrument_id), m_Amount(0), m_log("c:\\random_algo_data.log", ios::app)
{
	lastVol=0;
	totalAmount=0;
	totalMoney=0;
	m_BidPrice=0;
	m_AskPrice=0;
}

RandomAlgorithm::~RandomAlgorithm()
{
	m_log.close();
}

void RandomAlgorithm::OnMinuteData(const CMinuteData& data)
{
	return;
}

int mkk=1;
int isIni=0;
void RandomAlgorithm::OnHalfMinuteData(const CHalfMinuteData& data)
{
	if (isIni<1)
	{
		InitInstance();
	
		isIni++;
	
	}
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
	val = data.m_Volume-lastVol;
//	val = data.m_Volume;
	lastVol = data.m_Volume;
	tv.SetData(&val,1);
	trm.SetData(&val,1);
	tre.SetData(&val,1);



	MinKsymbProcess(2, newopen, bidprice, to, th, tl, tc, tv);



	OrderInfoShort res;

	bidprice.GetData(&(res.price), 1);
	
	newopen.GetData(&(res.amount), 1);

	string strTime(data.m_Time, 1, 5);
	if (strTime>"15:12")
	{
		isIni=0;
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
}
int	RandomAlgorithm::SendStrategy(const OrderInfoShort & res)
{
	//第一行就是真实的发送指令，第二行是本地模拟写log
	if( res.amount != 0 )
	{
		Algorithm::SendStrategy(res);	
	}
	m_log<<res.m_instrumentID+",  "+res.day+" "+res.time<<",  Amount, "<< res.amount <<", Price, "<<res.price<<endl;

	return 0;
}

void RandomAlgorithm::OnTickData(const CThostFtdcDepthMarketDataField& data)
{
	m_AskPrice = data.AskPrice1;
	m_BidPrice = data.BidPrice1;

                                                  
}
void RandomAlgorithm::OnTradeData(const CThostFtdcTradeField& data)
{
}
void RandomAlgorithm::OnAccountData(const CThostFtdcTradingAccountField& data)
{
}
void RandomAlgorithm::OnPositionData(const CThostFtdcInvestorPositionField& data)
{
}

BOOL RandomAlgorithm::InitInstance()
{
	this->RegisterInstrument(m_InstrumentID);
	try{
		DbConn conn(dbAccessPool);
		//conn.m_db->getData(m_InstrumentID, startDayBuffer, startTimeBuffer,
		//	currDayBuffer, currTimeBuffer, m_historyData);
		conn.m_db->getData(m_InstrumentID, 1000, m_historyData);
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
	mwArray intime(size,1, mxDOUBLE_CLASS);

	

	mwArray insp(1,1, mxDOUBLE_CLASS);
	mwArray inp(1,1, mxDOUBLE_CLASS);
	mwArray inw(1,1, mxDOUBLE_CLASS);
	mwArray inwl(1,1, mxDOUBLE_CLASS);
	mwArray inkb(1,1, mxDOUBLE_CLASS);
	mwArray inks(1,1, mxDOUBLE_CLASS);
	mwArray inkm(1,1, mxDOUBLE_CLASS);
	mwArray inul(1,1, mxDOUBLE_CLASS);
	mwArray indl(1,1, mxDOUBLE_CLASS);

	double* openPrice = new double[size];
	double* highPrice = new double[size];
	double* lowPrice = new double[size];
	double* closePrice = new double[size];
	
	
	int * intimeA=new int[size];
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


/*
	val = 120;
	insp.SetData(&val,1);
	val = 60;
	inp.SetData(&val,1);
	val = 120;
	inw.SetData(&val,1);
	val = 240;
	inwl.SetData(&val,1);
	val = 0.01;
	inkb.SetData(&val,1);
	val = 5;
	inks.SetData(&val,1);
	val =0.01;
	inkm.SetData(&val,1);
	val =5;
	inul.SetData(&val,1);
	val =2;
	indl.SetData(&val,1);
*/	


	val = 120;
	insp.SetData(&val,1);
	val = 60;
	inp.SetData(&val,1);
	val = 120;
	inw.SetData(&val,1);
	val = 240;
	inwl.SetData(&val,1);
	val = 1.8;
	inkb.SetData(&val,1);
	val = 60;
	inks.SetData(&val,1);
	val =5.7;
	inkm.SetData(&val,1);
	val =3;
	inul.SetData(&val,1);
	val =3;
	indl.SetData(&val,1);


	IniMethod(1, mwErrorCode,
		mwOpen, mwHigh, mwLow, mwClose,
		 intime,
		 insp,
		 inp,
		 inw,
		 inwl,
		 inkb,
		 inks,
		 inkm,
		 inul,
		 indl);

	mwErrorCode.GetData(&val,1);
                                     
	delete []openPrice;
	delete []highPrice;
	delete []lowPrice;
	delete []closePrice;
	delete []intimeA;

  

	return TRUE;
}