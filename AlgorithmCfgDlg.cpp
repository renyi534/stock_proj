// AlgorithmCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tradesystem.h"
#include "AlgorithmCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern char *ppInstrumentID[30];			// 行情订阅列表
extern int iInstrumentID;					// 行情订阅数量
/////////////////////////////////////////////////////////////////////////////
// CAlgorithmCfgDlg dialog


CAlgorithmCfgDlg::CAlgorithmCfgDlg(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CAlgorithmCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgorithmCfgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAlgorithmCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgorithmCfgDlg)
	DDX_Control(pDX, IDC_ALGO_TREE, m_AlgorithmTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlgorithmCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CAlgorithmCfgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmCfgDlg message handlers

BOOL CAlgorithmCfgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//SetWindowLong(m_AlgorithmTree.
	for(int i=1; i<6; i++)
	{
		CString algoStr;
		algoStr.Format("量化算法%d", i);
		HTREEITEM subTree=this->m_AlgorithmTree.InsertItem(algoStr);
		for(int j=0; j<iInstrumentID; j++)
		{
			m_AlgorithmTree.InsertItem(TVIF_TEXT,
				ppInstrumentID[j], 0, 0, 0, 0, 0, subTree, NULL);
		}
		m_AlgorithmTree.Expand(subTree, TVE_EXPAND); 
		m_AlgorithmTree.Expand(subTree, TVE_COLLAPSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
