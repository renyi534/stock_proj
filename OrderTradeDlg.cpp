// OrderTradeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tradesystem.h"
#include "OrderTradeDlg.h"
#include "ListCtrlCl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderTradeDlg dialog


COrderTradeDlg::COrderTradeDlg(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(COrderTradeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderTradeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COrderTradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderTradeDlg)
	DDX_Control(pDX, IDC_TRADE_LIST, m_TradeList);
	DDX_Control(pDX, IDC_ORDER_LIST, m_OrderList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderTradeDlg, CDialog)
	//{{AFX_MSG_MAP(COrderTradeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderTradeDlg message handlers

BOOL COrderTradeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	DWORD exstyle;
	int nColInterval;
	CRect rect;	
	// TODO: Add extra initialization here
	exstyle = m_OrderList.GetExtendedStyle();
	m_OrderList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_OrderList.GetClientRect(&rect);
    	nColInterval = rect.Width()/10;
	m_OrderList.InsertColumn(0, _T("报单编号"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(1, _T("合约"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(2, _T("买卖"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(3, _T("开平"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(4, _T("报单手数"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(5, _T("未成交手数"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(6, _T("今成交手数"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(7, _T("报单价格"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(8, _T("报单时间"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(9, _T("报单类型"), LVCFMT_CENTER, rect.Width()-9*nColInterval);

	m_OrderList.SetBkColor(RGB(50,10,10));
	m_OrderList.SetHeaderHeight(1.5);          //设置头部高度
	m_OrderList.SetHeaderFontHW(16,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_OrderList.SetTextColor(RGB(0,255,255));  //设置文本颜色
	m_OrderList.SetHeaderBKColor(100,255,100,8); //设置头部背景色
	m_OrderList.SetFontHW(13,0);               //设置字体高度，和宽度,0表示缺省宽度

	exstyle = m_TradeList.GetExtendedStyle();
	m_TradeList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_TradeList.GetClientRect(&rect);
    	nColInterval = rect.Width()/9;
	m_TradeList.InsertColumn(0, _T("成交编号"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(1, _T("报单编号"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(2, _T("合约"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(3, _T("买卖"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(4, _T("开平"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(5, _T("成交价格"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(6, _T("成交手数"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(7, _T("成交时间"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(8, _T("成交类型"), LVCFMT_CENTER, rect.Width()-8*nColInterval);	

	m_TradeList.SetBkColor(RGB(50,10,10));
	m_TradeList.SetHeaderHeight(1.5);          //设置头部高度
	m_TradeList.SetHeaderFontHW(16,0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_TradeList.SetTextColor(RGB(0,255,255));  //设置文本颜色
	m_TradeList.SetHeaderBKColor(100,255,100,8); //设置头部背景色
	m_TradeList.SetFontHW(13,0);               //设置字体高度，和宽度,0表示缺省宽度
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
