#pragma once
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "stdafx.h"
#include "OneMinuteData.h"
#include <map>
#include <iostream>
#include <fstream>


using namespace std;
class DbConn;
class CMdSpi : public CThostFtdcMdSpi
{
public:
	CMdSpi(CThostFtdcMdApi* api):m_pUserApi(api),m_requestID(0),m_log("c:\\marketdata.log",ios::app){}
	~CMdSpi();
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	
	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);


private:
	void genOneMinuteData(CThostFtdcDepthMarketDataField&);
	void genHalfMinuteData(CThostFtdcDepthMarketDataField&);
	void resetOneMinuteData(CMinuteData& , string instrument_id);
	void ReqUserLogin();
	void SubscribeMarketData();
	// 
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// UserApi����
	CThostFtdcMdApi* m_pUserApi;

	typedef map<string,CMinuteData> CMinuteDataMap;	
	typedef pair<string,CMinuteData> CMinuteDataPair;	
	CMinuteDataMap m_one_minute_data_map;
	typedef map<string,CHalfMinuteData> CHalfMinuteDataMap;	
	typedef pair<string,CHalfMinuteData> CHalfMinuteDataPair;	
	CHalfMinuteDataMap m_half_minute_data_map;
	typedef map<string,CThostFtdcDepthMarketDataField> CTickDataMap;	
	typedef pair<string,CThostFtdcDepthMarketDataField> CTickDataPair;
	CTickDataMap m_tick_data_map;
	ofstream m_log;
	int m_requestID;
	friend class CTradeSystemView;
};
