///////////////////////////////////////////////////////////////////////////////////////////////////////
/// ThreadChain.h: interface for the testchain class.
// Author - Alex.C.P (alexcpn@gmail.com)
/// Design Pattern - Chain of Responsility + Modifications for threadpool
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(THREADCHAIN_67A4DFA4)
#define THREADCHAIN_67A4DFA4

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadChain.h"

namespace PluginThreadPool
{

	static int threadIDgen=0;

template <typename T>
class ThreadPool;

template <typename T>	

/// The ThreadPool class creates a chain of ThreadChain objects, each
/// ThreadChain owns a thread
/// Design Pattern - Chain of Responsility + Modifications for threadpool

class ThreadChain
{
	friend class ThreadPool;
public:

	///The constructor which created the thread
	explicit ThreadChain(ThreadChain* p,ThreadPool<T>* pool)
		:busy(0),next(p),starttime(0),m_winthread(0),m_ref(0),threadId(++threadIDgen),deleteMe(0),threadpool(pool),
		m_timeout(-1),lastactivetime(0)
	{
		
		AddRef();

		///Create an even in a non signalled state
		g_Event =CreateEvent(0,1,0,0); ///Manually reset event
		threadChkMutex = CreateMutex(0,0,0);
		

		///Create a thread ,where the work of calling the objects method is done
		m_winthread = AfxBeginThread(ThreadChain::HandleRequestProxy,this,0,0,CREATE_SUSPENDED);
		m_winthread->m_bAutoDelete = FALSE;
		m_winthread->ResumeThread();

		///Increment the thread count, note threadpool acts like a controller
		threadpool->IncrementThreadCount();
		

	};

	///This method just checks if ITS Thread is already busy processing a request
	///If its thread is free it signals an event which sets it thread to 
	///hand the request and it returns true;
	bool canHandle()
	{
		//printf("In canHandle of %d\n",threadId);
			
		if(!busy)
		{
			SetEvent(g_Event);
			return true;
		}

		
		if(busy) ///if the associated thread is already processing a request
		{
			if(next ==0)
			{	
				///Means there is no next thread so  check if threads should be added
				///if the thread count is already max nothing to be done
				
				for(int i=threadpool->m_threadCount;i<=threadpool->maxthreadCount;++i)
				{
					printf("Generating a new thread  threadCount=%d\n",threadpool->m_threadCount+1);   ///acp check this
					next = new ThreadChain<T>(0,threadpool);///create a new thread
					break; ///Only add one thread at a time
				}
			
			}
			///If the thread count is already max nothing to be done
			if(!next)
			{
				//printf("Max thread Count reacehed\n");
				return false; ///max thread count reached
			}

			///Else see if the next obejct in the chain is free
			/// to handle the request
			return next->canHandle();

		}
		
		return false;

	}

	///The proxy thread function that call the method-HandleRequest
	///in a thread---
	static UINT HandleRequestProxy(LPVOID param)
	{
		ThreadChain* temp = (ThreadChain*) param;
		temp->HandleRequest();
		//AfxEndThread(1,false);
		return 1;

	}
	
	///This is the method that is called by the thread, it waits for an event
	/// note- exceptions thrown by the excecute method are not caught
	void HandleRequest()
	{
		//threadId = GetCurrentThreadId();
		printf("In HandleRequest of thread=%d\n",threadId);

		///The deleteMe is set in the dtor usually if this threads time out is over
		while(!deleteMe) 
		{
			WaitForSingleObject(g_Event,INFINITE);///This event is signalled from the canHandle mthd

			///Set the start time - for checking if the 	thread is hang or not
			WaitForSingleObject(threadChkMutex,INFINITE);
			{
				starttime = GetTickCount();
				ReleaseMutex(threadChkMutex );
			}

			///Now set that this object as busy-it is going to handle a request
			busy=true;
			//printf("Thread %d is busy\n",threadId);


			///printf("Handling a request in thread=%d\n",threadId);

			///there is no need of syncronizhing this 
			///as only one thread will be calling GetRequest at a time

			Command<T>* temp  = threadpool->GetRequest();

			if(temp!=NULL)
			{
				printf("Handling a request in thread=%d\n",threadId);
				m_timeout = temp->GetTimeOut()*1000; /// The time out is in seconds
				temp->execute();
				delete temp;
				
			}
			///else
				///printf("GetRequest gave null!!");
		
			///Okay task is over - so set it to not busy state
			busy=false;
			//printf("Thread %d is NOT busy\n",threadId);

		
			///Reset the start time
			WaitForSingleObject(threadChkMutex,INFINITE);
			{
				starttime=0;
				
				// Now check if the minimum thread count is exceeded; then delete this thread also
				// so that there is no unnecessary max threads
				//this has to be via last used time event
				lastactivetime = GetTickCount();
				ReleaseMutex(threadChkMutex);
			}
			
			
			///Reset the event to non signalled state
			ResetEvent(g_Event);

		}///end of while(!deleteMe) 

		///We are here because the thread has taken more than the timeout value
		///to exceute

		printf("Out of HandleRequest\n");
		
		///call the dtor todo - in case of a thread that is 
		///permenantly hanging the flow will not reach here, 
		///so the handles used by this atleast have to be closed ??
		
		delete this; //ACP check

		//Note before deleting this make sure that it is removed from thread chain
	}

