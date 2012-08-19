//
// MATLAB Compiler: 4.3 (R14SP3)
// Date: Sat Aug 18 15:22:07 2012
// Arguments: "-B" "macro_default" "-W" "cpplib:libMethod_4" "-T" "link:lib"
// "MinKsymbProcess4.m" "IniMethod4.m" "TickStopLoss4.m" "GetInnerState4.m" 
//

#include "mclmcr.h"

#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned char __MCC_libMethod_4_session_key[] = {
        '8', 'D', 'D', 'E', '8', 'C', '6', 'B', 'C', 'F', '6', '0', '4', '5',
        'E', 'C', '9', '6', '4', '2', 'E', 'F', '7', '2', 'B', '2', '9', '0',
        'E', 'E', '1', 'B', 'A', 'C', '6', '6', '4', '3', '3', '3', 'F', 'B',
        '4', '3', '8', '5', '7', 'B', 'B', 'F', 'C', '1', '7', '5', 'F', '3',
        '7', '4', 'D', '7', 'D', '1', '0', '2', 'E', 'C', '9', '5', '6', '1',
        '9', '3', '4', '8', 'F', '7', 'E', 'F', 'E', '6', 'A', '2', 'F', 'B',
        'C', '9', '8', 'F', '0', 'F', '6', 'F', '8', '8', '4', '4', '9', '7',
        'B', '4', '5', 'C', '5', '9', 'B', 'D', 'D', '2', 'F', 'D', 'E', '2',
        '1', '5', '9', '1', '8', '9', 'E', '9', 'F', '6', '3', 'A', '3', '8',
        '0', '1', '7', '7', '8', 'A', '4', '7', '7', '0', 'E', '1', 'A', '1',
        '1', '9', '3', 'C', '2', '0', 'E', '8', 'F', 'F', 'E', 'E', '0', '7',
        'C', 'D', '1', '8', '4', 'E', '1', 'A', '5', '8', 'D', 'D', '7', '4',
        '9', '6', '3', 'D', 'B', 'F', '2', 'C', '2', 'E', '6', '6', '4', '3',
        'D', '9', '6', 'C', '3', '4', 'B', 'C', '6', '0', '7', '9', '0', 'B',
        'F', '3', 'B', '3', '7', '2', '8', '2', 'A', '7', '1', 'B', '4', 'A',
        'E', 'A', '6', '0', 'C', '3', '7', 'D', 'A', 'C', '7', 'E', '0', 'E',
        'B', '5', '0', 'D', '1', '4', '3', '8', '1', 'B', '2', '0', 'A', '4',
        'A', 'C', '5', '9', 'B', '8', '5', 'C', '7', '7', '0', 'E', '0', '9',
        'F', '3', '7', '9', '\0'};

extern const unsigned char __MCC_libMethod_4_public_key[] = {
        '3', '0', '8', '1', '9', 'D', '3', '0', '0', 'D', '0', '6', '0', '9',
        '2', 'A', '8', '6', '4', '8', '8', '6', 'F', '7', '0', 'D', '0', '1',
        '0', '1', '0', '1', '0', '5', '0', '0', '0', '3', '8', '1', '8', 'B',
        '0', '0', '3', '0', '8', '1', '8', '7', '0', '2', '8', '1', '8', '1',
        '0', '0', 'C', '4', '9', 'C', 'A', 'C', '3', '4', 'E', 'D', '1', '3',
        'A', '5', '2', '0', '6', '5', '8', 'F', '6', 'F', '8', 'E', '0', '1',
        '3', '8', 'C', '4', '3', '1', '5', 'B', '4', '3', '1', '5', '2', '7',
        '7', 'E', 'D', '3', 'F', '7', 'D', 'A', 'E', '5', '3', '0', '9', '9',
        'D', 'B', '0', '8', 'E', 'E', '5', '8', '9', 'F', '8', '0', '4', 'D',
        '4', 'B', '9', '8', '1', '3', '2', '6', 'A', '5', '2', 'C', 'C', 'E',
        '4', '3', '8', '2', 'E', '9', 'F', '2', 'B', '4', 'D', '0', '8', '5',
        'E', 'B', '9', '5', '0', 'C', '7', 'A', 'B', '1', '2', 'E', 'D', 'E',
        '2', 'D', '4', '1', '2', '9', '7', '8', '2', '0', 'E', '6', '3', '7',
        '7', 'A', '5', 'F', 'E', 'B', '5', '6', '8', '9', 'D', '4', 'E', '6',
        '0', '3', '2', 'F', '6', '0', 'C', '4', '3', '0', '7', '4', 'A', '0',
        '4', 'C', '2', '6', 'A', 'B', '7', '2', 'F', '5', '4', 'B', '5', '1',
        'B', 'B', '4', '6', '0', '5', '7', '8', '7', '8', '5', 'B', '1', '9',
        '9', '0', '1', '4', '3', '1', '4', 'A', '6', '5', 'F', '0', '9', '0',
        'B', '6', '1', 'F', 'C', '2', '0', '1', '6', '9', '4', '5', '3', 'B',
        '5', '8', 'F', 'C', '8', 'B', 'A', '4', '3', 'E', '6', '7', '7', '6',
        'E', 'B', '7', 'E', 'C', 'D', '3', '1', '7', '8', 'B', '5', '6', 'A',
        'B', '0', 'F', 'A', '0', '6', 'D', 'D', '6', '4', '9', '6', '7', 'C',
        'B', '1', '4', '9', 'E', '5', '0', '2', '0', '1', '1', '1', '\0'};

