// MessageRouter.h: interface for the MessageRouter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGEROUTER_H__E15C451A_729C_41C4_90A2_4E14A8C12B2B__INCLUDED_)
#define AFX_MESSAGEROUTER_H__E15C451A_729C_41C4_90A2_4E14A8C12B2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Algorithm.h"
class Algorithm;
class CompositeAlgorithm;
class MessageRouter  
{
public:
	void AddAlgorithm(string algo_name, string instrument, int slot, string config_file="");
	void AddCompositeAlgorithm(string algo_name, string instrument, 
								 int slot, vector<string> algo_list, double max_loss);
	virtual ~MessageRouter();
	void sendData(const CMinuteData& data);
	void sendData(const CTenMinuteData& data);
	void sendData(const CHalfMinuteData& data);
	void sendData(const CThostFtdcDepthMarketDataField& data);
	void sendData(const CThostFtdcTradeField& data);
	void sendData(const CThostFtdcTradingAccountField& data);
	void sendData(const CThostFtdcInvestorPositionField& data);
	static MessageRouter Router;
	void InitAlgorithm();
	MessageRouter(string broker_id, string investor_id);
private:
	struct AlgoInstrumentPair
	{
		string AlgoName;
		string Instrument;
		string config_file;
		int slot;
	};

	struct CompAlgoInfo
	{
		string Name;
		string Instrument;
		int slot;
		double max_loss;
		vector<AlgoInstrumentPair> AlgoList;
	};

	Algorithm* createAlgorithm(AlgoInstrumentPair algoInstrument);
	CompositeAlgorithm* createCompositeAlgorithm(CompAlgoInfo compAlgoInfo);

	vector<Algorithm*> m_algorithms;

	vector<AlgoInstrumentPair> m_algoInstrument;
	vector<CompAlgoInfo> m_compAlgoInfo;

	string m_BrokerId;
	string m_InvestorId;
};

#endif // !defined(AFX_MESSAGEROUTER_H__E15C451A_729C_41C4_90A2_4E14A8C12B2B__INCLUDED_)
