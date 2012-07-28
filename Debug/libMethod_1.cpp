//
// MATLAB Compiler: 4.3 (R14SP3)
// Date: Sun Jul 22 17:33:34 2012
// Arguments: "-B" "macro_default" "-W" "cpplib:libMethod_1" "-T" "link:lib"
// "MinKsymbProcess.m" "IniMethod.m" "GetInnerState.m" 
//

#include <stdio.h>
#define EXPORTING_libMethod_1 1
#include "libMethod_1.h"
#ifdef __cplusplus
extern "C" {
#endif

extern mclComponentData __MCC_libMethod_1_component_data;

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
#ifndef LIB_libMethod_1_C_API 
#define LIB_libMethod_1_C_API /* No special import/export declaration */
#endif

LIB_libMethod_1_C_API 
bool libMethod_1InitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler
)
{
    if (_mcr_inst != NULL)
        return true;
    if (!mclmcrInitialize())
        return false;
    if (!mclInitializeComponentInstance(&_mcr_inst,
                                        &__MCC_libMethod_1_component_data,
                                        true, NoObjectType, LibTarget,
                                        error_handler, print_handler))
        return false;
    return true;
}

LIB_libMethod_1_C_API 
bool libMethod_1Initialize(void)
{
    return libMethod_1InitializeWithHandlers(mclDefaultErrorHandler,
                                             mclDefaultPrintHandler);
}

LIB_libMethod_1_C_API 
void libMethod_1Terminate(void)
{
    if (_mcr_inst != NULL)
        mclTerminateInstance(&_mcr_inst);
}


LIB_libMethod_1_C_API 
void mlxMinKsymbProcess(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
    mclFeval(_mcr_inst, "MinKsymbProcess", nlhs, plhs, nrhs, prhs);
}

LIB_libMethod_1_C_API 
void mlxIniMethod(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
    mclFeval(_mcr_inst, "IniMethod", nlhs, plhs, nrhs, prhs);
}

LIB_libMethod_1_C_API 
void mlxGetInnerState(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
    mclFeval(_mcr_inst, "GetInnerState", nlhs, plhs, nrhs, prhs);
}

LIB_libMethod_1_CPP_API 
void MinKsymbProcess(int nargout, mwArray& newopen, mwArray& price
                     , const mwArray& to, const mwArray& th, const mwArray& tl
                     , const mwArray& tc, const mwArray& tv)
{
    mclcppMlfFeval(_mcr_inst, "MinKsymbProcess", nargout, 2, 5,
                   &newopen, &price, &to, &th, &tl, &tc, &tv);
}

LIB_libMethod_1_CPP_API 
void IniMethod(int nargout, mwArray& erroCode, const mwArray& inop
               , const mwArray& inhp, const mwArray& inlp, const mwArray& incp
               , const mwArray& intime, const mwArray& insp
               , const mwArray& inp, const mwArray& inw, const mwArray& inwl
               , const mwArray& inkb, const mwArray& inks, const mwArray& inkm
               , const mwArray& inul, const mwArray& indl)
{
    mclcppMlfFeval(_mcr_inst, "IniMethod", nargout, 1, 14, &erroCode,
                   &inop, &inhp, &inlp, &incp, &intime, &insp, &inp,
                   &inw, &inwl, &inkb, &inks, &inkm, &inul, &indl);
}

LIB_libMethod_1_CPP_API 
void GetInnerState(int nargout, mwArray& s_m, mwArray& s_e
                   , mwArray& s_atr, mwArray& s_stop, mwArray& s_trend)
{
    mclcppMlfFeval(_mcr_inst, "GetInnerState", nargout, 5, 0,
                   &s_m, &s_e, &s_atr, &s_stop, &s_trend);
}
