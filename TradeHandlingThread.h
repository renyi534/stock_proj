// TradeHandlingThread.h: interface for the Algorithm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRADEHANDLINGTHREAD_H__F1FFFDD3_5236_4857_B14B_1F689122F5FD__INCLUDED_)
#define AFX_TRADEHANDLINGTHREAD_H__F1FFFDD3_5236_4857_B14B_1F689122F5FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ThreadPool.h"
//    User Defines Messages
#define        WM_ACTION_ITEM           WM_USER + 1
#define        WM_MARKET_DATA           WM_USER + 2
#define        WM_TRADE_INFO            WM_USER + 3
#define        WM_POSITION_INFO         WM_USER + 4
#define        WM_ACCOUNT_INFO          WM_USER + 5


extern CWinThread* TradeHandlingThread;
extern CThreadPool gThreadPool;
extern DWORD WINAPI ExecSQL(LPVOID param);
#endif
