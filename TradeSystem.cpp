// TradeSystem.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TradeSystem.h"

#include "MainFrm.h"
#include "TradeSystemDoc.h"
#include "TradeSystemView.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "MdSpi.h"
#include "DbAccessorPool.h"
#include "TradeConn.h"
#include "MessageRouter.h"
#include "matlab\\libMethod_1.h"
#include "matlab\\libMethod_2.h"
#include "matlab\\libMethod_3.h"
#include "matlab\\libMethod_4.h"
#include "Markup.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 配置参数
char FRONT_ADDR_MD[200] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";	// 安信模拟行情
//char FRONT_ADDR_MD[200] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";//东证期货模拟行情


//char FRONT_ADDR_MD[200] = "tcp://zzqh-md2.financial-trading-platform.com:41213";  //中证真实行情


char FRONT_ADDR_TRADE[200] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";//安信模拟交易
//char FRONT_ADDR_TRADE[200] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";//东证期货模拟交易地质

TThostFtdcBrokerIDType	BROKER_ID = "2030";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "354204";			// 投资者代码
TThostFtdcPasswordType  PASSWORD = "888888";			// 用户密码

/*
TThostFtdcBrokerIDType	BROKER_ID = "4000";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "601000047";			// 投资者代码
TThostFtdcPasswordType  PASSWORD = "123456";			// 用户密码
*/
char *ppInstrumentID[30];			// 行情订阅列表
int iInstrumentID = 0;									// 行情订阅数量
char DB_CONN[128]="DSN=PostgreSQL35W;UID=postgres;PWD=123";
char HS300_URL[128]="http://hq.sinajs.cn/list=sz399300"; 
TThostFtdcInstrumentIDType INSTRUMENT_ID = "IF1207";	// 合约代码

DbAccessorPool dbAccessPool;
TradeConn* tradeConn; //(FRONT_ADDR_MD,FRONT_ADDR_TRADE,TERT_RESUME);
/////////////////////////////////////////////////////////////////////////////
// CTradeSystemApp

BEGIN_MESSAGE_MAP(CTradeSystemApp, CWinApp)
	//{{AFX_MSG_MAP(CTradeSystemApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemApp construction

CTradeSystemApp::CTradeSystemApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTradeSystemApp object

CTradeSystemApp theApp;


void CTradeSystemApp::LoadConfig()
{
	// the read sequence must be the same as config file
	CFile file;
	file.Open(".\\config.xml", CFile::modeRead);
	int len = (int) file.GetLength();
	char* buf = new char[len+1];
	file.Read(buf,len);
	file.Close();
    CMarkup xml;
    xml.SetDoc( buf );
    xml.FindChildElem("TRADE");
    CString trade = xml.GetChildData();
    strcpy( FRONT_ADDR_TRADE, (LPCSTR)trade);

    xml.FindChildElem("MARKET");
    CString market = xml.GetChildData();
    strcpy( FRONT_ADDR_MD, (LPCSTR)market);
 
    xml.FindChildElem("BROKER_ID");
    CString broker = xml.GetChildData();
    strcpy( BROKER_ID, (LPCSTR)broker);

    xml.FindChildElem("INVESTOR_ID");
    CString investor = xml.GetChildData();
    strcpy( INVESTOR_ID, (LPCSTR)investor);
    
    xml.FindChildElem("PASSWORD");
    CString passwd = xml.GetChildData();
    strcpy( PASSWORD, (LPCSTR)passwd);

    xml.FindChildElem("INSTRUMENT");
    xml.IntoElem();
    
    while ( xml.FindChildElem("ITEM") )
    {
        CString item = xml.GetChildData();
		int len = strlen( (LPCSTR) item );
		ppInstrumentID[iInstrumentID] = new char[len+1];
        strcpy(ppInstrumentID[iInstrumentID++], (LPCSTR) item);
    }
    xml.OutOfElem();

    xml.FindChildElem("DB_CONN");
    CString db_conn = xml.GetChildData();
    strcpy( DB_CONN, (LPCSTR)db_conn);

    xml.FindChildElem("HS300_URL");
    CString hs300_url = xml.GetChildData();
    strcpy( HS300_URL, (LPCSTR)hs300_url);

	delete [] buf;
}

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemApp initialization


BOOL CTradeSystemApp::InitInstance()
{
    LoadConfig();
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTradeSystemDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTradeSystemView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// first init db access
	dbAccessPool.Init();
	tradeConn= new TradeConn(FRONT_ADDR_MD,FRONT_ADDR_TRADE,TERT_QUICK);
	if( !libMethod_1Initialize())   
	{   
		AfxMessageBox("Could not initialize libMethod_1Initialize!");   
		return -1;   
	}   // 为变量分配内存空间，可以查帮助*/

	if( !libMethod_2Initialize())   
	{   
		AfxMessageBox("Could not initialize libMethod_2Initialize!");   
		return -1;   
	}   // 为变量分配内存空间，可以查帮助*/

	if( !libMethod_3Initialize())   
	{   
		AfxMessageBox("Could not initialize libMethod_3Initialize!");   
		return -1;   
	}   // 为变量分配内存空间，可以查帮助*/

	if( !libMethod_4Initialize())   
	{   
		AfxMessageBox("Could not initialize libMethod_4Initialize!");   
		return -1;   
	}   // 为变量分配内存空间，可以查帮助*/

	// then init algorithm
	MessageRouter::Router.InitAlgorithm();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTradeSystemApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTradeSystemApp message handlers


int CTradeSystemApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	// terminate the lib   
	libMethod_1Terminate();   // terminate MCR   
	libMethod_2Terminate();   // terminate MCR   
	libMethod_3Terminate();   // terminate MCR   
	libMethod_4Terminate();   // terminate MCR
	
	mclTerminateApplication(); 
	for( int i=0 ; i< iInstrumentID; i++)
	{
		delete [] ppInstrumentID[i];
	}
	
	delete tradeConn;
	return CWinApp::ExitInstance();
}
