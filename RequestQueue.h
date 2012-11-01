/////////////////////////////////////////////////////////////////////////////////////////////////////
// RequestQueue.h: interface for the ThreadPoolclass.
// Author - Alex.C.P (alexcpn@gmail.com)
// This class models the container Queue for the threadpool
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(REQUESTQUEUE__INCLUDED)
#define REQUESTQUEUE__INCLUDED



#include <list>
#include <map>
#include <queue>
#include "utility.h"

using namespace std;

namespace PluginThreadPool
{

template <typename T>

class CRequestQueue
{

private:

typedef priority_queue<Command<T>*,//value type
							deque<Command<T>*>,//container type
							Command<T>::Compare >  REQUESTQUEUE; //(will not compile in vc7)
							//typename Command<T>::Compare >  REQUESTQUEUE; //comapritor (will compile in VC7)
							
    
	typedef typename map<long,REQUESTQUEUE >::iterator REQUESTQUEUEITR;

	//todo make this a priority queue - priority_queue
	std::map<long,REQUESTQUEUE >  RequestQueue;
	typename std::map<long,REQUESTQUEUE >::iterator CurrentItr;

	CRITICAL_SECTION critsec;
	int  m_count;

public:

	//ctor
	CRequestQueue():m_count(0)
	{
			InitializeCriticalSection(&critsec);
			CurrentItr = RequestQueue.begin();
	}

	//dtor
	~CRequestQueue()
	{
		DeleteCriticalSection(&critsec);
	}
	// Method invoked by the client through the ThreadPool proxy
	// for inserting the created Command object
	void QueueRequest(Command<T>* cmdPtr)
	{
		if(cmdPtr==0)
			return;
		
		CLock lock(&critsec);

		//Get the key associcated with this command
		unsigned int key = cmdPtr->GetKey();

		//Check if there exist an entry in the map
		
		//printf("Trying to insert entry with key=%u\n",key);

		REQUESTQUEUEITR itr = RequestQueue.find(key);
		if(itr != RequestQueue.end())
		{
			//if yes get the associated queue and insert
	//		printf("Entry Exisit, pushing to end of queue\n");
			(*itr).second.push(cmdPtr);

		}
		else
		{	//else creare a queue and associate
//			printf("Entry Does not Exisit, Creating a map entry\n");
			REQUESTQUEUE reqQue;
			reqQue.push(cmdPtr);
			RequestQueue[key] = reqQue;

		}

		//Increment the total elements of the queue counter
		++m_count;
		printf("Total number of elements=%d\n",m_count);
	}

		//This method is called by a thread when it is free to procees
	//a request. It deques the request -fifo
	Command<T>* GetRequest()
	{
		CLock lock(&critsec);//Actually only one thread calls this at a time- so this does not seem really needed

		try
		{
			//check if the map is empty
			if( RequestQueue.begin() == RequestQueue.end() )
			{
				printf("No elements are there in the map\n");
				return 0;
			}

			//The map is not empty so check where the current iterator points
			while( CurrentItr != RequestQueue.end() )
			{
				//not in the end so check the queue associated with this map entry

				if((*CurrentItr).second.size()==0)
				{
					//printf("Queue Is Empty!\n");
					//if the current queue associated with this iter is iterate to the 
					//next node and see 
					++CurrentItr;	
				}
				else
				{	//ok the queue associated with this is not empty
					//so break
					break;
				}
			}

			//The current iterator is at the end, set it at the begining and return
			if(CurrentItr == RequestQueue.end())
			{
				CurrentItr =RequestQueue.begin();
				return 0;
			}
			
			//we reach here if the Currentitr is valid and also there is a non empty
			//quue associated with this.
	
			//out the element	
			Command<T>* temp=0;
			
			 //Command<T>*   =	(*CurrentItr).second.front();
			temp =	(*CurrentItr).second.top();//Added for priority queue

			(*CurrentItr).second.pop();

			printf("Size of Queue=%d\n",(*CurrentItr).second.size());
			
			++CurrentItr;
			//decrement the counter
			--m_count;
			//printf("**Total number of elements=%d\n",m_count);
			return temp;

		}
		catch(...)
		{
			printf("Exception Caught in GetRequest.....\n");
			throw;
		}
	
	}

	bool empty()// todo
	{
		//printf("IsEmpty--Total number of elements=%d\n",m_count);
		
		if(m_count)
			return false;
		else
			return true;

	}

};

}

#endif