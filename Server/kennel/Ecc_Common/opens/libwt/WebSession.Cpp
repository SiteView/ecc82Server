/*
* Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
*
* See the LICENSE file for terms of use.
*/
#include <boost/lexical_cast.hpp>

#define NO_FCGI_DEFINES

//#include <cgi-util.h>

//#include <iostream>

#include "WebSession.h"
#include "JavaScript.h"
#include "DomElement.h"
#include "WApplication"
#include "WWidget"
#include "WContainerWidget"
#include "WText"
//#include <winsock2.h>
#include "WSignalInstance_"
#include "WSlotInstance_"
#include "WConnection_"
#include "../../kennel/svdb/svapi/svdbapi.h"
#include "../../base/stlini.h"
#include <fcgi_stdio.h>
#include <fcgio.h>

//extern char** environ;
WebSession *WebSession::instance_ = 0;
std::string  WebSession::js_af_up ="";
const int MESSAGE_COUNTER_SIZE = 5;

void WriteLogFile(const char *pszFile, const char *pszMsg)
{
    FILE *pFile = fopen(pszFile, "a+");
    if (pFile)
    {
        fputs(pszMsg, pFile);
        fputs("\n", pFile);
        fclose(pFile);
    }
}


void WebSession::WriteLog(char * filename, char * m_Str)
{
    FILE *pf = fopen("c:\\start.log", "a+");
    fputs(m_Str, pf);
    fclose(pf);
}

void WebSession::WriteLog1(char * filename, char * m_Str)
{
    FILE *pf = fopen("c:\\free.log", "a+");
    fputs(m_Str, pf);
    fclose(pf);
}

WebSession::WebSession(const std::string sessionId,
					   bool standalone, bool bSave, std::string reportname, std::string szSavePath)
                       : sessionId_(sessionId),
                       application_(0),
                       standalone_(standalone),
                       timeout_(false),
                       javaScript_(false),
                       ajax_(false),
                       submit_(false),
					   m_bSave(bSave),
					   m_reportname(reportname),
					   m_savepath(szSavePath)
{
    char pidEventName[64];
    sprintf(pidEventName,"SiteView-Cgi-Event-%d",GetCurrentProcessId());	
	char modEventName[64]={0};
	sprintf(modEventName,"ModFcgi-Event-%d",GetCurrentProcessId());	

    hCgiEvent = CreateEvent(NULL,TRUE,FALSE,pidEventName);
//    ResetEvent(hCgiEvent);
	hModEvent= CreateEvent(NULL,TRUE,FALSE,modEventName);
    instance_ = this;

    cgi_entries = NULL;
    cgi_cookies = NULL;
    cgi_num_entries = 0;
    cgi_num_cookies = 0;
    cgi_errno = CGIERR_NONE;
    cgi_request_method = CGIREQ_NONE;
    cgi_content_type = CGITYPE_NONE;
    cgi_query = NULL;
    redict_ = false;
    nFirst =0;

	bSave = false;
    m_bReload = false;

	std::string szPath = GetSiteViewRootPath();
	szPath += "\\data\\svdbconfig.ini";

	INIFile theINI = LoadIni(szPath.c_str());

	std::string szTSec = GetIniSetting(theINI, "svdb", "DefaultLanguage");
	szTSec += "_code";
	m_code = GetIniSetting(theINI, "svdb", szTSec.c_str());
	if(m_code.size()<=0)
		m_code = "gb2312";
	//OutputDebugString("----------websession code language------------\n");
	//OutputDebugString(m_code.c_str());
	//OutputDebugString("\n");
}
#define  DEBUG_CGI

bool WebSession::accept()
{
    DWORD ret = WaitForSingleObject(hCgiEvent, 600 * 1000);
    ResetEvent(hCgiEvent);
    if(ret == WAIT_TIMEOUT)
    {
        //OutputDebugString("WAIT_TIMEOUT\n");
		timeout_ =true;

        return false;
    }
	strPostData ="";
    if(FCGI_Accept() == 0)
    {
        //char ** myenviron;
		environ=FCGI_GetEnv();
      //  myenviron =FCGI_GetEnv();
     //   char myenviron1[4096]={0};
     //   char *ca,*cb;
     //   for(int i=0 ;myenviron[i];++i)
     //   {	
     //       strcpy(myenviron1,myenviron[i]);
     //       ca =myenviron1;
     //       cb =strchr(ca,'=');
     //       if(cb){
     //           *cb =0;
     //           cb++;
     //           if(cb)
     //           {
     //               SetEnvironmentVariable(ca,cb);
					///*
					//OutputDebugString(ca);
     //               OutputDebugString("=====");
     //               OutputDebugString(cb);
					//OutputDebugString("\n");*/
     //           }
     //        }
     //    }
		
		//if (strcmp(getenv("REQUEST_METHOD"), "POST") == 0)
		//{
		//	  
		//		strPostData=FuncGetDataByPostMethod();

		//		OutputDebugString(" accept  Post Data is    ");

		//		OutputDebugString(strPostData.c_str());

		//		OutputDebugString("end ----------------\n");
		//}
         return TRUE;
     }else
        return FALSE;


}
int WebSession::GetSessionIdbyCookie()
{
    const char *strsvsid;
    strsvsid=cgi_getcookie("svsid");
    if(strsvsid==NULL) return GetCurrentProcessId();
    else return atoi(strsvsid);

}
std::string GetWebUserID()
{
	/*
	const char *strsvsid;
	std::string strUserId;
	strsvsid=WebSession::cgi_getcookie("userid");
    if(strsvsid==NULL) return "0";
    else
	{
		strUserId=strsvsid;
		return  strUserId;
	}
*/


	
    char *cookies;

	//OutputDebugString("get cookie\n");

    cookies=getenv( "HTTP_COOKIE" );
	if(cookies==NULL) return "";

	//OutputDebugString(cookies);
    char *strsvsid;
    char strUid[30];
    memset(strUid,0,30);
    std::string strUserid="";
    strsvsid = strstr(cookies,"olduserid=");

    int nUid;
    if(strsvsid !=NULL)
    {
        sscanf(strsvsid,"olduserid=%d",&nUid);

    }
    sprintf(strUid,"%d",nUid);

    std::string returnUid;
    returnUid =strUid;

    return returnUid;
}

std::string GetWebIdcUserID()
{
	/*
	const char *strsvsid;
	std::string strUserId;
	strsvsid=WebSession::cgi_getcookie("userid");
    if(strsvsid==NULL) return "0";
    else
	{
		strUserId=strsvsid;
		return  strUserId;
	}
*/


	
    char *cookies;

	//OutputDebugString("get cookie\n");

    cookies=getenv( "HTTP_COOKIE" );
	if(cookies==NULL) return "";

	//OutputDebugString(cookies);
    char *strsvsid;
    char strUid[30];
    memset(strUid,0,30);
    std::string strUserid="";
    strsvsid = strstr(cookies,"idcuserid=");

	//OutputDebugString("strsvsid cookie tmp=\r\n");
	//OutputDebugString(strsvsid);

    string striduserid = "idcusriderror", strTmp = "";
    if(strsvsid !=NULL)
    {
        strTmp = strsvsid;
		if(strTmp.find("idcuserid=") != -1)
		{
			//OutputDebugString("idcuserid cookie tmp=\r\n");
			//OutputDebugString(strTmp.c_str());
			striduserid = strTmp.erase(0, 10);
			int nLength = strTmp.find(";");
			if(nLength > 0)
				striduserid = strTmp.substr(0, nLength);
			//OutputDebugString(striduserid.c_str());
		}
    }    

    return striduserid;
}

int  WebSession::GetUserIdbyCookie()
{
	/*
    const char *cookies;
	char aaa[2000]={0};
    if(!(cookies = GetWtEnv("HTTP_COOKIE")))
    {
		sprintf(aaa,"GetUserIdbyCookie error is  %d\n",0);
		OutputDebugString(aaa);
        return 0;
    }
    char *strsvsid;
    int nUid;
   
    strsvsid = strstr(cookies,"userid=");
    if(strsvsid !=NULL)
    {
        sscanf(strsvsid,"userid=%d",&nUid);

    }
	
	sprintf(aaa,"GetUserIdbyCookie is %s %d\n",cookies, nUid);
	OutputDebugString(aaa);

	return nUid;
    */
	const char *strsvsid;
	
    strsvsid=cgi_getcookie("olduserid");
    if(strsvsid==NULL) return 0;
    else
	{
		
		return  atoi(strsvsid);
	}
}

string WebSession::GetIdcUserIdbyCookie()
{
    const char *strsvsid;
    strsvsid=cgi_getcookie("idcuserid");
    if(strsvsid==NULL) 
		return "idcuseriderror";
    else
		return string(strsvsid);
}

void WebSession::start(void( *ptr)(int, char **), int argc , char **argv )
{
	char aaa[2000]={0};

    if (standalone_)
    {
        ptr(argc, argv);
    }
    else 
    {
        if (accept()) 
        {
            url_ = std::string(GetWtEnv("SCRIPT_NAME")) + "?sessionid=" + sessionId_;
            cgi_init();
            ajax_=true;
            javaScript_=true;
            ptr(argc,argv);
            if (!timeout_)
                generateResponse();
        }
    }

    //std::cerr << "Session " << sessionId_ << " terminated cleanly."
      //  << std::endl;
}

int WebSession::WApplicationExecCallback(WApplication *application)
{
    return instance_->eventLoop(application);
}

int WebSession::eventLoop(WApplication *application)
{
    application_ = application;

    int result = eventLoop();

    application_ = 0;

    return result;
}

int WebSession::eventLoop()
{
    if (standalone_) 
    {

		OutputDebugString("standalone_\n");
        handleRequestData();
        generateResponse();
        return 0;
    }
    else
    {
        handleRequestData();
        generateResponse();


        while (accept()) 
        {
            

            WResource *resource = handleRequestData();



            if (application_ && application_->isQuited()) 
            {
                return 0;
            }

            generateResponse(resource);
        }
        //OutputDebugString("go out loop\n");

        /*
        * problem or time-out: in any case quit the application nicely.
        */
        return -1;

    }
}

