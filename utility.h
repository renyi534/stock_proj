//Utility classes
// Author - Alex.C.P (alexcpn@gmail.com)

#if !defined(UTILITYCLOCK__INCLUDED)
#define UTILITYCLOCK__INCLUDED



///
/// Utiltiy classes
///
class CLock
{
private:
	CRITICAL_SECTION* m_critsec;

public:
	CLock(CRITICAL_SECTION* critsec):m_critsec(critsec)
	{
		EnterCriticalSection(m_critsec);
	}


	~CLock()
	{
		LeaveCriticalSection(m_critsec);
	}


};




#endif // !defined(UTILITYCLOCK__INCLUDED)