
#include "stdafx.h"

#define		_LASTRUN			"lastrun"
#define		_CPU_BUSY			"cpu_busy"
#define		_IO_BUSY			"io_busy"
#define		_IDLE				"idle"
#define		_PACK_RECEIVED		"pack_received"
#define		_PACK_SENT			"pack_sent"
#define		_CONNECTIONS		"connections"
#define		_PACK_ERRORS		"pack_errors"
#define		_TOTAL_READ			"total_read"
#define		_TOTAL_WRITE		"total_write"
#define		_TOTAL_ERRORS		"total_errors"

BOOL SYBASE_MONITOR(char *dbconn, char *uid, char *pwd, char *custpath, char *szReturn);
