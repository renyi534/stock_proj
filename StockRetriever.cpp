// StockRetriever.cpp: implementation of the StockRetriever class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tradesystem.h"
#include "StockRetriever.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StockRetriever::StockRetriever()
{

}

StockRetriever::~StockRetriever()
{

}

CString  StockRetriever::GetStockInfo(CString url)
{
	CString strHtml;
	CHttpFile* file =NULL;
	try{  
		file = (CHttpFile*)m_session.OpenURL(url);  
	}catch(CInternetException * m_pException){  
		file = NULL;  
		m_pException->m_dwError;  
		m_pException->Delete();  
		m_session.Close();  
		AfxMessageBox("CInternetException");  
	}  
	CString strLine;  
	if(file != NULL){  
		while(file->ReadString(strLine) != NULL){  
			strHtml += strLine;  
		}  
	}else{  
		AfxMessageBox("fail");  
	}  
	m_session.Close();  
	file->Close();  
	delete file;  
	file = NULL;  
	return strHtml;
}