	///The method to handle the hung threads
	void HandleHungThreads(ThreadChain<T>* prev)
	{
		///printf("In HandleHungThreads\n");

		ThreadChain<T>* next_p= GetNext();
		bool bIsHung =false;

		if(IsHung() || ReleaseIdleThread())
		{
			printf("In HandleHungThreads Thread %d is hung/idle!!\n",threadId);
			bIsHung = true;
			if(this == threadpool->root) ///case if root is hanging
			{
				threadpool->root = next_p;
				prev= next_p;
			}
			else
				prev->next = GetNext(); // reomove this item from thread chain link
			
		}
		
		if(next == 0) ///propogate to the next object
		{
			return;
		}
		
		if(bIsHung)///if this is a hung thread that is waking up
		{
			Release(); ///then release it
			next_p->HandleHungThreads(prev);
			//Release(); //dont release here 
			
		}
		else
			next->HandleHungThreads(this);

		///printf("Out HandleHungThreads\n");
		
	}

	///For diagnostics
	void printChain()
	{
		printf("Thread ID=%d\n",threadId);
		
		///propogate to the next object
		if(next == 0)
			return;
		
		next->printChain();
	}



	///Get the next object in the chain
	ThreadChain * GetNext()
	{
		return next;
	}

	///Method to check if the thread associated
	///witht this thread is hung
	bool IsHung()
	{
		///printf("In IsHung method\n");
		WaitForSingleObject(threadChkMutex,INFINITE);

		if( (starttime ==0 ) || ( m_timeout == -1) )
		{
			ReleaseMutex(threadChkMutex);
			return false;
		}

		DWORD  now = GetTickCount() ;
		if( (now - starttime) > m_timeout)
		{
			printf("Thread IsHung Id=%d\n",threadId);
			ReleaseMutex(threadChkMutex);
			return true;
		}

		ReleaseMutex(threadChkMutex);
		return false;
	}

	//To check if this thread is idle and can be released
	// Note - it is a prerequisite that this is called only after
	// checking if the thread has hung
	bool ReleaseIdleThread()
	{
		WaitForSingleObject(threadChkMutex,INFINITE);

		//thread count is equal to the minimum thread count
		if(threadpool->m_threadCount <= threadpool->minthreadCount)
		{
			ReleaseMutex(threadChkMutex);
			return false;
		}

		//printf("Excess thread[%d] ThreadCount=%d\n",threadId,threadpool->m_threadCount);

		bool bReleasethis= false;

		if(lastactivetime == 0)
		{
			printf("This thread was never executed\n");
			bReleasethis=false;
		}else if(GetTickCount()-lastactivetime  > MAX_IDLETIME)
		{
			printf("Max time exceeded for this thread\n");
			bReleasethis=true;
		}
		
		ReleaseMutex(threadChkMutex);
		return bReleasethis;

	}


	///reference couting
	void AddRef(){++m_ref;}

		

	///reference couting
	void Release()
	{
		--m_ref;
		if(m_ref ==0 )
		{
			threadpool->DecrementThreadCount();
			printf("Deleting thread id=%d, ThreadCount=%d\n",threadId,threadpool->m_threadCount);
			deleteMe=true;
			SetEvent(g_Event); //delete this object
		}

	}
	
	///dtor
	 ~ThreadChain()
	 {
		
		 printf("In ThreadChain dtor=%d\n",threadId);
		 delete m_winthread;
		 CloseHandle(g_Event);
		 CloseHandle(threadChkMutex);//ACP test
		 printf("Releasing-%d\n",this);
		 next=0;
		 threadpool=0;
		 //printf("Out ThreadChain dtor\n");

	 };
	

private:

	///there is no meaning in copying a thread,
	///so making copy ctor and copy assignment private
	ThreadChain(const ThreadChain&){};
	ThreadChain& operator=(const ThreadChain&){};
	ThreadChain* next; ///pointer to the next obj in the chain
	bool busy;
	
	unsigned long starttime;
	unsigned long lastactivetime;
	CWinThread* m_winthread ;
	HANDLE g_Event;
	int m_ref;
	int threadId;
	bool deleteMe;
	ThreadPool<T>* threadpool; ///pointer to the parent thread pool
	DWORD m_timeout;
	HANDLE threadChkMutex;
};

}




#endif /// !defined(THREADCHAIN_67A4DFA4)
