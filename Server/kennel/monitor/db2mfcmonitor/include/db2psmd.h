/******************************************************************************
**
** Source File Name = DB2PSMD.H
**
** (C) COPYRIGHT International Business Machines Corp. 2001
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              SQL Debugger and Trace Internal APIs
**
** Operating System = Common C Include File
**
** Last Changed =  %E%  %U%
**
******************************************************************************/

#ifndef _DB2PSMD_H_
#define _DB2PSMD_H_


/******************************************************************************
** Misc Defines
******************************************************************************/
#ifdef DB2API
 #undef DB2API
#endif

#if defined(__IBMCPP__) || defined(__IBMC__)
 #define DB2API __cdecl
#else
 #define DB2API
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/******************************************************************************
** PSMD Flow Control Codes
******************************************************************************/
#define PD_FLOW_CONTINUE        0
#define PD_FLOW_TERMINATE       999


/******************************************************************************
** PSMD Error Codes
******************************************************************************/
#define PD_ERR_TIMEOUT     6


/******************************************************************************
** PSMD Routine Types
******************************************************************************/
#define PD_RTYPE_PROCEDURE 0
#define PD_RTYPE_FUNCTION  1
#define PD_RTYPE_TRIGGER   2
#define PD_RTYPE_STATEMENT 3


/******************************************************************************
** PSMD Variable Types
******************************************************************************/
#define PD_VTYPE_SMALLINT  0
#define PD_VTYPE_INT       1
#define PD_VTYPE_LARGEINT  2
#define PD_VTYPE_FLOAT     3
#define PD_VTYPE_DOUBLE    4
#define PD_VTYPE_TCHAR     5
#define PD_VTYPE_BCHAR     6
#define PD_VTYPE_TSQLCHAR  7
#define PD_VTYPE_BSQLCHAR  8
#define PD_VTYPE_TSQLLOB   9
#define PD_VTYPE_BSQLLOB   10

#define PD_VTYPE_DATE      11
#define PD_VTYPE_TIME      12
#define PD_VTYPE_TIMESTAMP 13

#define PD_VTYPE_GRAPHIC   14
#define PD_VTYPE_SQLDBCHAR 15
#define PD_VTYPE_DBCLOB    16

#define PD_VTYPE_DECIMAL   17


/******************************************************************************
** PSMD CodeGen APIs
******************************************************************************/
#define pd_DefineHandle() \
   void* psmdHandle = 0

#ifndef PSMDBG
 #define pd_EnterRoutine(Line, pName, pSchema, pSpecific, Type)
 #define pd_AtLine(SqlCode, pSqlState, Line, Indent)
 #define pd_VarRegister(pVarName, pVarScope, pVarAddr, VarType, VarSize, VarScale)
 #define pd_VarMayChanged(pVarName, pVarScope)
 #define pd_EnterException(SqlCode, pSqlState, Line, Indent)
 #define pd_ExitException()
 #define pd_ExitRoutine(Line, rc)

#else
 #define pd_EnterRoutineC(Line, pName, pSchema, pSpecific, Type, CCSID) \
   pda_EnterRoutineC(&psmdHandle, Line, pName, pSchema, pSpecific, Type, CCSID)

 #define pd_EnterRoutine(Line, pName, pSchema, pSpecific, Type) \
   pda_EnterRoutine(&psmdHandle, Line, pName, pSchema, pSpecific, Type)

 #define pd_AtLine(SqlCode, pSqlState, Line, Indent) \
   if (0 != psmdHandle) pda_AtLine(&psmdHandle, SqlCode, pSqlState, Line, Indent)

 #define pd_VarRegisterN(pVarName, pVarScope, pVarAddr, pIndicator, VarType, VarSize, VarScale, CCSID) \
   if (0 != psmdHandle) pda_VarRegisterN(&psmdHandle, pVarName, pVarScope, pVarAddr, pIndicator, \
                                         VarType, VarSize, VarScale, CCSID)

 #define pd_VarRegister(pVarName, pVarScope, pVarAddr, VarType, VarSize, VarScale) \
   if (0 != psmdHandle) pda_VarRegister(&psmdHandle, pVarName, pVarAddr, VarType, VarSize, VarScale)

 #define pd_VarMayChanged(pVarName, pVarScope) \
   if (0 != psmdHandle) pda_VarMayChanged(&psmdHandle, pVarName, 1)

 #define pd_EnterException(SqlCode, pSqlState, Line, Indent) \
   { void* psmdHandle = 0; \
     pda_EnterException(&psmdHandle, SqlCode, pSqlState, Line, Indent)

 #define pd_ExitException() \
     if (0 != psmdHandle) pda_ExitException(&psmdHandle); \
   }

 #define pd_ExitRoutine(Line, rc) \
   if (0 != psmdHandle) pda_ExitRoutine(&psmdHandle, Line, rc)

#endif


/******************************************************************************
** PSMD Internal Implementation APIs
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

 void DB2API pda_EnterRoutineC(
        void**         ppHandle,
        int            Line,
        char*          pName,
        char*          pSchema,
        char*          pSpecific,
        int            Type,
        int            CCSID);

 void DB2API pda_EnterRoutine(
        void**         ppHandle,
        int            Line,
        char*          pName,
        char*          pSchema,
        char*          pSpecific,
        int            Type);

 int  DB2API pda_AtLine(
        void**         ppHandle,
        int            SqlCode,
        char*          pSqlState,
        int            Line,
        int            Indent);

 void DB2API pda_VarRegisterN(
        void**         ppHandle,
        char*          pVarName,
        char*          pVarScope,
        void*          pVarAddr,
        void*          pIndicator,
        int            VarType,
        int            VarSize,
        int            VarScale,
        int            CCSID);

 void DB2API pda_VarRegister(
        void**         ppHandle,
        char*          pVarName,
        void*          pVarAddr,
        int            VarType,
        int            VarSize,
        int            VarScale);

 void DB2API pda_VarMayChanged(
        void**         ppHandle,
        char*          pVarName,
        int            TestAll);

 void DB2API pda_EnterException(
        void**         ppHandle,
        int            SqlCode,
        char*          pSqlState,
        int            Line,
        int            Indent);

 void DB2API pda_ExitException(
        void**         ppHandle);

 void DB2API pda_ExitRoutine(
        void**         ppHandle,
        int            Line,
        int            rc);

 void DB2API pda_SetSessionId(
        double         SessionId);

 double DB2API pda_GetSessionId();

 void DB2API pda_CleanupSession(
        double         SessionId);

 void DB2API pda_CompileForDebug(
        int            DebugMode);


 /* PLATFORM SPECIFIC INFORMATION (LUW, z/OS, iSeries) */
 #define PSMD_REQUESTREPLY_XML_SZ  4000
 #define PSMD_REQUESTREPLY_VERSION "1.0"
 #define PSMD_DB2_PLATFORM         "LUW"
 #define PSMD_DB2_VERSION          "8.2"
 #define PSMD_DEBUGGER_LEVEL       "2.2"
 #define PSMD_DEBUGGER_BUILD       "040112"

 void DB2API pda_DebuggerLevel(
        char*          pRequestXML,
        char*          pReplyXML);

/******************************************************************************
** PSMD Trace APIs
******************************************************************************/
 #define PSMD_TRACE_MODE_OFF   0
 #define PSMD_TRACE_MODE_BASIC 1

 int DB2API pda_SetTraceMode(
        int            TraceMode);

 int DB2API pda_GetTraceMode();

#ifdef __cplusplus
};
#endif


/******************************************************************************
******************************************************************************/

#endif