WResource *WebSession::handleRequestData()
{

    cgi_init();


    const char *resourceId = cgi_getentrystr("resource", 0);
    if (resourceId)
    {
        response_ = RESPONSE_RESOURCE;

        return WResource::decodeResource(resourceId);
    }
    else
    {
        bool isReload = cgi_getentrystr("signal", 0) == NULL;

        for (unsigned i = 0; i < currentFormObjects_.size(); ++i) 
        {
            WObject *obj = currentFormObjects_[i];
            std::string objname = obj->formName();

            int length;
            const char *value = cgi_getentrystr(objname.c_str(), &length);
            const char *filename = cgi_getentryfilename(objname.c_str());
            if (value != NULL) 
            {
                obj->setFormData(value, length, filename);
            } 
            else 
            {
                if (!isReload)
                    obj->setNoFormData();
            }
        }

        /*
        * Get the signal.
        */
        if (cgi_getentrystr("signal", 0) != NULL)
        {
            std::string signalValue = cgi_getentrystr("signal", 0);
            std::cerr << "signal: '" << signalValue << "'" << std::endl;
            if (signalValue.substr(0, 4) == "none") 
            {
                response_ = RESPONSE_EVAL;
            } 
            else if (signalValue.substr(0, 4) == "load") 
            {
                response_ = RESPONSE_EVAL_LOAD;
            } 
            else
            {
                char tmp[4096] = {0};
                int nSize = 4095;
	            GetEnvironmentVariable( "REMOTE_ADDR", tmp, nSize);
    //            std::string szMsg  = "host name:" ;
    //            szMsg = szMsg + tmp;
    //            szMsg = szMsg + "msg id is :" ;
    //            szMsg = szMsg + cgi_getentrystr("msg", 0);
    //            szMsg = szMsg + "\tget signal: ";
    //            szMsg = szMsg + signalValue + "\n";
				//OutputDebugString(szMsg.c_str());
				
				WSignalInstance_ *signal = WSignalInstance_::decodeSignal(signalValue);
				if (!standalone_) 
				{
					if(signal==NULL)
					{
						OutputDebugString("signal is null\n");
						response_ = RESPONSE_EVAL;
						return 0;
					}
				}			
				
				
				

				/*
				* Get the (integer) argument
				*/
				int argValue;
				void *args[1] = { &argValue };

				if (cgi_getentrystr("arg", 0) != NULL)
					argValue = cgi_getentryint("arg");

				/*
				* First trigger all static connections of this signal,
				* and ignore their JavaScript changes
				*/
				signal->trigger(WObject::STATIC, args);
				WWidget *mainWidget = application_->root();
				DomElement *mainElement = mainWidget->webWidget()->createSDomElement();
				delete mainElement;      

				/*
				* Next trigger all dynamic connections
				*/
				signal->trigger(WObject::DYNAMIC, args);


				if (ajax_)
					response_ = RESPONSE_EVAL;
				else
					response_ = RESPONSE_HTML;

            }

            if (cgi_getentrystr("request", 0) != NULL)
            {

                response_ = RESPONSE_HTML;
            }
        } 
        else
            response_ = RESPONSE_HTML;

        if (isReload && application_)
            application_->refresh();

        return 0;
    }
}

std::string
WebSession::createFormsVar(const std::vector<WObject *>& formObjects)
{
    std::string formObjectsVar;
    for (unsigned i = 0; i < formObjects.size(); ++i) 
    {
        if (i != 0)
            formObjectsVar += ',';
        formObjectsVar += "'" + formObjects[i]->formName() + "'";
    }

    return formObjectsVar;
}

