// TradeSystemView.cpp : implementation of the CTradeSystemView class
//

#include "stdafx.h"
#include "TradeSystem.h"
#include "TradeConn.h"
#include "TradeSystemDoc.h"
#include "TradeSystemView.h"
#include "DataSimulator.h"
#include "StockRetriever.h"
#include "Algorithm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char *ppInstrumentID[30];			// 行情订阅列表
extern int iInstrumentID;									// 行情订阅数量
extern char HS300_URL[];
extern CTradeSystemApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CTradeSystemView

IMPLEMENT_DYNCREATE(CTradeSystemView, CFormView)

BEGIN_MESSAGE_MAP(CTradeSystemView, CFormView)
//{{AFX_MSG_MAP(CTradeSystemView)
ON_BN_CLICKED(IDC_SIMU_START, OnSimuStart)
ON_BN_CLICKED(IDC_CLEAR_SHORT, OnClearShort)
ON_BN_CLICKED(IDC_CLEAR_LONG, OnClearLong)
ON_WM_TIMER()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ACCOUNT_LIST, OnItemchangedAccountList)
	//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemView construction/destruction

CTradeSystemView::CTradeSystemView()
: CFormView(CTradeSystemView::IDD)
{
	//{{AFX_DATA_INIT(CTradeSystemView)
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_instrument_error_cnt.insert(instrument_error_cnt_pair("", 0));
}

CTradeSystemView::~CTradeSystemView()
{
}

void CTradeSystemView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradeSystemView)
	DDX_Control(pDX, IDC_TRADE_TAB, m_TradeTab);
	DDX_Control(pDX, IDC_POSITION_LIST, m_PositionList);
	DDX_Control(pDX, IDC_INSTRUMENT_LIST, m_InstrumentList);
	DDX_Control(pDX, IDC_ACCOUNT_LIST, m_AccountList);
	DDX_Control(pDX, IDC_TRADE_STATUS, m_TradeStatus);
	DDX_Control(pDX, IDC_MD_STATUS, m_MDStatus);
	//}}AFX_DATA_MAP
}

BOOL CTradeSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return CFormView::PreCreateWindow(cs);
}

void CTradeSystemView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
	
	InitListCtrl();
	int nPageID =0;
	
	m_OrderTradeDlg.Create(IDD_ORDER_TRADE, this);
	
	m_AlgorithmCfgDlg.Create(IDD_ALGORITHM_DLG, this);
	
	m_TradeTab.AddSSLPage(_T("成交记录"), nPageID++, &m_OrderTradeDlg);
	m_TradeTab.AddSSLPage(_T("算法管理"), nPageID++, &m_AlgorithmCfgDlg);
	m_TradeTab.AddSSLPage(_T("资金管理"), nPageID++, IDD_RISK_DLG);
	m_TradeTab.AddSSLPage(_T("风险控制"), nPageID++, IDD_RISK_DLG);
	
	m_RefreshFormTimer = SetTimer(1, 4000, 0);	
	m_RefreshPosTimer  = SetTimer(2, 10000, 0);	
	m_CorrectionPosTimer  = SetTimer(3, 20000, 0);
	//m_HS300Timer  = SetTimer(4, 500, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemView printing

BOOL CTradeSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTradeSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTradeSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTradeSystemView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemView diagnostics

