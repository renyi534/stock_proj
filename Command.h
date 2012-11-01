/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Command.h: interface for the Command class.
/// Author - alexcpn@gmail.com
/// Design Pattern - The command pattern -
/// (http:///home.earthlink.net/~huston2/dp/patterns.html)
/// //////////////////////////////////////////////////////////////////////////////////////////////////////


#if !defined(COMMAND_43241FQDS)
#define COMMAND_43241FQDS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <functional>
#include <string>

namespace PluginThreadPool
{

//----------------------------------------------------------------------------------
//a method adapter class for method with signature - int _Class::method(int,int,int)
template<typename _Ret,typename _Class,typename _arg1,typename _arg2,typename _arg3>
class mem_fun3_t
{
	public:
		explicit mem_fun3_t(_Ret (_Class::*_Pm)(_arg1,_arg2,_arg3))

		:m_Ptr(_Pm) //okay here we store the member function pointer for later use
		{}

	//this operator call comes from the bind method
	_Ret operator()(_Class *_P, _arg1 arg1, _arg2 arg2, _arg3 arg3) const
	{
		return ((_P->*m_Ptr)(arg1,arg2,arg3));
	}

	private:
		_Ret (_Class::*m_Ptr)(_arg1,_arg2,_arg3);// method pointer signature
		
	
};
//----------------------------------------------------------------------------------
//helper method for the above class
template<typename _Ret,typename _Class,typename _arg1,typename _arg2,typename _arg3>
mem_fun3_t<_Ret,_Class,_arg1,_arg2,_arg3> mem_fun3 ( _Ret (_Class::*_Pm)(_arg1,_arg2,_arg3) )
{
	return (mem_fun3_t<_Ret,_Class,_arg1,_arg2,_arg3>(_Pm));
}
//----------------------------------------------------------------------------------
//a binder class for function with three parameter for illustration
// int function(int,int,int)

template<typename _Func,typename _Ptr,typename _arg1,typename _arg2,typename _arg3>
class binder3
{
public:
	//This is the  constructor that does the binding part
	binder3(_Func fn,_Ptr ptr,_arg1 i,_arg2 j,_arg3 k)
		:m_ptr(ptr),m_fn(fn),m1(i),m2(j),m3(k){}

	//and this is the function object 
	void operator()()  const
	{
		  m_fn(m_ptr,m1,m2,m3);//that calls the operator
	}

private:
	_Ptr m_ptr;
	_Func m_fn;
	_arg1 m1;
	_arg2 m2;
	_arg3 m3;

};

//----------------------------------------------------------------------------------
//a helper function to call binder
template <typename _Func, typename _P1,typename _arg1,typename _arg2,typename _arg3>
binder3<_Func, _P1, _arg1, _arg2, _arg3> bind3(_Func func, _P1 p1,_arg1 i,_arg2 j,_arg3 k)
{
      return binder3<_Func, _P1, _arg1, _arg2, _arg3> (func, p1,i,j,k);
}
//--------------------------------------------------------
//a binder class for function with signature - int fun(void)

template<typename _Func,typename _Ptr>
class binder
{
public:
	//This is the  constructor that does the binding part
	binder(_Func fn,_Ptr ptr)
		:m_ptr(ptr),m_fn(fn){}

	//and this is the function object that class it
	void operator() () const
	{
		  m_fn(m_ptr);
	}
	
private:
	_Ptr m_ptr;
	_Func m_fn;

};

//a helper function to call binder

template <typename _Func, typename _P1>
binder<_Func, _P1> bind(_Func func, _P1 p1)
{
      return binder<_Func, _P1>(func, p1);
}



const int PRIO_LOW =10;
const int PRIO_NORMAL =20;
const int PRIO_HIGH =30;


//***************************************************************************************
/// Command.h: interface for the Command class.
//***************************************************************************************
template <typename T>
class Command
{
	T* m_objptr; //pointer to the object
	
	//for a function that takes no argument and void return type
	void (T::*method)();   												 ;
	//for a function that takes no argument but non void return type
	typedef binder< std::mem_fun_t<int,T>, T* >								F0; 
	//for a function that takes 1 parameter as argument
	typedef std::binder1st<	std::mem_fun1_t<int,T,int> >					F1; 
	//for a function that takes three parameter as argumet
	typedef binder3<mem_fun3_t<int,T,int,int,int> ,T* ,int,int,int>			F3;

	// Note this is not the best design , but putting here for illustration
	// the F0, F1 or F3 datatype can itself be templatized

	//pointer to the method of the object
	F0* method0;  
	F1* method1;
	F3* method3;
	
	long m_timeout;//the max tolerated time for a task to finish
	std::string m_key;
	long m_value;
	int m_par1;

public:

	///Takes the pointer to an object and a pointer to the method

	explicit Command(T* pObj,void (T::*p_method)() ,long timeout,const char* key,long priority = PRIO_NORMAL )
		:m_objptr(pObj),m_timeout(timeout),m_key(key),m_value(priority),method0(0),method1(0),method3(0)
	{
		
		method = p_method;
	}



	explicit Command(T* pObj,F0* p_method,long timeout,const char* key,long priority = PRIO_NORMAL ):
				m_objptr(pObj),m_timeout(timeout),m_key(key),m_value(priority),method1(0),method3(0),method(0)
	{
		
		method0 = p_method;
	}

	//just to illustrate the usage with a method signature taking one parameter
	explicit Command(T* pObj,F1* p_method,int par1,long timeout,const char* key,long priority = PRIO_NORMAL ):
				m_objptr(pObj),m_timeout(timeout),m_key(key),m_value(priority),method0(0),method3(0),method(0)
	{
		method1 = p_method;
		m_par1 = par1;
		
	}

	//just to illustrate the usage with a method signature taking more than one parameter
	explicit Command(T* pObj,F3* p_method,long timeout,const char* key,long priority = PRIO_NORMAL ):
				m_objptr(pObj),m_timeout(timeout),m_key(key),m_value(priority),method1(0),method0(0),method(0)
	{
			method3 = p_method;
	}

	/// Calls the method of the object
	
	inline void execute()
	{

		if(method)
		{
			(m_objptr->*method)();
		}
		else if (method0)
		{
			(*method0)();
		}
		else if (method1)
		{
			(*method1)(m_par1);
		}
		else if (method3)
		{
			(*method3)();
		}

	}

	///This is the timout value beyond which if this task's
	///excecute method takes it is deemed as hanging
	///NOTE - Timeout is in seconds
	
	inline unsigned long GetTimeOut()
	{
		return m_timeout;
	}

	///return a key associated with this command object

	unsigned int GetKey()
	{
		return RSHash(m_key);
	}

	
	 bool operator < (const Command<T>& a) const
	{
		return m_value < a.m_value;
	}

	///Deletes the Command objecy
	///Note do not call 'delete m_objptr' as we do not own this
	
	~Command(){}; 

private:
	///The hash function used to create a key from the NE name
	///http:///www.partow.net/programming/hashfunctions/#RSHashFunction

	unsigned int RSHash(const std::string& str)
	{
	   unsigned int b    = 378551;
	   unsigned int a    = 63689;
	   unsigned int hash = 0;

	   for(size_t i = 0; i < str.length(); i++)
	   {
		  hash = hash * a + str[i];
		  a    = a * b;
	   }

	   return hash;
	}

	///Function object for use with the priority queue to check the priority 
	///of the Command object
	class Compare
	{
	public:
		
		bool operator()(const Command<T>* a1,const Command<T>* a2) const
		{
			return  *a1 < *a2;
		}
	};

};

}


#endif 