void WebSession::serveMainpage(WWidget *mainWidget)
{
    WWebWidget *mainWebWidget = mainWidget->webWidget();

    if (ajax_) 
    {
        /*
        * first collect the static code, but ignore the java script code
        * that results.
        */

        application_->setLoading(true);

        DomElement *mainElement = mainWebWidget->createSDomElement();
        delete mainElement;

        JavaScript js;
        mainWidget->updateStaticSlotCode(mainWebWidget, js);
    }
    else
        application_->setLoading(false);

    /*
    * next create the element to render. This automatically creates
    * loading stubs for invisible widgets, which is excellent for both
    * javascript and non-javascript versions.
    */
    //char chmsg[128] = {0};
    //sprintf(chmsg, "DomElement object count:%d\n", DomElement::m_nObjCount);
    //OutputDebugString(chmsg);
    DomElement *mainElement = mainWebWidget->createSDomElement();

    std::vector<WObject *> formObjects;
    mainWebWidget->getFormObjects(formObjects);
    std::string formObjectsVar = createFormsVar(formObjects);


    const std::string JSDeclareFormVars
        = "var formObjects = new Array(" + formObjectsVar + ");\n";
    //"function redict(){self.location='emailtest.exe?';}\n";
    currentFormObjectsVar_ = formObjectsVar;
    currentFormObjects_ = formObjects;

    static const std::string scriptName
        = (standalone_ ? "noname" : GetWtEnv("SCRIPT_NAME"));

   // static const std::string postAjax
	std::string postAjax
        = ""
//cxy add 2007/08/07 抛出所有与服务器端通讯超时错误
//		"xmlHttpReq.timeout = 3000;\n"
//"if(signalName != 'load' && signalName != 'none') \n requestTimer = setTimeout(function() {hiddenbar(); bSend='true'; document.location.reload();alert('提示：连接服务器超时, 重新装载程序!');}, 15000);\n"
"if(signalName != 'load' && signalName != 'none') \n requestTimer = setTimeout(function() {hiddenbar(); bSend='true'; document.location.reload();}, 30000);\n"
//		 else alert(signalName);
	    //"requestTimer = setTimeout(function() {xmlHttpReq.abort(); hiddenbar(); bSend='true'; alert('连接服务器超时错误!');}, 1000);\n"
		//"requestTimer = setTimeout(function() {alert('连接服务器超时错误!');}, 50);\n"
//cxy add end

		"  xmlHttpReq.open('POST', url+query, true);\n"
        //"  self.xmlHttpReq.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded; charset=UTF-8\");\n"
       // "  self.xmlHttpReq.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded; charset=GB2312\");\n"
	   "  xmlHttpReq.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded;";
//		postAjax += m_code;
		postAjax += "\");\n"
	   "  xmlHttpReq.setRequestHeader(\"Content-Length\",querydata.length);\n"
		"  xmlHttpReq.setRequestHeader(\"Connection\",\"close\");\n"
		"  xmlHttpReq.onreadystatechange = callback;\n"
		"  xmlHttpReq.send(querydata);\n"
		"bSend='false';window.clearTimeout(keepAliveTimer);\n"
        "keepAliveTimer = setTimeout('keepAlive()', " + 
        boost::lexical_cast<std::string>(SESSION_TIME_OUT * 1000 * 60 * 3) +
        ");\n"
        "return 0;}"
        "else{\n"
        "return 1;} \n"
//cxy add 2007/08/07 抛出所有通讯相关错误 并隐藏球球	
		"}catch (e) { \n"
		"		hiddenbar(); \n"
		//"		alert('无法正常连接服务器, 错误1!');         \n"
		"		alert(e.name + ': ' + e.message); \n"
		"	} \n"
//cxy add end
        "}\n";
		/*"}\n"
        "}\n";*/

    static const std::string getAjax
        = ""
        "  xmlHttpReq.open('Get', url + query, true);\n"
        "  xmlHttpReq.onreadystatechange = callback;\n"
        "  xmlHttpReq.send('');\n"
        "bSend='false';window.clearTimeout(keepAliveTimer);\n"
        "keepAliveTimer = setTimeout('keepAlive()', " + 
        boost::lexical_cast<std::string>(SESSION_TIME_OUT * 1000 * 60 * 3) +
        ");\n"
        "return 0;}"
        "else{\n"
        "return 1;}\n"
        "}\n";


    static const std::string JSUpdateFunction
        = ""
        "var url = '" + application_->url() + "';\n"
        "var appUrl = '" + scriptName + "';\n"
        "function getElement(name) {\n"
        "  var el = document.getElementById(name);\n"
        "  if (!el)\n"
        "    for (var i = 0; i < window.frames.length; ++i) {\n"
        "      el = window.frames[i].document.getElementById(name);\n"
        "      if (el)\n"
        "        return el;\n"
        "    }\n"
        "  return el;\n"
        "}\n"
        "var message_tx_num = 0;\n"
        "var message_rx_num = 0;\n"
        "var messages = new Array();\n"
        "var requests = new Array();\n"
        "var bSend = 'true';\n"
//cxy add 2007/08/07 抛出所有通讯超时错误 
		"var  requestTimer = null;\n"
//cxy add end
        "function callback() {\n"
        "  var i;\n"
//cxy add 2007/08/07 抛出所有通讯相关错误 并隐藏球球	
		" if(requests.length > 0){ \n"
//cxy add end
        "  for (i = message_rx_num; i < requests.length; ++i) {\n"
        "    if ((requests[i] != null)\n"
        "        && (requests[i].readyState == 4)) {\n"
        "      try {\n"
        "        var msg = requests[i].responseText.substring("
        + boost::lexical_cast<std::string>(MESSAGE_COUNTER_SIZE) + ");\n"
        "        var message_num = parseInt(requests[i].responseText.substring(0, "
        + boost::lexical_cast<std::string>(MESSAGE_COUNTER_SIZE) + "));\n"
        "        requests[i] = null;\n"
		//"		   alert(msg);\n"	
        "        if (message_rx_num != message_num) {\n"
        "           bSend='true';\n"
        "          if ((message_tx_num - message_rx_num) > 5)\n"
        "            document.location.reload();\n"
        "          messages[message_num] = msg;\n"
        "        } else {\n"
        //"		   alert(msg);\n"	
        "          bSend='true';\n"
        "          eval(msg);\n"
        "          message_rx_num++;\n"
        "          while (typeof messages[message_rx_num] != 'undefined') {\n"
        "            eval(messages[message_rx_num]);\n"
        "            messages[message_rx_num]=null;\n"
        "            message_rx_num++;\n"
        "          }\n"
        "        }\n"
        "      } catch (e) {\n"
        "        alert(e.name + ': ' + e.message);\n"
        //"        document.location.reload();\n"
        "      }\n"
//cxy add 2007/08/07 抛出所有通讯相关错误 并隐藏球球
			"clearTimeout(requestTimer);  \n"
        "    }\n"		
	" else \n"
	" { \n"
	//" 	hiddenbar(); \n"
//	"     alert('无法正常连接服务器, 错误2!'); \n"
	" } \n"
  "  }\n"
" 	hiddenbar(); \n"
  " } \n"
  " else \n"
  " { \n"
  " 		hiddenbar(); \n"
 " 	    alert('无法正常连接服务器, 错误3!'); \n"
 "  } \n"
//cxy add end
       "}\n"
        "function update(signalName) {\n"
//cxy add 2007/08/07 抛出所有通讯相关错误 并隐藏球球
		"try { \n"
		"clearTimeout(requestTimer);  \n"
//cxy add end
        "if(bSend=='true'){\n"
        "  var xmlHttpReq = false;\n"
        "  if (window.XMLHttpRequest) {\n"
        "    xmlHttpReq = new XMLHttpRequest();\n"
        "  } else if (window.ActiveXObject) {\n"
        "    xmlHttpReq = new ActiveXObject(\"Microsoft.XMLHTTP\");\n"
        "  }\n"
        "  requests.push(xmlHttpReq);\n"

/*
        "  var query = '&signal=' \n"
        "     + signalName + '&msg=' + message_tx_num++ + '&rand=' + Math.round(Math.random()*1000000);\n"
        "  for (x in formObjects) {\n"
        "    var el = getElement(formObjects[x]);\n"
        "    if (el.type != 'file'"
        "        && (((el.type != 'checkbox')&&(el.type != 'radio')) || el.checked))\n"
        //    "      query += '&' + document.getElementById(formObjects[x]).name + '=' + escape(el.value);\n"
        "      query += '&' + formObjects[x] + '=' + escape(el.value);\n"

        "  }\n" 
		*/
		"  var query ='&rand=' + Math.round(Math.random()*1000000);\n"
		"  var querydata = 'signal=' \n" 
		"      + signalName  + '&msg=' + message_tx_num++;\n"

		"  for (x in formObjects) {\n"
        "    var el = getElement(formObjects[x]);\n"
        "    if (el.type != 'file'"
        "        && (((el.type != 'checkbox')&&(el.type != 'radio')) || el.checked))\n"
        //    "      query += '&' + document.getElementById(formObjects[x]).name + '=' + escape(el.value);\n"
        "      querydata += '&' + formObjects[x] + '=' + escape(el.value);\n"  //encodeURIComponent

        "  }\n" 

		" query += \"&timestamp=\""
		" +new Date().getTime();  \n"
  		//+ getAjax;
		+postAjax;

    std::string JSKeepAliveFunction;

    if(application_->getRefreshTime()<=0)

    {
        JSKeepAliveFunction+= "var keepAliveTimer;";
        JSKeepAliveFunction+= "function keepAlive() {\n";
        if(m_bReload)
            JSKeepAliveFunction+= 		"  update('load');\n";
        else
            JSKeepAliveFunction+= 		"  update('none');\n";

        //JSKeepAliveFunction+="  keepAliveTimer = setTimeout('keepAlive()', "; 


        //JSKeepAliveFunction+= boost::lexical_cast<std::string>(SESSION_TIME_OUT * 1000 * 60 * 3);
        //JSKeepAliveFunction+= ");\n";

        JSKeepAliveFunction+="}\n";
        JSKeepAliveFunction+="function doOnLoad() {\n";
        JSKeepAliveFunction+="  update('load');\n";
        JSKeepAliveFunction+="  keepAliveTimer = setTimeout('keepAlive()', ";
        JSKeepAliveFunction+= boost::lexical_cast<std::string>(SESSION_TIME_OUT * 1000 * 60 * 3);
        JSKeepAliveFunction+= ");\n";
        JSKeepAliveFunction+="}\n";;
    }else {

        JSKeepAliveFunction+= "var keepAliveTimer;";
        JSKeepAliveFunction+= "function keepAlive() {\n";
        //JSKeepAliveFunction+= 		"  update('load');\n";
        JSKeepAliveFunction+= 		"  update('";
        JSKeepAliveFunction+= 		application_->getRefreshFunc();
        JSKeepAliveFunction+= 		"');\n";
        //JSKeepAliveFunction+="  keepAliveTimer = setTimeout('keepAlive()', "; 


        //JSKeepAliveFunction+= boost::lexical_cast<std::string>(application_->getRefreshTime() * 1000 * 60);
        //JSKeepAliveFunction+= ");\n";

        JSKeepAliveFunction+="}\n";
        JSKeepAliveFunction+="function doOnLoad() {\n";
        JSKeepAliveFunction+="  update('";
        JSKeepAliveFunction+=application_->getRefreshFunc();
        JSKeepAliveFunction+="');\n";
        JSKeepAliveFunction+="  keepAliveTimer = setTimeout('keepAlive()', ";
        JSKeepAliveFunction+= boost::lexical_cast<std::string>(application_->getRefreshTime() * 1000 * 60);
        JSKeepAliveFunction+= ");\n";
        JSKeepAliveFunction+="}\n";;

    }


    if (ajax_) {
        /*
        * Start HTML page
        */
		
        FCGI_printf("Content-type: text/html\r\n"			
            "Content-Language: zh-CN\r\n"
			"Expires: Sun, 06 Nov 1994 08:49:37 GMT\r\n"
			"Pragma: no-cache\r\n"
//            "Set-Cookie:svsid=%d&userid=%d;\r\n\r\n"
            "Set-Cookie: svsid=%d\r\n"
			"Set-Cookie: idcuserid=%s\r\n"
			"Set-Cookie: olduserid=%d\r\n\r\n"			


            //"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">"
            "<html lang=\"en\" dir=\"ltr\">\n"
            "<head>\n"

            "<meta http-equiv='Pragma' content='no-cache' />\n"
			//"<meta http-equiv=\"Content-Type\" Content=\"text/html; Charset=gb2312\">\n"
            "<meta http-equiv=\"Content-Type\" Content=\"text/html; Charset=%s"
			 "\">\n"
            "<link rel=\"stylesheet\" href=\"../css.css\" type=\"text/css\">\n"  //wangpeng
            "<title>%s</title>\n"
            "<style type=\"text/css\">\n%s\n</style>\n"
            "<script language=\"JavaScript\">\n"
            "%s%s%s"
            "function hide(o) { "
            "getElement(o).style.display = 'none'; }\n"
            "function inline(o) { "
            "getElement(o).style.display = 'inline'; }\n"
//            "function block(o) { "
  //          "getElement(o).style.display = 'block'; }\n"
  "function block(o) { "
            "getElement(o).style.display = ''; }\n"
            "</script>\n"
            "</head>\n",
            GetSessionIdbyCookie(),
            GetIdcUserIdbyCookie().c_str(),
			GetUserIdbyCookie(),			
			m_code.c_str(),
            application_->title().c_str(),
            application_->styleSheet().cssText().c_str(),
            JSDeclareFormVars.c_str(),
            JSKeepAliveFunction.c_str(),
            JSUpdateFunction.c_str());


        FCGI_printf("%s",  application_->getBeforBodyStr().c_str() );


        //Sleep(2000);
        std::string   mianhtml =mainElement->asHTML();

		

        //OutputDebugString("end put value\n");
        //Sleep(5000);

        if(nFirst==0)
        {

            FCGI_printf("<body onload=\"doOnLoad(); \" %s >\n"
                "<form method='post' action='%s' id='form' "
                "enctype='multipart/form-data'>\n"
                "  <input name='signal' id='signal' type='hidden' "
                "value='none'></input>\n"
                "  <input name='request' id='request' type='hidden' "
                "value='page'></input>%s\n</form>\n",
                //"","",
                application_->getBodyAttribute().c_str(),
                application_->url().c_str(),
                mianhtml.c_str());


            mianhtml="";
            //Sleep(2000);
            //	char aaa[200];
            //	sprintf(aaa," mainElement->asHTML() is  %d\n",mianhtml.size());
            //  OutputDebugString(aaa);
            //mainElement->asHTML().c_str());
            //nFirst++;

        }else{
            FCGI_printf("<body onload=\"doOnLoad(); \" %s >\n"
                "<form method='post' action='%s' id='form' "
                "enctype='multipart/form-data'>\n"
                "  <input name='signal' id='signal' type='hidden' "
                "value='none'></input>\n"
                "  <input name='request' id='request' type='hidden' "
                "value='page'></input>%s\n</form>\n",
                //"","",
                application_->getBodyAttribute().c_str(),
                application_->url().c_str(),

                "aaaa");

        }


    }
    else
    {
        /*
        * Start HTML pagextree
        */


        FCGI_printf("Content-type: text/html\r\n"
			"Expires: Sun, 06 Nov 1994 08:49:37 GMT\r\n"
			"Pragma: no-cache\r\n"
			"Set-Cookie: svsid=%d\r\n"
			"Set-Cookie: idcuserid=%s\r\n"
			"Set-Cookie: olduserid=%d\r\n\r\n"			
            //"Set-Cookie: svsid=%d&userid=%d;\r\n\r\n"
            //"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">"
            "<html lang=\"en\" dir=\"ltr\">\n"
            "<head>\n"
            "<meta http-equiv='refresh' content='%d' />"
            "<meta http-equiv='Pragma' content='no-cache' />\n",
            GetSessionIdbyCookie(),
            GetIdcUserIdbyCookie().c_str(),
			GetUserIdbyCookie(),			
            SESSION_TIME_OUT * 59/2);

        if (javaScript_) 
        {
            FCGI_printf("<script language=\"JavaScript\">\n"
                "function update(signalName) {\n"
                "  document.forms[0].signal.value = signalName;\n"
                "  document.forms[0].submit();\n"
                "}\n"
                "</script>\n");
        }

        FCGI_printf("<title>%s</title>\n"
            "<style type=\"text/css\">\n%s\n</style>\n"
            "</head>\n",		
            application_->title().c_str(),
            application_->styleSheet().cssText().c_str());
        FCGI_printf("<body>\n"

            "<form method='post' action='%s' id='form' "
            "enctype='multipart/form-data'>",
            //	application_->url().c_str());
            application_->submiturl().c_str());

        if (javaScript_) 
        {
            FCGI_printf("<input name='signal' id='signal' type='hidden' "
                "value='none'></input>\n");
        }

		//保存为文件
		//OutputDebugString("----------------------before save file name output----------------------\n");
		if(m_bSave)
		{
			//OutputDebugString("----------------------save file name output----------------------\n");
			
			//std::string str = "C:\\Program Files\\apache group\\apache2\\htdocs\\report\\";
			std::string str = m_savepath;
			str += m_reportname;
			//OutputDebugString("-----------save file path---------------------\n");
			//OutputDebugString(str.c_str());
			//OutputDebugString("------------------end--------------------\n");
			//OutputDebugString(str.c_str());
			//OutputDebugString("\n");

			FILE * pf = fopen(str.c_str(), "w+");
			std::string strMainHtml;
			strMainHtml = mainElement->asHTML();
			std::string htmlhead = 
				"<html lang=\"en\" dir=\"ltr\">\n"
				"<head>\n"
				"<meta http-equiv='Pragma' content='no-cache' />\n"
				"<meta http-equiv=\"Content-Type\" Content=\"text/html; Charset=";
			htmlhead += m_code;//gb2312\">\n"
			htmlhead += "\">\n"
				"<link rel=\"stylesheet\" href=\"../css.css\" type=\"text/css\">\n" ;
			//fwrite(htmlhead.c_str(), htmlhead.size(), htmlhead.size(), pf);
			//fwrite(strMainHtml.c_str(), strMainHtml.size(), strMainHtml.size() + 300, pf);
			fputs(htmlhead.c_str(), pf);
			fputs(strMainHtml.c_str(), pf);
			std::string htmlend = "</head>\n"
				"</html>\n";
			fputs(htmlend.c_str(), pf);
			//fwrite(htmlend.c_str(), htmlend.size(), htmlend.size(), pf);
			
			fclose(pf);
		}
		else
		{
			FCGI_printf("%s</form>", mainElement->asHTML().c_str());
		}

		
    }


    if (application_->oaplDownloadPath_.length() > 0)
    {

    }
    FCGI_printf("</body></html>");
    //OutputDebugString("\n---------------- end ----------------\n"
    //  "---------------- end ----------------\n");
    delete mainElement;

    expectLoadEvent_ = true;
}