#ifdef _DEBUG
void CTradeSystemView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTradeSystemView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTradeSystemDoc* CTradeSystemView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTradeSystemDoc)));
	return (CTradeSystemDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemView message handlers

void CTradeSystemView::OnSimuStart() 
{
	// TODO: Add your control notification handler code here
	CTime time;
	m_DateStart.GetTime(time);
	int year = time.GetYear();
	int month = time.GetMonth();
	int day = time.GetDay();
	
	char endDayBuffer[20],  startDayBuffer[20];
	sprintf(startDayBuffer, "%d%02d%02d", year, month, day);
	
	m_DateEnd.GetTime(time);
	year = time.GetYear();
	month = time.GetMonth();
	day = time.GetDay();
	
	sprintf(endDayBuffer, "%d%02d%02d", year, month, day);
	
	CString str = GetActiveInstrument();
	DataSimulator simu((LPCSTR)str);
	simu.SetRange(startDayBuffer, endDayBuffer);
	simu.Start();
}

void CTradeSystemView::OnClearShort() 
{
	// TODO: Add your control notification handler code here
	
	
	CString instrument =GetActiveInstrument();
	TradeConn* tradeConn = this->GetCurrConn();

	if (tradeConn != NULL)
	{
		tradeConn->m_TradeSpi->CancelAllOrders((LPCSTR)instrument);

		CThostFtdcDepthMarketDataField tickData = 
			tradeConn->m_UserSpi->m_tick_data_map[(LPCSTR)instrument];
		tradeConn->m_TradeSpi->ClearShortPos((LPCSTR)instrument, tickData.AskPrice1);
	}
}

void CTradeSystemView::OnClearLong() 
{
	// TODO: Add your control notification handler code here
	CString instrument =GetActiveInstrument();;
	TradeConn* tradeConn = this->GetCurrConn();
	
	if (tradeConn != NULL)
	{
		tradeConn->m_TradeSpi->CancelAllOrders((LPCSTR)instrument);
		CThostFtdcDepthMarketDataField tickData = 
			tradeConn->m_UserSpi->m_tick_data_map[(LPCSTR)instrument];
		tradeConn->m_TradeSpi->ClearLongPos((LPCSTR)instrument, tickData.BidPrice1);	
	}
}

BOOL CTradeSystemView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(m_RefreshFormTimer);
	KillTimer(m_RefreshPosTimer);
	KillTimer(m_CorrectionPosTimer);
	//KillTimer(m_HS300Timer);
	return CFormView::DestroyWindow();
}

void CTradeSystemView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == m_RefreshPosTimer )
	{
		for(int i =0; i<theApp.GetConnCount(); i++)
		{
			TradeConn * tradeConn = GetConnById(i);
			tradeConn->m_TradeSpi->ReqQryTradingAccount();
		}
	}
	else if (nIDEvent == m_RefreshFormTimer)
	{
		RefreshForm();
	}
	else if (m_CorrectionPosTimer == nIDEvent)
	{
		CorrectAlgoPos();
	}
/*	else if( m_HS300Timer == nIDEvent )
	{
		static CThostFtdcDepthMarketDataField old_data;
		StockRetriever ret;
		CThostFtdcDepthMarketDataField data;
		memset(&data, 0, sizeof(data));
		bool isOk = ret.GetStockData(HS300_URL, data);
		if (isOk)
		{
			if( !(
				strcmp(data.TradingDay, old_data.TradingDay ) == 0 &&
				strcmp(data.UpdateTime, old_data.UpdateTime ) == 0
				) )
			{
				old_data = data;
				if (tradeConn != NULL)
					tradeConn->m_UserSpi->OnRtnDepthMarketData(&data);
			}
		}
	}*/
	CFormView::OnTimer(nIDEvent);
}

CTradeSystemView* CTradeSystemView::GetCurrView()
{
	CFrameWnd* pWnd = (CFrameWnd*)AfxGetMainWnd();
	
	if ( pWnd != NULL )
		return (CTradeSystemView*)pWnd->GetActiveView();
	else
		return NULL;
}

CString CTradeSystemView::GetActiveInstrument()
{
    POSITION pos = m_PositionList.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	
	int nItem = m_PositionList.GetNextSelectedItem(pos);
	
	return m_PositionList.GetItemText(nItem, 0);
}

