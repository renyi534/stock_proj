// DbAccessorPool.cpp: implementation of the DbAccessorPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TradeSystem.h"
#include "DbAccessorPool.h"
#include "DbAccessor.h"
#include "utility.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DbAccessorPool::DbAccessorPool(int size):
	m_size(size),
	m_dbVector()
{

}

DbAccessorPool::~DbAccessorPool()
{
	for(int i=0;i<m_size;i++)
	{
		DbAccessorItem item=this->m_dbVector[i];
		delete item.m_dbAccessor;
	}
	CloseHandle(m_sem); 
	DeleteCriticalSection(&m_critsec);
}

DbAccessor& DbAccessorPool::getDbAccessor()
{
	WaitForSingleObject(m_sem, INFINITE);
	CLock	critLock(&m_critsec);

	for(int i=0;i<m_size;i++)
	{
		DbAccessorItem item=this->m_dbVector[i];
		if(!item.is_busy)
		{
			item.is_busy=true;
			return *item.m_dbAccessor;
		}
	}
	//return (DbAccessor&)(*NULL);
	ASSERT("getDbAccessor wrong");
}

void DbAccessorPool::releaseDbAccessor(DbAccessor& acc)
{
	CLock	critLock(&m_critsec);
	
	DbAccessor* acc_addr= &acc;
	for(int i=0;i<m_size;i++)
	{
		DbAccessorItem item=this->m_dbVector[i];
		if(item.m_dbAccessor==acc_addr)
		{
			item.is_busy=false;
			ReleaseSemaphore(m_sem, 1, NULL);
			return;
		}
	}
	
	ASSERT("releaseDbAccessor wrong");
}

void DbAccessorPool::Init(string conn_str)
{
	m_sem = CreateSemaphore(NULL, m_size, m_size, NULL);
	for(int i=0;i<m_size;i++)
	{
		DbAccessor* accessor= new DbAccessor(conn_str);
		DbAccessorItem item(accessor,false);
		this->m_dbVector.push_back(item);
	}
	InitializeCriticalSection(&m_critsec);
}
