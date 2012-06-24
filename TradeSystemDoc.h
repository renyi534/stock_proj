// TradeSystemDoc.h : interface of the CTradeSystemDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRADESYSTEMDOC_H__51D0027E_BF87_4033_A437_F0AA302E5085__INCLUDED_)
#define AFX_TRADESYSTEMDOC_H__51D0027E_BF87_4033_A437_F0AA302E5085__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTradeSystemDoc : public CDocument
{
protected: // create from serialization only
	CTradeSystemDoc();
	DECLARE_DYNCREATE(CTradeSystemDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradeSystemDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTradeSystemDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTradeSystemDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADESYSTEMDOC_H__51D0027E_BF87_4033_A437_F0AA302E5085__INCLUDED_)