void CTradeSystemView::RefreshForm()
{
	CString str;

	int accountCount = theApp.GetConnCount();
	int i=0;
	for (i=0; i< accountCount; i++)
	{
		TradeConn* tradeConn = GetConnById(i);
		m_AccountList.SetItemText(i,0,tradeConn->m_TradeSpi->m_account.BrokerID);
		m_AccountList.SetItemText(i,1,tradeConn->m_TradeSpi->m_account.AccountID);
		
		str.Format("%.2f", tradeConn->m_TradeSpi->m_account.Balance);
		m_AccountList.SetItemText(i,2,str);
		
		str.Format("%.2f", tradeConn->m_TradeSpi->m_account.Available);
		m_AccountList.SetItemText(i,3,str);
		
		str.Format("%.2f", tradeConn->m_TradeSpi->m_account.WithdrawQuota);
		m_AccountList.SetItemText(i,4,str);
		
		str.Format("%.2f", tradeConn->m_TradeSpi->m_account.FrozenCash);
		m_AccountList.SetItemText(i,5,str);
		
		str.Format("%.2f", tradeConn->m_TradeSpi->m_account.CloseProfit);
		m_AccountList.SetItemText(i,6,str);
		
		str.Format("%.2f", tradeConn->m_TradeSpi->m_account.PositionProfit);
		m_AccountList.SetItemText(i,7,str);
		
		str.Format("%.2f", tradeConn->m_TradeSpi->m_account.CurrMargin);
		m_AccountList.SetItemText(i,8,str);
	}
	int instrument_count = m_InstrumentList.GetItemCount();
	
	for (i=0; i< instrument_count; i++)
	{
		TradeConn* tradeConn = GetCurrConn();
	
		CString curr_ins = m_InstrumentList.GetItemText(i, 0);
		CThostFtdcDepthMarketDataField tick_data;
		memset(&tick_data, 0, sizeof(tick_data));
		
		CMdSpi::CTickDataMap::iterator tick_iter = 
			tradeConn->m_UserSpi->m_tick_data_map.find((LPCSTR)curr_ins);
		
		if ( tick_iter != tradeConn->m_UserSpi->m_tick_data_map.end() )
		{
			tick_data = tick_iter->second;
		}
		
		m_InstrumentList.SetItemText(i, 1, tick_data.UpdateTime );
		str.Format("%.2f", tick_data.LastPrice);
		m_InstrumentList.SetItemText(i, 2, str );
		str.Format("%.2f", tick_data.BidPrice1);
		m_InstrumentList.SetItemText(i, 3, str );
		str.Format("%u", tick_data.BidVolume1);
		m_InstrumentList.SetItemText(i, 4, str );
		str.Format("%.2f", tick_data.AskPrice1);
		m_InstrumentList.SetItemText(i, 5, str );
		str.Format("%u", tick_data.AskVolume1);
		m_InstrumentList.SetItemText(i, 6, str );
		str.Format("%u", tick_data.Volume);
		m_InstrumentList.SetItemText(i, 7, str );
		str.Format("%.2f", tick_data.OpenInterest);
		m_InstrumentList.SetItemText(i, 8, str );
		str.Format("%.2f", tick_data.OpenPrice);
		m_InstrumentList.SetItemText(i, 9, str );
		str.Format("%.2f", tick_data.PreSettlementPrice);
		m_InstrumentList.SetItemText(i, 10, str );
		str.Format("%.2f", tick_data.HighestPrice);
		m_InstrumentList.SetItemText(i, 11, str );
		str.Format("%.2f", tick_data.LowestPrice);
		m_InstrumentList.SetItemText(i, 12, str );
		str.Format("%.2f", tick_data.PreClosePrice);
		m_InstrumentList.SetItemText(i, 13, str );
		str.Format("%.2f", tick_data.Turnover);
		m_InstrumentList.SetItemText(i, 14, str );
	}
	
	UpdatePosition();
}

