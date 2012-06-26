// DbAccessorPool.h: interface for the DbAccessorPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBACCESSORPOOL_H__2003CAD9_0CC0_4A67_8BF2_ECEDC245D9CE__INCLUDED_)
#define AFX_DBACCESSORPOOL_H__2003CAD9_0CC0_4A67_8BF2_ECEDC245D9CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <afxmt.h>


using namespace std;
class DbAccessor;
struct DbAccessorItem{
	DbAccessorItem(DbAccessor* accessor, bool busy):
		m_dbAccessor(accessor)
	{
		is_busy=busy;
	}

	DbAccessor* m_dbAccessor;
	bool		is_busy;
};
class DbAccessorPool  
{
public:
	void Init();
	DbAccessorPool(int size=2);
	virtual ~DbAccessorPool();
	DbAccessor& getDbAccessor();
	void releaseDbAccessor(DbAccessor&);
private:
	vector<DbAccessorItem>	m_dbVector;
	const int				m_size;
	HANDLE 					m_sem;
	CRITICAL_SECTION		m_critsec;
};

#endif // !defined(AFX_DBACCESSORPOOL_H__2003CAD9_0CC0_4A67_8BF2_ECEDC245D9CE__INCLUDED_)
