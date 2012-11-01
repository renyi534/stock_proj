// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8D7F99BF_1591_4F52_8428_1832B5C7A849__INCLUDED_)
#define AFX_STDAFX_H__8D7F99BF_1591_4F52_8428_1832B5C7A849__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxdb.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

// The debugger cannot handle symbols that are longer than 255 characters.
// STL frequently creates symbols that are longer than 255 characters.
// When symbols are longer than 255 characters, the warning is disabled.
#pragma warning(disable:4786)
#pragma warning(disable:4503)

#include <iostream>
#include <afxsock.h>		// MFC socket extensions
#include <iostream>
#include <Windows.h>
#include <Winbase.h>
#pragma   comment(lib, ".\\ThostTraderApi\\thostmduserapi.lib ")
#pragma   comment(lib, ".\\ThostTraderApi\\thosttraderapi.lib ")


#pragma comment( lib, "mclmcrrt.lib" )
#pragma comment( lib, "libmx.lib" )
#pragma comment( lib, "libmat.lib" )
#pragma comment( lib, "mclmcr.lib" )
#include "mat.h"
#include "mclmcr.h"
#include "matrix.h"
#include "mclcppclass.h"
/*#include "string"
typedef std::basic_string<unsigned char> MyString;
#define string MyString*/
/*#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h> */

//#include "debug_new.h" 
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8D7F99BF_1591_4F52_8428_1832B5C7A849__INCLUDED_)