void CTradeSystemView::InitListCtrl()
{
	DWORD exstyle = m_AccountList.GetExtendedStyle();
	m_AccountList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | WS_EX_STATICEDGE );
	CRect rect;
	m_AccountList.GetClientRect(&rect);
	int nColInterval = rect.Width()/9;
	m_AccountList.InsertColumn(0, _T("经纪代码"), LVCFMT_CENTER, nColInterval);
	m_AccountList.InsertColumn(1, _T("帐号"), LVCFMT_CENTER, nColInterval);
	m_AccountList.InsertColumn(2, _T("动态权益"), LVCFMT_CENTER, nColInterval);
	m_AccountList.InsertColumn(3, _T("可用资金"), LVCFMT_CENTER, nColInterval);
	m_AccountList.InsertColumn(4, _T("可取资金"), LVCFMT_CENTER, nColInterval);
	m_AccountList.InsertColumn(5, _T("冻结资金"), LVCFMT_CENTER, nColInterval);
	m_AccountList.InsertColumn(6, _T("平仓盈亏"), LVCFMT_CENTER, nColInterval);
	m_AccountList.InsertColumn(7, _T("持仓盈亏"), LVCFMT_CENTER, nColInterval);
	m_AccountList.InsertColumn(8, _T("保证金"), LVCFMT_CENTER, rect.Width()-8*nColInterval);
	

	int accountCount = theApp.GetConnCount();
	vector<TradeConn*> conn_list;
	theApp.GetTradeConnList(conn_list);
	int i =0;
	for (i=0; i< accountCount; i++)
	{
		m_AccountList.InsertItem(i,conn_list[i]->m_BrokerId.c_str());
		m_AccountList.SetItemText(i,1,conn_list[i]->m_InvestorId.c_str());
	}

	m_AccountList.SetBkColor(RGB(BK_R,BK_G,BK_B));
	m_AccountList.SetHeaderHeight(1.5);          //设置头部高度
	m_AccountList.SetHeaderFontHW(16,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_AccountList.SetTextColor(RGB(TX_R,TX_G,TX_B));  //设置文本颜色
	m_AccountList.SetHeaderBKColor(head_BK_R,head_BK_G,head_BK_B,8); //设置头部背景色
	m_AccountList.SetFontHW(15,0);               //设置字体高度，和宽度,0表示缺省宽度
	
	exstyle = m_InstrumentList.GetExtendedStyle();
	m_InstrumentList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_InstrumentList.GetClientRect(&rect);
	nColInterval = rect.Width()/16;
	m_InstrumentList.InsertColumn(0, _T("合约"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(1, _T("更新时间"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(2, _T("最新价"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(3, _T("买一价"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(4, _T("买一量"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(5, _T("卖一价"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(6, _T("卖一量"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(7, _T("成交量"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(8, _T("持仓量"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(9, _T("今开盘"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(10, _T("昨结算"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(11, _T("最高价"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(12, _T("最低价"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(13, _T("昨收盘"), LVCFMT_CENTER, nColInterval);
	m_InstrumentList.InsertColumn(14, _T("成交额"), LVCFMT_CENTER, rect.Width()-14*nColInterval);



	m_InstrumentList.SetBkColor(RGB(BK_R,BK_G,BK_B));
	m_InstrumentList.SetHeaderHeight(1.5);          //设置头部高度
	m_InstrumentList.SetHeaderFontHW(16,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_InstrumentList.SetTextColor(RGB(TX_R,TX_G,TX_B));  //设置文本颜色
	m_InstrumentList.SetHeaderBKColor(head_BK_R,head_BK_G,head_BK_B,8); //设置头部背景色
	m_InstrumentList.SetFontHW(13,0);               //设置字体高度，和宽度,0表示缺省宽度

	exstyle = m_PositionList.GetExtendedStyle();
	m_PositionList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_PositionList.GetClientRect(&rect);
    nColInterval = rect.Width()/13;
	m_PositionList.InsertColumn(0, _T("合约"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(1, _T("总空仓"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(2, _T("昨空仓"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(3, _T("今空仓"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(4, _T("总多仓"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(5, _T("昨多仓"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(6, _T("今多仓"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(7, _T("持仓成本(空)"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(8, _T("持仓成本(多)"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(9, _T("持仓盈亏(空)"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(10, _T("持仓盈亏(多)"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(11, _T("占用保证金(空)"), LVCFMT_CENTER, nColInterval);
	m_PositionList.InsertColumn(12, _T("占用保证金(多)"), LVCFMT_CENTER, rect.Width()-12*nColInterval);

	m_PositionList.SetBkColor(RGB(BK_R,BK_G,BK_B));
	m_PositionList.SetHeaderHeight(1.5);          //设置头部高度
	m_PositionList.SetHeaderFontHW(13,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_PositionList.SetTextColor(RGB(TX_R,TX_G,TX_B));  //设置文本颜色
	m_PositionList.SetHeaderBKColor(head_BK_R,head_BK_G,head_BK_B,8); //设置头部背景色
	m_PositionList.SetFontHW(13,0);               //设置字体高度，和宽度,0表示缺省宽度

	for (i =0 ; i< iInstrumentID; i++)
	{
		m_InstrumentList.InsertItem(i, ppInstrumentID[i]);
		m_PositionList.InsertItem(i, ppInstrumentID[i]);
	}
	
	m_InstrumentList.InsertItem(i+1, "沪深300");
	
	
}

void CTradeSystemView::UpdatePosition()
{
	int instrument_count = m_PositionList.GetItemCount();
	
	TradeConn* tradeConn = GetCurrConn();
	for ( int i=0; i< instrument_count; i++)
	{
		CString instrument = m_PositionList.GetItemText(i, 0);
		
		InvestorPosition pos;
		memset(&pos, 0, sizeof(pos));
		
		if( tradeConn == NULL )
			return;
		
		CTraderSpi::inv_pos_map::iterator pos_iter = 
			tradeConn->m_TradeSpi->m_inv_pos.find((LPCSTR)instrument);
		
		if ( pos_iter != tradeConn->m_TradeSpi->m_inv_pos.end() )
		{
			pos = pos_iter->second;
		}
		CString str;
		str.Format("%u", pos.Short+pos.YdShort);
		m_PositionList.SetItemText(i, 1, str );
		str.Format("%u", pos.YdShort);
		m_PositionList.SetItemText(i, 2, str );
		str.Format("%u", pos.Short);
		m_PositionList.SetItemText(i, 3, str );
		
		str.Format("%u", pos.Long+pos.YdLong);
		m_PositionList.SetItemText(i, 4, str );
		str.Format("%u", pos.YdLong);
		m_PositionList.SetItemText(i, 5, str );
		str.Format("%u", pos.Long);
		m_PositionList.SetItemText(i, 6, str );
		
		str.Format("%.2f", pos.ShortPositionCost+pos.YdShortPositionCost);
		m_PositionList.SetItemText(i, 7, str );
		
		str.Format("%.2f", pos.LongPositionCost+pos.YdLongPositionCost);
		m_PositionList.SetItemText(i, 8, str );
		
		str.Format("%.2f", pos.ShortPositionProfit+pos.YdShortPositionProfit);
		m_PositionList.SetItemText(i, 9, str );
		
		str.Format("%.2f", pos.LongPositionProfit+pos.YdLongPositionProfit);
		m_PositionList.SetItemText(i, 10, str );
		
		str.Format("%.2f", pos.ShortUseMargin+pos.YdShortUseMargin);
		m_PositionList.SetItemText(i, 11, str );
		
		str.Format("%.2f", pos.LongUseMargin+pos.YdLongUseMargin);
		m_PositionList.SetItemText(i, 12, str );
	}
}

void CTradeSystemView::CorrectAlgoPos()
{
	for (int j=0; j< theApp.GetConnCount(); j++)
	{
		TradeConn* tradeConn = GetConnById(j);
		for (int i=0; i<iInstrumentID; i++)
		{
			CString instrument=ppInstrumentID[i];
			
			InvestorPosition pos;
			memset(&pos, 0, sizeof(pos));
			
			if( tradeConn == NULL )
				return;
			
			CTraderSpi::inv_pos_map::iterator pos_iter = 
				tradeConn->m_TradeSpi->m_inv_pos.find((LPCSTR)instrument);
			
			if ( pos_iter != tradeConn->m_TradeSpi->m_inv_pos.end() )
			{
				pos = pos_iter->second;
			}
			
			int algoPos =0;
			if ( tradeConn->m_TradeSpi->m_algo_instrument_pos.find((LPCSTR)instrument) 
				!= tradeConn->m_TradeSpi->m_algo_instrument_pos.end() )
			{
				algoPos = tradeConn->m_TradeSpi->m_algo_instrument_pos[(LPCSTR)instrument];
			}
			
			if( pos.Long-pos.Short !=  algoPos)
			{
				if(m_instrument_error_cnt.find((LPCSTR)instrument) != 
					m_instrument_error_cnt.end())
				{
					int new_cnt = m_instrument_error_cnt[(LPCSTR)instrument]+1;
					m_instrument_error_cnt[(LPCSTR)instrument] = new_cnt;
				}
				else
				{
					m_instrument_error_cnt.insert(instrument_error_cnt_pair((LPCSTR)instrument, 1));
				}
				
				if ( m_instrument_error_cnt[(LPCSTR)instrument] >= 3 )
				{
					
					tradeConn->m_TradeSpi->CancelAllOrders((LPCSTR)instrument);
					
					OrderInfoShort orderShort;
					orderShort.m_instrumentID = (LPCSTR)instrument;
					orderShort.amount = 
						algoPos
						-(pos.Long-pos.Short);
					
					if ( orderShort.amount > 0 )
					{
						orderShort.price = tradeConn->m_UserSpi->m_tick_data_map[(LPCSTR)instrument].AskPrice1;
					}
					else
					{
						orderShort.price = tradeConn->m_UserSpi->m_tick_data_map[(LPCSTR)instrument].BidPrice1;				
					}
					m_instrument_error_cnt.erase((LPCSTR)instrument);
					tradeConn->m_TradeSpi->ReqOrderInsert(orderShort, true);
				}
			}
			else
			{
				m_instrument_error_cnt.erase((LPCSTR)instrument);
			}
		}
	}
}

TradeConn* CTradeSystemView::GetCurrConn()
{
    int nItem =0;
	POSITION pos = m_AccountList.GetFirstSelectedItemPosition();
	
	if (pos != NULL)
		nItem = m_AccountList.GetNextSelectedItem(pos);
	
	return GetConnById(nItem);
}

TradeConn* CTradeSystemView::GetConnById(int nItem)
{
	CString broker = m_AccountList.GetItemText(nItem, 0);
	CString investor = m_AccountList.GetItemText(nItem, 1);
	CTradeSystemApp* app =(CTradeSystemApp*)AfxGetApp();
	TradeConn* conn = theApp.GetTradeConn((LPCSTR)broker, (LPCSTR)investor);
	ASSERT(conn != NULL);
	return conn;
}

void CTradeSystemView::OnItemchangedAccountList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
    if(pNMListView->uChanged == LVIF_STATE)
    {
		if(pNMListView->uNewState)
		{
			TradeConn* conn = GetCurrConn();

			if(conn != NULL)
			{
				conn->m_TradeSpi->ShowTradeDetail();
			}
           //TRACE0("选择改变且有选中的行\r\n");
		}
        else
		{
           //TRACE0("选择改变且没有选中的行\r\n");
		}
    }
    else
	{
		//item content change. Not state change.
	}
	*pResult = 0;
}
