//
// MATLAB Compiler: 4.3 (R14SP3)
// Date: Sat Aug 18 15:22:07 2012
// Arguments: "-B" "macro_default" "-W" "cpplib:libMethod_4" "-T" "link:lib"
// "MinKsymbProcess4.m" "IniMethod4.m" "TickStopLoss4.m" "GetInnerState4.m" 
//

#include <stdio.h>
#define EXPORTING_libMethod_4 1
#include "libMethod_4.h"
#ifdef __cplusplus
extern "C" {
#endif

extern mclComponentData __MCC_libMethod_4_component_data;

#ifdef __cplusplus
}
#endif


static HMCRINSTANCE _mcr_inst = NULL;


#if defined( _MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__) || defined(__LCC__)
#include <windows.h>

static char path_to_dll[_MAX_PATH];

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, void *pv)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        char szDllPath[_MAX_PATH];
        char szDir[_MAX_DIR];
        if (GetModuleFileName(hInstance, szDllPath, _MAX_PATH) > 0)
        {
             _splitpath(szDllPath, path_to_dll, szDir, NULL, NULL);
            strcat(path_to_dll, szDir);
        }
	else return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }
    return TRUE;
}
#endif
static int mclDefaultPrintHandler(const char *s)
{
    return fwrite(s, sizeof(char), strlen(s), stdout);
}

static int mclDefaultErrorHandler(const char *s)
{
    int written = 0, len = 0;
    len = strlen(s);
    written = fwrite(s, sizeof(char), len, stderr);
    if (len > 0 && s[ len-1 ] != '\n')
        written += fwrite("\n", sizeof(char), 1, stderr);
    return written;
}


/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libMethod_4_C_API 
#define LIB_libMethod_4_C_API /* No special import/export declaration */
#endif

LIB_libMethod_4_C_API 
bool libMethod_4InitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler
)
{
    if (_mcr_inst != NULL)
        return true;
    if (!mclmcrInitialize())
        return false;
    if (!mclInitializeComponentInstance(&_mcr_inst,
                                        &__MCC_libMethod_4_component_data,
                                        true, NoObjectType, LibTarget,
                                        error_handler, print_handler))
        return false;
    return true;
}

LIB_libMethod_4_C_API 
bool libMethod_4Initialize(void)
{
    return libMethod_4InitializeWithHandlers(mclDefaultErrorHandler,
                                             mclDefaultPrintHandler);
}

LIB_libMethod_4_C_API 
void libMethod_4Terminate(void)
{
    if (_mcr_inst != NULL)
        mclTerminateInstance(&_mcr_inst);
}


LIB_libMethod_4_C_API 
void mlxMinKsymbProcess4(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
    mclFeval(_mcr_inst, "MinKsymbProcess4", nlhs, plhs, nrhs, prhs);
}

LIB_libMethod_4_C_API 
void mlxIniMethod4(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
    mclFeval(_mcr_inst, "IniMethod4", nlhs, plhs, nrhs, prhs);
}

LIB_libMethod_4_C_API 
void mlxTickStopLoss4(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
    mclFeval(_mcr_inst, "TickStopLoss4", nlhs, plhs, nrhs, prhs);
}

LIB_libMethod_4_C_API 
void mlxGetInnerState4(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
    mclFeval(_mcr_inst, "GetInnerState4", nlhs, plhs, nrhs, prhs);
}

LIB_libMethod_4_CPP_API 
void MinKsymbProcess4(int nargout, mwArray& newopen, mwArray& price
                      , const mwArray& to, const mwArray& th
                      , const mwArray& tl, const mwArray& tc)
{
    mclcppMlfFeval(_mcr_inst, "MinKsymbProcess4", nargout, 2,
                   4, &newopen, &price, &to, &th, &tl, &tc);
}

LIB_libMethod_4_CPP_API 
void IniMethod4(int nargout, mwArray& erroCode, const mwArray& inop
                , const mwArray& inhp, const mwArray& inlp
                , const mwArray& incp, const mwArray& inn1
                , const mwArray& inkb, const mwArray& inkm, const mwArray& inw
                , const mwArray& inwl, const mwArray& inks)
{
    mclcppMlfFeval(_mcr_inst, "IniMethod4", nargout, 1, 10,
                   &erroCode, &inop, &inhp, &inlp, &incp,
                   &inn1, &inkb, &inkm, &inw, &inwl, &inks);
}

LIB_libMethod_4_CPP_API 
void TickStopLoss4(int nargout, mwArray& stopopen
                   , const mwArray& ia, const mwArray& ib)
{
    mclcppMlfFeval(_mcr_inst, "TickStopLoss4", nargout,
                   1, 2, &stopopen, &ia, &ib);
}

LIB_libMethod_4_CPP_API 
void GetInnerState4(int nargout, mwArray& s_m, mwArray& s_e, mwArray& s_ud
                    , mwArray& s_pacounter, mwArray& s_nacounter
                    , mwArray& s_pisstart, mwArray& s_nisstart)
{
    mclcppMlfFeval(_mcr_inst, "GetInnerState4", nargout, 7,
                   0, &s_m, &s_e, &s_ud, &s_pacounter,
                   &s_nacounter, &s_pisstart, &s_nisstart);
}
