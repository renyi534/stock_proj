#include "stdafx.h"
#include "TradeHandlingThread.h"
#include "ThreadPool.h"
#include "Algorithm.h"
#include "queue"
#include "map"
#include "utility.h"
#include "DbConn.h"
#include "TradeSystem.h"
#include <math.h>
using namespace std;

CThreadPool gThreadPool(2);

extern CTradeSystemApp theApp;

extern DbAccessorPool dbAccessPool;

static void HandleAlgoResponse(MSG& msg)
{
	OrderInfo* data= NULL;
	OrderInfoShort* data_short = NULL;
	//queue<Algorithm*> algo_queue;
	if( msg.lParam == 1)
	{
		data= (OrderInfo*)msg.wParam;
		TradeConn* tradeConn = 
			theApp.GetTradeConn(data->broker_id, data->investor_id);

		ASSERT(tradeConn != NULL);
		tradeConn->m_TradeSpi->ReqOrderInsert(*data);
		delete data;
	}
	else
	{
		data_short = (OrderInfoShort*)msg.wParam;
		TradeConn* tradeConn = 
			theApp.GetTradeConn(data_short->broker_id, data_short->investor_id);

		ASSERT(tradeConn != NULL);
		tradeConn->m_TradeSpi->ReqOrderInsert(*data_short);
		delete data_short;
	}

}


UINT ThrdFunc (LPVOID n)
{
    int    TNumber    =    ( int )n;
    //    Here we will wait for the messages
    while ( true )
    {
        MSG    msg;
        //BOOL    MsgReturn  =  PeekMessage ( &msg , NULL , 
        //    THRD_MESSAGE_SOMEWORK , THRD_MESSAGE_EXIT , PM_REMOVE );
        BOOL    MsgReturn    =    GetMessage ( &msg , NULL , 
            WM_ACTION_ITEM , WM_ACTION_ITEM );
            
        if ( MsgReturn )
        {
            switch ( msg.message )
            {
            case WM_ACTION_ITEM:
				HandleAlgoResponse(msg);
                break;
            }
        }
    }    
    return 0;
}

CWinThread* TradeHandlingThread=AfxBeginThread(ThrdFunc,NULL,THREAD_PRIORITY_NORMAL);

extern bool WriteDb;
DWORD WINAPI ExecSQL(LPVOID param)
{
    // VERY IMPORTANT: param is a pointer to UserPoolData
	UserPoolData* pPoolData = (UserPoolData*) param;
    char* sql = (char*)pPoolData->pData;
	
	if(WriteDb)
	{
		try{
			DbConn conn(dbAccessPool);
			conn.m_db->execSql(sql);
		}
		catch(...)
		{
		}
	}
	delete [] sql;
	return 1;
}