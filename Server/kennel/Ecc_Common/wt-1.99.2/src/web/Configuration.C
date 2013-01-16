/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * All rights reserved.
 */

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

#include "Configuration.h"
#include "cppfileno.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <vector>
#include <iostream>
#include <fstream>
#include "WtRandom.h"
#ifdef WIN32
#include <process.h> // for getpid()
#endif

XERCES_CPP_NAMESPACE_USE

namespace {

using namespace Wt;

class StrX
{
public:
  StrX(const XMLCh* const toTranscode)
  {
    fLocalForm = XMLString::transcode(toTranscode);
  }

  ~StrX()
  {
    XMLString::release(&fLocalForm);
  }

  const char* localForm() const { return fLocalForm; }

private:
  char*   fLocalForm;
};

std::ostream& operator<<(std::ostream& target, const StrX& toDump)
{
  target << toDump.localForm();
  return target;
}

class XStr
{
public:
  XStr(const char* const toTranscode)
  {
    fRemoteForm = XMLString::transcode(toTranscode);
  }

  ~XStr()
  {
    XMLString::release(&fRemoteForm);
  }

  const XMLCh* remoteForm() const { return fRemoteForm; }

private:
  XMLCh*   fRemoteForm;
};

std::ostream& operator<<(std::ostream& target, const XStr& toDump)
{
  target << toDump.remoteForm();
  return target;
}

class DOMTreeErrorReporter : public ErrorHandler
{
public:
  DOMTreeErrorReporter()
    : sawErrors_(false)
  { }

  ~DOMTreeErrorReporter()
  { }

  void warning(const SAXParseException& toCatch)
  {
  }

  void error(const SAXParseException& toCatch)
  {
    sawErrors_ = true;
    std::cerr << "Error at file \"" << StrX(toCatch.getSystemId())
	      << "\", line " << toCatch.getLineNumber()
	      << ", column " << toCatch.getColumnNumber()
	      << "\n   Message: " << StrX(toCatch.getMessage())
	      << std::endl;
  }

  void fatalError(const SAXParseException& toCatch)
  {
    sawErrors_ = true;
    std::cerr << "Fatal error at file \"" << StrX(toCatch.getSystemId())
	      << "\", line " << toCatch.getLineNumber()
	      << ", column " << toCatch.getColumnNumber()
	      << "\n   Message: " << StrX(toCatch.getMessage())
	      << std::endl;
  }

  void resetErrors()
  {
    sawErrors_ = false;
  }

  bool sawErrors() const { return sawErrors_; }

private:
  bool sawErrors_;
};

DOMElement *singleChildElement(DOMElement *element, const char* tagName)
{
  XStr XtagName(tagName);

  DOMNodeList *children
    = element->getElementsByTagName(XtagName.remoteForm());

  if (children->getLength() == 0)
    return 0;
  else if (children->getLength() > 1)
    throw Configuration::Exception("Expected only one child (1).");
  else {
    DOMElement *element = dynamic_cast<DOMElement *>(children->item(0));
    
    if (!element)
      throw Configuration::Exception("Expected a DOMElement");

    return element;
  }
}

bool attributeValue(DOMElement *element, const char *attributeName,
		    std::string& result)
{
  XStr XAttributeName(attributeName);

  DOMAttr *attr = element->getAttributeNode(XAttributeName.remoteForm());

  if (attr) {
    result = std::string(StrX(attr->getValue()).localForm());

    return true;
  } else
    return false;
}

std::string singleChildElementValue(DOMElement *element, const char* tagName)
{
  DOMElement *child = singleChildElement(element, tagName);

  if (!child)
    return std::string();

  DOMNodeList *children = child->getChildNodes();
  if (children->getLength() == 0)
    return std::string();
  else if (children->getLength() > 1)
    throw Configuration::Exception("Expected only one child (2).");
  else {
    DOMText *text = dynamic_cast<DOMText *>(children->item(0));

    if (!text)
      throw Configuration::Exception("Expected a DOMText");

    std::string result = std::string(StrX(text->getData()).localForm());
    boost::trim(result);

    return result;
  }
}

std::vector<DOMElement *>
childElements(DOMElement *element, const char *tagName)
{
  XStr XtagName(tagName);

  DOMNodeList *children
    = element->getElementsByTagName(XtagName.remoteForm());

  std::vector<DOMElement *> result;

  for (unsigned i = 0; i < children->getLength(); ++i)
    result.push_back(dynamic_cast<DOMElement *>(children->item(i)));

  return result;
}

}

