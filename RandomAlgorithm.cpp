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
	m_InstrumentID(instrument_id), m_Amount(0), m_log("c:\\random_algo_data.log")
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
void RandomAlgorithm::OnHalfMinuteData(const CHalfMinuteData& data)
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
	val = data.m_Volume-lastVol;
//	val = data.m_Volume;
	lastVol = data.m_Volume;
	tv.SetData(&val,1);
	trm.SetData(&val,1);
	tre.SetData(&val,1);



	MinKsymbProcess(6, newopen, newm, newe, newrm, newre
                    , bidprice, to, th, tl, tc, tv, trm, tre);



	OrderInfoShort res;

	bidprice.GetData(&(res.price), 1);
	double amount;
	newopen.GetData(&(amount), 1);

	res.amount = amount;
	//mkk = -mkk;
	res.day= data.m_Day;
	res.time = data.m_Time;
	res.milliSec =0;
	res.m_instrumentID = data.m_InstrumentID;

/*	if (res.amount<0)
		res.price = res.price+5;
	else
		res.price = res.price-5;*/

	res.totalAmount = newrm;
	totalAmount += amount;


	if( res.time>"15:13:29")
	{
		res.amount=-totalAmount;
		res.totalAmount=0;
	}


	if (amount>0)
	{
		res.price=m_AskPrice;
	}
	if(amount<0)
	{
		res.price=m_BidPrice;
	}

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
	double val;
	val = data.AskPrice1;
	mwArray ia(1,1, mxDOUBLE_CLASS);
	ia.SetData(&val,1);

	val = data.BidPrice1;
	mwArray ib(1,1, mxDOUBLE_CLASS);
	ib.SetData(&val,1);

	val = data.AskVolume1;
	mwArray iav(1,1, mxDOUBLE_CLASS);
	iav.SetData(&val,1);

	val = data.BidVolume1;
	mwArray ibv(1,1, mxDOUBLE_CLASS);
	ibv.SetData(&val,1);

	mwArray flag(1,1, mxDOUBLE_CLASS);


	TransPriAndVol(1, flag,ia,ib,iav,ibv);
                                                  
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
	


/*	val = 120;
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
	val =5;
	inul.SetData(&val,1);
	val =3;
	indl.SetData(&val,1);

*/
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