void WebSession::serveJavascriptUpdate(WWidget *mainWidget)
{
    JavaScript js;
    WWebWidget *mainWebWidget = mainWidget->webWidget();

    if (application_)
        application_->setLoading(false);




    

    if (currentMainWidget_ != mainWidget) 
    {
        DomElement *current
            = DomElement::getForUpdate(currentElementName_, DomElement::DIV);
        current->removeFromParent();
        current->createJSStatements(js);
        delete current;
    }

    if (mainWidget != currentMainWidget_) 
    {
        DomElement *form = DomElement::getForUpdate("form", DomElement::DIV);
        DomElement *mainElement = mainWebWidget->createSDomElement();

        mainElement->setId(mainWidget);
        form->addChild(mainElement);
        form->createJSStatements(js);
        delete form;
    }
    else
    {

        std::vector<DomElement *> changes;
        mainWebWidget->getSDomChanges(changes);
        for (unsigned i = 0; i < changes.size(); ++i) 
        {
            changes[i]->createJSStatements(js);
            delete changes[i];
        }
    }

    mainWidget->updateStaticSlotCode(mainWebWidget, js);



    std::vector<WObject *> formObjects;
    mainWebWidget->getFormObjects(formObjects);
    std::string formObjectsVar = createFormsVar(formObjects);

    if (formObjectsVar != currentFormObjectsVar_) 
    {
        js.addStatement("formObjects = new Array(" + formObjectsVar + ");");
        currentFormObjectsVar_ = formObjectsVar;
        currentFormObjects_ = formObjects;
    }

    if (!application_)
        js.addStatement("clearTimeout(keepAliveTimer);");
    else
    {
        if (application_->needSubmit_) 
        {
            js.addStatement("getElement('form').submit();");
        }

        application_->needSubmit_ = false;
    }

	std::string msgNum ;
	if(cgi_getentrystr("msg", 0)==NULL)
			msgNum ="";
	else
		msgNum = cgi_getentrystr("msg", 0);

    if (msgNum.size() != (unsigned)MESSAGE_COUNTER_SIZE)
        msgNum.insert(msgNum.size(), MESSAGE_COUNTER_SIZE - msgNum.size(), ' ');

    std::string response;

    //  js.addStatement("function redict(){self.location='emailtest.exe?';}");

    //msgNum ="alert('adfdfd');"; 
    response = msgNum + js.statements();
    if(js_af_up.size()>0)
    {
        response+= js_af_up;
        js_af_up="";
    }
    //char  aa[10]={0};
    //OutputDebugString(response.c_str());
    //OutputDebugString("\n");


    //WriteLog("",(char *)response.c_str());
    //  OutputDebugString("\n----------------------------------\n");

    std::cerr << "Responding: " << std::endl
        << ">>>>>>>" << std::endl
        << response << std::endl
        << "<<<<<<<" << std::endl;



	
	FCGI_printf("Content-Type: text/plain;charset=");
	FCGI_printf(m_code.c_str());
	//gb2312
	FCGI_printf("\r\nContent-Language: zh-CN\r\nPragma: no-cache\r\nExpires: Sun, 06 Nov 1994 08:49:37 GMT\r\nSet-Cookie: svsid=%d\r\nSet-Cookie: idcuserid=%s\r\nSet-Cookie: olduserid=%d\r\n\r\n%s\n", GetSessionIdbyCookie(),GetIdcUserIdbyCookie().c_str(), GetUserIdbyCookie(), response.c_str());

    expectLoadEvent_ = false;
}

void WebSession::serveResource(WResource *resource)
{
    if (resource) 
    {
        FCGI_printf("Content-Type: %s\r\n\r\n", resource->resourceMimeType().c_str());

        resource->streamResourceData(FCGI_stdout);
    }
}

void WebSession::generateResponse(WResource *resource)
{
    /*

    FCGI_printf("Content-Type: text/plain\r\nSet-Cookie: svsid=%d&userid=%d;\r\n\r\n%s\n", 
    GetSessionIdbyCookie(),"1", "aaa");	
    cgi_quit();
    FCGI_Finish();

    return;
    */
    if (response_ == RESPONSE_RESOURCE) 
    {
        serveResource(resource);    
    }
    else 
    {
        static std::string GOOD_BYE = "Good bye.";
		if(js_af_up.size()>0)
			GOOD_BYE= "Waiting ......";

        WWidget *mainWidget = 0;
        if (application_) 
        {
            mainWidget = application_->root();
        }
        else
            mainWidget = new WText(GOOD_BYE, 0);

        if (response_ == RESPONSE_EVAL) 
        {
            
            serveJavascriptUpdate(mainWidget);
        }
        else if (response_ == RESPONSE_EVAL_LOAD) 
        {
            if (expectLoadEvent_)
            {
                serveJavascriptUpdate(mainWidget);
                /* FCGI_printf("Content-Type: text/plain\r\nSet-Cookie: svsid=%d&userid=%d;\r\n\r\n%s\n", 
                GetSessionIdbyCookie(),"1", "aaa");	
                cgi_quit();
                FCGI_Finish();

                return;
                */
            }
            else
            {
                /*
                * Apparently the browser used its local copy of the html,
                * thus it is out of sync with our widget tree: We force
                * a reload. This fixes the forward/backward button behaviour
                * when returning to the Wt application.
                */
                std::string msgNum = cgi_getentrystr("msg", 0);
                if (msgNum.size() != (unsigned)MESSAGE_COUNTER_SIZE)
                    msgNum.insert(msgNum.size(), MESSAGE_COUNTER_SIZE - msgNum.size(), ' ');

                FCGI_printf("Content-Type: text/plain\r\nPragma: no-cache\r\nExpires: Sun, 06 Nov 1994 08:49:37 GMT\r\nSet-Cookie: svsid=%d\r\nSet-Cookie: idcuserid=%s\r\nSet-Cookie: olduserid=%d\r\n\r\n%sdocument.location.reload();\n", 
                    GetSessionIdbyCookie(),GetIdcUserIdbyCookie().c_str(),GetUserIdbyCookie(), msgNum.c_str());
            }
        }
        else
        {

            if (application_)
            {
                
                serveMainpage(mainWidget);
            }
            else
                serveResource(mainWidget);
        }
        //return;

        setCurrentWidget(mainWidget);
    }

    FCGI_Finish();
    cgi_quit();

}

void WebSession::setCurrentWidget(WWidget *w)
{
    currentMainWidget_ = w;
    currentElementName_ = currentMainWidget_->formName();
}



//****************************start cgiutil******************************************
char * cgi_error_strings[CGIERR_NUM_ERRS] = {
    "", "Not an integer", "Not a double", "Not a boolean",
        "Unknown method", "Incorrect Content Type",
        "NULL Query String", "Bad Content Length",
        "Content Length Discrepancy", "No Cookies", "Cookie Not Found",
        "Not That Many"
};


