#if !defined(AFX_ALGORITHMCFGDLG_H__266497FC_BF5A_4370_81B5_14B79F7A6193__INCLUDED_)
#define AFX_ALGORITHMCFGDLG_H__266497FC_BF5A_4370_81B5_14B79F7A6193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgorithmCfgDlg.h : header file
//

#include "TabPageSSL.h"
/////////////////////////////////////////////////////////////////////////////
// CAlgorithmCfgDlg dialog

class CAlgorithmCfgDlg : public CTabPageSSL
{
// Construction
public:
	CAlgorithmCfgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgorithmCfgDlg)
	enum { IDD = IDD_ALGORITHM_DLG };
	CTreeCtrl	m_AlgorithmTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgorithmCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgorithmCfgDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGORITHMCFGDLG_H__266497FC_BF5A_4370_81B5_14B79F7A6193__INCLUDED_)
