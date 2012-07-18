// TradeSystemView.h : interface of the CTradeSystemView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRADESYSTEMVIEW_H__3A9C758A_78B6_4B62_A5EE_3B47F107DA59__INCLUDED_)
#define AFX_TRADESYSTEMVIEW_H__3A9C758A_78B6_4B62_A5EE_3B47F107DA59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTradeSystemView : public CFormView
{
protected: // create from serialization only
	CTradeSystemView();
	DECLARE_DYNCREATE(CTradeSystemView)

public:
	//{{AFX_DATA(CTradeSystemView)
	enum { IDD = IDD_TRADESYSTEM_FORM };
	CStatic	m_CloseProfit;
	CStatic	m_PositionProfit;
	CStatic	m_Balance;
	CStatic	m_YdShortPos;
	CStatic	m_YdLongPos;
	CStatic	m_AlgoPos;
	CStatic	m_ShortPos;
	CStatic	m_LongPos;
	CDateTimeCtrl	m_DateEnd;
	CDateTimeCtrl	m_DateStart;
	CStatic	m_Volume;
	CStatic	m_TradeTotal;
	CStatic	m_TradeStatus;
	CEdit	m_TradeDetail;
	CStatic	m_Price;
	CStatic	m_OrderTotal;
	CEdit	m_OrderDetail;
	CStatic	m_OpenInterest;
	CStatic	m_MDStatus;
	CStatic	m_Cash;
	CComboBox	m_Instruments;
	//}}AFX_DATA

	double m_BidPrice;
	double m_AskPrice;
// Attributes
public:
	CTradeSystemDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradeSystemView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	static CTradeSystemView* GetCurrView();
	virtual ~CTradeSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_RefreshFormTimer;
	int m_RefreshPosTimer;
	int m_CorrectionPosTimer;
	int m_HS300Timer;
// Generated message map functions
protected:
	//{{AFX_MSG(CTradeSystemView)
	afx_msg void OnSimuStart();
	afx_msg void OnClearShort();
	afx_msg void OnClearLong();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TradeSystemView.cpp
inline CTradeSystemDoc* CTradeSystemView::GetDocument()
   { return (CTradeSystemDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADESYSTEMVIEW_H__3A9C758A_78B6_4B62_A5EE_3B47F107DA59__INCLUDED_)
