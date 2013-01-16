      *************************************************************************
      * 
      * Source File Name: SQLENV
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1987, 1997
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function = Include File defining:
      *              Environment Commands - Constants
      *              Environment Commands - Structures
      *              Environment Commands - Function Prototypes
      *              Environment Commands - Labels for SQLCODES
      * 
      * Operating System: Windows NT
      * 
      **************************************************************************
      * Interface Parameters/Structures/Return Codes

      * Parameters for START USING DATABASE
      * USE = Share
       78  SQL-USE-SHR               value    "S".
      * USE = Exclusive
       78  SQL-USE-EXC               value    "X".
      * USE = Exclusive on Single Node
       78  SQL-USE-EXC-SN            value    "N".

      * Sizes for entries in sqledinfo, sqleninfo, sqledbstat, sqleusrstat,
      * sql_dir_entry, sqle_reg_nwbindery
      * Alias name
       78  SQL-ALIAS-SZ              VALUE    8.
      * User Id
       78  SQL-USERID-SZ             VALUE    30.
      * Max User Id Size
       78  SQL-MAX-USERID-SZ         VALUE    1024.
      * Auth Id
       78  SQL-AUTHID-SZ             VALUE    30.
      * Database name
       78  SQL-DBNAME-SZ             VALUE    8.
      * plugin name
       78  SQL-PLUGIN-NAME-SZ        VALUE    32.
      * GSS API plugin list
       78  SQL-SRVCON-GSSPLUGIN-LIST-SZ VALUE 255.

      * Drive (Qualified) - OS/2
       78  SQL-DRIVE-SZ-DB2OS2       VALUE    2.
      * Drive (Qualified) - Windows
       78  SQL-DRIVE-SZ-DB2DOS       VALUE    2.
      * Drive (Qualified) - NT
       78  SQL-DRIVE-SZ-DB2WINT      VALUE    12.
      * Path (Qualified) - AIX/UNIX
       78  SQL-DRIVE-SZ-DB2AIX       VALUE    215.
       78  SQL-DRIVE-SZ              value    12.

      * Internal db name
       78  SQL-INAME-SZ              VALUE    8.
      * Node name
       78  SQL-NNAME-SZ              VALUE    8.
      * Instance Name
       78  SQL-INSTNAME-SZ           VALUE    8.
      * Type of database
       78  SQL-DBTYP-SZ              VALUE    20.
      * Comment
       78  SQL-CMT-SZ                VALUE    30.
      * Local_lu
       78  SQL-LOCLU-SZ              VALUE    8.
      * Partner_lu
       78  SQL-RMTLU-SZ              VALUE    8.
      * Mode
       78  SQL-MODE-SZ               VALUE    8.
      * length of tpname
       78  SQL-TPNAME-SZ             VALUE    64.
      * Reserved
       78  SQL-NRESERVE-SZ           VALUE    0.
      * Alias name
       78  SQL-DBSTAT-ALIAS-SZ       VALUE    16.
      * Database name
       78  SQL-DBSTAT-DBNAME-SZ      VALUE    16.
      * Host database name
       78  SQL-LONG-NAME-SZ          VALUE    18.
      * collating sequence
       78  SQL-CS-SZ                 VALUE    256.
      * Parameter string
       78  SQL-PARAMETER-SZ          VALUE    512.
      * Network ID
       78  SQL-NETID-SZ              VALUE    8.
      * Maximum Path size
       78  SQL-PATH-SZ               VALUE    1024.
      * Host Name
       78  SQL-HOSTNAME-SZ           VALUE    255.
      * Computer Name
       78  SQL-COMPUTERNAME-SZ       VALUE    15.
      * Profile Name
       78  SQL-PROFILE-SZ            VALUE    235.
      * Option Name
       78  SQL-OPTION-SZ             VALUE    10.
      * DCE principal name size
       78  SQL-DCEPRIN-SZ            VALUE    1024.
      * Service Name
       78  SQL-SERVICE-NAME-SZ       VALUE    14.
      * TP Monitor Name
       78  SQL-TPMON-NAME-SZ         VALUE    19.
      * Symbolic Destination Name
       78  SQL-SYM-DEST-NAME-SZ      VALUE    8.
      * TM Database Name
       78  SQL-TMDATABASE-NAME-SZ    VALUE    8.
      * AR Library Name
       78  SQL-AR-SZ                 VALUE    8.
      * System Name
       78  SQL-SYSTEM-NAME-SZ        VALUE    21.
      * Remote Instance Name
       78  SQL-REMOTE-INSTNAME-SZ    VALUE    8.

      * Default DRDA accounting string.
       78  SQL-DFTDRDAACT-SZ         VALUE    25.
      * Dir/Route Path/Obj Name
       78  SQL-DIR-NAME-SZ           VALUE    255.
      * Client Comm. Protocol
       78  SQL-CLIENT-COMM-SZ        VALUE    31.
      * Sysadm Group name
       78  SQL-SYSADM-SZ             VALUE    8.
      * Group name limits have been increased to support 30 characters in
      * v8.2.  This will be further increased to 128 characters in a future
      * release.
      * Future Group name length
       78  SQL-SYSADM-GROUP-SZ       VALUE    128.
      * Netware fileserver name
       78  SQL-FILESERVER-SZ         VALUE    48.
      * Netware bindery name
       78  SQL-OBJECTNAME-SZ         VALUE    48.
      * Netware IPX socket
       78  SQL-IPXSOCKET-SZ          VALUE    4.
      * Netware userid
       78  SQL-NW-UID-SZ             VALUE    48.
      * Netware password
       78  SQL-NW-PSWD-SZ            VALUE    128.
      * Collate info size (256 + 4)
       78  SQL-COLLATE-INFO-SZ       VALUE    260.
      * Max accounting string
       78  SQL-ACCOUNT-STR-SZ        VALUE    199.
      * Discover protocols string size
       78  SQL-DSCVRCOMM-SZ          VALUE    35.

      * After an SQL CONNECT, the 5th token in the SQLCA identifies the DBMS
      * an application has connected to.  The following are values returned
      * from IBM DBMS.
       78  SQL-DBMS-ES10-OS2         value    "QOS/2 DBM".
       78  SQL-DBMS-DB2-OS2          value    "QDB2/2".
       78  SQL-DBMS-DB2-NT           value    "QDB2/NT".
       78  SQL-DBMS-DB2-NT64         value    "QDB2/NT64".
       78  SQL-DBMS-DB2-95           value    "QDB2/Windows 95".
       78  SQL-DBMS-DB2-AIX          value    "QDB2/6000".
       78  SQL-DBMS-DB2-AIX64        value    "QDB2/AIX64".
       78  SQL-DBMS-DB2-AIX-PE       value    "QDB2/6000 PE".
       78  SQL-DBMS-DB2-HPUX         value    "QDB2/HPUX".
       78  SQL-DBMS-DB2-HPUX64       value    "QDB2/HP64".
       78  SQL-DBMS-DB2-HPUXIA       value    "QDB2/HPUX-IA".
       78  SQL-DBMS-DB2-HPUXIA64     value    "QDB2/HPUX-IA64".
       78  SQL-DBMS-DB2-SUN          value    "QDB2/SUN".
       78  SQL-DBMS-DB2-SUN64        value    "QDB2/SUN64".
       78  SQL-DBMS-DB2-SNI          value    "QDB2/SNI".
       78  SQL-DBMS-DB2-SCO          value    "QDB2/SCO".
       78  SQL-DBMS-DB2-SGI          value    "QDB2/SGI".
       78  SQL-DBMS-DB2-LINUXX8664   value    "QDB2/LINUXX8664".
       78  SQL-DBMS-DB2-LINUXPPC64   value    "QDB2/LINUXPPC64".
       78  SQL-DBMS-DB2-LINUXPPC     value    "QDB2/LINUXPPC".
       78  SQL-DBMS-DB2-LINUXIA64    value    "QDB2/LINUXIA64".
       78  SQL-DBMS-DB2-LINUXZ64     value    "QDB2/LINUXZ64".
       78  SQL-DBMS-DB2-LINUX390     value    "QDB2/LINUX390".
       78  SQL-DBMS-DB2-LINUX        value    "QDB2/LINUX".
       78  SQL-DBMS-DB2-DYNIX        value    "QDB2/PTX".
       78  SQL-DBMS-DB2-MVS          value    "QDB2".
       78  SQL-DBMS-OS400            value    "QAS".
       78  SQL-DBMS-SQLDS-VM         value    "QSQLDS/VM".
       78  SQL-DBMS-SQLDS-VSE        value    "QSQLDS/VSE".
       78  SQL-DBMS-LU62-SPM         value    "QLU62SPM".

      * Parameters for Entry Type in sqledinfo
      * Database is LDAP
       78  SQL-LDAP                  value    "4".
      * Database is DCE
       78  SQL-DCE                   value    "3".
      * Database is Home
       78  SQL-HOME                  value    "2".
      * Database is Remote
       78  SQL-REMOTE                value    "1".
      * Database is Indirect
       78  SQL-INDIRECT              value    "0".

      * Parameters for adapter number in sqlectnd
      * Adapter number 0
       78  SQL-ADAPTER-0             VALUE    0.
      * Adapter number 1
       78  SQL-ADAPTER-1             VALUE    1.
      * Minimum adapter number
       78  SQL-ADAPTER-MIN           VALUE    0.
      * Maximum adapter number
       78  SQL-ADAPTER-MAX           VALUE    255.

      * Definitions of constants used for Structure IDs
      * DCS directory entry id
       78  SQL-DCS-STR-ID            VALUE    256.
      * Catalog node struct id
       78  SQL-NODE-STR-ID           VALUE    512.

      * Parameters for protocol types in sqlectnd
      * APPC
       78  SQL-PROTOCOL-APPC         VALUE    x"00".
      * NETBIOS
       78  SQL-PROTOCOL-NETB         VALUE    x"01".
      * APPN
       78  SQL-PROTOCOL-APPN         VALUE    x"02".
      * TCPIP
       78  SQL-PROTOCOL-TCPIP        VALUE    x"03".
      * APPC using CPIC
       78  SQL-PROTOCOL-CPIC         VALUE    x"04".
      * IPX/SPX
       78  SQL-PROTOCOL-IPXSPX       VALUE    x"05".
      * Local IPC
       78  SQL-PROTOCOL-LOCAL        VALUE    x"06".
      * Named Pipe
       78  SQL-PROTOCOL-NPIPE        VALUE    x"07".
      * TCPIP using SOCKS
       78  SQL-PROTOCOL-SOCKS        VALUE    x"08".

      * Security Type for APPC using CPIC
      * None
       78  SQL-CPIC-SECURITY-NONE    VALUE    0.
      * Same
       78  SQL-CPIC-SECURITY-SAME    VALUE    1.
      * Program
       78  SQL-CPIC-SECURITY-PROGRAM VALUE    2.

      * Security Type for TCPIP
      * None
       78  SQL-TCPIP-SECURITY-NONE   VALUE    0.
      * TCPIP SOCKS Support
       78  SQL-TCPIP-SECURITY-SOCKS  VALUE    1.

      * Authentication Types
      * Authenticate on Server
       78  SQL-AUTHENTICATION-SERVER VALUE    0.
      * Authenticate on Client
       78  SQL-AUTHENTICATION-CLIENT VALUE    1.
      * Authenticate via DDCS
       78  SQL-AUTHENTICATION-DCS    VALUE    2.
      * Authenticate via DCE
       78  SQL-AUTHENTICATION-DCE    VALUE    3.
      * Auth at Server with encrypt
       78  SQL-AUTHENTICATION-SVR-ENCRYPT VALUE 4.
      * Auth via DDCS with encrypt
       78  SQL-AUTHENTICATION-DCS-ENCRYPT VALUE 5.
      * Auth via DCE or Server with Encrypt (Valid only at server)
       78  SQL-AUTHENTICATION-DCE-SVR-ENC VALUE 6.
      * Auth via Kerberos
       78  SQL-AUTHENTICATION-KERBEROS VALUE  7.
      * Auth via Kerberos or Server with Envrypt (Valid only at the server
       78  SQL-AUTHENTICATION-KRB-SVR-ENC VALUE 8.
      * Auth via GSS API plugin
       78  SQL-AUTHENTICATION-GSSPLUGIN VALUE 9.
      * Auth via GSS API plugin or Server with Encrypt (Valid only at
      * server)
       78  SQL-AUTHENTICATION-GSS-SVR-ENC VALUE 10.
      * Auth at server with encrypted data
       78  SQL-AUTHENTICATION-DATAENC VALUE   11.
      * Auth at server with or without encrypted data (Valid only at server)
       78  SQL-AUTHENTICATION-DATAENC-CMP VALUE 12.
      * Authentication Not Specified
       78  SQL-AUTHENTICATION-NOT-SPEC VALUE  255.

      * Data Flow Encryption Types
      * No encryption for connection
       78  SQL-NO-ENCRYPT            VALUE    1.
      * DH_DES_56
       78  SQL-DH-DES-56             VALUE    2.
      * DH_3DES_128
       78  SQL-DH-3DES-128           VALUE    3.
      * DH_3DES_192
       78  SQL-DH-3DES-192           VALUE    4.

      * Parameters for Create Database API Collating Sequences
      * Coll. Seq. from System
       78  SQL-CS-SYSTEM             VALUE    0.
      * Coll. Seq. from User
       78  SQL-CS-USER               VALUE    -1.
      * Coll. Seq. (None)
       78  SQL-CS-NONE               VALUE    -2.
      * Coll. Seq. from pre-v5
       78  SQL-CS-COMPATIBILITY      VALUE    -4.
      * SYSTEM table + NLS function
       78  SQL-CS-SYSTEM-NLSCHAR     VALUE    -8.
      * USER table + NLS function
       78  SQL-CS-USER-NLSCHAR       VALUE    -9.
      * UTF-8S collation
       78  SQL-CS-IDENTITY-16BIT     VALUE    -10.
      * Unicode Collation Algorithm V400 Normalization ON collation
       78  SQL-CS-UCA400-NO          VALUE    -11.
      * Unicode Collation Algorithm V400 for Thai collation
       78  SQL-CS-UCA400-LTH         VALUE    -12.
      * Unicode Collation Algorithm V400 for Slovakian collation
       78  SQL-CS-UCA400-LSK         VALUE    -13.

      * Defines for Start Database Manager OPTION parameter and Stop
      * Database Manager OPTION and CALLERAC parameters
       78  SQLE-NONE                 VALUE    0.
       78  SQLE-FORCE                VALUE    1.
       78  SQLE-DROP                 VALUE    2.
       78  SQLE-CONTINUE             VALUE    3.
       78  SQLE-TERMINATE            VALUE    4.
       78  SQLE-ADDNODE              VALUE    5.
       78  SQLE-RESTART              VALUE    6.
       78  SQLE-STANDALONE           VALUE    7.

      * Defines for Start Database Manager & Add Node Tablespace type
      * No Temp Tablespaces
       78  SQLE-TABLESPACES-NONE     VALUE    0.
      * Temp Tablespaces like Specified Node
       78  SQLE-TABLESPACES-LIKE-NODE VALUE   1.
      * Temp Tablespaces like Catalog Node
       78  SQLE-TABLESPACES-LIKE-CATALOG VALUE 2.

      * Parameters for Drop Node Action
      * Verify
       78  SQL-DROPNODE-VERIFY       VALUE    1.

      * Parameters for indicating the stored procedure invocation was via
      * the CALL statement
       78  SQL-CALLPROC              value    "$SQL$CAL".

      * Default values for Segmented Tables parameters
      * Default number of segments-nonOS2
       78  SQL-DEF-NUMSEGS           VALUE    1.
      * OBSOLETE: Default max pages per seg
       78  SQL-DEF-SEGPAGES          VALUE    76800.
      * default extent size
       78  SQL-DEF-EXTSIZE           VALUE    32.

      * DUOW Connection Setting types used by sqleqryc() and
      * sqlesetc().Associated values are in sql.h, used in common with the
      * precompiler.
      * Connect type
       78  SQL-CONNECT-TYPE          VALUE    1.
      * Rules for changing connections
       78  SQL-RULES                 VALUE    2.
      * Disconnect type at syncpoint
       78  SQL-DISCONNECT            VALUE    3.
      * Syncpoint type
       78  SQL-SYNCPOINT             VALUE    4.
      * Maximum concurrent connections
       78  SQL-MAX-NETBIOS-CONNECTIONS VALUE  5.
      * Deferred Prepare
       78  SQL-DEFERRED-PREPARE      VALUE    6.
      * Node to connect to
       78  SQL-CONNECT-NODE          VALUE    7.
      * Node to attach to 
       78  SQL-ATTACH-NODE           VALUE    8.

      * SET CLIENT INFORMATION types used by sqleseti() and sqleqryi().
      * Client user name
       78  SQLE-CLIENT-INFO-USERID   VALUE    1.
      * Client workstation name
       78  SQLE-CLIENT-INFO-WRKSTNNAME VALUE  2.
      * Client application name
       78  SQLE-CLIENT-INFO-APPLNAME VALUE    3.
      * Client accounting string
       78  SQLE-CLIENT-INFO-ACCTSTR  VALUE    4.
      * Client programid identifier
       78  SQLE-CLIENT-INFO-PROGRAMID VALUE   5.
      * Client autocommit
       78  SQLE-CLIENT-INFO-AUTOCOMMIT VALUE  6.

      * Constants to be used to set AUTOCOMMIT.
       78  SQLE-CLIENT-AUTOCOMMIT-ON value    "Y".
       78  SQLE-CLIENT-AUTOCOMMIT-OFF value   "N".

      * SET CLIENT INFORMATION types maximum information lengths
      * Maximum client user information length
       78  SQLE-CLIENT-USERID-MAX-LEN VALUE   255.
      * Maximum client workstation information length
       78  SQLE-CLIENT-WRKSTNNAME-MAX-LEN VALUE 255.
      * Maximum client application information length
       78  SQLE-CLIENT-APPLNAME-MAX-LEN VALUE 255.
      * Maximum client accounting information length
       78  SQLE-CLIENT-ACCTSTR-MAX-LEN VALUE  200.
      * Maximum client program identifier length
       78  SQLE-CLIENT-PROGRAMID-MAX-LEN VALUE 80.
      * Maximum length for autocommit
       78  SQLE-CLIENT-AUTOCOMMIT-MAX-LEN VALUE 1.


      * A structure to identify an SQL statement.  The eyecatcher $SQL$CALL
      * identifies this as an EXEC SQL CALL statement.
       01 SQLSTMTID.
      * eye catcher
           05 SQLCSTID               PIC X(8).
      * creator id
           05 SQLUSER                PIC X(128).
      * package name
           05 SQL-PKGNAME            PIC X(8).
      * consistency token
           05 SQL-CONTOKEN           PIC X(8).
      * reserved
           05 SQL-SECTNUM            PIC S9(4) COMP-5.
      * reserved
           05 SQL-WCHAR-INFO         PIC X.
      * reserved
           05 SQL-F-CACHE-ROWS       PIC X.
      * reserved
           05 SQL-BUFFER             PIC X(38).


      *************************************************************************
      * Database Description Block structure
      **************************************************************************
      * TableSpace types
       78  SQL-TBS-TYP-SMS           value    "S".
       78  SQL-TBS-TYP-DMS           value    "D".
       78  SQL-TBS-TYP-AUTO          value    "A".

      * TableSpace container types
      * path (directory)  SMS only
       78  SQL-TBSC-TYP-PATH         value    "P".
      * device (raw disk) DMS only
       78  SQL-TBSC-TYP-DEV          value    "D".
      * file (cooked file) DMS only
       78  SQL-TBSC-TYP-FILE         value    "F".

      * version 2 database descriptor
       78  SQLE-DBDESC-2             value    "SQLDBD02".
      * version 2 tableSpace descriptor
       78  SQLE-DBTSDESC-1           value    "SQLTS001".

      * Initial TableSpace names
      * system catalogs
       78  SQLCATTS-NAME             value    "SYSCATSPACE".
      * user tables
       78  SQLUSRTS-NAME             value    "USERSPACE1".
      * temp tables
       78  SQLTMPTS-NAME             value    "TEMPSPACE1".

      * tableSpace container descriptor
       01 SQLETSCDESC.
      * container type
           05 SQLCTYPE               PIC X.
      * reserved
           05 SQL-PAD1               PIC X(3).
      * container size (in pages)
           05 SQLCSIZE               PIC S9(9) COMP-5.
      * length of container name
           05 SQLCLEN                PIC S9(4) COMP-5.
      * container name (includes 1 byte for C NULL terminator)
           05 SQLCONTR               PIC X(256).
      * 2 bytes padding before next container
           05 SQL-PAD2               PIC X(2).


       01 SQLEDBDESC.
      * eye catcher
           05 SQLDBDID               PIC X(8).
      * comment code page
           05 SQLDBCCP               PIC S9(9) COMP-5.
      * Source of collating seq
           05 SQLDBCSS               PIC S9(9) COMP-5.
      * user-defined collating seq
           05 SQLDBUDC               PIC X(256).
      * comment
           05 SQLDBCMT               PIC X(30).
           05 FILLER                 PIC X.
      * reserved
           05 SQL-PAD                PIC X(1).
      * obsolete: use sqltsext
           05 SQLDBSGP               PIC 9(9) COMP-5.
      * number of segments
           05 SQLDBNSG               PIC S9(4) COMP-5.
      * reserved
           05 SQL-PAD2               PIC X(2).
      * default extent size
           05 SQLTSEXT               PIC S9(9) COMP-5.
           05 SQLCATTS               USAGE IS POINTER.
           05 SQLUSRTS               USAGE IS POINTER.
           05 SQLTMPTS               USAGE IS POINTER.


      * Old Database Directory Scan data structure
       01 SQLEDINFO.
      * Alias name
           05 SQL-ALIAS              PIC X(8).
      * Database name
           05 SQL-DBNAME             PIC X(8).
      * Drive/Path
           05 SQL-DRIVE              PIC X(12).
      * Database subdirectory
           05 SQL-INTNAME            PIC X(8).
      * Node name
           05 SQL-NODENAME           PIC X(8).
      * Release information
           05 SQL-DBTYPE             PIC X(20).
      * Comment
           05 SQL-COMMENT            PIC X(30).
      * Code page of comment
           05 SQL-COM-CODEPAGE       PIC S9(4) COMP-5.
      * Entry type - defines above
           05 SQL-TYPE               PIC X.
           05 FILLER                 PIC X(1).
      * Authentication type
           05 SQL-AUTHENTICATION     PIC 9(4) COMP-5.
      * Global database name
           05 SQL-GLBDBNAME          PIC X(255).
      * dce principal bin string
           05 SQL-DCEPRINCIPAL       PIC X(1024).
           05 FILLER                 PIC X(1).
      * Catalog node number
           05 SQL-CAT-NODENUM        PIC S9(4) COMP-5.
      * Node number
           05 SQL-NODENUM            PIC S9(4) COMP-5.

      * Node Directory Scan data structure
       01 SQLENINFO.
      * Node name
           05 SQL-NODE-NAME          PIC X(8).
      * Local_lu_name
           05 SQL-LOCAL-LU           PIC X(8).
      * Partner_lu_name
           05 SQL-PARTNER-LU         PIC X(8).
      * Mode
           05 SQL-MODE               PIC X(8).
      * Comment
           05 SQL-COMMENT            PIC X(30).
      * Comment code page
           05 SQL-COM-CODEPAGE       PIC 9(4) COMP-5.
      * NetBIOS Adapter #
           05 SQL-ADAPTER            PIC 9(4) COMP-5.
      * APPN Network ID
           05 SQL-NETWORKID          PIC X(8).
      * Protocol type
           05 SQL-PROTOCOL           PIC X.
      * CPIC Sym. Dest. Name
           05 SQL-SYM-DEST-NAME      PIC X(8).
           05 FILLER                 PIC X(1).
      * CPIC Security Type
           05 SQL-SECURITY-TYPE      PIC 9(4) COMP-5.
      * TCP/IP Host name
           05 SQL-HOSTNAME           PIC X(255).
      * TCP/IP Service name
           05 SQL-SERVICE-NAME       PIC X(14).
      * IPX/SPX fileserver name
           05 SQL-FILESERVER         PIC X(48).
      * IPX/SPX objectname
           05 SQL-OBJECTNAME         PIC X(48).
      * LOCAL Instance name
           05 SQL-INSTANCE-NAME      PIC X(8).
      * Computer name
           05 SQL-COMPUTERNAME       PIC X(15).
      * System name
           05 SQL-SYSTEM-NAME        PIC X(21).
      * Remote instance name
           05 SQL-REMOTE-INSTNAME    PIC X(8).
      * Catalog node type
           05 SQL-CATALOG-NODE-TYPE  PIC X.
      * OS type
           05 SQL-OS-TYPE            PIC 9(4) COMP-5.
      * Change_password_lu_name
           05 SQL-CHGPWD-LU          PIC X(8).
      * TRANSaction_Program_Name
           05 SQL-TRANSPN            PIC X(64).
      * LAN_Adapter_Address
           05 SQL-LANADDR            PIC X(12).

      * General Catalog Node structures and defines

       01 SQL-NODE-STRUCT.
      * Structure Identifier
           05 STRUCT-ID              PIC 9(4) COMP-5.
      * Codepage for comment
           05 CODEPAGE               PIC 9(4) COMP-5.
      * Comment
           05 COMMENT                PIC X(30).
           05 FILLER                 PIC X.
      * Node name
           05 NODENAME               PIC X(8).
           05 FILLER                 PIC X.
      * Protocol Type
           05 PROTOCOL               PIC X.
           05 FILLER                 PIC X(1).

      * For APPC protocol
       01 SQL-NODE-APPC.
      * Local_lu name
           05 LOCAL-LU               PIC X(8).
           05 FILLER                 PIC X.
      * Alias Partner_lu name
           05 PARTNER-LU             PIC X(8).
           05 FILLER                 PIC X.
      * Mode
           05 TRANS-MODE             PIC X(8).
           05 FILLER                 PIC X.

      * For NetBIOS Protocol
       01 SQL-NODE-NETB.
      * Adapter Number
           05 ADAPTER                PIC 9(4) COMP-5.
      * Remote Workstation name
           05 REMOTE-NNAME           PIC X(8).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).

      * For APPN protocol
       01 SQL-NODE-APPN.
      * Network ID
           05 NETWORKID              PIC X(8).
           05 FILLER                 PIC X.
      * Remoter lu name
           05 REMOTE-LU              PIC X(8).
           05 FILLER                 PIC X.
      * Local_lu name
           05 LOCAL-LU               PIC X(8).
           05 FILLER                 PIC X.
      * Mode
           05 TRANS-MODE             PIC X(8).
           05 FILLER                 PIC X.
      * Change password lu name
           05 CHGPWD-LU              PIC X(8).
           05 FILLER                 PIC X.
      * Transaction Program Name
           05 TRANSPN                PIC X(64).
           05 FILLER                 PIC X.
      * LAN Adapter Address
           05 LANADDR                PIC X(12).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).
      * Security Type
           05 SECURITY-TYPE          PIC 9(4) COMP-5.

      * For TCPIP protocol
       01 SQL-NODE-TCPIP.
      * Host name
           05 HOSTNAME               PIC X(255).
           05 FILLER                 PIC X.
      * Service Name
           05 SERVICE-NAME           PIC X(14).
           05 FILLER                 PIC X.

      * For NPIPE protocol
       01 SQL-NODE-NPIPE.
      * Computer name
           05 COMPUTERNAME           PIC X(15).
           05 FILLER                 PIC X.
      * Instance Name
           05 INSTANCE-NAME          PIC X(8).
           05 FILLER                 PIC X.

      * For LOCAL protocol
       01 SQL-NODE-LOCAL.
      * Instance Name
           05 SQL-INSTANCE-NAME      PIC X(8).
           05 FILLER                 PIC X.

      * For APPC using CPIC protocol
       01 SQL-NODE-CPIC.
      * Sym Dest Name
           05 SYM-DEST-NAME          PIC X(8).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).
      * Security Type
           05 SECURITY-TYPE          PIC 9(4) COMP-5.

      * For IPX/SPX protocol
       01 SQL-NODE-IPXSPX.
      * Fileserver name
           05 SQL-FILESERVER         PIC X(48).
           05 FILLER                 PIC X.
      * Object name
           05 SQL-OBJECTNAME         PIC X(48).
           05 FILLER                 PIC X.

      * Values for sqlefrce
      * Force Mode (Asynchronous)
       78  SQL-ASYNCH                VALUE    0.
      * Force Count (All Users)
       78  SQL-ALL-USERS             VALUE    -1.

      * Product name
       78  SQL-PRODNAME-SZ           VALUE    30.
      * DDCS Directory Entry Data Structure
       01 SQL-DIR-ENTRY.
      * Structure Identifier
           05 STRUCT-ID              PIC 9(4) COMP-5.
      * Release level of entry
           05 RELEASE-LVL            PIC 9(4) COMP-5.
      * Codepage of comment
           05 CODEPAGE               PIC 9(4) COMP-5.
      * Directory entry comment
           05 COMMENT                PIC X(30).
           05 FILLER                 PIC X.
      * Local DB name
           05 LDB                    PIC X(8).
           05 FILLER                 PIC X.
      * Target (host) DB name
           05 TDB                    PIC X(18).
           05 FILLER                 PIC X.
      * AR library name
           05 AR                     PIC X(8).
           05 FILLER                 PIC X.
      * Parameter string
           05 PARM                   PIC X(512).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).

      *************************************************************************
      * Database Codepage and Territory code info structures
      **************************************************************************

       01 SQLEDBCINFO.
      * database codeset
           05 SQLDBCP                PIC X(9).
           05 FILLER                 PIC X.
      * database territory
           05 SQLDBCC                PIC X(5).
           05 FILLER                 PIC X.

      *************************************************************************
      * V2 Database Codepage and Territory code info structure
      **************************************************************************

       01 SQLEDBCOUNTRYINFO.
      * database codeset
           05 SQLDBCODESET           PIC X(9).
           05 FILLER                 PIC X.
      * database territory
           05 SQLDBLOCALE            PIC X(5).
           05 FILLER                 PIC X.



      * Connection setting structure for use with distributed 
      * unit of work

       01 SQLE-CONN-SETTING.
           05 SQLE-CONN-SETTING-ITEM OCCURS 7 TIMES.
             10 SQLE-CONN-TYPE       PIC S9(4) COMP-5.
             10 SQLE-CONN-VALUE      PIC S9(4) COMP-5.

      * SET CLIENT Information structure for use with sqleseti 
      * and sqleqryi

       01 SQLE-CLIENT-INFO.
           05 SQLE-CLIENT-INFO-ITEM OCCURS 4 TIMES.
             10 SQLE-CLIENT-INFO-TYPE     PIC S9(4) COMP-5.
             10 SQLE-CLIENT-INFO-LENGTH   PIC S9(4) COMP-5.
             10 SQLE-CLIENT-INFO-P-VALUE  USAGE IS POINTER.

      *************************************************************************
      * Database Codepage and Territory code info structure using
      * SQL_CODESET_SIZE and SQL_LOCALE_SIZE
      **************************************************************************

       01 SQLE-DB-TERRITORY-INFO.
      * database codeset
           05 SQLDBCODESET           PIC X(17).
           05 FILLER                 PIC X.
      * database territory
           05 SQLDBLOCALE            PIC X(33).
           05 FILLER                 PIC X.



      *************************************************************************
      * Register/Deregister constants and data structures
      **************************************************************************

      * Defines for LOCATION TO REGISTER parameter
       78  SQL-NWBINDERY             VALUE    0.

      * Register/deregister info. for NW bindery data structure
       01 SQLE-REG-NWBINDERY.
           05 SQL-UID                PIC X(48).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).
           05 SQL-UID-LEN            PIC 9(4) COMP-5.
           05 SQL-PSWD               PIC X(128).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).
           05 SQL-PSWD-LEN           PIC 9(4) COMP-5.

      *************************************************************************
      * Start/Stop Database Manager constants and data structures
      **************************************************************************

      * V5 Start Options structure Eye Catcher value
       78  SQLE-STARTOPTID-V51       value    "SQLOPT01".


      * V5 Start Database Manager structure
       01 SQLE-START-OPTIONS.
      * Structure Eye Catcher
           05 SQLOPTID               PIC X(8).
      * Profile specified?
           05 SQL-ISPROFILE          PIC 9(9) COMP-5.
      * Profile
           05 SQL-PROFILE            PIC X(235).
           05 FILLER                 PIC X.
      * Node number specified?
           05 SQL-ISNODENUM          PIC 9(9) COMP-5.
      * Node number
           05 SQL-NODENUM            PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * Start option
           05 SQL-OPTION             PIC 9(9) COMP-5.
      * Hostname specified?
           05 SQL-ISHOSTNAME         PIC 9(9) COMP-5.
      * Hostname
           05 SQL-HOSTNAME           PIC X(255).
           05 FILLER                 PIC X.
      * Port specified?
           05 SQL-ISPORT             PIC 9(9) COMP-5.
      * Port
           05 SQL-PORT               PIC S9(9) COMP-5.
      * Netname specified?
           05 SQL-ISNETNAME          PIC 9(9) COMP-5.
      * Netname
           05 SQL-NETNAME            PIC X(255).
           05 FILLER                 PIC X.
      * Addnode tablespace type
           05 SQL-TBLSPACE-TYPE      PIC 9(9) COMP-5.
      * Addnode tablespace node
           05 SQL-TBLSPACE-NODE      PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * Computername specified?
           05 SQL-ISCOMPUTER         PIC 9(9) COMP-5.
      * Computername
           05 SQL-COMPUTER           PIC X(15).
           05 FILLER                 PIC X.
      * Logon Account user name
           05 SQL-P-USER-NAME        USAGE IS POINTER.
      * Logon Account password
           05 SQL-P-PASSWORD         USAGE IS POINTER.

      * V5 Stop Database Manager structure
       01 SQLEDBSTOPOPT.
      * Profile specified?
           05 SQL-ISPROFILE          PIC 9(9) COMP-5.
      * Profile
           05 SQL-PROFILE            PIC X(235).
           05 FILLER                 PIC X.
      * Node number specified?
           05 SQL-ISNODENUM          PIC 9(9) COMP-5.
      * Node number
           05 SQL-NODENUM            PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * Option
           05 SQL-OPTION             PIC 9(9) COMP-5.
      * Caller action
           05 SQL-CALLERAC           PIC 9(9) COMP-5.

      *************************************************************************
      * Add Node data structures
      **************************************************************************

      * V5 Add Node Option structure Eye Catcher value
       78  SQLE-ADDOPTID-V51         value    "SQLADD01".


      * Add Node structure
       01 SQLE-ADDN-OPTIONS.
      * Structure Eye Catcher
           05 SQLADDID               PIC X(8).
      * Temporary Tablespace Type
           05 SQL-TBLSPACE-TYPE      PIC 9(9) COMP-5.
      * Temporary Tablespace Node
           05 SQL-TBLSPACE-NODE      PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).

      * GENERIC INTERFACES

      * SQL Return Codes in SQLCODE for Environment Commands

      * Request is still executing
       78  SQLE-RC-STILL-EXECUTING   VALUE    16.
      * Connect to other DB not allowed
       78  SQLE-RC-INLUW             VALUE    -752.
      * Only SBCS data allowed
       78  SQLE-RC-W863              VALUE    863.

      * Agent heap too small
       78  SQLE-RC-E953              VALUE    -953.
      * Requestor comm heap too small
       78  SQLE-RC-E957              VALUE    -957.
      * Server comm heap too small
       78  SQLE-RC-E959              VALUE    -959.
      * Rqstr remote svcs heap too small
       78  SQLE-RC-E961              VALUE    -961.
      * Svr remote svcs heap too small
       78  SQLE-RC-E962              VALUE    -962.
      * Application control shared memory set cannot be allocated
       78  SQLE-RC-NO-APP-CTRL-SEG   VALUE    -987.

      * New log path is invalid
       78  SQLE-RC-INVNEWLOGP        VALUE    993.
      * Current log path is invalid
       78  SQLE-RC-INVLOGP           VALUE    995.

      * Invalid alias
       78  SQLE-RC-INVALIAS          VALUE    -1000.
      * Invalid database name
       78  SQLE-RC-INVDBNAME         VALUE    -1001.
      * Invalid drive
       78  SQLE-RC-INVDRIVE          VALUE    -1002.

      * Invalid password - OS/2
       78  SQLE-RC-INVPSW-DB2OS2     VALUE    -1003.
      * Invalid password - NT
       78  SQLE-RC-INVPSW-DB2NT      VALUE    -1003.
      * Invalid password - DOS
       78  SQLE-RC-INVPSW-DB2DOS     VALUE    -1003.
      * Invalid password - Windows
       78  SQLE-RC-INVPSW-DB2WIN     VALUE    -1003.
      * Invalid password - Mac
       78  SQLE-RC-INVPSW-DB2MAC     VALUE    -1003.
      * Invalid password - AIX
       78  SQLE-RC-INVPSW-DB2AIX     VALUE    -10002.

       78  SQLE-RC-INVPSW            value    -1003.

      * Insuf storage on file system
       78  SQLE-RC-INSSTOR           VALUE    -1004.
      * Duplicate alias
       78  SQLE-RC-DUPALIAS          VALUE    -1005.
      * Appl code page does not match db
       78  SQLE-RC-WRONGCODEPG       VALUE    -1006.
      * Invalid new password
       78  SQLE-RC-INV-NEWPSW        VALUE    -1008.
      * Invalid remote command
       78  SQLE-RC-INVREMOTE         VALUE    -1009.

      * Invalid type
       78  SQLE-RC-INVTYPE           VALUE    -1010.
      * No drive for indirect entry
       78  SQLE-RC-NODRIVE           VALUE    -1011.
      * No nodename for remote entry
       78  SQLE-RC-NONODE            VALUE    -1012.
      * Alias or database name not found
       78  SQLE-RC-NODB              VALUE    -1013.
      * No more entries
       78  SQLE-RC-NOMORE            VALUE    1014.
      * Database needs restart
       78  SQLE-RC-DB-RESTART        VALUE    -1015.
      * Invalid local_lu alias
       78  SQLE-RC-INVLLU            VALUE    -1016.
      * Invalid mode
       78  SQLE-RC-INVMODE           VALUE    -1017.
      * Duplicate node name
       78  SQLE-RC-DUPNODE           VALUE    -1018.
      * Invalid node name
       78  SQLE-RC-INVNODE           VALUE    -1019.

      * Node could not be cataloged
       78  SQLE-RC-MAXNODE           VALUE    -1020.
      * Nodename not found
       78  SQLE-RC-NOTNODE           VALUE    -1021.

      * Insuf system resources - OS/2
       78  SQLE-RC-INSSYS-DB2OS2     VALUE    -1022.
      * Insuf system resources - NT
       78  SQLE-RC-INSSYS-DB2NT      VALUE    -1022.
      * Insuf system resources - DOS
       78  SQLE-RC-INSSYS-DB2DOS     VALUE    -1022.
      * Insuf system resources - Windows
       78  SQLE-RC-INSSYS-DB2WIN     VALUE    -1022.
      * Insuf system resources - Macintosh
       78  SQLE-RC-INSSYS-DB2MAC     VALUE    -1022.
      * Insuf system resources - AIX
       78  SQLE-RC-INSSYS-DB2AIX     VALUE    -10003.

       78  SQLE-RC-INSSYS            value    -1022.

      * Communication conversation failed
       78  SQLE-RC-NOCONV            VALUE    -1023.
      * No database connection exists
       78  SQLE-RC-NOSUDB            VALUE    -1024.
      * Databases are active
       78  SQLE-RC-DBACT             VALUE    -1025.
      * Database manager already started
       78  SQLE-RC-INVSTRT           VALUE    -1026.
      * Node directory not found
       78  SQLE-RC-NONODEDIR         VALUE    -1027.
      * Partner lu not specified
       78  SQLE-RC-INVRLU            VALUE    -1029.

      * Database directory is full
       78  SQLE-RC-MAXDB             VALUE    -1030.
      * Database directory not found
       78  SQLE-RC-NODBDIR           VALUE    -1031.
      * Database manager not started
       78  SQLE-RC-NOSTARTG          VALUE    -1032.
      * Database directory being updated
       78  SQLE-RC-DIRBUSY           VALUE    -1033.
      * Database is damaged
       78  SQLE-RC-DBBAD             VALUE    -1034.
      * Database already in use
       78  SQLE-RC-DB-INUSE          VALUE    -1035.
      * Database file error
       78  SQLE-RC-FILEDB            VALUE    -1036.
      * No entry in Node directory
       78  SQLE-RC-NODE-DIR-EMPTY    VALUE    1037.
      * Node directory file error
       78  SQLE-RC-FILENODE          VALUE    -1038.

      * Directory file error - OS/2
       78  SQLE-RC-FILEDIR-DB2OS2    VALUE    -1039.
      * Directory file error - NT
       78  SQLE-RC-FILEDIR-DB2NT     VALUE    -1039.
      * Directory file error - DOS
       78  SQLE-RC-FILEDIR-DB2DOS    VALUE    -1039.
      * Directory file error - Windows
       78  SQLE-RC-FILEDIR-DB2WIN    VALUE    -1039.
      * Directory file error - Mac
       78  SQLE-RC-FILEDIR-DB2MAC    VALUE    -1039.
      * Directory file error - AIX
       78  SQLE-RC-FILEDIR-DB2AIX    VALUE    -10004.

       78  SQLE-RC-FILEDIR           value    -1039.

      * Max number of applications reached
       78  SQLE-RC-MAXAPPLS          VALUE    -1040.
      * Max number of databases started
       78  SQLE-RC-MAXDBS            VALUE    -1041.
      * System error
       78  SQLE-RC-SYSERR            VALUE    -1042.
      * Catalog bootstrap failure
       78  SQLE-RC-CATBOOT           VALUE    -1043.
      * Request interrupted by ctrl+break
       78  SQLE-RC-INTRRPT           VALUE    -1044.
      * Invalid level of indirection
       78  SQLE-RC-INVINDIR          VALUE    -1045.
      * Invalid userid
       78  SQLE-RC-INVAUTHID         VALUE    -1046.
      * Appl already connect to another db
       78  SQLE-RC-APCONN            VALUE    -1047.

      * Invalid use specified - OS/2
       78  SQLE-RC-USEINVALID-DB2OS2 VALUE    -1048.
      * Invalid use specified - NT
       78  SQLE-RC-USEINVALID-DB2NT  VALUE    -1048.
      * Invalid use specified - DOS
       78  SQLE-RC-USEINVALID-DB2DOS VALUE    -1048.
      * Invalid use specified - Windows
       78  SQLE-RC-USEINVALID-DB2WIN VALUE    -1048.
      * Invalid use specified - MacOS
       78  SQLE-RC-USEINVALID-DB2MAC VALUE    -1048.
      * Invalid use specified - AIX
       78  SQLE-RC-USEINVALID-DB2AIX VALUE    -10005.

       78  SQLE-RC-USEINVALID        value    -1048.

      * Appl state in error
       78  SQLE-RC-APPSERR           VALUE    -900.

      * Cannot uncatalog 'Home' database
       78  SQLE-RC-UNCHOME           VALUE    -1050.
      * Db direct. drive does not exist
       78  SQLE-RC-NODIRDRV          VALUE    -1051.
      * Database drive does not exist
       78  SQLE-RC-NODBDRV           VALUE    -1052.
      * Interrupt already in progress
       78  SQLE-RC-INTBUSY           VALUE    -1053.
      * Commit in progress - no int's
       78  SQLE-RC-COMMINP           VALUE    -1054.
      * Rollback in progress - no int's
       78  SQLE-RC-ROLLINP           VALUE    -1055.
      * Cannot be interrupted - no int's
       78  SQLE-RC-NOINTER           VALUE    -1360.
      * Windows Client Execution Timeout
       78  SQLE-RC-TIMEOUT           VALUE    1361.
      * Maximum allowable scans exceeded
       78  SQLE-RC-MAXSCAN           VALUE    -1056.
      * No entries in directory
       78  SQLE-RC-NODENTRY          VALUE    1057.
      * Invalid input handle
       78  SQLE-RC-INVHAND           VALUE    -1058.
      * Open scan not issued
       78  SQLE-RC-NOSCAN            VALUE    -1059.

      * User lacks connect privilege
       78  SQLE-RC-NOCONNECT         VALUE    -1060.
      * User lacks connect privilege for Quiesced Db
       78  SQLE-RC-QSNOCONNECT       VALUE    -20157.
      * RESTART was successful, but indoubt transactions exist
       78  SQLE-RC-RESTART-WITH-INDOUBTS VALUE 1061.
      * Database path not found
       78  SQLE-RC-BADPATH           VALUE    -1062.
      * Database manager started OK
       78  SQLE-RC-START-OK          VALUE    -1063.
      * Database manager stopped OK
       78  SQLE-RC-STOP-OK           VALUE    -1064.
      * N utilities not bound
       78  SQLE-RC-DB-BADBINDS       VALUE    1065.

      * Message file not found - OS/2
       78  SQLE-RC-NOMSG-DB2OS2      VALUE    -1068.
      * Message file not found - NT
       78  SQLE-RC-NOMSG-DB2NT       VALUE    -1068.
      * Message file not found - DOS
       78  SQLE-RC-NOMSG-DB2DOS      VALUE    -1068.
      * Message file not found - Windows
       78  SQLE-RC-NOMSG-DB2WIN      VALUE    -1068.
      * Message file not found - MacOS
       78  SQLE-RC-NOMSG-DB2MAC      VALUE    -1068.
      * Message file not found - AIX
       78  SQLE-RC-NOMSG-DB2AIX      VALUE    -10007.

       78  SQLE-RC-NOMSG             value    -1068.

      * DB invalid type for Drop
       78  SQLE-RC-INVDROP           VALUE    -1069.

      * Invalid Database Name pointer
       78  SQLE-RC-INVDBNAME-PTR     VALUE    -1070.
      * Invalid Alias pointer
       78  SQLE-RC-INVALIAS-PTR      VALUE    -1071.
      * Resources in inconsistent state
       78  SQLE-RC-RESOURCE-ERROR    VALUE    -1072.
      * Invalid Node Directory release
       78  SQLE-RC-BAD-ND-REL        VALUE    -1073.
      * Invalid Password pointer
       78  SQLE-RC-INVPSW-PTR        VALUE    -1074.
      * Invalid Comment pointer
       78  SQLE-RC-INVCOMM-PTR       VALUE    -1075.
      * Invalid Count pointer
       78  SQLE-RC-INVCNT-PTR        VALUE    -1076.
      * Invalid Handle pointer
       78  SQLE-RC-INVHAND-PTR       VALUE    -1077.
      * Invalid Buffer pointer
       78  SQLE-RC-INVBUFF-PTR       VALUE    -1078.
      * Invalid Node pointer
       78  SQLE-RC-INVNODE-PTR       VALUE    -1079.
      * Invalid Userid pointer
       78  SQLE-RC-INVUSERID-PTR     VALUE    -1150.
      * Invalid Parms pointer
       78  SQLE-RC-INVPARM-PTR       VALUE    -1151.

      * Invalid Local Lu pointer
       78  SQLE-RC-INVLLU-PTR        VALUE    -1080.
      * Invalid Remote Lu pointer
       78  SQLE-RC-INVRLU-PTR        VALUE    -1081.
      * Invalid Mode pointer
       78  SQLE-RC-INVMODE-PTR       VALUE    -1082.
      * Bad Database Description Block
       78  SQLE-RC-BAD-DBDB          VALUE    -1083.
      * Cannot Allocate Kernel Segments
       78  SQLE-RC-KSEGSFAIL         VALUE    -1084.
      * Cannot Allocate Application heap
       78  SQLE-RC-APPHEAPFAIL       VALUE    -1085.
      * Unexpected OS/2 error
       78  SQLE-RC-OS2ERROR          VALUE    -1086.
      * Bind list could not be opened
       78  SQLE-RC-BIND-LIST         VALUE    1087.
      * Error occurred during bind
       78  SQLE-RC-BIND-ERROR        VALUE    1088.
      * Binding was interrupted
       78  SQLE-RC-BIND-INTRRPT      VALUE    1089.

      * Release number of appl is invalid
       78  SQLE-RC-BAD-APP-REL       VALUE    -1090.
      * Release number of database bad
       78  SQLE-RC-BAD-DB-REL        VALUE    -1091.

      * Authorization error
       78  SQLE-RC-INSAUTH           VALUE    -1092.
      * User not logged on
       78  SQLE-RC-NOLOGON           VALUE    -1093.
      * Node directory being updated
       78  SQLE-RC-NDBUSY            VALUE    -1094.
      * Max node scans open
       78  SQLE-RC-MAX-NDSCAN        VALUE    -1095.
      * Invalid type for requester node
       78  SQLE-RC-REQTYPE           VALUE    -1096.
      * Node not found for remote db
       78  SQLE-RC-NODERR            VALUE    -1097.
      * Appl is already connected to db
       78  SQLE-RC-APCONN-SAME       VALUE    -1098.

      * Write protect error on diskette
       78  SQLE-RC-WRPROT-ERR-DB2OS2 VALUE    -1099.
      * Write protect error on diskette
       78  SQLE-RC-WRPROT-ERR-DB2NT  VALUE    -1099.
      * Write protect error on diskette
       78  SQLE-RC-WRPROT-ERR-DB2DOS VALUE    -1099.
      * Write protect error on diskette
       78  SQLE-RC-WRPROT-ERR-DB2WIN VALUE    -1099.
      * Write protect error on diskette
       78  SQLE-RC-WRPROT-ERR-DB2MAC VALUE    -1099.
      * Write protect error on filesystem
       78  SQLE-RC-WRPROT-ERR-DB2AIX VALUE    -10021.

       78  SQLE-RC-WRPROT-ERR        value    -1099.

      * Node not cataloged for database
       78  SQLE-RC-NODE-WARN         VALUE    1100.
      * Remote communications error
       78  SQLE-RC-REMCONN-ERR       VALUE    -1101.
      * No database name provided in call
       78  SQLE-RC-MIG-NODB          VALUE    -1102.
      * Migration was successful
       78  SQLE-RC-MIG-OK            VALUE    1103.
      * Invalid program name pointer
       78  SQLE-RC-INVPROG-PTR       VALUE    -1104.

      * Invalid disconnect from database
       78  SQLE-RC-INV-SPDB-DB2OS2   VALUE    -1105.
      * Invalid disconnect from database
       78  SQLE-RC-INV-SPDB-DB2NT    VALUE    -1105.
      * Invalid disconnect from database
       78  SQLE-RC-INV-SPDB-DB2DOS   VALUE    -1105.
      * Invalid disconnect from database
       78  SQLE-RC-INV-SPDB-DB2WIN   VALUE    -1105.
      * Invalid disconnect from database
       78  SQLE-RC-INV-SPDB-DB2MAC   VALUE    -1105.
      * Invalid disconnect from database
       78  SQLE-RC-INV-SPDB-DB2AIX   VALUE    -10017.

       78  SQLE-RC-INV-SPDB          value    -1105.

      * Function could not be executed
       78  SQLE-RC-INVALID-PROC-DB2OS2 VALUE  -1106.
      * Function could not be executed
       78  SQLE-RC-INVALID-PROC-DB2NT VALUE   -1106.
      * Function could not be executed
       78  SQLE-RC-INVALID-PROC-DB2DOS VALUE  -1106.
      * Function could not be executed
       78  SQLE-RC-INVALID-PROC-DB2WIN VALUE  -1106.
      * Function could not be executed
       78  SQLE-RC-INVALID-PROC-DB2MAC VALUE  -1106.
      * Function could not be executed
       78  SQLE-RC-INVALID-PROC-DB2AIX VALUE  -10010.

       78  SQLE-RC-INVALID-PROC      value    -1106.

      * Program interrupted - OS/2
       78  SQLE-RC-INTRP-PROC-DB2OS2 VALUE    -1107.
      * Program interrupted - NT
       78  SQLE-RC-INTRP-PROC-DB2NT  VALUE    -1107.
      * Program interrupted - DOS
       78  SQLE-RC-INTRP-PROC-DB2DOS VALUE    -1107.
      * Program interrupted - Windows
       78  SQLE-RC-INTRP-PROC-DB2WIN VALUE    -1107.
      * Program interrupted - MacOS
       78  SQLE-RC-INTRP-PROC-DB2MAC VALUE    -1107.
      * Program interrupted - AIX
       78  SQLE-RC-INTRP-PROC-DB2AIX VALUE    -10011.

       78  SQLE-RC-INTRP-PROC        value    -1106.

      * System error on library load
       78  SQLE-RC-SYSERR-PROC-DB2OS2 VALUE   -1108.
      * System error on library load
       78  SQLE-RC-SYSERR-PROC-DB2NT VALUE    -1108.
      * System error on library load
       78  SQLE-RC-SYSERR-PROC-DB2DOS VALUE   -1108.
      * System error on library load
       78  SQLE-RC-SYSERR-PROC-DB2WIN VALUE   -1108.
      * System error on library load
       78  SQLE-RC-SYSERR-PROC-DB2MAC VALUE   -1108.
      * System error on library load
       78  SQLE-RC-SYSERR-PROC-DB2AIX VALUE   -10012.

       78  SQLE-RC-SYSERR-PROC       value    -1108.

      * Library could not be loaded
       78  SQLE-RC-NOFILE-PROC-DB2OS2 VALUE   -1109.
      * Library could not be loaded
       78  SQLE-RC-NOFILE-PROC-DB2NT VALUE    -1109.
      * Library could not be loaded
       78  SQLE-RC-NOFILE-PROC-DB2DOS VALUE   -1109.
      * Library could not be loaded
       78  SQLE-RC-NOFILE-PROC-DB2WIN VALUE   -1109.
      * Library could not be loaded
       78  SQLE-RC-NOFILE-PROC-DB2MAC VALUE   -1109.
      * Library could not be loaded
       78  SQLE-RC-NOFILE-PROC-DB2AIX VALUE   -10013.

       78  SQLE-RC-NOFILE-PROC       value    -1109.

      * Program error
       78  SQLE-RC-ERROR-PROC        VALUE    -1110.

      * Invalid DARI prog name format
       78  SQLE-RC-BADPGN-PROC-DB2OS2 VALUE   -1111.
      * Invalid DARI prog name format
       78  SQLE-RC-BADPGN-PROC-DB2NT VALUE    -1111.
      * Invalid DARI prog name format
       78  SQLE-RC-BADPGN-PROC-DB2DOS VALUE   -1111.
      * Invalid DARI prog name format
       78  SQLE-RC-BADPGN-PROC-DB2WIN VALUE   -1111.
      * Invalid DARI prog name format
       78  SQLE-RC-BADPGN-PROC-DB2MAC VALUE   -1111.
      * Invalid DARI prog name format
       78  SQLE-RC-BADPGN-PROC-DB2AIX VALUE   -10014.

       78  SQLE-RC-BADPGN-PROC       value    -1111.

      * Insuf memory to load lib
       78  SQLE-RC-INSMEM-PROC-DB2OS2 VALUE   -1112.
      * Insuf memory to load lib
       78  SQLE-RC-INSMEM-PROC-DB2NT VALUE    -1112.
      * Insuf memory to load lib
       78  SQLE-RC-INSMEM-PROC-DB2DOS VALUE   -1112.
      * Insuf memory to load lib
       78  SQLE-RC-INSMEM-PROC-DB2WIN VALUE   -1112.
      * Insuf memory to load lib
       78  SQLE-RC-INSMEM-PROC-DB2MAC VALUE   -1112.
      * Insuf memory to load lib
       78  SQLE-RC-INSMEM-PROC-DB2AIX VALUE   -10015.

       78  SQLE-RC-INSMEM-PROC       value    -1112.

      * Data type in output SQLDA changed
       78  SQLE-RC-SQLDA-DATATYPE    VALUE    -1113.
      * Data length in output SQLDA changed
       78  SQLE-RC-SQLDA-LENGTH      VALUE    -1114.
      * Num of sqlvars changed in SQLDA
       78  SQLE-RC-SQLDA-VARS        VALUE    -1115.
      * Backup pending
       78  SQLE-RC-BKP-PEND          VALUE    -1116.
      * Roll forward pending
       78  SQLE-RC-ROLLFWD-PEND      VALUE    -1117.
      * Need to rerun the Backup process
       78  SQLE-RC-BKP-INPROG        VALUE    -1118.
      * Need to rerun the Restore process
       78  SQLE-RC-RST-INPROG        VALUE    -1119.
      * Need to rerun either Backup or Restore process
       78  SQLE-RC-BR-INPROG         VALUE    -1120.

      * Invalid Node structure pointer
       78  SQLE-RC-INVNODESTR-PTR    VALUE    -1121.
      * Invalid Protocol structure pointer
       78  SQLE-RC-INVPROTOCOL-PTR   VALUE    -1122.
      * Invalid protocol type
       78  SQLE-RC-INVPROTOCOL       VALUE    -1123.
      * Invalid remote workstation name
       78  SQLE-RC-INVRNNAME         VALUE    -1124.
      * Invalid adapter number
       78  SQLE-RC-INVADAPTER        VALUE    -1125.
      * Invalid network id
       78  SQLE-RC-INVNETID          VALUE    -1126.
      * Invalid real partner LU name
       78  SQLE-RC-INVPLU            VALUE    -1127.

      * Insuf system resources for DARI
       78  SQLE-RC-DARI-INSSYS       VALUE    -1129.
      * Max DARI process limit reached
       78  SQLE-RC-DARI-MAXDARI      VALUE    -1130.
      * DARI process abnormally terminated
       78  SQLE-RC-DARI-ABEND        VALUE    -1131.
      * Invalid DB2 request in DARI
       78  SQLE-RC-DARI-INV-RQST     VALUE    -1132.
      * SQLVAR's sqldata or sqlind ptrs were altered
       78  SQLE-RC-DARI-VAR-POINTER-CHG VALUE -1133.
      * DB2 request is not allowed when DB auth is client
       78  SQLE-RC-DARI-RQST-AUTH-ERR VALUE   -1134.
      * Invalid numsegs on create db
       78  SQLE-RC-BAD-NUMSEGS       VALUE    -1135.
      * Invalid extSize on create db
       78  SQLE-RC-BAD-EXTSIZE       VALUE    -1136.
      * Mounted Segment Directories on a drop database request
       78  SQLE-RC-MOUNTED-SEGS      VALUE    -1137.

      * Deadlocks event monitor not created during create db
       78  SQLE-RC-WARN-DLMON        VALUE    1187.

      * API or command option has an invalid value
       78  SQLE-RC-INVALID-VALUE     VALUE    -1197.

      * Invalid object specified
       78  SQLE-RC-INVOS-OBJ         VALUE    -1200.
      * Invalid status specified
       78  SQLE-RC-INVOS-STAT        VALUE    -1201.
      * Status has not been collected
       78  SQLE-RC-INVOS-NOSTAT      VALUE    -1202.
      * No users connected to database
       78  SQLE-RC-INVOS-NOUSER      VALUE    -1203.
      * Active codepage is not supported
       78  SQLE-RC-UNSUPP-CODEPG     VALUE    -1204.

      * Invalid territory information
       78  SQLE-RC-INV-CNTRYINFO-DB2OS2 VALUE -1205.
      * Invalid territory information
       78  SQLE-RC-INV-CNTRYINFO-DB2NT VALUE  -1205.
      * Invalid territory information
       78  SQLE-RC-INV-CNTRYINFO-DB2DOS VALUE -1205.
      * Invalid territory information
       78  SQLE-RC-INV-CNTRYINFO-DB2WIN VALUE -1205.
      * Invalid territory information
       78  SQLE-RC-INV-CNTRYINFO-DB2MAC VALUE -1205.
      * Invalid territory information
       78  SQLE-RC-INV-CNTRYINFO-DB2AIX VALUE -1205.

       78  SQLE-RC-INV-CNTRYINFO     value    -1205.

      * Invalid Computer Name
       78  SQLE-RC-INV-COMPUTERNAME  VALUE    -1211.
      * Invalid Instance Name
       78  SQLE-RC-INV-INSTANCENAME  VALUE    -1212.
      * Invalid Change Password LU
       78  SQLE-RC-INVCHGPWDLU       VALUE    -1213.
      * Invalid Transaction Pgm Name
       78  SQLE-RC-INVTPNAME         VALUE    -1214.
      * Invalid LAN Adapter Addr
       78  SQLE-RC-INVLANADDRESS     VALUE    -1215.
      * DB2 Shared Memory Set alloc failed
       78  SQLE-RC-NO-SHRD-SEG       VALUE    -1220.
      * ASL heap cannot be allocated
       78  SQLE-RC-NO-ASL-HEAP       VALUE    -1221.
      * ASL heap is too small
       78  SQLE-RC-ASL-TOO-SMALL     VALUE    -1222.
      * No more agents available
       78  SQLE-RC-NO-AGENT-AVAIL    VALUE    -1223.
      * DB2 agent not active
       78  SQLE-RC-AGENT-GONE        VALUE    -1224.
      * Op. Sys. couldn't spawn a process
       78  SQLE-RC-PROC-LIMIT        VALUE    -1225.
      * Max number of coords reached
       78  SQLE-RC-MAXCOORDS         VALUE    -1226.
      * Drop database warning
       78  SQLE-RC-DROPDB-WARN       VALUE    1228.

      * At least one agent not forced
       78  SQLE-RC-AGENT-NOT-FORCED  VALUE    1230.
      * Invalid Force Users count
       78  SQLE-RC-INVCOUNT          VALUE    -1231.
      * Invalid Force Users mode
       78  SQLE-RC-INVFRCE-MODE      VALUE    -1232.

      * Invalid TableSpace descriptor
       78  SQLE-RC-INV-TBS-DESC      VALUE    -1241.

      * Cannot set connection options - existing connections
       78  SQLE-RC-NO-SETCONNOPT     VALUE    -1246.

      * db2uexit file not found
       78  SQLE-RC-NOUEXIT           VALUE    -1267.

       78  SQLC-RC-NPIPE-BROKEN      VALUE    -1281.
       78  SQLC-RC-NPIPE-BUSY        VALUE    -1282.
       78  SQLC-RC-NPIPE-PIPE-INUSE  VALUE    -1283.
       78  SQLC-RC-NPIPE-PIPE-NOT-FOUND VALUE -1284.
       78  SQLC-RC-NPIPE-INVALID-NAME VALUE   -1285.
       78  SQLC-RC-NPIPE-NO-RESOURCE VALUE    -1286.
       78  SQLC-RC-NPIPE-INST-NOT-FOUND VALUE -1287.

      * Directory Services failed
       78  SQLE-RC-DS-FAILED         VALUE    -1291.
      * Bad global name
       78  SQLE-RC-DS-BAD-GLB-NAME   VALUE    -1292.
      * Bad global directory entry
       78  SQLE-RC-DS-BAD-GLB-DIR-ENTRY VALUE -1293.
      * Bad DIR_PATH_NAME
       78  SQLE-RC-DS-BAD-DIR-PATH-NAME VALUE -1294.
      * Bad ROUTE_OBJ_NAME
       78  SQLE-RC-DS-BAD-ROUTE-NAME VALUE    -1295.
      * Command not supported
       78  SQLE-RC-DS-UNSUPPORTED-CMD VALUE   -1297.

      * Invalid DCE principal name
       78  SQLE-RC-DCE-INVPN         VALUE    -1300.
      * Error DCE keytab file
       78  SQLE-RC-DCE-ERR-KEYTAB    VALUE    -1301.
      * DCE principal and DB2 authid mapping error
       78  SQLE-RC-DCE-ERR-MAPPING   VALUE    -1302.
      * Security daemon could not be restarted
       78  SQLE-RC-SECD-ERR-RESTART  VALUE    -1303.
      * Invalid security type for TCPIP protocol
       78  SQLE-RC-INVSTCP           VALUE    -1304.
      * DCE internal error
       78  SQLE-RC-DCE-ERR           VALUE    -1305.
      * DCE invalid server principal name
       78  SQLE-RC-DCE-INV-PRINC     VALUE    -1309.

      * DCS Directory file access error
       78  SQLE-RC-FILEDCS           VALUE    -1310.
      * DCS Directory not found
       78  SQLE-RC-DCSDIR-NF         VALUE    -1311.
      * DCS Directory is empty
       78  SQLE-RC-NO-ENTRY          VALUE    1312.
      * DCS Directory is full
       78  SQLE-RC-MAX-ENTRY         VALUE    -1313.
      * Entry parameter pointer invalid
       78  SQLE-RC-INVENTRY-PTR      VALUE    -1314.
      * Local DB name has invalid chars
       78  SQLE-RC-INVLDB            VALUE    -1315.
      * DCS Directory entry not found
       78  SQLE-RC-LDB-NF            VALUE    -1316.
      * DCS Directory duplicate entry
       78  SQLE-RC-DUPLDB            VALUE    -1317.
      * Invalid element length
       78  SQLE-RC-INVLENGTH         VALUE    -1318.
      * Entries have not been collected
       78  SQLE-RC-ENTRYNOT-COL      VALUE    -1319.

      * Cannot access DCS Dir at this time
       78  SQLE-RC-GDBUSY            VALUE    -1320.
      * Invalid structure ID
       78  SQLE-RC-INVSTRUCT-ID      VALUE    -1321.
      * Error writing to audit log
       78  SQLE-RC-AUD-WRITE-ERR     VALUE    -1322.
      * Error accessing db2audit.cfg
       78  SQLE-RC-AUD-CFG-FILE-ERR  VALUE    -1323.
      * Remote function not supported
       78  SQLE-RC-DRDANSP           VALUE    -1325.
      * File or directory access denied
       78  SQLE-RC-ACCD              VALUE    -1326.
      * Implicit connect - invalid dbname
       78  SQLE-RC-IMPLCONN-INVDB    VALUE    -1327.
      * Implicit connect - alias not found
       78  SQLE-RC-IMPLCONN-NODB     VALUE    -1328.
      * Input path too long
       78  SQLE-RC-PATH-TOO-LONG     VALUE    -1329.

      * Invalid symbolic destination name
       78  SQLE-RC-INVSDNAME         VALUE    -1330.
      * Invalid CPIC security type
       78  SQLE-RC-INVSTYPE          VALUE    -1331.
      * Invalid Host Name
       78  SQLE-RC-INV-HOSTNAME      VALUE    -1332.
      * Invalid Service Name
       78  SQLE-RC-INV-SERNAME       VALUE    -1333.

      * Double-hops not allowed
       78  SQLE-RC-DOUBLE-REMOTE     VALUE    -1334.
      * AR name has invalid chars
       78  SQLE-RC-INVAR             VALUE    -1335.

      * File server is unknown
       78  SQLE-RC-UNKNOWN-FILESERVER VALUE   -1340.
      * Invalid File Server
       78  SQLE-RC-INV-FSERVER       VALUE    -1342.
      * Invalid Object Name
       78  SQLE-RC-INV-OBJNAME       VALUE    -1343.

      * Backup or Restore is active
       78  SQLE-RC-BR-ACTIVE         VALUE    -1350.

      * Quiesce is failed
       78  SQLE-RC-QUIESCE-FAILED    VALUE    -1371.
      * Unquiesce is failed
       78  SQLE-RC-UNQUIESCE-FAILED  VALUE    -1373.
      * Invalid Instance Name
       78  SQLE-RC-INV-INSTANCE      VALUE    -1390.
      * Another Instance is using the DB
       78  SQLE-RC-INSTANCE-USING    VALUE    -1391.
      * Invalid DB2 Path Name
       78  SQLE-RC-INV-DB2PATH       VALUE    -1393.
      * The DB2 service failed to logon
       78  SQLE-RC-SERVICE-LOGON-FAILURE VALUE -1397.

      * Unsupported authentication type
       78  SQLE-RC-BAD-AUTH          VALUE    -1400.
      * Authentication types do not match
       78  SQLE-RC-DIFF-AUTH         VALUE    -1401.
      * Authentication failed due to unexpected error
       78  SQLE-RC-AUTH-ERR          VALUE    -1402.
      * Invalid user name and/or password
       78  SQLE-RC-AUTH-FAILURE      VALUE    -1403.
      * Password has expired
       78  SQLE-RC-PASSWORD-EXPIRED  VALUE    -1404.
      * Password without userid
       78  SQLE-RC-PASSWD-WITHOUT-USERID VALUE -1425.
      * Error getting DB2INSTDFT
       78  SQLE-RC-DB2INSTDFT-ERROR  VALUE    -1426.
      * No current attachment
       78  SQLE-RC-NOT-INSTANCE-ATTACHED VALUE -1427.
      * Attached to wrong instance
       78  SQLE-RC-WRONG-ATTACH      VALUE    -1428.
      * Relative path not allowed
       78  SQLE-RC-RELPATH-NOT-ALLOWED VALUE  -1431.
      * Connected to wrong database
       78  SQLE-RC-WRONG-CONNECT     VALUE    -1433.

      * Ctx parm invalid
       78  SQLE-RC-CTX-INV-PARM      VALUE    -1441.
      * App Ctx not in use
       78  SQLE-RC-CTX-NOTINUSE      VALUE    -1442.
      * Already using Ctx
       78  SQLE-RC-CTX-USING         VALUE    -1443.
      * App Ctx in use
       78  SQLE-RC-CTX-INUSE         VALUE    -1444.
      * Thread does not have context
       78  SQLE-RC-CTX-NO-CTX        VALUE    -1445.

      * Invalid registration info. ptr.
       78  SQLE-RC-INVREGINFO-PTR    VALUE    -1450.
      * Reg. issued from invalid node
       78  SQLE-RC-REG-INVNODE       VALUE    -1451.
      * Invalid registration location
       78  SQLE-RC-INVREGLOC         VALUE    -1452.
      * Invalid file server in DBM cfg.
       78  SQLE-RC-INVCFG-FSNAME     VALUE    -1453.
      * Invalid object name in DBM cfg.
       78  SQLE-RC-INVCFG-OBJNAME    VALUE    -1454.
      * Invalid IPX socket in DBM cfg.
       78  SQLE-RC-INVCFG-IPXSOCKET  VALUE    -1455.
      * Object name already exists
       78  SQLE-RC-DUPLICATE-OBJNAME VALUE    -1456.
      * NWDS connection exists, cannot log into NW fileserver
       78  SQLE-RC-NWDS-CONNEXISTS   VALUE    -1457.
      * DB2 server reg./dereg. not needed
       78  SQLE-RC-REG-NOT-NEEDED    VALUE    -1458.
      * Invalid TCPIP Security 
       78  SQLE-RC-INVSTYPE-TCP      VALUE    -1461.

      * Database is started but only one bufferpool is started
       78  SQLE-RC-ONE-BUFFERPOOL    VALUE    1478.

      * An error occured when starting the DB2 security daemon
       78  SQLE-RC-SECD-FAILURE      VALUE    -1525.

      * FCM startup error when using VI
       78  SQLE-RC-VI-ERROR          VALUE    -1526.

      * Function is no longer supported
       78  SQLE-RC-UNSUPP-FUNCTION   VALUE    -1650.

      * Invalid server level for request
       78  SQLE-RC-INV-SERVERLVL     VALUE    -1651.
      * File I/O error
       78  SQLE-RC-FILEIO-ERR        VALUE    -1652.
      * Invalid profile path
       78  SQLE-RC-INV-PROFILE-PATH  VALUE    -1653.
      * Instance path error
       78  SQLE-RC-INSTPATH-ERR      VALUE    -1654.

      * Generator failed
       78  SQLE-RC-GENERATOR-FAILED  VALUE    -1660.

      * Discover is disabled in DBM CFG
       78  SQLE-RC-DSCVR-DISABLED    VALUE    -1670.
      * Search discovery failed
       78  SQLE-RC-SEARCH-DSCVR-FAILED VALUE  -1671.
      * Invalid discover address list
       78  SQLE-RC-INV-DSCVR-ADDRLST VALUE    -1673.
      * Invalid discover address
       78  SQLE-RC-INV-DSCVR-ADDR    VALUE    -1674.
      * Invalid admin. server
       78  SQLE-RC-INV-ADMINSERVER   VALUE    -1675.

      * Invalid Schema name found
       78  SQLE-RC-INV-SCHEMA        VALUE    -1700.
      * DB cannot be migrated
       78  SQLE-RC-DB-NOT-MIGR       VALUE    -1701.
      * Fail to create db2event dir
       78  SQLE-RC-CRT-EVENT-FAIL    VALUE    -1703.
      * DB migration failed
       78  SQLE-RC-DB-MIG-FAIL       VALUE    -1704.
      * Fail to update directory entry
       78  SQLE-RC-UPDATE-DIR-FAIL   VALUE    1705.

      * Invalid Request Info pointer
       78  SQLE-RC-INV-REQINFO-PTR   VALUE    -1800.
      * Invalid Request Type
       78  SQLE-RC-INV-REQUEST-TYPE  VALUE    -1801.
      * No entry belongs to Request Type
       78  SQLE-RC-NO-NODE-ENTRY     VALUE    -1802.
      * Node already exists in node directory
       78  SQLE-RC-NODE-EXIST        VALUE    -1803.

      * DB is already activated
       78  SQLE-RC-DB-ACTIVATED      VALUE    1490.
      * DB is still active
       78  SQLE-RC-DB-NOT-STOPPED    VALUE    -1491.
      * DB is not active
       78  SQLE-RC-DB-NOT-UP         VALUE    -1492.
      * Application is connected to a database
       78  SQLE-RC-APP-IS-CONNECTED  VALUE    -1493.
      * There is already DB connection
       78  SQLE-RC-ACT-DB-CONNECTED  VALUE    1494.
      * There is still DB connection
       78  SQLE-RC-DEACT-DB-CONNECTED VALUE   1495.
      * DB is not activated
       78  SQLE-RC-DEACT-DB-NOT-ACTIVATED VALUE 1496.
      * Error occurs on some nodes
       78  SQLE-RC-ACTDEACT-ERROR    VALUE    1497.
      * Invalid parameter
       78  SQLE-RC-INVALID-PARM      VALUE    -2032.

      * Quiesce is pending
       78  SQLE-RC-QUIESCE-PENDING   VALUE    -3807.
      * Unquiesce is pending
       78  SQLE-RC-UNQUIESCE-PENDING VALUE    -3808.
      * Error not Admin Server
       78  SQLE-RC-E4411             VALUE    -4411.
      * Admin Server not started
       78  SQLE-RC-NOADMSTART        VALUE    -4414.

      * Communications support failed
       78  SQLE-RC-COMM-FAILED       VALUE    5043.

      * Vendor cfg file not found
       78  SQLE-RC-NO-VENDOR-CFG     VALUE    -5500.
      * Vendor cfg file invalid 
       78  SQLE-RC-BAD-VENDOR-CFG    VALUE    -5501.

      * System database directory is not shared by all PDB nodes
       78  SQLE-RC-SDIRERR           VALUE    -6022.
      * The path specified for the database directory is not valid
       78  SQLE-RC-PATH-NOT-QUAL     VALUE    -6027.
      * Database is not found in local database directory
       78  SQLE-RC-LOCALDB-NOT-FOUND VALUE    -6028.
      * Invalid parameter for start/stop
       78  SQLE-RC-INV-PARM          VALUE    -6030.
      * Error in db2nodes.cfg file
       78  SQLE-RC-ERR-DB2NODES-CFG  VALUE    -6031.
      * Some nodes have not been started properly
       78  SQLE-RC-STARTED-PARTIALLY VALUE    6032.
      * Some nodes have not been stopped properly
       78  SQLE-RC-STOPPED-PARTIALLY VALUE    6033.
      * The node is not in used by any database
       78  SQLE-RC-NODE-NOT-INUSE    VALUE    6034.
      * The node is used by a database
       78  SQLE-RC-NODE-INUSE        VALUE    6035.
      * Start/stop command in progress
       78  SQLE-RC-START-STOP-IN-PROG VALUE   -6036.
      * Timeout reached for start/stop
       78  SQLE-RC-NODE-TIMEOUT      VALUE    -6037.
      * Invalid action for DROP NODE
       78  SQLE-RC-INVDROP-ACTION    VALUE    -6046.
      * Communication error in start/stop
       78  SQLE-RC-COMM-ERR          VALUE    -6048.
      * Need to stop the node before starting the node with the restart
      * option
       78  SQLE-RC-NODE-NEED-STOP    VALUE    -6072.
      * Add Node failed
       78  SQLE-RC-ADD-NODE-FAIL     VALUE    -6073.
      * Add Node failed because Create/Drop Db in progress
       78  SQLE-RC-ADD-NODE-CRDP     VALUE    -6074.
      * Add Node operation successful
       78  SQLE-RC-ADD-NODE-OK       VALUE    6075.
      * Prompt for db2stop DROP NODENUM
       78  SQLE-RC-DROP-PROMPT       VALUE    6076.
      * Drop node OK. Files still exist
       78  SQLE-RC-DROP-OK           VALUE    6077.
      * Drop node failed
       78  SQLE-RC-DROP-FAILED       VALUE    -6078.
      * Drop node cancelled successfully
       78  SQLE-RC-DROP-CANCEL       VALUE    6079.
      * Add Node succeeded but no databases created on the node
       78  SQLE-RC-ADD-NODE-NODB     VALUE    6080.
      * Timeout reached in stop FORCE
       78  SQLE-RC-FORCE-TIMEOUT-ERR VALUE    -6081.

      * No DB2 license
       78  SQLE-RC-DB2-SERVER-LICENSE VALUE   -8000.
      * Out of DB2 client licenses
       78  SQLE-RC-DB2-LICENSE       VALUE    -8001.
      * Out of DDCS clients
       78  SQLE-RC-DDCS-LICENSE      VALUE    -8002.

      * Federated cfg file not found
       78  SQLE-RC-NO-FED-CFG        VALUE    -5180.
      * Federated cfg file invalid
       78  SQLE-RC-BAD-FED-CFG       VALUE    -5181.

      * Invalid port number
       78  SQLE-RC-BAD-PORT          VALUE    -1888.
      * Update Alternate Server Ignored
       78  SQLE-RC-IGNORE-UPD-ALT-SVR VALUE   1889.
      * Invalid host name
       78  SQLE-RC-BAD-HOSTNAME      VALUE    -1890.
      * Invalid host name pointer
       78  SQLE-RC-HOSTNAME-PTR      VALUE    -1891.
      * Invalid port number pointer
       78  SQLE-RC-PORT-PTR          VALUE    -1892.

      *************************************************************************
      * 
      * The following functions and symbols are obsolete and may not be supported
      * in future releases. The obsolete functions are provided for backward compatibility
      * and exported from DB2API.LIB. All applications should be migrated to use new APIs.
      * Note: Some of the function parameters may be NO-OP.
      *       Some structures are larger (eg. SQLEDINFO) in V2.
      * 
      **************************************************************************
      * Authentication Not Specified
       78  SQL-AUTHENTICATION-UNDEF  VALUE    255.
      * Invalid numsegs on create db
       78  SQLE-RC-BAD-SEGPAGES      VALUE    -1136.

      * No CS/6000 license
       78  SQLE-RC-CS-LICENSE        VALUE    -8003.
      * No SNA/6000 license
       78  SQLE-RC-SNA-LICENSE       VALUE    -8004.

      * Authentication Undefined
       78  SQL-AUTHENTICATION-UNDEF  VALUE    255.

      * PE V1.2 Start Database Manager structure
       01 SQLEDBSTRTOPT.
      * Profile specified?
           05 SQL-ISPROFILE          PIC 9(9) COMP-5.
      * Profile
           05 SQL-PROFILE            PIC X(235).
           05 FILLER                 PIC X.
      * Node number specified?
           05 SQL-ISNODENUM          PIC 9(9) COMP-5.
      * Node number
           05 SQL-NODENUM            PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * Command specified?
           05 SQL-ISCOMMAND          PIC 9(9) COMP-5.
      * Command
           05 SQL-SCOMMAND           PIC X(10).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).
      * Hostname specified?
           05 SQL-ISHOSTNAME         PIC 9(9) COMP-5.
      * Hostname
           05 SQL-HOSTNAME           PIC X(255).
           05 FILLER                 PIC X.
      * Port specified?
           05 SQL-ISPORT             PIC 9(9) COMP-5.
      * Port
           05 SQL-PORT               PIC S9(9) COMP-5.
      * Netname specified?
           05 SQL-ISNETNAME          PIC 9(9) COMP-5.
      * Netname
           05 SQL-NETNAME            PIC X(255).
           05 FILLER                 PIC X.

      *************************************************************************
      * 
      * End of obsolete functions and symbols
      * 
      **************************************************************************
