// TradeSystem.h : main header file for the TRADESYSTEM application
//

#if !defined(AFX_TRADESYSTEM_H__B1842401_7C71_45BD_8FEC_8533BCEE40C5__INCLUDED_)
#define AFX_TRADESYSTEM_H__B1842401_7C71_45BD_8FEC_8533BCEE40C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <map>
#include <vector>
#include "TradeConn.h"
/////////////////////////////////////////////////////////////////////////////
// CTradeSystemApp:
// See TradeSystem.cpp for the implementation of this class
//

class CTradeSystemApp : public CWinApp
{
public:
	int GetConnCount();
	void AddTradeConn(string broker_id, string investor_id, TradeConn* conn);
	TradeConn* GetTradeConn(string broker_id, string investor_id);
	void GetTradeConnList(vector<TradeConn*>& conn_list);
	CTradeSystemApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradeSystemApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTradeSystemApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    void LoadConfig();
	map<string, TradeConn*>  m_TradeConnMap;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADESYSTEM_H__B1842401_7C71_45BD_8FEC_8533BCEE40C5__INCLUDED_)
