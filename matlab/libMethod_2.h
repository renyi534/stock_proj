//
// MATLAB Compiler: 4.3 (R14SP3)
// Date: Wed Jul 11 22:25:45 2012
// Arguments: "-B" "macro_default" "-W" "cpplib:libMethod_2" "-T" "link:lib"
// "MinKsymbProcess_hs.m" "IniMethod_hs.m" 
//

#ifndef __libMethod_2_h
#define __libMethod_2_h 1

#if defined(__cplusplus) && !defined(mclmcr_h) && defined(__linux__)
#  pragma implementation "mclmcr.h"
#endif
#include "mclmcr.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_libMethod_2
#define PUBLIC_libMethod_2_C_API __global
#else
#define PUBLIC_libMethod_2_C_API /* No import statement needed. */
#endif

#define LIB_libMethod_2_C_API PUBLIC_libMethod_2_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_libMethod_2
#define PUBLIC_libMethod_2_C_API __declspec(dllexport)
#else
#define PUBLIC_libMethod_2_C_API __declspec(dllimport)
#endif

#define LIB_libMethod_2_C_API PUBLIC_libMethod_2_C_API


#else

#define LIB_libMethod_2_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libMethod_2_C_API 
#define LIB_libMethod_2_C_API /* No special import/export declaration */
#endif

extern LIB_libMethod_2_C_API 
bool libMethod_2InitializeWithHandlers(mclOutputHandlerFcn error_handler,
                                       mclOutputHandlerFcn print_handler);

extern LIB_libMethod_2_C_API 
bool libMethod_2Initialize(void);

extern LIB_libMethod_2_C_API 
void libMethod_2Terminate(void);


extern LIB_libMethod_2_C_API 
void mlxMinKsymbProcess_hs(int nlhs, mxArray *plhs[],
                           int nrhs, mxArray *prhs[]);

extern LIB_libMethod_2_C_API 
void mlxIniMethod_hs(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_libMethod_2
#define PUBLIC_libMethod_2_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libMethod_2_CPP_API __declspec(dllimport)
#endif

#define LIB_libMethod_2_CPP_API PUBLIC_libMethod_2_CPP_API

#else

#if !defined(LIB_libMethod_2_CPP_API)
#if defined(LIB_libMethod_2_C_API)
#define LIB_libMethod_2_CPP_API LIB_libMethod_2_C_API
#else
#define LIB_libMethod_2_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libMethod_2_CPP_API void MinKsymbProcess_hs(int nargout
                                                       , mwArray& newopen
                                                       , const mwArray& tph
                                                       , const mwArray& tpl
                                                       , const mwArray& tpc);

extern LIB_libMethod_2_CPP_API void IniMethod_hs(int nargout
                                                 , mwArray& erroCode
                                                 , const mwArray& inph
                                                 , const mwArray& inpl
                                                 , const mwArray& inpc
                                                 , const mwArray& intt
                                                 , const mwArray& inmoney
                                                 , const mwArray& inm
                                                 , const mwArray& inf1
                                                 , const mwArray& inf2
                                                 , const mwArray& inf3);

#endif

#endif
