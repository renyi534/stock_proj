#pragma once
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "stdafx.h"
#include "OneMinuteData.h"
#include <map>
#include <vector>
#include <iostream>
#include <fstream>


using namespace std;
class DbConn;
class KSeriesGenerator;
struct TradeConn;
class CMdSpi : public CThostFtdcMdSpi
{
public:
	CMdSpi(CThostFtdcMdApi* api, string, string, string, TradeConn*);
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

	void ReqUserLogin();
	void SubscribeMarketData();
	// 
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// UserApi����
	CThostFtdcMdApi* m_pUserApi;


	typedef map<string,CThostFtdcDepthMarketDataField> CTickDataMap;	
	typedef pair<string,CThostFtdcDepthMarketDataField> CTickDataPair;
	CTickDataMap m_tick_data_map;


	ofstream m_log;
	int m_requestID;
	friend class CTradeSystemView;

	vector<KSeriesGenerator*> m_series_generator;
	string m_BrokerId;
	string m_InvestorId;
	string m_Passwd;
	TradeConn* m_Conn;
	CRITICAL_SECTION		m_data_critsec;
	
};
