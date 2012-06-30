// TradeSystemView.cpp : implementation of the CTradeSystemView class
//

#include "stdafx.h"
#include "TradeSystem.h"
#include "TradeConn.h"
#include "TradeSystemDoc.h"
#include "TradeSystemView.h"
#include "DataSimulator.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern TradeConn* tradeConn;
extern char *ppInstrumentID[30];			// 行情订阅列表
extern int iInstrumentID;									// 行情订阅数量
/////////////////////////////////////////////////////////////////////////////
// CTradeSystemView

IMPLEMENT_DYNCREATE(CTradeSystemView, CFormView)

BEGIN_MESSAGE_MAP(CTradeSystemView, CFormView)
	//{{AFX_MSG_MAP(CTradeSystemView)
	ON_BN_CLICKED(IDC_SIMU_START, OnSimuStart)
	ON_BN_CLICKED(IDC_CLEAR_SHORT, OnClearShort)
	ON_BN_CLICKED(IDC_CLEAR_LONG, OnClearLong)
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

	m_Instruments.SetCurSel(0);
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
	tradeConn->m_TradeSpi->ClearShortPos((LPCSTR)instrument, m_AskPrice);
}

void CTradeSystemView::OnClearLong() 
{
	// TODO: Add your control notification handler code here
	CString instrument;
	m_Instruments.GetWindowText(instrument);
	tradeConn->m_TradeSpi->ClearLongPos((LPCSTR)instrument, m_BidPrice);	
}
