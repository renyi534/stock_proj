//
// MATLAB Compiler: 4.3 (R14SP3)
// Date: Sun Jun 10 17:29:33 2012
// Arguments: "-B" "macro_default" "-G" "-W" "cpplib:libMethod_1" "-T"
// "link:lib" "MinKsymbProcess.m" "IniMethod.m" "GetK.m" "MyFigure.m" 
//

#ifndef __libMethod_1_h
#define __libMethod_1_h 1

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

#ifdef EXPORTING_libMethod_1
#define PUBLIC_libMethod_1_C_API __global
#else
#define PUBLIC_libMethod_1_C_API /* No import statement needed. */
#endif

#define LIB_libMethod_1_C_API PUBLIC_libMethod_1_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_libMethod_1
#define PUBLIC_libMethod_1_C_API __declspec(dllexport)
#else
#define PUBLIC_libMethod_1_C_API __declspec(dllimport)
#endif

#define LIB_libMethod_1_C_API PUBLIC_libMethod_1_C_API


#else

#define LIB_libMethod_1_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libMethod_1_C_API 
#define LIB_libMethod_1_C_API /* No special import/export declaration */
#endif

extern LIB_libMethod_1_C_API 
bool libMethod_1InitializeWithHandlers(mclOutputHandlerFcn error_handler,
                                       mclOutputHandlerFcn print_handler);

extern LIB_libMethod_1_C_API 
bool libMethod_1Initialize(void);

extern LIB_libMethod_1_C_API 
void libMethod_1Terminate(void);


extern LIB_libMethod_1_C_API 
void mlxMinKsymbProcess(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libMethod_1_C_API 
void mlxIniMethod(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libMethod_1_C_API 
void mlxGetK(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libMethod_1_C_API 
void mlxMyFigure(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_libMethod_1
#define PUBLIC_libMethod_1_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libMethod_1_CPP_API __declspec(dllimport)
#endif

#define LIB_libMethod_1_CPP_API PUBLIC_libMethod_1_CPP_API

#else

#if !defined(LIB_libMethod_1_CPP_API)
#if defined(LIB_libMethod_1_C_API)
#define LIB_libMethod_1_CPP_API LIB_libMethod_1_C_API
#else
#define LIB_libMethod_1_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libMethod_1_CPP_API void MinKsymbProcess(int nargout
                                                    , mwArray& newopen
                                                    , mwArray& newm
                                                    , mwArray& newe
                                                    , mwArray& newrm
                                                    , mwArray& newre
                                                    , mwArray& price
                                                    , const mwArray& to
                                                    , const mwArray& th
                                                    , const mwArray& tl
                                                    , const mwArray& tc
                                                    , const mwArray& trm
                                                    , const mwArray& tre);

extern LIB_libMethod_1_CPP_API void IniMethod(int nargout, mwArray& erroCode
                                              , const mwArray& inop
                                              , const mwArray& inhp
                                              , const mwArray& inlp
                                              , const mwArray& incp
                                              , const mwArray& inm
                                              , const mwArray& inrm
                                              , const mwArray& ine
                                              , const mwArray& inre
                                              , const mwArray& insp
                                              , const mwArray& inp
                                              , const mwArray& inw
                                              , const mwArray& inwl
                                              , const mwArray& inkb
                                              , const mwArray& inks
                                              , const mwArray& inkm
                                              , const mwArray& inul
                                              , const mwArray& indl);

extern LIB_libMethod_1_CPP_API void GetK(int nargout, mwArray& o, mwArray& h
                                         , mwArray& l, mwArray& c
                                         , mwArray& v, mwArray& num
                                         , const mwArray& isini);

extern LIB_libMethod_1_CPP_API void MyFigure(const mwArray& f11
                                             , const mwArray& f12
                                             , const mwArray& f21
                                             , const mwArray& f22
                                             , const mwArray& isd);

#endif

#endif
