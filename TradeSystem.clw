; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTradeSystemView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "tradesystem.h"
LastPage=0

ClassCount=5
Class1=CMainFrame
Class2=CTradeSystemApp
Class3=CAboutDlg
Class4=CTradeSystemDoc
Class5=CTradeSystemView

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TRADESYSTEM_FORM

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CTradeSystemApp]
Type=0
BaseClass=CWinApp
HeaderFile=TradeSystem.h
ImplementationFile=TradeSystem.cpp
LastObject=CTradeSystemApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=TradeSystem.cpp
ImplementationFile=TradeSystem.cpp
LastObject=CAboutDlg

[CLS:CTradeSystemDoc]
Type=0
BaseClass=CDocument
HeaderFile=TradeSystemDoc.h
ImplementationFile=TradeSystemDoc.cpp

[CLS:CTradeSystemView]
Type=0
BaseClass=CFormView
HeaderFile=TradeSystemView.h
ImplementationFile=TradeSystemView.cpp
Filter=D
VirtualFilter=VWC
LastObject=IDC_ACCOUNT_LIST

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TRADESYSTEM_FORM]
Type=1
Class=CTradeSystemView
ControlCount=24
Control1=IDC_MD_STATUS,static,1342308352
Control2=IDC_TRADE_STATUS,static,1342308352
Control3=IDC_ORDER_TOTAL,static,1342308352
Control4=IDC_TRADE_TOTAL,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_ORDER_DETAIL,edit,1353779396
Control7=IDC_TRADE_DETAIL,edit,1353779396
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_DATETIME_START,SysDateTimePick32,1342242848
Control12=IDC_STATIC,static,1342308352
Control13=IDC_DATETIME_END,SysDateTimePick32,1342242848
Control14=IDC_SIMU_START,button,1342242816
Control15=IDC_LONG_POS,static,1342308352
Control16=IDC_SHORT_POS,static,1342308352
Control17=IDC_CLEAR_SHORT,button,1342242816
Control18=IDC_CLEAR_LONG,button,1342242816
Control19=IDC_ALGO_POS,static,1342308352
Control20=IDC_YDLONG_POS,static,1342308352
Control21=IDC_YDSHORT_POS,static,1342308352
Control22=IDC_ACCOUNT_LIST,SysListView32,1350633473
Control23=IDC_INSTRUMENT_LIST,SysListView32,1350631425
Control24=IDC_POSITION_LIST,SysListView32,1350631425

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_APP_ABOUT
CommandCount=10

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

