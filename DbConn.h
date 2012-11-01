#if !defined(DB_CONN_43241FQDS)
#define DB_CONN_43241FQDS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class DbAccessorPool;
class DbConn
{
private:
	DbAccessorPool&			m_dbPool;
public:
	DbAccessor*				m_db;

	DbConn(DbAccessorPool& dbPool)
		:m_dbPool(dbPool)
	{
		m_db  = &m_dbPool.getDbAccessor();
	}

	virtual ~DbConn()
	{
		m_dbPool.releaseDbAccessor(*m_db);
	}
};

#endif // !defined(DB_CONN_43241FQDS)