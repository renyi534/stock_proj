# Microsoft Developer Studio Project File - Name="TradeSystem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TradeSystem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TradeSystem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TradeSystem.mak" CFG="TradeSystem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TradeSystem - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TradeSystem - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TradeSystem - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "TradeSystem - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "D:\Program Files\MATLAB71\extern\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"D:\Program Files\MATLAB71\extern\lib\win32\microsoft\\"

!ENDIF 

# Begin Target

# Name "TradeSystem - Win32 Release"
# Name "TradeSystem - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AlgoResHandlingThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Algorithm.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgorithmCfgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataSimulator.cpp
# End Source File
# Begin Source File

SOURCE=.\DbAccessor.cpp
# End Source File
# Begin Source File

SOURCE=.\DbAccessorPool.cpp
# End Source File
# Begin Source File

SOURCE=.\DtAlgorithm.cpp
# End Source File
# Begin Source File

SOURCE=.\HalfMinuteSeriesGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\HsAlgorithm.cpp
# End Source File
# Begin Source File

SOURCE=.\KSeriesGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\mdspi.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageRouter.cpp
# End Source File
# Begin Source File

SOURCE=.\OneMinuteSeriesGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderTradeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RandomAlgorithm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StockRetriever.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrlSSL.cpp
# End Source File
# Begin Source File

SOURCE=.\TabPageSSL.cpp
# End Source File
# Begin Source File

SOURCE=.\TenMinuteSeriesGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadPool.cpp
# End Source File
# Begin Source File

SOURCE=.\TraderSpi.cpp
# End Source File
# Begin Source File

SOURCE=.\TradeSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\TradeSystem.rc
# End Source File
# Begin Source File

SOURCE=.\TradeSystemDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TradeSystemView.cpp
# End Source File
# Begin Source File

SOURCE=.\WeightedAlgorithm.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Algorithm.h
# End Source File
# Begin Source File

SOURCE=.\AlgorithmCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataSimulator.h
# End Source File
# Begin Source File

SOURCE=.\DbAccessor.h
# End Source File
# Begin Source File

SOURCE=.\DbAccessorPool.h
# End Source File
# Begin Source File

SOURCE=.\DbConn.h
# End Source File
# Begin Source File

SOURCE=.\DtAlgorithm.h
# End Source File
# Begin Source File

SOURCE=.\HalfMinuteSeriesGenerator.h
# End Source File
# Begin Source File

SOURCE=.\HsAlgorithm.h
# End Source File
# Begin Source File

SOURCE=.\KSeriesGenerator.h
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_1.h
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_2.h
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_3.h
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_4.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# Begin Source File

SOURCE=.\MdSpi.h
# End Source File
# Begin Source File

SOURCE=.\MessageRouter.h
# End Source File
# Begin Source File

SOURCE=.\OneMinuteData.h
# End Source File
# Begin Source File

SOURCE=.\OneMinuteSeriesGenerator.h
# End Source File
# Begin Source File

SOURCE=.\OrderTradeDlg.h
# End Source File
# Begin Source File

SOURCE=.\RandomAlgorithm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RunObject.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StockRetriever.h
# End Source File
# Begin Source File

SOURCE=.\TabCtrlSSL.h
# End Source File
# Begin Source File

SOURCE=.\TabPageSSL.h
# End Source File
# Begin Source File

SOURCE=.\TenMinuteSeriesGenerator.h
# End Source File
# Begin Source File

SOURCE=.\ThostTraderApi\ThostFtdcMdApi.h
# End Source File
# Begin Source File

SOURCE=.\ThostTraderApi\ThostFtdcTraderApi.h
# End Source File
# Begin Source File

SOURCE=.\ThostTraderApi\ThostFtdcUserApiDataType.h
# End Source File
# Begin Source File

SOURCE=.\ThostTraderApi\ThostFtdcUserApiStruct.h
# End Source File
# Begin Source File

SOURCE=.\ThreadPool.h
# End Source File
# Begin Source File

SOURCE=.\TradeConn.h
# End Source File
# Begin Source File

SOURCE=.\TradeHandlingThread.h
# End Source File
# Begin Source File

SOURCE=.\TraderSpi.h
# End Source File
# Begin Source File

SOURCE=.\TradeSystem.h
# End Source File
# Begin Source File

SOURCE=.\TradeSystemDoc.h
# End Source File
# Begin Source File

SOURCE=.\TradeSystemView.h
# End Source File
# Begin Source File

SOURCE=.\utility.h
# End Source File
# Begin Source File

SOURCE=.\WeightedAlgorithm.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TradeSystem.ico
# End Source File
# Begin Source File

SOURCE=.\res\TradeSystem.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TradeSystemDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\data_extraction.sql
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_1.dll

!IF  "$(CFG)" == "TradeSystem - Win32 Release"

!ELSEIF  "$(CFG)" == "TradeSystem - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_2.dll
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_3.dll
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_4.dll
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ThostTraderApi\thostmduserapi.dll
# End Source File
# Begin Source File

SOURCE=.\ThostTraderApi\thosttraderapi.dll
# End Source File
# Begin Source File

SOURCE=.\ThostTraderApi\thostmduserapi.lib
# End Source File
# Begin Source File

SOURCE=.\ThostTraderApi\thosttraderapi.lib
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_1.lib
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_2.lib
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_3.lib
# End Source File
# Begin Source File

SOURCE=.\matlab\libMethod_4.lib
# End Source File
# End Target
# End Project