/* Debugging.  If set, this library will attempt to append debugging
information to a file named "debug.txt" in the current directory. */

/* #define DEBUG */


/* Converts hexadecimal to decimal (character): */

char x2c(char *what)
{
    char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
    return (digit);
}


/* Unescapes "%"-escaped characters in a query: */

#ifdef WIN32
char* UnicodeToGB(const wchar_t* szUnicodeString)
{
    UINT nCodePage = 936; //GB2312
    int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);
    char* pBuffer=new char[nLength+1];
    memset(pBuffer, '\0', nLength + 1);
    WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);
    pBuffer[nLength]=0;
    return pBuffer;
}
#endif

void unescape_url(char *url)
{
    int x,y,len;
    //char * pUrl = url;
    len = strlen(url);
    wchar_t pTmp[2] = {0};
    for (x=0, y=0; url[y]; ++x, ++y)
    {
        if ((url[x] = url[y]) == '%'
            && y < len - 2)   /* 2.0.4 - MJ Pomraning (pilcrow@mailbag.com) */
        {
            if(url[y+1] == 'u')
            {     
                sscanf(url + y + 2, "%4x", &pTmp[0]);
                char *pTemp = UnicodeToGB(pTmp);
                if(pTemp)
                {
                    strncpy(url + x, pTemp, 2);
                    delete []pTemp;
                }
                x += 1;
                y += 5;
            }
            else
            {
                url[x] = x2c(&url[y+1]);
                y+=2;
            }
        }
    }
    url[x] = '\0'; 
}


/* Converts pluses back to spaces in a query: */

void plustospace(char *str)
{
  int x;
  
  for (x=0; str[x]; x++)
    if (str[x] == '+')
      str[x] = ' ';
}


/* Internal use: Write debugging stuff to a file */

void debug(char * str1, char * str2)
{
#ifdef DEBUG
  FILE * fi;
  
  fi = fopen("/tmp/cgi-debug.txt", "a");
  if (fi != NULL)
    {
      fprintf(fi, "%s:%s\n", str1, str2);
      fclose(fi);
    }
#endif
}


/*
 * Internal use: Read a line and return its length:
 * FIXME: We should in fact support reading of binary files, but how
 * do we know the end of the file ?
 */
int lineread(FILE * stream, char * buf, int count)
{
  char * c;

  c = fgets(buf, count, stream);
 
  if (c != NULL)
    return(strlen(buf));
  else
    return(0);
}

static char *getValue(const char *cgi_query, const char *field)
{
  if (strstr(cgi_query, field) != NULL) {
    char *result =(char*)malloc(strlen(strstr(cgi_query, field)) -
			  strlen(field) + 1);
    strcpy(result, strstr(cgi_query, field) + strlen(field));

    /* Truncate after quote: */
  
    if (strchr(result, '\"') != NULL) {
      strcpy(strchr(result, '\"'), "\0");
    }

    return result;
  } else
    return NULL;
}

/* Initialize the CGI.  Grab data from the browser and prepare it for us. */

