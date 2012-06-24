/*
 * MATLAB Compiler: 4.3 (R14SP3)
 * Date: Sat Jun 09 10:17:59 2012
 * Arguments: "-B" "macro_default" "-W" "lib:libMethod_1" "-T" "link:lib"
 * "MinKsymbProcess.m" "IniMethod.m" "GetK.m" "MyFigure.m" 
 */

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
void mlxGetK(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
    mclFeval(_mcr_inst, "GetK", nlhs, plhs, nrhs, prhs);
}

LIB_libMethod_1_C_API 
void mlxMyFigure(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
    mclFeval(_mcr_inst, "MyFigure", nlhs, plhs, nrhs, prhs);
}

LIB_libMethod_1_C_API 
void mlfMinKsymbProcess(int nargout, mxArray** newopen, mxArray** newm
                        , mxArray** newe, mxArray** newrm
                        , mxArray** newre, mxArray** price
                        , mxArray* to, mxArray* th, mxArray* tl
                        , mxArray* tc, mxArray* trm, mxArray* tre)
{
    mclMlfFeval(_mcr_inst, "MinKsymbProcess", nargout, 6, 6, newopen, newm,
                newe, newrm, newre, price, to, th, tl, tc, trm, tre);
}

LIB_libMethod_1_C_API 
void mlfIniMethod(int nargout, mxArray** erroCode, mxArray* inop
                  , mxArray* inhp, mxArray* inlp, mxArray* incp
                  , mxArray* inm, mxArray* inrm, mxArray* ine, mxArray* inre
                  , mxArray* insp, mxArray* inp, mxArray* inw, mxArray* inwl
                  , mxArray* inkb, mxArray* inks, mxArray* inkm
                  , mxArray* inul, mxArray* indl)
{
    mclMlfFeval(_mcr_inst, "IniMethod", nargout, 1, 17, erroCode,
                inop, inhp, inlp, incp, inm, inrm, ine, inre, insp,
                inp, inw, inwl, inkb, inks, inkm, inul, indl);
}

LIB_libMethod_1_C_API 
void mlfGetK(int nargout, mxArray** o, mxArray** h, mxArray** l
             , mxArray** c, mxArray** v, mxArray* isini)
{
    mclMlfFeval(_mcr_inst, "GetK", nargout, 5, 1, o, h, l, c, v, isini);
}

LIB_libMethod_1_C_API 
void mlfMyFigure(mxArray* f11, mxArray* f12, mxArray* f21
                 , mxArray* f22, mxArray* isd)
{
    mclMlfFeval(_mcr_inst, "MyFigure", 0, 0, 5, f11, f12, f21, f22, isd);
}