namespace Wt {

Configuration::Configuration(int argc, char **argv, WApplication::ApplicationCreator createApplication)
  : createApplication_(createApplication),
    sessionPolicy_(SharedProcess),
    numProcesses_(10),
    maxNumSessions_(100),
    maxRequestSize_(128),
    sessionTracking_(URL),
    reloadIsNewSession_(false),
    sessionTimeout_(600),
    valgrindPath_(""),
    allowDebug_(false),
    runDirectory_(RUNDIR),
    sessionIdLength_(16),
    pid_(getpid())
{
  srand48(getpid());

  {
    std::ifstream test(WT_CONFIG_XML);
    if (!test)
      return;
  }
  
  XMLPlatformUtils::Initialize();
  readConfiguration(WT_CONFIG_XML, argv[0]);
  XMLPlatformUtils::Terminate();
}

void Configuration::readConfiguration(const char *configurationFile,
				      const char *location)
{

  XercesDOMParser parser;
  parser.setLoadExternalDTD(false);

  DOMTreeErrorReporter errReporter;
  parser.setErrorHandler(&errReporter);

  try {
    LocalFileInputSource isis(XStr(configurationFile).remoteForm());
    parser.parse(isis);

    DOMDocument *doc = parser.getDocument();
    DOMElement  *root = doc->getDocumentElement();
    
    std::vector<DOMElement *> applications
      = childElements(root, "application-settings");

    for (unsigned i = 0; i < applications.size(); ++i) {
      DOMElement *app = applications[i];

      std::string appLocation;
      if (!attributeValue(app, "location", appLocation))
	throw Exception("application-settings requires attribute 'location'");

      if (appLocation == "*" || appLocation == location) {
	DOMElement *sess = singleChildElement(app, "session-management");

	if (sess) {
	  DOMElement *dedicated
	    = singleChildElement(sess, "dedicated-process");
	  DOMElement *shared = singleChildElement(sess, "shared-process");
	  std::string tracking
	    = singleChildElementValue(sess, "tracking");
	  std::string reload
	    = singleChildElementValue(sess, "reload-is-new-session");
	  std::string timeoutStr
	    = singleChildElementValue(sess, "timeout");

	  if (dedicated && shared)
	    throw Exception("application-settings requires either "
			    "dedicated-process or shared-process, not both");

	  if (dedicated) {
	    sessionPolicy_ = DedicatedProcess;

	    std::string maxnumStr
	      = singleChildElementValue(dedicated, "max-num-sessions");

	    if (!maxnumStr.empty())
	      maxNumSessions_ = boost::lexical_cast<int>(maxnumStr);
	  }

	  if (shared) {
	    sessionPolicy_ = SharedProcess;

	    std::string numProcessesStr
	      = singleChildElementValue(shared, "num-processes");

	    if (!numProcessesStr.empty())
	      numProcesses_ = boost::lexical_cast<int>(numProcessesStr);
	  }

	  if (!tracking.empty()) {
	    if (tracking == "Auto")
	      sessionTracking_ = CookiesURL;
	    else if (tracking == "URL")
	      sessionTracking_ = URL;
	    else
	      throw Exception("session-tracking: expecting 'Auto' or 'URL'");
	  }

	  if (!timeoutStr.empty())
	    sessionTimeout_ = boost::lexical_cast<int>(timeoutStr);

	  if (singleChildElement(sess, "reload-is-new-session")) {
	    if (reload == "true")
	      reloadIsNewSession_ = true;
	    else if (reload == "false")
	      reloadIsNewSession_ = false;
	    else
	      throw Exception("reload-is-new-session: "
			      "expecting 'true' or 'false'");
	  }
	}

	std::string maxRequestStr
	  = singleChildElementValue(app, "max-request-size");

	if (!maxRequestStr.empty())
	  maxRequestSize_ = boost::lexical_cast<int>(maxRequestStr);

	std::string debug
	  = singleChildElementValue(app, "enable-debug");

	if (!debug.empty())
	  if (debug == "true")
	    allowDebug_ = true;
	  else if (debug == "false")
	    allowDebug_ = false;
	  else
	    throw Exception("enable-debug: expecting 'true' or 'false'");

	std::string valgrind
	  = singleChildElementValue(app, "valgrind-path");

	if (singleChildElement(app, "valgrind-path"))
	  valgrindPath_ = valgrind;

	std::string run
	  = singleChildElementValue(app, "run-directory");

	if (!run.empty())
	  runDirectory_ = run;

	std::string sessionId
	  = singleChildElementValue(app, "session-id-length");

	if (!sessionId.empty())
	  sessionIdLength_ = boost::lexical_cast<int>(sessionId);
      }
    }
  } catch (Exception& e) {
    throw;
  } catch (std::exception& e) {
    throw Exception(std::string("Error: ") + e.what());
  } catch (...) {
    throw Exception("Exception of unknown type!\n");
  }
}

std::string Configuration::generateSessionId() const
{
  std::string session_id = "";
  for (int i = 0; i < sessionIdLength(); ++i) {
    int d = lrand48() % (26 + 26 + 10);

    char c = (d < 10 ? ('0' + d)
	      : (d < 36 ? ('A' + d - 10)
		 : 'a' + d - 36));
    session_id.push_back(c);
  }

  std::string socketPath = runDirectory_ + "/" + session_id;

  struct stat finfo;
  if (stat(socketPath.c_str(), &finfo) != -1) // exists already
    return generateSessionId();
  else {
    if (sessionPolicy_ == SharedProcess) {
      std::ofstream f(socketPath.c_str());
      f << pid_ << std::endl;
      f.flush();
      fsync(fileno(f));

      int dirfd = open(runDirectory_.c_str(), O_RDWR);
      fsync(dirfd);
      close(dirfd);
    }
    return session_id;
  }
}

Configuration::Exception::Exception(const std::string what)
  : what_(what)
{ }

Configuration::Exception::~Exception() throw()
{ }

}