int WebSession::cgi_init(void)
{
  int cl, i, length_gotten, in_name;
//  char * boundary;

  //cgi_quit();
 // WriteLog1("", "cgi_query malloc\n");
  
  /* Default, no errors, no name/value pairs ("entries"): */

  debug("cgi-init", "starting");
  
  cgi_errno = CGIERR_NONE;
  cgi_num_entries = 0;
  length_gotten = 0;
 
  //OutputDebugString(" before cgi_parse_cookies \n");
  cgi_parse_cookies();
 
  
  /* Check for REQUEST_METHOD (set by HTTP server): */
  
  if (GetWtEnv("REQUEST_METHOD") == NULL)
   {
      /* None set?  Assume the user is invoking the CGI from a shell prompt
	 (for debugging): */
      
      cgi_request_method = CGIREQ_NONE;
   }
  else
   {
      /* Determine the exact request method, and grab the data (if any)
	 in the appropriate manner: */
      
 //     if (strcmp(GetWtEnv("REQUEST_METHOD"), "POST") == 0)
	//{
	//  /* Post method (data is sent to us via "stdin"): */
	//  
	//  cgi_request_method = CGIREQ_POST;
	//  
	//  
	//  if (GetWtEnv("CONTENT_TYPE") == NULL)
	//    {
	//      /* Content type is not set! */
	//      
	//      cgi_errno = CGIERR_INCORRECT_TYPE;
	//      cgi_content_type = CGITYPE_UNKNOWN;
	//      
	//      return(cgi_errno);
	//    }
	//  else if (strstr(GetWtEnv("CONTENT_TYPE"),
	//		  "application/x-www-form-urlencoded") == 
 //                  GetWtEnv("CONTENT_TYPE"))
	//    {
	//      cgi_content_type = CGITYPE_APPLICATION_X_WWW_FORM_URLENCODED;
	//      
	//      
	//      /* How much data do we expect? */
	//      
	//      if (GetWtEnv("CONTENT_LENGTH") == NULL ||
	//	  sscanf(GetWtEnv("CONTENT_LENGTH"), "%d", &cl) != 1)
	//	{
	//	  cgi_errno = CGIERR_BAD_CONTENT_LENGTH;
	//	  return(cgi_errno);
	//	}
	//      
	//      
	//      /* Create space for it: */
	//      
	//
	//      cgi_query = (char*)malloc(cl + 1);
	//      /* 2.0.1 - Tadek Orlowski (orlowski@epnet.com) ... "+1" */
	//      
	//      if (cgi_query == NULL)
	//	{
	//	  cgi_errno = CGIERR_OUT_OF_MEMORY;
	//	  return(cgi_errno);
	//	}
	//      
	//      
	//      /* Read it in: */
	//      
	//      fgets(cgi_query, cl + 1, stdin);
	//      OutputDebugString(cgi_query);
	//      
	//      /* Verify that we got as much data as we expected: */
	//      
	//      if (cgi_query == NULL || strlen(cgi_query) != cl)
	//	cgi_errno = CGIERR_CONTENT_LENGTH_DISCREPANCY;
	//    }
	//  else if (strstr(GetWtEnv("CONTENT_TYPE"),
	//		  "multipart/form-data") == GetWtEnv("CONTENT_TYPE"))
	//    {
	//      cgi_content_type = CGITYPE_MULTIPART_FORM_DATA;
	//    
	//	  
	//      cgi_query = (char*)malloc(2050);
	//      if (cgi_query == NULL)
	//	{
	//	  cgi_errno = CGIERR_OUT_OF_MEMORY;
	//	  return(cgi_errno);
	//	}
	//      
	//      
	//      /* Determine the boundary string: */
	//      
	//      if (strstr(GetWtEnv("CONTENT_TYPE"),
	//		 "boundary=") == NULL)
	//	{
	//	  cgi_errno = CGIERR_NO_BOUNDARY;
	//	  return(cgi_errno);
	//	}
	//      
	//      /* boundary = strdup(strstr(GetWtEnv("CONTENT_TYPE"),
	//			       "boundary=") + 9); */

 //             boundary = (char*)malloc(strlen(strstr(GetWtEnv("CONTENT_TYPE"),
 //                               "boundary=")) - strlen("boundary=") + 1);

 //             strcpy(boundary,
 //                    strstr(GetWtEnv("CONTENT_TYPE"), "boundary=") +
 //                    strlen("boundary="));
	//      
	//      debug("boundary", boundary);
	//      
	//      
	//      /* Read in until there's no more: */
	//      
	//      in_multipart_headers = 0;
	//      which_entry = -1;
	//      
	//      do
	//	{
	//	  length_gotten = lineread(stdin, cgi_query, 2048);
	//	  
	//	  debug("cgi_query", cgi_query);
	//	  
	//	  if (length_gotten > 0)
	//	    {
	//	      if (strstr(cgi_query, boundary) == cgi_query + 2 &&
	//		  cgi_query[0] == '-' && cgi_query[1] == '-')
	//		{
	//		  /* We got a boundary! */
	//		  	      
	//		  if (which_entry != -1) {
	//		    /* Remove trailing newline */
	//		    if (cgi_entries[which_entry].content_length >= 1)
	//		      cgi_entries[which_entry].val
	//			[cgi_entries[which_entry].content_length - 2] = '\0';
	//		  }

	//		  in_multipart_headers = 1;
	//		  which_entry = -1;
	//		}
	//	      else /* (Not a boundary) */
	//		{
	//		  if (in_multipart_headers == 1)
	//		    {
	//		      /* We had just got a boundary, read headers: */
	//		      
	//		      if (cgi_query[0] == '\r' || cgi_query[0] == '\n')
	//			{
	//			  /* Blank line, end of headers: */
	//			  
	//			  in_multipart_headers = 0;
	//			}
	//		      else /* (Not a blank line) */
	//			{
	//			  /* What kind of header is it? */
	//			  
	//			  if (strstr(cgi_query,  "Content-Disposition: ") ==  cgi_query)
	//			    {
	//			      /* Content-disposition: */
	//			      
	//			      /* For now, just look for "name=" */
	//			      
	//			      if (strstr(cgi_query, "name=\"") != NULL)
	//				{
	//				  /* Add a new entry: */
	//				  
	//				  which_entry = cgi_num_entries;
	//				  cgi_num_entries++;
	//				  
	//				  
	//				  /* Make more room: */
	//				  

	//				  cgi_entries =
	//				    (struct entry_type*)realloc(cgi_entries,
	//					    sizeof(cgi_entry_type) *
	//					    cgi_num_entries);
	//				  
	//				  if (cgi_entries == NULL)
	//				    {
	//				      cgi_errno = CGIERR_OUT_OF_MEMORY;
	//				      return(cgi_errno);
	//				    }
	//				  
	//				  
	//				  /* Fill in the name slot: */
	//				  
	//				  cgi_entries[which_entry].need_free=1;
	//				  cgi_entries[which_entry].name =
	//				    getValue(cgi_query, "name=\"");

	//				  cgi_entries[which_entry].file_name =
	//				    getValue(cgi_query, "filename=\"");

	//				  /* Set default content-type: */
	//				  
	//				  cgi_entries[which_entry].
	//				    content_type =
	//				    "application/octet-stream";
	//				  
	//				  
	//				  /* Set default content-length: */
	//				  
	//				  cgi_entries[which_entry].
	//				    content_length = 0;
	//				  
	//				  
	//				  /* Set default value: */
	//				  
	//				  cgi_entries[which_entry].val =
	//				    (char*)malloc(1);
	//				  
	//				  cgi_entries[which_entry].val[0] =
	//				    '\0';
	//				  
	//				  
	//				  debug("entry.name",
	//					cgi_entries[which_entry].name);

	//				}
	//			    }
	//			  else if (strstr(cgi_query,
	//					  "Content-Type: ") ==
	//				   cgi_query)
	//			    {
	//			      /* Content-type: */
	//			      
	//			      cgi_entries[which_entry].content_type =
	//				(char*)malloc(strlen(strstr(cgi_query,
	//						     "Content-Type: ")) - strlen("Content-Type: ") + 1);
	//			      cgi_entries[which_entry].content_type =
	//				strcpy(cgi_entries[which_entry].content_type,
	//				       strstr(cgi_query,
	//					      "Content-Type: ") +
	//				       strlen("Content-Type: "));
	//			      

	//				/* strdup(strstr(cgi_query,
	//				   "Content-Type: ") +
	//				   14); */
	//			      
	//			      debug("entry.content_type",
	//				    cgi_entries[which_entry].
	//				    content_type);
	//			    }
	//			}
	//		    }
	//		  else /* in_multipart_headers == 0 */
	//		    {
	//		      /* If we're recording into a particular
	//			 entry, copy the data: */
	//		      
	//		      if (which_entry != -1)
	//			{
	//			  /* Make more room: */
	//			  
	//			  cgi_entries[which_entry].val =
	//			    (char*)realloc(cgi_entries[which_entry].val,
	//				    cgi_entries[which_entry].
	//				    content_length + length_gotten + 1);
	//			  
	//			  if (cgi_entries[which_entry].val == NULL)
	//			    {
	//			      cgi_errno = CGIERR_OUT_OF_MEMORY;
	//			      return(cgi_errno);
	//			    }
	//			  
	//			  
	//			  /* Append the data: */
	//			  
	//			  memcpy(cgi_entries[which_entry].val +
	//				 (cgi_entries[which_entry].
	//				  content_length),
	//				 cgi_query, length_gotten);

	//			  cgi_entries[which_entry].content_length =
	//			    (cgi_entries[which_entry].content_length +
	//			     length_gotten);
	//			  cgi_entries[which_entry].val
	//			    [cgi_entries[which_entry].content_length]
	//			    = '\0';
	//			}
	//		    }
	//		}
	//	    }
	//	}
	//      while (length_gotten > 0);
	//	if(cgi_query != NULL)
	//	{
	//	
	//      free(cgi_query);
	//      cgi_query = NULL;
	//	}
	//    }
	//  else
	//    {
	//      /* Content type is unrecognized! */
	//      
	//      cgi_errno = CGIERR_INCORRECT_TYPE;
	//      cgi_content_type = CGITYPE_UNKNOWN;
	//      
	//      return(cgi_errno);
	//    }
	//}
      if (strcmp(GetWtEnv("REQUEST_METHOD"), "POST") == 0)
	{
	  /* For now, assume Content Type of
	     "application/x-www-form-urlencoded"
	     (Is this a bad assumption?) */
	  char * buf_tmp = NULL;
	  
	  cgi_content_type = CGITYPE_APPLICATION_X_WWW_FORM_URLENCODED;
	  
	  
	  /* GET method (data sent via "QUERY_STRING" env. variable): */
	  
	  cgi_request_method = CGIREQ_GET;
	  
	  
	  /* Get a pointer to the data: */
	 
	  char *buf_tmp_query = GetWtEnv("QUERY_STRING");
	  string strPost;
	  strPost=FuncGetDataByPostMethod();

	  //OutputDebugString("\nPost Data is    ");

	  //OutputDebugString(strPost.c_str());

	  //OutputDebugString("\n");
	  buf_tmp =NULL;
	  if(buf_tmp_query!=NULL)
	  {
			buf_tmp=(char *)malloc(strPost.size()+ strlen(buf_tmp_query)+2) ;
			memset(buf_tmp, 0,strPost.size()+ strlen(buf_tmp_query)+2);
		   if (buf_tmp== NULL)
			{
				cgi_errno = CGIERR_OUT_OF_MEMORY;
				return(cgi_errno);
			}

			if(strPost.size()>0)
				sprintf(buf_tmp,"%s&%s" ,buf_tmp_query ,strPost.c_str()  );
			else
				sprintf(buf_tmp,"%s" ,buf_tmp_query  );
	  }
	  else
	  {

		    if(strPost.size()>0)
			{
				buf_tmp=(char *)malloc(strPost.size()+1) ;
				memset(buf_tmp,0,strPost.size()+1);
				if (buf_tmp== NULL)
				{
					cgi_errno = CGIERR_OUT_OF_MEMORY;
					return(cgi_errno);
				}

				sprintf(buf_tmp,"%s",strPost.c_str());
				
			}
	  }

	  
//	  if(buf_tmp == NULL)
	  
	  if(buf_tmp==NULL)
	    {
	      /* Does the "QUERY_STRING" env. variable not exist!? */
	      
	      /* cgi_errno = CGIERR_NULL_QUERY_STRING; */
	      
	      /* return(cgi_errno); */
	      
	      cl = 0;
	      cgi_query = (char*)malloc(sizeof(char));
	      if (cgi_query == NULL)
	      {
		cgi_errno = CGIERR_OUT_OF_MEMORY;
		return(cgi_errno);
	      }
	      cgi_query[0] = '\0';
	    }
	  else
	    {
	      /* Determine the content length by seeing how big the
		 string is: */
	      
	      cl = strlen(buf_tmp);

	      cgi_query = (char*)malloc(sizeof(char) * (cl + 1));
	      if (cgi_query == NULL)
	      {
				cgi_errno = CGIERR_OUT_OF_MEMORY;
				return(cgi_errno);
	      }
 //           WriteLogFile("c:\\querystring.log", buf_tmp);
	      memcpy(cgi_query, buf_tmp, cl);
	      cgi_query[cl] = '\0';
		  free(buf_tmp);
	    }
	}
      else
	{
	  /* Something else? We can't handle it! */
	  
	  cgi_request_method = CGIREQ_UNKNOWN;
	  cgi_errno = CGIERR_UNKNOWN_METHOD;
	  cgi_num_entries = 0;
	  
	  return(cgi_errno);
	}      
      
      
      if (cgi_content_type != CGITYPE_MULTIPART_FORM_DATA)
	{
	  /* How many entries (name/value pairs) do we need to
	     allocate space for? (They should be separated by "&"'s) */
	  
	  cgi_num_entries = 0;
	  
      
	  for (i = 0; i <= cl; i++)
	    if (cgi_query[i] == '&' || cgi_query[i] == '\0')
	      cgi_num_entries++;
	  
	  
	  /* Allocate the space for that many structures: */

	  cgi_entries = (struct entry_type*)malloc(sizeof(cgi_entry_type) * cgi_num_entries);
	  if (cgi_entries == NULL)
	    {
	      cgi_errno = CGIERR_OUT_OF_MEMORY;
	      return(cgi_errno);
	    }
	  
	  
	  /* Grab each name/value pair: */
	  
	  cgi_num_entries = 0;
	  
	  
	  /* (Begin with the first half of the first pair): */
	  
	  if (cgi_query[0] != '\0' && cgi_query[0] != '&')
	    {
	      cgi_entries[0].need_free = 0;
	      cgi_entries[0].name = cgi_query;
	      cgi_entries[0].content_type = "text/html";
	      cgi_entries[0].file_name = NULL;
	    }
	  
	  
	  /* Go through the entire string of characters: */
	  
	  in_name = 1;
	  for (i = 0; i <= cl; i++)
	    {
	      if (cgi_query[i] == '&')
		{
		  /* "&" represents the end of a name/value pair: */
		  
		  cgi_entries[cgi_num_entries].need_free = 0;
		  cgi_entries[cgi_num_entries].name = cgi_query + i + 1;
		  cgi_entries[cgi_num_entries].content_type = "text/html";
		  cgi_entries[cgi_num_entries].file_name = NULL;
		  cgi_query[i] = '\0';
                  in_name = 1;
		}
	      else if (cgi_query[i] == '=' && in_name)
		{
		  /* "=" is the end of the name half of a name/value pair: */
		  
		  cgi_entries[cgi_num_entries].val = cgi_query + i + 1;
		  
		  /*  plustospace(cgi_entries[cgi_num_entries].val);
		      unescape_url(cgi_entries[cgi_num_entries].val); */
		  
		  cgi_num_entries++;
		  in_name = 0;
		  
		  cgi_query[i] = '\0';
		}
	    }
	  
	  for (i = 0; i < cgi_num_entries; i++)
	    {
	      plustospace(cgi_entries[i].val);
	      unescape_url(cgi_entries[i].val);
		  //std::wstring strw=  boost::from_utf8(cgi_entries[i].val);
		  ////char* aaa[2000];
		  //OutputDebugString(strw);
		  
	      cgi_entries[i].content_length = strlen(cgi_entries[i].val);
	    }
	  
	}
      
      
      /* Fix any NULL strings to be empty strings */
      /* 2.0.4 - MJ Pomraning (pilcrow@mailbag.com) */
      
      for (i = 0; i < cgi_num_entries; i++)
	{
	  if (cgi_entries[i].name == NULL)
	    cgi_entries[i].name = "";
	  if (cgi_entries[i].val == NULL) {
	    cgi_entries[i].val = "";
	    cgi_entries[i].content_length = 0;
	  }
	}
    }
  
  return(CGIERR_NONE);
}


