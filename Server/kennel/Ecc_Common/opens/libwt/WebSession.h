// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef WEBSESSION_H_
#define WEBSESSION_H_

#include <string>
#include <vector>
#include <windows.h>


class WApplication;
class WWidget;
class WObject;
class WResource;
class JavaScript;

/*
 * time (in minutes) without a keep-alive request
 * before the session timeouts and self-destructs
 */
const int SESSION_TIME_OUT = 1;

typedef struct entry_type
{
  char * name;
  char * val;
  char * content_type;
  char * file_name;
  int content_length;
  int need_free;
} cgi_entry_type;
  
extern cgi_entry_type * cgi_entries;
  
  
/* Cookie data (name/value pairs): */
  
typedef struct cookie_type
{
  char * name;
  char * val;
} cgi_cookie_type;


/* Number of unique name/value entries found (by cgi_init() below). */



/* Possible error states: */

enum {
  CGIERR_NONE,
  CGIERR_NOT_INTEGER,
  CGIERR_NOT_DOUBLE,
  CGIERR_NOT_BOOL,
  CGIERR_UNKNOWN_METHOD,
  CGIERR_INCORRECT_TYPE,
  /*  CGIERR_NULL_QUERY_STRING, */
  CGIERR_BAD_CONTENT_LENGTH,
  CGIERR_CONTENT_LENGTH_DISCREPANCY,
  CGIERR_CANT_OPEN,
  CGIERR_OUT_OF_MEMORY,
  CGIERR_NO_BOUNDARY,
  CGIERR_NO_COOKIES,
  CGIERR_COOKIE_NOT_FOUND,
  CGIERR_N_OUT_OF_BOUNDS,
  CGIERR_NUM_ERRS
};



/* Current error state. */


enum {
  CGIREQ_NONE,
  CGIREQ_GET,
  CGIREQ_POST,
  CGIREQ_UNKNOWN
};

/*
  Request method:
*/

enum {
  CGITYPE_NONE,
  CGITYPE_APPLICATION_X_WWW_FORM_URLENCODED,
  CGITYPE_MULTIPART_FORM_DATA,
  CGITYPE_UNKNOWN
};



class __declspec(dllexport) WebSession
{
public:
  WebSession(const std::string sessionId,
	  bool standalone = false, bool bSave = false, std::string reportname = "", std::string szSavePath = "");

  static std::string   js_af_up;

  bool m_bReload;

  void start(void( *ptr)(int , char **), int argc = 0, char ** argv= NULL);

  static int WApplicationExecCallback(WApplication *application);

  bool haveJavaScript() const { return javaScript_; }
  bool havaAjax() const { return ajax_; }
  void WriteLog(char * filename, char * m_Str);
 void WriteLog1(char * filename, char * m_Str);

  bool          javaScript_;
  bool          ajax_;
  
  std::string   submitUrl_;
  bool          submit_;

  std::string   redictaddr_;
  bool          redict_;

  bool          m_bSave;
  std::string m_reportname;
  std::string m_savepath;

  std::string m_code;

   //wangpeng  update ºóµÄjavascript
  
  std::string strPostData;
  HANDLE hCgiEvent;
  HANDLE hModEvent;
  bool accept();

  //********************************start cgiutil head***********************
  cgi_entry_type * cgi_entries ;
  cgi_cookie_type * cgi_cookies ;
  int cgi_num_entries;
  int cgi_num_cookies ;
  int cgi_errno;
  int cgi_request_method ;
  int cgi_content_type ;
  char * cgi_query ;


  int cgi_init(void);



void cgi_quit(void);

void cgi_quit1(void);


int cgi_parse_cookies(void);
const char * cgi_getcookie(const char * cookie_name);



int cgi_getnumentries(const char *field_name);



const char * cgi_getentrystr(const char *field_name, int *length);
const char * cgi_getnentrystr(const char *field_name, int n, int *length);



const char * cgi_getentrytype(const char *field_name);
const char * cgi_getnentrytype(const char *field_name, int n);


const char * cgi_getentryfilename(const char *field_name);
const char * cgi_getnentryfilename(const char *field_name, int n);



int cgi_getentryint(const char *field_name);
int cgi_getnentryint(const char *field_name, int n);


double cgi_getentrydouble(const char *field_name);
double cgi_getnentrydouble(const char *field_name, int n);



int cgi_getentrybool(const char *field_name, int def);
int cgi_getnentrybool(const char *field_name, int def, int n);



int cgi_dump_no_abort(const char * filename);


void cgi_dump(const char * filename);


int cgi_goodemailaddress(const char * addr);

void cgi_error(const char * reason);



const char * cgi_strerror(int err);

int GetSessionIdbyCookie();
int GetUserIdbyCookie();
std::string GetIdcUserIdbyCookie();
char  strUid[30];

private:
  std::string   url_;
  std::string   sessionId_;
  WApplication *application_;
  bool          standalone_;

  bool          timeout_;
  bool		expectLoadEvent_;
  

  WWidget      *currentMainWidget_;
  std::string   currentElementName_;

  void setCurrentWidget(WWidget *w);

  enum ResponseType { RESPONSE_EVAL, RESPONSE_HTML, RESPONSE_RESOURCE,
		      RESPONSE_EVAL_LOAD };
  ResponseType response_;

  std::vector<WObject *> currentFormObjects_;
  std::string		 currentFormObjectsVar_;

  int  eventLoop(WApplication *application);
  int  eventLoop();

  

  WResource *handleRequestData();
  void generateResponse(WResource *resource = 0);

  void serveResource(WResource *resource);
  void serveJavascriptUpdate(WWidget *mainWidget);
  void serveMainpage(WWidget *mainWidget);

  void updateStaticSlotCode(WWidget *mainWidget, JavaScript& js);

  std::string createFormsVar(const std::vector<WObject *>& formObjects);
  std::string FuncGetDataByPostMethod();

  //int wmain(void * ptr);

  static WebSession *instance_;
  friend class DomElement;
  friend class WApplication;

  
  //for env ÄÚ´æÐ¹Â¶  wangpeng
	char szEnv[4096];  
  char *GetWtEnv(const char* pKey);  
  int nFirst;
};
std::string GetWebUserID();
std::string GetWebIdcUserID();
#endif // WEBSESSION_H_
