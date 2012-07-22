// HsAlgorithm.cpp: implementation of the HsAlgorithm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TradeSystem.h"
#include "HsAlgorithm.h"
#include <math.h>
#include "DbConn.h"
#include "matlab\\libMethod_2.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern DbAccessorPool dbAccessPool;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HsAlgorithm::HsAlgorithm(string instrument_id):
	m_InstrumentID(instrument_id), m_Amount(0), m_log("c:\\hs_algo_data.log", ios::app)
{
	lastVol=0;
	totalAmount=0;
	totalMoney=0;
	m_BidPrice=0;
	m_AskPrice=0;
}

HsAlgorithm::~HsAlgorithm()
{
	m_log.close();
}
int hs_mkk=1;
void HsAlgorithm::OnMinuteData(const CMinuteData& data)
{
	// Filter IF's minute K series
	if( m_InstrumentID == data.m_InstrumentID )
		return;

	double val = 0;
	mwArray newopen(1,1, mxDOUBLE_CLASS);

	mwArray th(1,1, mxDOUBLE_CLASS);
	mwArray tl(1,1, mxDOUBLE_CLASS);
	mwArray tc(1,1, mxDOUBLE_CLASS);
	


	val = data.m_HighPrice;
	th.SetData(&val,1);
	val = data.m_LowPrice;
	tl.SetData(&val,1);
	val = data.m_ClosePrice;
	tc.SetData(&val,1);

	MinKsymbProcess_hs(1, newopen, th, tl, tc);



	OrderInfoShort res;


	newopen.GetData(&(res.amount), 1);


	res.day= data.m_Day;
	res.time = data.m_Time;
	res.milliSec =0;
	res.m_instrumentID = m_InstrumentID;

/*	if (res.amount<0)
		res.price = res.price+5;
	else
		res.price = res.price-5;*/




	if( res.time>"14:55:29")
	{
		res.amount=-totalAmount;
		res.totalAmount=0;
	}

	if (res.amount>=0)
	{
		res.price=m_AskPrice;
	}
	if (res.amount<0)
	{
		res.price=m_BidPrice;
	}

	SendStrategy(res);

	totalAmount += res.amount;
}


void HsAlgorithm::OnHalfMinuteData(const CHalfMinuteData& data)
{

}
int	HsAlgorithm::SendStrategy(const OrderInfoShort & res)
{
	//第一行就是真实的发送指令，第二行是本地模拟写log
	if( res.amount != 0 )
	{
		Algorithm::SendStrategy(res);	
	}
	m_log<<res.m_instrumentID+",  "+res.day+" "+res.time<<",  Amount, "<< res.amount <<", Price, "<<res.price<<endl;

	return 0;
}

void HsAlgorithm::OnTickData(const CThostFtdcDepthMarketDataField& data)
{
	// record IF's latest price
	if( m_InstrumentID == data.InstrumentID )
	{
		m_AskPrice = data.AskPrice1;
		m_BidPrice = data.BidPrice1;    
	}
}

void HsAlgorithm::OnTradeData(const CThostFtdcTradeField& data)
{
}
void HsAlgorithm::OnAccountData(const CThostFtdcTradingAccountField& data)
{
}
void HsAlgorithm::OnPositionData(const CThostFtdcInvestorPositionField& data)
{
}

BOOL HsAlgorithm::InitInstance()
{
	string hs300_ins = "沪深300";
	this->RegisterInstrument(m_InstrumentID);
	this->RegisterInstrument(hs300_ins);
	try{
		DbConn conn(dbAccessPool);
		//conn.m_db->getData(m_InstrumentID, startDayBuffer, startTimeBuffer,
		//	currDayBuffer, currTimeBuffer, m_historyData);
		conn.m_db->getData(hs300_ins, 10, m_historyData);
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


	mwArray inrm(size,1, mxDOUBLE_CLASS);

	mwArray inre(size,1, mxDOUBLE_CLASS);

	mwArray intt(1,1, mxDOUBLE_CLASS);
	mwArray inf1(1,1, mxDOUBLE_CLASS);
	mwArray inf2(1,1, mxDOUBLE_CLASS);
	mwArray inf3(1,1, mxDOUBLE_CLASS);
	

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
	
	inrm.SetData(zaa, size);

	inre.SetData(closePrice, size);
	mwErrorCode.SetData(&val,1);


	val = 10;
	intt.SetData(&val,1);
	val = 4;
	inf1.SetData(&val,1);
	val = 6;
	inf2.SetData(&val,1);
	val = 3;
	inf3.SetData(&val,1);
	
	


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


extern LIB_libMethod_2_CPP_API void IniMethod_hs(int nargout
                                                 , mwArray& erroCode
                                                 , const mwArray& inph
                                                 , const mwArray& inpl
                                                 , const mwArray& inpc
                                                 , const mwArray& intt
                                                 , const mwArray& inmoney
                                                 , const mwArray& inm
                                                 , const mwArray& inf1
                                                 , const mwArray& inf2
                                                 , const mwArray& inf3);*/
	IniMethod_hs(1, mwErrorCode,
		mwHigh, mwLow, mwClose,
		intt,
		inre,
		inrm,		
		inf1,
		inf2,
		inf3);

	mwErrorCode.GetData(&val,1);
                                     
	delete []openPrice;
	delete []highPrice;
	delete []lowPrice;
	delete []closePrice;
	delete []zaa;

  

	return TRUE;
}