static const char * MCC_libMethod_4_matlabpath_data[] = 
    { "libMethod_4/", "work_dt/", "toolbox/compiler/deploy/",
      "$TOOLBOXMATLABDIR/general/", "$TOOLBOXMATLABDIR/ops/",
      "$TOOLBOXMATLABDIR/lang/", "$TOOLBOXMATLABDIR/elmat/",
      "$TOOLBOXMATLABDIR/elfun/", "$TOOLBOXMATLABDIR/specfun/",
      "$TOOLBOXMATLABDIR/matfun/", "$TOOLBOXMATLABDIR/datafun/",
      "$TOOLBOXMATLABDIR/polyfun/", "$TOOLBOXMATLABDIR/funfun/",
      "$TOOLBOXMATLABDIR/sparfun/", "$TOOLBOXMATLABDIR/scribe/",
      "$TOOLBOXMATLABDIR/graph2d/", "$TOOLBOXMATLABDIR/graph3d/",
      "$TOOLBOXMATLABDIR/specgraph/", "$TOOLBOXMATLABDIR/graphics/",
      "$TOOLBOXMATLABDIR/uitools/", "$TOOLBOXMATLABDIR/strfun/",
      "$TOOLBOXMATLABDIR/imagesci/", "$TOOLBOXMATLABDIR/iofun/",
      "$TOOLBOXMATLABDIR/audiovideo/", "$TOOLBOXMATLABDIR/timefun/",
      "$TOOLBOXMATLABDIR/datatypes/", "$TOOLBOXMATLABDIR/verctrl/",
      "$TOOLBOXMATLABDIR/codetools/", "$TOOLBOXMATLABDIR/helptools/",
      "$TOOLBOXMATLABDIR/winfun/", "$TOOLBOXMATLABDIR/demos/",
      "$TOOLBOXMATLABDIR/timeseries/", "$TOOLBOXMATLABDIR/hds/",
      "toolbox/local/", "toolbox/compiler/" };

static const char * MCC_libMethod_4_classpath_data[] = 
    { "" };

static const char * MCC_libMethod_4_libpath_data[] = 
    { "" };

static const char * MCC_libMethod_4_app_opts_data[] = 
    { "" };

static const char * MCC_libMethod_4_run_opts_data[] = 
    { "" };

static const char * MCC_libMethod_4_warning_state_data[] = 
    { "" };


mclComponentData __MCC_libMethod_4_component_data = { 

    /* Public key data */
    __MCC_libMethod_4_public_key,

    /* Component name */
    "libMethod_4",

    /* Component Root */
    "",

    /* Application key data */
    __MCC_libMethod_4_session_key,

    /* Component's MATLAB Path */
    MCC_libMethod_4_matlabpath_data,

    /* Number of directories in the MATLAB Path */
    35,

    /* Component's Java class path */
    MCC_libMethod_4_classpath_data,
    /* Number of directories in the Java class path */
    0,

    /* Component's load library path (for extra shared libraries) */
    MCC_libMethod_4_libpath_data,
    /* Number of directories in the load library path */
    0,

    /* MCR instance-specific runtime options */
    MCC_libMethod_4_app_opts_data,
    /* Number of MCR instance-specific runtime options */
    0,

    /* MCR global runtime options */
    MCC_libMethod_4_run_opts_data,
    /* Number of MCR global runtime options */
    0,
    
    /* Component preferences directory */
    "libMethod_4_7A2331AC6862460D6B3E64C6F78F8A1B",

    /* MCR warning status data */
    MCC_libMethod_4_warning_state_data,
    /* Number of MCR warning status modifiers */
    0,

    /* Path to component - evaluated at runtime */
    NULL

};

#ifdef __cplusplus
}
#endif