/* Free up memory that was allocated when we called "cgi_init()": */

void WebSession::cgi_quit1(void)
{
//	return ;

	if (cgi_query != NULL) {
			
			free(cgi_query);
			cgi_query = NULL;
	
	}
	


  if (cgi_entries != NULL) {
    int i;
    for (i = 0; i < cgi_num_entries; ++i) {
      if (cgi_entries[i].need_free) {
	if (cgi_entries[i].name != NULL)
	  free(cgi_entries[i].name);
	if (cgi_entries[i].file_name != NULL)
	  free(cgi_entries[i].file_name);
	if (cgi_entries[i].val != NULL)
	  free(cgi_entries[i].val);
      }
    }
	

    free(cgi_entries);

    cgi_entries = NULL;
  }

  if (cgi_cookies != NULL) {
	  for(int i =0;i< cgi_num_cookies;i++)
	  {
		  if(cgi_cookies[i].name!=NULL)
		  {
			  free(cgi_cookies[i].name);
			  cgi_cookies[i].name =NULL;
		  }
		  if(cgi_cookies[i].val!=NULL)
		  {
			  free(cgi_cookies[i].val);
			  cgi_cookies[i].name =NULL;
		  }
		  
	  }
		  
		  
    free(cgi_cookies);
    cgi_cookies = NULL;
  }
	
  cgi_errno = CGIERR_NONE;
  cgi_request_method = CGIREQ_NONE;
  cgi_content_type = CGITYPE_NONE;
  
}


void WebSession::cgi_quit(void)
{
    if (cgi_query != NULL)
    {			
        free(cgi_query);
        cgi_query = NULL;
    }

    if (cgi_entries != NULL) 
    {
        int i;
        int nCount = cgi_num_entries;
        for (i = 0; i < nCount; ++i) 
        {
            if (cgi_entries[i].need_free) 
            {
                if (cgi_entries[i].name != NULL)
	                free(cgi_entries[i].name);
                if (cgi_entries[i].file_name != NULL)
	                free(cgi_entries[i].file_name);
                if (cgi_entries[i].val != NULL)
	                free(cgi_entries[i].val);
                cgi_num_entries --;
            }
        }

        free(cgi_entries);
        cgi_entries = NULL;
    }

    if (cgi_cookies != NULL) 
    {
        for(int i =0;i< cgi_num_cookies;i++)
        {
            if(cgi_cookies[i].name!=NULL)
            {
	            free(cgi_cookies[i].name);
	            cgi_cookies[i].name =NULL;
            }
            if(cgi_cookies[i].val!=NULL)
            {
	            free(cgi_cookies[i].val);
	            cgi_cookies[i].name =NULL;
            }
        }
        cgi_num_cookies = 0;
        free(cgi_cookies);
        cgi_cookies = NULL;
    }

    cgi_errno = CGIERR_NONE;
    cgi_request_method = CGIREQ_NONE;
    cgi_content_type = CGITYPE_NONE;  
}



/* Parse cookies. Return number found. Internal use only, should be
   called from cgi_init() */

#define MAX_COOKIE_STUFF 1024

int WebSession::cgi_parse_cookies()
{
  char *cookies;
  char *cookie;
  char *name, *value;
  char sscanf_string[1024]; /* Gonna be a bad day if we overflow this */
  int num_cookies;
  cgi_cookie_type * tmp_cookie;

  //OutputDebugString("in cgi_parse_cookies \n");
  
  if(!(cookies = GetWtEnv("HTTP_COOKIE")))
    {
      /* no cookies */
      OutputDebugString("no cookie \n");
      cgi_errno = CGIERR_NO_COOKIES;
      return 0;
    }
	
	//OutputDebugString(cookies);
	//OutputDebugString("---------------\n");
  
  
  memset((void *)sscanf_string,0,sizeof(sscanf_string));
  sprintf(sscanf_string, " %%%i[^=]=%%%i[^;]",
	  MAX_COOKIE_STUFF,MAX_COOKIE_STUFF);
  
  num_cookies = 0;
  cookie = cookies;
  do
    {
      /* If it's not our first cookie, the strstr from the while will
	 leave us with a ';' at the start of cookie */
      
      if(num_cookies != 0) cookie++;
      num_cookies++;
      name = (char*)malloc((size_t)MAX_COOKIE_STUFF);
      value = (char*)malloc((size_t)MAX_COOKIE_STUFF);
      if(sscanf(cookie,sscanf_string,name,value) != 2)
	{
	  free(name);
	  free(value);
	  continue;
	}
      tmp_cookie = (struct cookie_type*)realloc((void *)cgi_cookies,
			   sizeof(cgi_cookie_type) * num_cookies);
      if(tmp_cookie != NULL)
	{
	  cgi_cookies = tmp_cookie;
	}
      else
	{
	  /* Ran out of memory, but we can put in a best effort anyway */
	  
	  cgi_num_cookies = num_cookies;
	  return num_cookies;
	}
	//char aaaa[2000]={0};
	//sprintf(aaaa,"%s =%s\n",name,value);
	//OutputDebugString(aaaa);
      
      cgi_cookies[num_cookies - 1].name = name;
      cgi_cookies[num_cookies - 1].val = value;
    }
  while((cookie = strstr(cookie, ";")) != NULL);
  
  cgi_num_cookies = num_cookies;
  return num_cookies;
}


/* Grab a cookie, if it exists.  Return NULL if it doesn't: */
/* (Based on code by Pete Cassidy (pcassidy@iol.ie) - May 10, 2000) */
/* (Restructured by Chris Wareham (chris.wareham@catchword.com) -
    Nov 1, 2000) */


const char *WebSession::cgi_getcookie(const char *cookie_name)
{
  int i;

  for (i = 0; i < cgi_num_cookies; i++)
    {
      if(strcmp(cgi_cookies[i].name,cookie_name) == 0)
	{
	  cgi_errno = CGIERR_NONE;

	  return cgi_cookies[i].val;
	}
    }

  cgi_errno = CGIERR_COOKIE_NOT_FOUND;
  
  return(NULL);
}


#ifdef SKIP_JUNK

/* ------------------------------------------------------------------------- */
/* THIS IS THE OLD cgi_getcookie(), AND IS CURRENTLY DISABLED */

const char * WebSession::cgi_getcookie(const char * cookie_name)
{
  char * cookieval, * tmpcookie, * rawcookie, * left, * right;
  int done;


  /* Get raw cookie data: */

  rawcookie = GetWtEnv("HTTP_COOKIE");
  if (rawcookie == NULL)
    {
      /* No cookies at all?  The cookie we want can't exist! */

      cgi_errno = CGIERR_NO_COOKIES;
      return(NULL);
    }


  /* Strtok is destructive, so make a temporary copy of the raw cookie data: */

  tmpcookie = malloc(sizeof(char) * (strlen(rawcookie) + 1));
  if (tmpcookie == NULL)
    {
      cgi_errno = CGIERR_OUT_OF_MEMORY;
      return(NULL);
    }

  strcpy(tmpcookie, rawcookie);


  /* Tokenize out all cookies and check for the one we're looking for: */

  left = strtok(tmpcookie, ";");
  cookieval = NULL;
  done = 0;

  do
    {
      /* Grab the righthand size of the current cookie pair's "=" sign: */

      right = strchr(left, '=') + (1 * sizeof(char));


      /* Change the "=" into a NULL character, to get the lefthand side: */

      *strchr(left, '=') = '\0';


      /* See if this is our cookie: */

      if (strcmp(left, cookie_name) == 0)
	{
          /* If so, set our return-string to the value (righthand side): */

          cookieval = malloc(sizeof(char) * (strlen(right) + 1));
          if (cookieval == NULL)
            {
              cgi_errno = CGIERR_OUT_OF_MEMORY;
              return(NULL);
            }

          strcpy(cookieval, right);
          done = 1;
	}


      /* Jump to next cookie: */

      if (!done)
	{
          left = strtok(NULL, ";");
          if (left == NULL)
            {
              /* No more to parse? */

              done = 1;
            }
          else
            {
              /* Skip the extra space: */

              left++;
            }
	}
    }
  while (!done);


  /* Free the temporary copy of the raw cookie data: */

  free(tmpcookie);


  /* Return the cookie value (which may be NULL if we never found it): */

  if (cookieval == NULL)
    cgi_errno = CGIERR_COOKIE_NOT_FOUND;
  else
    cgi_errno = CGIERR_NONE;

  return(cookieval);
}

/* ------------------------------------------------------------------------- */

#endif


/* Count number of entries named 'field_name': */
/* Stephen Woodbridge <woodbri@swoodbridge.com> */

int WebSession::cgi_getnumentries(const char *field_name)
{
  int cnt, i;
  
  cnt = 0;
 // char aa[200];
//  sprintf( aa,"cgi_num_entries is %d, field is %s\n", cgi_num_entries,field_name);
 // OutputDebugString(aa);
  if(cgi_entries)
  {
    for (i = 0; i < cgi_num_entries; i++)
        {
        if (strcmp(cgi_entries[i].name, field_name) == 0)
	        cnt++;
        }
  }
  return cnt;
}



/* Grab a value and return it as a string: */

const char * WebSession::cgi_getentrystr(const char *field_name, int *length)
{
  return cgi_getnentrystr(field_name, 0, length);
}


/* Grab a value and return it as a string: */

