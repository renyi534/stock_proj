#if !defined(AFX_ORDERTRADEDLG_H__FDB37CB9_C116_4E8A_8C04_5A96F4B3536D__INCLUDED_)
#define AFX_ORDERTRADEDLG_H__FDB37CB9_C116_4E8A_8C04_5A96F4B3536D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrderTradeDlg.h : header file
//
#include "TabPageSSL.h"
/////////////////////////////////////////////////////////////////////////////
// COrderTradeDlg dialog

class COrderTradeDlg : public CTabPageSSL
{
// Construction
public:
	COrderTradeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COrderTradeDlg)
	enum { IDD = IDD_ORDER_TRADE };
	CListCtrl	m_TradeList;
	CListCtrl	m_OrderList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderTradeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrderTradeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERTRADEDLG_H__FDB37CB9_C116_4E8A_8C04_5A96F4B3536D__INCLUDED_)
