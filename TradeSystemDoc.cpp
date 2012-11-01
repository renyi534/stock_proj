// TradeSystemDoc.cpp : implementation of the CTradeSystemDoc class
//

#include "stdafx.h"
#include "TradeSystem.h"
#include "TradeConn.h"
#include "TradeSystemDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern TradeConn tradeConn;

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemDoc

IMPLEMENT_DYNCREATE(CTradeSystemDoc, CDocument)

BEGIN_MESSAGE_MAP(CTradeSystemDoc, CDocument)
	//{{AFX_MSG_MAP(CTradeSystemDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemDoc construction/destruction

CTradeSystemDoc::CTradeSystemDoc()
{

}

CTradeSystemDoc::~CTradeSystemDoc()
{
}

BOOL CTradeSystemDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTradeSystemDoc serialization

void CTradeSystemDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemDoc diagnostics

#ifdef _DEBUG
void CTradeSystemDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTradeSystemDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemDoc commands
