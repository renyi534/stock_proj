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
		// NOTE: the ClassWizard will add member initialization here
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
	DDX_Control(pDX, IDC_CLOSE_PROFIT, m_CloseProfit);
	DDX_Control(pDX, IDC_POS_PROFIT, m_PositionProfit);
	DDX_Control(pDX, IDC_BALANCE, m_Balance);
	DDX_Control(pDX, IDC_YDSHORT_POS, m_YdShortPos);
	DDX_Control(pDX, IDC_YDLONG_POS, m_YdLongPos);
	DDX_Control(pDX, IDC_ALGO_POS, m_AlgoPos);
	DDX_Control(pDX, IDC_SHORT_POS, m_ShortPos);
	DDX_Control(pDX, IDC_LONG_POS, m_LongPos);
	DDX_Control(pDX, IDC_DATETIME_END, m_DateEnd);
	DDX_Control(pDX, IDC_DATETIME_START, m_DateStart);
	DDX_Control(pDX, IDC_VOLUME, m_Volume);
	DDX_Control(pDX, IDC_TRADE_TOTAL, m_TradeTotal);
	DDX_Control(pDX, IDC_TRADE_STATUS, m_TradeStatus);
	DDX_Control(pDX, IDC_TRADE_DETAIL, m_TradeDetail);
	DDX_Control(pDX, IDC_PRICE, m_Price);
	DDX_Control(pDX, IDC_ORDER_TOTAL, m_OrderTotal);
	DDX_Control(pDX, IDC_ORDER_DETAIL, m_OrderDetail);
	DDX_Control(pDX, IDC_OPEN_INTEREST, m_OpenInterest);
	DDX_Control(pDX, IDC_MD_STATUS, m_MDStatus);
	DDX_Control(pDX, IDC_CASH, m_Cash);
	DDX_Control(pDX, IDC_INSTRUMENT_COMBO, m_Instruments);
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
	ResizeParentToFit();
	for ( int i =0 ; i< iInstrumentID; i++)
		this->m_Instruments.AddString(ppInstrumentID[i]);
	
	m_Instruments.AddString("沪深300");
	m_Instruments.SetCurSel(0);
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
	
	CString str;
	m_Instruments.GetWindowText(str);
	DataSimulator simu((LPCSTR)str);
	simu.SetRange(startDayBuffer, endDayBuffer);
	simu.Start();
}

void CTradeSystemView::OnClearShort() 
{
	// TODO: Add your control notification handler code here


	CString instrument;
	m_Instruments.GetWindowText(instrument);
	tradeConn->m_TradeSpi->CancelAllOrders((LPCSTR)instrument);
	tradeConn->m_TradeSpi->ClearShortPos((LPCSTR)instrument, m_AskPrice);
}

void CTradeSystemView::OnClearLong() 
{
	// TODO: Add your control notification handler code here
	CString instrument;
	m_Instruments.GetWindowText(instrument);
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
	CString instrument ;
	m_Instruments.GetWindowText(instrument);
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

	CThostFtdcDepthMarketDataField tick_data;
	memset(&tick_data, 0, sizeof(tick_data));

	CMdSpi::CTickDataMap::iterator tick_iter = 
		tradeConn->m_UserSpi->m_tick_data_map.find((LPCSTR)instrument);

	if ( tick_iter != tradeConn->m_UserSpi->m_tick_data_map.end() )
	{
		tick_data = tick_iter->second;
	}

	if( nIDEvent == m_RefreshPosTimer )
	{
		tradeConn->m_TradeSpi->ReqQryTradingAccount();
	}
	else if (nIDEvent == m_RefreshFormTimer)
	{
		CString str;
		str.Format("Algorithm Position:%d", tradeConn->m_TradeSpi->m_AlgoPos);
		this->m_AlgoPos.SetWindowText(str);

		str.Format("Long Position:%d", pos.Long);
		this->m_LongPos.SetWindowText(str);
		
		str.Format("Short Position:%d", pos.Short);
		this->m_ShortPos.SetWindowText(str);
		
		str.Format("Yd Short Position:%d", pos.YdShort);
		this->m_YdShortPos.SetWindowText(str);
		
		str.Format("Yd Long Position:%d", pos.YdLong);
		this->m_YdLongPos.SetWindowText(str);

		str.Format("Cash:%f", tradeConn->m_TradeSpi->m_account.Available);
		m_Cash.SetWindowText(str);

		str.Format("Balance:%f", tradeConn->m_TradeSpi->m_account.Balance);
		m_Balance.SetWindowText(str);

		str.Format("Position_Profit:%f", 
			tradeConn->m_TradeSpi->m_account.PositionProfit);
		m_PositionProfit.SetWindowText(str);

		str.Format("Close_Profit:%f", 
			tradeConn->m_TradeSpi->m_account.CloseProfit);
		m_CloseProfit.SetWindowText(str);

		str.Format("Curr Price: %f", tick_data.LastPrice);
		m_Price.SetWindowText( str);
		str.Format("Curr Volume: %d", tick_data.Volume);
		m_Volume.SetWindowText( str);
		str.Format("Open Interest: %f", tick_data.OpenInterest);
		m_OpenInterest.SetWindowText( str);
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
		ret.GetStockData(HS300_URL, data);
		/*if( !(
				strcmp(data.TradingDay, old_data.TradingDay ) == 0 &&
				strcmp(data.UpdateTime, old_data.UpdateTime ) == 0
			 ) )*/
		{
			old_data = data;
			tradeConn->m_UserSpi->OnRtnDepthMarketData(&data);
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
