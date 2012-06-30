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
	m_InstrumentID(instrument_id), m_Amount(0), m_log("c:\\random_algo_data.log",ios::app)
{
	totalAmount=0;
	totalMoney=0;
}

RandomAlgorithm::~RandomAlgorithm()
{
	m_log.close();
}
int kkk=-1;
void RandomAlgorithm::OnMinuteData(const CMinuteData& data)
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
	trm.SetData(&val,1);
	tre.SetData(&val,1);



/*	MinKsymbProcess(6, newopen, newm, newe, newrm, newre
                    , bidprice, to, th, tl, tc, trm, tre);

*/

	OrderInfoShort res;

//	bidprice.GetData(&(res.price), 1);
	double amount;
//	newopen.GetData(&(amount), 1);


	amount = kkk;
	if (kkk==1)
	{
		kkk=-1;
	}
	else
	{
		kkk=1;
	}

	res.amount = amount;
	res.day= data.m_Day;
	res.time = data.m_Time;
	if( res.amount < 0)
		res.price = m_BidPrice;
	else
		res.price = m_AskPrice;
	res.milliSec =0;
	res.m_instrumentID = data.m_InstrumentID;
	res.totalAmount = newrm;
	totalAmount += amount;

	if( res.time=="15:13:00")
	{
		res.amount=-totalAmount;
		res.totalAmount=0;
	}

	

	if( res.amount != 0 )
		SendStrategy(res);
}

void RandomAlgorithm::OnHalfMinuteData(const CHalfMinuteData& data)
{
	int i = 40;
}
int	RandomAlgorithm::SendStrategy(const OrderInfoShort & res)
{
	//第一行就是真实的发送指令，第二行是本地模拟写log
	Algorithm::SendStrategy(res);	
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
	kkk=1;
	/*CTime currTime = CTime::GetCurrentTime();
	int year = currTime.GetYear();
	int month = currTime.GetMonth();
	int day = currTime.GetDay();
	int hour = currTime.GetHour();
	int minute = currTime.GetMinute();
	int second = currTime.GetSecond();

	int dayOfWeek = currTime.GetDayOfWeek();

	int backtrack = 2;

	if (dayOfWeek == 1)//sunday
		backtrack = 3;
	else if ( dayOfWeek <= 3 )//monday tuesday
		backtrack = 4;


	char currDayBuffer[20], currTimeBuffer[20], startDayBuffer[20], startTimeBuffer[20];
	sprintf(currDayBuffer, "%d%02d%02d", year, month, day);
	sprintf(currTimeBuffer, "%02d:%02d:%02d", hour, minute, second);

	CTimeSpan timeSpan(backtrack, 0,0,0);
	CTime startTime = currTime-timeSpan;
	int s_year = startTime.GetYear();
	int s_month = startTime.GetMonth();
	int s_day = startTime.GetDay();
	int s_hour = startTime.GetHour();
	int s_minute = startTime.GetMinute();
	int s_second = startTime.GetSecond();

	sprintf(startDayBuffer, "%d%02d%02d", s_year, s_month, s_day);
	sprintf(startTimeBuffer, "%02d:%02d:%02d", s_hour, s_minute, s_second);*/

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

	mwArray mwErrorCode(1,1, mxDOUBLE_CLASS);
	mwArray mwOpen( size, 1,mxDOUBLE_CLASS); 
	mwArray mwHigh( size, 1,mxDOUBLE_CLASS); 
	mwArray mwLow(size, 1, mxDOUBLE_CLASS); 
	mwArray mwClose( size,1, mxDOUBLE_CLASS); 
	// 1，1表示矩阵的大小（所有maltab只有一种变量，就是矩阵，为了和Cpp变量接轨，设置成1*1的矩阵，mxDOUBLE_CLASS表示变量的精度）   

	mwArray inm(size,1, mxDOUBLE_CLASS);
	mwArray inrm(size,1, mxDOUBLE_CLASS);
	mwArray ine(size,1, mxDOUBLE_CLASS);
	mwArray inre(size,1, mxDOUBLE_CLASS);

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
	int * zaa=new int[size];

	// set data，不用我解释了吧，很简单的，调用类里面的SetData函数给类赋值   
	for( int i=0; i< size; i++ )
	{
		openPrice[i] = m_historyData[i].m_OpenPrice;
		highPrice[i] = m_historyData[i].m_HighPrice;
		lowPrice[i] = m_historyData[i].m_LowPrice;
		closePrice[i] = m_historyData[i].m_ClosePrice;
		zaa[i]=0;
	}

	mwOpen.SetData(openPrice, size);
	mwHigh.SetData(highPrice, size);
	mwLow.SetData(lowPrice, size);
	mwClose.SetData(closePrice, size);
	inm.SetData(zaa, size);
	inrm.SetData(zaa, size);
	ine.SetData(closePrice, size);
	inre.SetData(closePrice, size);
	mwErrorCode.SetData(&val,1);
	val = 60;
	insp.SetData(&val,1);
	val = 50;
	inp.SetData(&val,1);
	val = 50;
	inw.SetData(&val,1);
	val = 70;
	inwl.SetData(&val,1);
	val = 0;
	inkb.SetData(&val,1);
	val = 40;
	inks.SetData(&val,1);
	val =0;
	inkm.SetData(&val,1);
	val =4;
	inul.SetData(&val,1);
	val =2;
	indl.SetData(&val,1);	

	IniMethod(1, mwErrorCode,
		mwOpen, mwHigh, mwLow, mwClose,
		 inm,
		 inrm,
		 ine,
		 inre,
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
	delete []zaa;

  

	return TRUE;
}