const char * WebSession::cgi_getnentrystr(const char *field_name, int n, int *length)
{
  int x;
  int cnt;
  
  cnt = 0;
  
  if (n < 0 || n > cgi_getnumentries(field_name))
  {

      cgi_errno = CGIERR_N_OUT_OF_BOUNDS;
      return(NULL);
  }

  if(cgi_entries == NULL)
  {


	  return NULL;
  }
  
  if (cgi_request_method != CGIREQ_NONE)
  {
      /* Look for the name: */
      
      for (x = 0; x < cgi_num_entries; x++)
	  {
		if (strcmp(cgi_entries[x].name, field_name) == 0)
	    {
	      if (cnt == n) 
		  {
			if (length)
				*length = cgi_entries[x].content_length;
			return (cgi_entries[x].val);
	      }

	      cnt++;
	    }
	}
      
      return(NULL);
  }
  else
  {
      /* printf("CGI-UTIL: \"%s\" ? ", field_name);
      fgets(buf, 512, stdin);
      buf[strlen(buf) - 1] = '\0'; */
      
      return(NULL);
  }
}



/* Grab a content-type and return it as a string: */

const char * WebSession::cgi_getentrytype(const char *field_name)
{
  return cgi_getnentrytype(field_name, 0);
}


/* Grab a content-type and return it as a string: */

const char * WebSession::cgi_getnentrytype(const char *field_name, int n)
{
  int x;
  int cnt;
  
  cnt = 0;


  if (n < 0 || n > cgi_getnumentries(field_name))
    {
      cgi_errno = CGIERR_N_OUT_OF_BOUNDS;
      return(NULL);
    }
  
  
  if (cgi_request_method != CGIREQ_NONE)
    {
      /* Look for the name: */
      
      for (x = 0; x < cgi_num_entries; x++)
	{
	  if (strcmp(cgi_entries[x].name, field_name) == 0)
	    {
	      if (cnt == n)
		return (cgi_entries[x].content_type);
	      
	      cnt++;
	    }
	}
      
      return(NULL);
    }
  else
    return(NULL);
}


/* Grab a content-type and return it as a string: */

const char * WebSession::cgi_getentryfilename(const char *field_name)
{
  return cgi_getnentryfilename(field_name, 0);
}


/* Grab a content-type and return it as a string: */

const char * WebSession::cgi_getnentryfilename(const char *field_name, int n)
{
  int x;
  int cnt;
  
  cnt = 0;


  if (n < 0 || n > cgi_getnumentries(field_name))
    {
      cgi_errno = CGIERR_N_OUT_OF_BOUNDS;
      return(NULL);
    }
  
  
  if (cgi_request_method != CGIREQ_NONE)
    {
      /* Look for the name: */
      
      for (x = 0; x < cgi_num_entries; x++)
	{
	  if (strcmp(cgi_entries[x].name, field_name) == 0)
	    {
	      if (cnt == n)
		return (cgi_entries[x].file_name);
	      
	      cnt++;
	    }
	}
      
      return(NULL);
    }
  else
    return(NULL);
}

/* Grab a value and return it as an integer: */

int WebSession::cgi_getentryint(const char *field_name)
{
  return cgi_getnentryint(field_name, 0);
}


/* Grab a value and return it as an integer: */

int WebSession::cgi_getnentryint(const char *field_name, int n)
{
  int v;
  
  v = 0;
  
  if (cgi_getentrystr(field_name, 0) != NULL)
    {
      if (sscanf(cgi_getnentrystr(field_name, n, 0), "%d", &v) != 1)
	cgi_errno = CGIERR_NOT_INTEGER;
    }
  else
    cgi_errno = CGIERR_NOT_INTEGER;
  
  return(v);
}


/* Grab a value and return it as a double: */

double WebSession::cgi_getentrydouble(const char *field_name)
{
  return cgi_getnentrydouble(field_name, 0);
}


/* Grab a value and return it as a double: */

double WebSession::cgi_getnentrydouble(const char *field_name, int n)
{
  double v;
  
  v = 0;
  
  if (cgi_getentrystr(field_name, 0) != NULL)
    {
      if (sscanf(cgi_getnentrystr(field_name, n, 0), "%lf", &v) != 1)
	cgi_errno = CGIERR_NOT_DOUBLE;
    }
  else
    cgi_errno = CGIERR_NOT_DOUBLE;
  
  return(v);
}


/* Grab a value and return it as a boolean (depending on if the
   value was "yes", "on" or "true", or "no", "off" or "false"): */

int WebSession::cgi_getentrybool(const char *field_name, int def)
{
  return cgi_getnentrybool(field_name, def, 0);
}


/* Grab a value and return it as a boolean (depending on if the
   value was "yes", "on" or "true", or "no", "off" or "false"): */

int WebSession::cgi_getnentrybool(const char *field_name, int def, int n)
{
  const char * temp;
  int v, cnt;
  
  
  cnt = 0;
  

  if (n < 0 || n > cgi_getnumentries(field_name))
    {
      cgi_errno = CGIERR_N_OUT_OF_BOUNDS;
      return(def);
    }
  
  
  /* Assume the default: */
  
  v = def;
  
  
  /* Get the value (if any): */
  
  temp = cgi_getnentrystr(field_name, n, 0);
  
  if (temp != NULL)
    {
      if (temp[0] == 'y' || temp[0] == 'Y' || /* Yes */
	  temp[1] == 'n' || temp[1] == 'N' || /* On */
	  temp[0] == 't' || temp[0] == 'T')   /* True */
	{
	  /* A "yes" or "on" is a 1: */
	  
	  v = 1;
	}
      else if (temp[0] == 'n' || temp[0] == 'N' || /* Yes */
	       temp[1] == 'f' || temp[1] == 'F' || /* Off */
	       temp[0] == 'f' || temp[0] == 'F')   /* False */
	{
	  /* A "no" or "off" is a 0: */
	  
	  v = 0;
	}
      else if (temp[0] != 0)
	{
	  /* We got something, but not "yes", "on", "no" or "off": */
	  
	  cgi_errno = CGIERR_NOT_BOOL;
	}
    }
  else
    cgi_errno = CGIERR_NOT_BOOL;
  
  
  return(v);
}


/* Open a file and send it to "stdout" (the browser): */
/* (Returns an error if we can't open the file) */

#define CGI_BUFLEN 1024

int WebSession::cgi_dump_no_abort(const char * filename)
{
  FILE * fi;
  char * c;
  char buf[CGI_BUFLEN + 1];
  
  
  cgi_errno = CGIERR_NONE;
  
  
  /* Open the file: */
  
  fi = fopen(filename, "r");
  if (fi == NULL)
    cgi_errno = CGIERR_CANT_OPEN;
  else
    {  
      /* Read data and push it to "stdout": */
      
      do
	{
	  c = fgets(buf, CGI_BUFLEN, fi);
	  if (c != NULL)
	    fputs(c, stdout);
	}
      while (!feof(fi));
      
      fclose(fi);
    }
  
  return(cgi_errno);
}


/* Open a file and send it to "stdout" (the browser): */
/* (Displays an error message and quits the CGI if we can't open the file) */

void WebSession::cgi_dump(const char * filename)
{
  if (cgi_dump_no_abort(filename) != CGIERR_NONE)
    {
      printf("Can't open %s - %s\n", filename, strerror(errno));
      exit(0);
    }
}


/* Display a simple error message and quit the CGI: */

void WebSession::cgi_error(const char * reason)
{
  printf("<h1>Error</h1>\n");
  printf("%s\n", reason);
  
  exit(0);
}


/* Returns whether or not an e-mail address appears to be in the correct
   syntax ("username@host.domain"): */

int WebSession::cgi_goodemailaddress(const char * addr)
{
  int i;
  
  
  /* No "@".. what? */
  
  if (strchr(addr, '@') == NULL)
    return 0;
  
  
  /* "@" or "." at the end or beginning? */
  
  if (addr[strlen(addr) - 1] == '@' ||
      addr[strlen(addr) - 1] == '.' ||
      addr[0] == '@' || addr[0] == '.')
    return 0;
  
  
  /* No "." after the "@"?  More than one "@"? */
  
  if (strchr(strchr(addr, '@'), '.') == NULL ||
      strchr(strchr(addr, '@') + 1, '@') != NULL)
    return 0;
  
  
  /* Any illegal characters within the string? */
  
  for (i = 0; i < strlen(addr); i++)
    {
      if (isalnum((unsigned char)addr[i]) == 0 &&
	  addr[i] != '.' && addr[i] != '@' && addr[i] != '_' &&
	  addr[i] != '-')
	return(0);
    }
  
  
  /* Must be ok... */
  
  return 1;
}


/* Returns the English string description for a particular cgi-util error
   value: */

const char * WebSession::cgi_strerror(int err)
{
  if (err < 0 || err > CGIERR_NUM_ERRS)
    return("");
  else
    return(cgi_error_strings[err]);
}

char * WebSession::GetWtEnv(const char* pKey)
{
	/*
	int nSize=4095;
	memset(szEnv,0,nSize+1);
	GetEnvironmentVariable( pKey, szEnv,nSize);
	if(strlen(szEnv)==0) return NULL;
	return szEnv;
	*/
	return getenv(pKey);
}

string WebSession::FuncGetDataByPostMethod()
{
	char *cl;
	int sum;
	
	//char aaaa[300]={0};
	cl=GetWtEnv("CONTENT_LENGTH");
	
	
	string strGet = "";	
	//sprintf(aaaa,"length is   %s\n" ,cl);
	//OutputDebugString(aaaa);
	
	if (cl)
	{
		sum = atoi(cl) + 1;
		
		char * buffer = NULL;
		buffer = (char *)malloc(sum);
		
		if(!buffer)
			return strGet;
		memset(buffer, 0, sum);

		//God know how
		fgets(buffer, sum, stdin);
        OutputDebugString("libwt(fgets)");
		OutputDebugString(buffer);
		OutputDebugString("------------\n");
		FCGI_fgets(buffer, sum, FCGI_stdin);
        OutputDebugString("libwt(FCGI_fgets)");
		OutputDebugString(buffer);
		OutputDebugString("------------\n");
		//OutputDebugString(buffer);
		//OutputDebugString("------------\n");

		strGet = buffer;		
		
		if (buffer)
		{
			free(buffer);
			buffer = NULL;
		}
	}

	return strGet;
}