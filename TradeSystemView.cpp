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

extern TradeConn* tradeConn;
extern char *ppInstrumentID[30];			// 行情订阅列表
extern int iInstrumentID;									// 行情订阅数量
extern char HS300_URL[];
/////////////////////////////////////////////////////////////////////////////
// CTradeSystemView

IMPLEMENT_DYNCREATE(CTradeSystemView, CFormView)

BEGIN_MESSAGE_MAP(CTradeSystemView, CFormView)
	//{{AFX_MSG_MAP(CTradeSystemView)
	ON_BN_CLICKED(IDC_SIMU_START, OnSimuStart)
	ON_BN_CLICKED(IDC_CLEAR_SHORT, OnClearShort)
	ON_BN_CLICKED(IDC_CLEAR_LONG, OnClearLong)
	ON_WM_TIMER()
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

}

CTradeSystemView::~CTradeSystemView()
{
}

void CTradeSystemView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradeSystemView)
	DDX_Control(pDX, IDC_ORDER_LIST, m_OrderList);
	DDX_Control(pDX, IDC_TRADE_LIST, m_TradeList);
	DDX_Control(pDX, IDC_POSITION_LIST, m_PositionList);
	DDX_Control(pDX, IDC_INSTRUMENT_LIST, m_InstrumentList);
	DDX_Control(pDX, IDC_ACCOUNT_LIST, m_AccountList);
	DDX_Control(pDX, IDC_DATETIME_END, m_DateEnd);
	DDX_Control(pDX, IDC_DATETIME_START, m_DateStart);
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

	m_RefreshFormTimer = SetTimer(1, 1000, 0);	
	m_RefreshPosTimer  = SetTimer(2, 10000, 0);	
	m_CorrectionPosTimer  = SetTimer(3, 20000, 0);
	m_HS300Timer  = SetTimer(4, 500, 0);
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
	tradeConn->m_TradeSpi->CancelAllOrders((LPCSTR)instrument);
	tradeConn->m_TradeSpi->ClearShortPos((LPCSTR)instrument, m_AskPrice);
}

void CTradeSystemView::OnClearLong() 
{
	// TODO: Add your control notification handler code here
	CString instrument =GetActiveInstrument();;
	tradeConn->m_TradeSpi->CancelAllOrders((LPCSTR)instrument);
	tradeConn->m_TradeSpi->ClearLongPos((LPCSTR)instrument, m_BidPrice);	
}

BOOL CTradeSystemView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(m_RefreshFormTimer);
	KillTimer(m_RefreshPosTimer);
	KillTimer(m_CorrectionPosTimer);
	KillTimer(m_HS300Timer);
	return CFormView::DestroyWindow();
}

