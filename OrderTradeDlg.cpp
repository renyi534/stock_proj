// OrderTradeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tradesystem.h"
#include "OrderTradeDlg.h"

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
	m_OrderList.InsertColumn(0, _T("�������"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(1, _T("��Լ"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(2, _T("����"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(3, _T("��ƽ"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(4, _T("��������"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(5, _T("δ�ɽ�����"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(6, _T("��ɽ�����"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(7, _T("�����۸�"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(8, _T("����ʱ��"), LVCFMT_CENTER, nColInterval);
	m_OrderList.InsertColumn(9, _T("��������"), LVCFMT_CENTER, rect.Width()-9*nColInterval);

	exstyle = m_TradeList.GetExtendedStyle();
	m_TradeList.SetExtendedStyle(exstyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_EX_STATICEDGE );
	m_TradeList.GetClientRect(&rect);
    	nColInterval = rect.Width()/9;
	m_TradeList.InsertColumn(0, _T("�ɽ����"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(1, _T("�������"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(2, _T("��Լ"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(3, _T("����"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(4, _T("��ƽ"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(5, _T("�ɽ��۸�"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(6, _T("�ɽ�����"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(7, _T("�ɽ�ʱ��"), LVCFMT_CENTER, nColInterval);
	m_TradeList.InsertColumn(8, _T("�ɽ�����"), LVCFMT_CENTER, rect.Width()-8*nColInterval);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
