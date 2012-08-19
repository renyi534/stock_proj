//
// MATLAB Compiler: 4.3 (R14SP3)
// Date: Sun Aug 19 21:07:02 2012
// Arguments: "-B" "macro_default" "-W" "cpplib:libMethod_4" "-T" "link:lib"
// "MinKsymbProcess4.m" "IniMethod4.m" "TickStopLoss4.m" "GetInnerState4.m" 
//

#ifndef __libMethod_4_h
#define __libMethod_4_h 1

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

#ifdef EXPORTING_libMethod_4
#define PUBLIC_libMethod_4_C_API __global
#else
#define PUBLIC_libMethod_4_C_API /* No import statement needed. */
#endif

#define LIB_libMethod_4_C_API PUBLIC_libMethod_4_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_libMethod_4
#define PUBLIC_libMethod_4_C_API __declspec(dllexport)
#else
#define PUBLIC_libMethod_4_C_API __declspec(dllimport)
#endif

#define LIB_libMethod_4_C_API PUBLIC_libMethod_4_C_API


#else

#define LIB_libMethod_4_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libMethod_4_C_API 
#define LIB_libMethod_4_C_API /* No special import/export declaration */
#endif

extern LIB_libMethod_4_C_API 
bool libMethod_4InitializeWithHandlers(mclOutputHandlerFcn error_handler,
                                       mclOutputHandlerFcn print_handler);

extern LIB_libMethod_4_C_API 
bool libMethod_4Initialize(void);

extern LIB_libMethod_4_C_API 
void libMethod_4Terminate(void);


extern LIB_libMethod_4_C_API 
void mlxMinKsymbProcess4(int nlhs, mxArray *plhs[],
                         int nrhs, mxArray *prhs[]);

extern LIB_libMethod_4_C_API 
void mlxIniMethod4(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libMethod_4_C_API 
void mlxTickStopLoss4(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libMethod_4_C_API 
void mlxGetInnerState4(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_libMethod_4
#define PUBLIC_libMethod_4_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libMethod_4_CPP_API __declspec(dllimport)
#endif

#define LIB_libMethod_4_CPP_API PUBLIC_libMethod_4_CPP_API

#else

#if !defined(LIB_libMethod_4_CPP_API)
#if defined(LIB_libMethod_4_C_API)
#define LIB_libMethod_4_CPP_API LIB_libMethod_4_C_API
#else
#define LIB_libMethod_4_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libMethod_4_CPP_API void MinKsymbProcess4(int nargout
                                                     , mwArray& newopen
                                                     , mwArray& price
                                                     , const mwArray& to
                                                     , const mwArray& th
                                                     , const mwArray& tl
                                                     , const mwArray& tc);

extern LIB_libMethod_4_CPP_API void IniMethod4(int nargout, mwArray& erroCode
                                               , const mwArray& inop
                                               , const mwArray& inhp
                                               , const mwArray& inlp
                                               , const mwArray& incp
                                               , const mwArray& inn1
                                               , const mwArray& inkb
                                               , const mwArray& inkm
                                               , const mwArray& inw
                                               , const mwArray& inwl
                                               , const mwArray& inks);

extern LIB_libMethod_4_CPP_API void TickStopLoss4(int nargout
                                                  , mwArray& stopopen
                                                  , const mwArray& ia
                                                  , const mwArray& ib);

extern LIB_libMethod_4_CPP_API void GetInnerState4(int nargout, mwArray& s_m
                                                   , mwArray& s_e
                                                   , mwArray& s_ud
                                                   , mwArray& s_pacounter
                                                   , mwArray& s_nacounter
                                                   , mwArray& s_pisstart
                                                   , mwArray& s_nisstart);

#endif

#endif