void CTradeSystemView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString instrument=GetActiveInstrument();

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

	if( nIDEvent == m_RefreshPosTimer )
	{
		tradeConn->m_TradeSpi->ReqQryTradingAccount();
	}
	else if (nIDEvent == m_RefreshFormTimer)
	{
		RefreshForm();
	}
	else if (m_CorrectionPosTimer == nIDEvent)
	{
		static errorInterval =0;
		if( pos.Long-pos.Short != tradeConn->m_TradeSpi->m_AlgoPos )
		{
			errorInterval++;

			if ( errorInterval >= 3 )
			{

				tradeConn->m_TradeSpi->CancelAllOrders((LPCSTR)instrument);
				
				OrderInfoShort orderShort;
				orderShort.m_instrumentID = (LPCSTR)instrument;
				orderShort.amount = tradeConn->m_TradeSpi->m_AlgoPos-(pos.Long-pos.Short);
				
				if ( orderShort.amount > 0 )
				{
					orderShort.price = m_AskPrice;
				}
				else
				{
					orderShort.price = m_BidPrice;				
				}
				errorInterval =0;
				tradeConn->m_TradeSpi->ReqOrderInsert(orderShort, true);
			}
		}
		else
		{
			errorInterval =0;
		}
	}
	else if( m_HS300Timer == nIDEvent )
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
				tradeConn->m_UserSpi->OnRtnDepthMarketData(&data);
			}
		}
	}
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
	
	m_AccountList.SetItemText(0,0,tradeConn->m_TradeSpi->m_account.BrokerID);
	m_AccountList.SetItemText(0,1,tradeConn->m_TradeSpi->m_account.AccountID);
	
	str.Format("%f", tradeConn->m_TradeSpi->m_account.Balance);
	m_AccountList.SetItemText(0,2,str);
	
	str.Format("%f", tradeConn->m_TradeSpi->m_account.Available);
	m_AccountList.SetItemText(0,3,str);
	
	str.Format("%f", tradeConn->m_TradeSpi->m_account.WithdrawQuota);
	m_AccountList.SetItemText(0,4,str);
	
	str.Format("%f", tradeConn->m_TradeSpi->m_account.FrozenCash);
	m_AccountList.SetItemText(0,5,str);
	
	str.Format("%f", tradeConn->m_TradeSpi->m_account.CloseProfit);
	m_AccountList.SetItemText(0,6,str);
	
	str.Format("%f", tradeConn->m_TradeSpi->m_account.PositionProfit);
	m_AccountList.SetItemText(0,7,str);
	
	str.Format("%f", tradeConn->m_TradeSpi->m_account.CurrMargin);
	m_AccountList.SetItemText(0,8,str);
	
	int instrument_count = m_InstrumentList.GetItemCount();
	
	for ( int i=0; i< instrument_count; i++)
	{
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
		str.Format("%f", tick_data.LastPrice);
		m_InstrumentList.SetItemText(i, 2, str );
		str.Format("%f", tick_data.BidPrice1);
		m_InstrumentList.SetItemText(i, 3, str );
		str.Format("%u", tick_data.BidVolume1);
		m_InstrumentList.SetItemText(i, 4, str );
		str.Format("%f", tick_data.AskPrice1);
		m_InstrumentList.SetItemText(i, 5, str );
		str.Format("%u", tick_data.AskVolume1);
		m_InstrumentList.SetItemText(i, 6, str );
		str.Format("%u", tick_data.Volume);
		m_InstrumentList.SetItemText(i, 7, str );
		str.Format("%f", tick_data.OpenInterest);
		m_InstrumentList.SetItemText(i, 8, str );
		str.Format("%f", tick_data.OpenPrice);
		m_InstrumentList.SetItemText(i, 9, str );
		str.Format("%f", tick_data.PreSettlementPrice);
		m_InstrumentList.SetItemText(i, 10, str );
		str.Format("%f", tick_data.HighestPrice);
		m_InstrumentList.SetItemText(i, 11, str );
		str.Format("%f", tick_data.LowestPrice);
		m_InstrumentList.SetItemText(i, 12, str );
		str.Format("%f", tick_data.PreClosePrice);
		m_InstrumentList.SetItemText(i, 13, str );
		str.Format("%f", tick_data.Turnover);
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
	m_AccountList.InsertColumn(0, _T("经纪代码"), LVCFMT_LEFT, nColInterval);
	m_AccountList.InsertColumn(1, _T("帐号"), LVCFMT_LEFT, nColInterval);
	m_AccountList.InsertColumn(2, _T("动态权益"), LVCFMT_LEFT, nColInterval);
	m_AccountList.InsertColumn(3, _T("可用资金"), LVCFMT_LEFT, nColInterval);
	m_AccountList.InsertColumn(4, _T("可取资金"), LVCFMT_LEFT, nColInterval);
	m_AccountList.InsertColumn(5, _T("冻结资金"), LVCFMT_LEFT, nColInterval);
	m_AccountList.InsertColumn(6, _T("平仓盈亏"), LVCFMT_LEFT, nColInterval);
	m_AccountList.InsertColumn(7, _T("持仓盈亏"), LVCFMT_LEFT, nColInterval);
	m_AccountList.InsertColumn(8, _T("保证金"), LVCFMT_LEFT, rect.Width()-8*nColInterval);

	m_AccountList.InsertItem(0,"");

	exstyle = m_InstrumentList.GetExtendedStyle();
	m_InstrumentList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_InstrumentList.GetClientRect(&rect);
       nColInterval = rect.Width()/16;
	m_InstrumentList.InsertColumn(0, _T("合约"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(1, _T("更新时间"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(2, _T("最新价"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(3, _T("买一价"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(4, _T("买一量"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(5, _T("卖一价"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(6, _T("卖一量"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(7, _T("成交量"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(8, _T("持仓量"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(9, _T("今开盘"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(10, _T("昨结算"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(11, _T("最高价"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(12, _T("最低价"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(13, _T("昨收盘"), LVCFMT_LEFT, nColInterval);
	m_InstrumentList.InsertColumn(14, _T("成交额"), LVCFMT_LEFT, rect.Width()-14*nColInterval);

	exstyle = m_PositionList.GetExtendedStyle();
	m_PositionList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_PositionList.GetClientRect(&rect);
    nColInterval = rect.Width()/13;
	m_PositionList.InsertColumn(0, _T("合约"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(1, _T("总空仓"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(2, _T("昨空仓"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(3, _T("今空仓"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(4, _T("总多仓"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(5, _T("昨多仓"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(6, _T("今多仓"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(7, _T("持仓成本(空)"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(8, _T("持仓成本(多)"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(9, _T("持仓盈亏(空)"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(10, _T("持仓盈亏(多)"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(11, _T("占用保证金(空)"), LVCFMT_LEFT, nColInterval);
	m_PositionList.InsertColumn(12, _T("占用保证金(多)"), LVCFMT_LEFT, rect.Width()-12*nColInterval);

	int i =0;
	for (  i =0 ; i< iInstrumentID; i++)
	{
		m_InstrumentList.InsertItem(i, ppInstrumentID[i]);
		m_PositionList.InsertItem(i, ppInstrumentID[i]);
	}

	m_InstrumentList.InsertItem(i+1, "沪深300");

	exstyle = m_OrderList.GetExtendedStyle();
	m_OrderList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_OrderList.GetClientRect(&rect);
    	nColInterval = rect.Width()/10;
	m_OrderList.InsertColumn(0, _T("报单编号"), LVCFMT_LEFT, nColInterval);
	m_OrderList.InsertColumn(1, _T("合约"), LVCFMT_LEFT, nColInterval);
	m_OrderList.InsertColumn(2, _T("买卖"), LVCFMT_LEFT, nColInterval);
	m_OrderList.InsertColumn(3, _T("开平"), LVCFMT_LEFT, nColInterval);
	m_OrderList.InsertColumn(4, _T("报单手数"), LVCFMT_LEFT, nColInterval);
	m_OrderList.InsertColumn(5, _T("未成交手数"), LVCFMT_LEFT, nColInterval);
	m_OrderList.InsertColumn(6, _T("今成交手数"), LVCFMT_LEFT, nColInterval);
	m_OrderList.InsertColumn(7, _T("报单价格"), LVCFMT_LEFT, nColInterval);
	m_OrderList.InsertColumn(8, _T("报单时间"), LVCFMT_LEFT, nColInterval);
	m_OrderList.InsertColumn(9, _T("报单类型"), LVCFMT_LEFT, rect.Width()-9*nColInterval);

	exstyle = m_TradeList.GetExtendedStyle();
	m_TradeList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_TradeList.GetClientRect(&rect);
    	nColInterval = rect.Width()/9;
	m_TradeList.InsertColumn(0, _T("成交编号"), LVCFMT_LEFT, nColInterval);
	m_TradeList.InsertColumn(1, _T("报单编号"), LVCFMT_LEFT, nColInterval);
	m_TradeList.InsertColumn(2, _T("合约"), LVCFMT_LEFT, nColInterval);
	m_TradeList.InsertColumn(3, _T("买卖"), LVCFMT_LEFT, nColInterval);
	m_TradeList.InsertColumn(4, _T("开平"), LVCFMT_LEFT, nColInterval);
	m_TradeList.InsertColumn(5, _T("成交价格"), LVCFMT_LEFT, nColInterval);
	m_TradeList.InsertColumn(6, _T("成交手数"), LVCFMT_LEFT, nColInterval);
	m_TradeList.InsertColumn(7, _T("成交时间"), LVCFMT_LEFT, nColInterval);
	m_TradeList.InsertColumn(8, _T("成交类型"), LVCFMT_LEFT, rect.Width()-8*nColInterval);
}

void CTradeSystemView::UpdatePosition()
{
	int instrument_count = m_PositionList.GetItemCount();
	
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

		str.Format("%f", pos.ShortPositionCost+pos.YdShortPositionCost);
		m_PositionList.SetItemText(i, 7, str );

		str.Format("%f", pos.LongPositionCost+pos.YdLongPositionCost);
		m_PositionList.SetItemText(i, 8, str );

		str.Format("%f", pos.ShortPositionProfit+pos.YdShortPositionProfit);
		m_PositionList.SetItemText(i, 9, str );

		str.Format("%f", pos.LongPositionProfit+pos.YdLongPositionProfit);
		m_PositionList.SetItemText(i, 10, str );

		str.Format("%f", pos.ShortUseMargin+pos.YdShortUseMargin);
		m_PositionList.SetItemText(i, 11, str );

		str.Format("%f", pos.LongUseMargin+pos.YdLongUseMargin);
		m_PositionList.SetItemText(i, 12, str );
	}
}
