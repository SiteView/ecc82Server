/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>

#include "WMessageResources"
#include "WMessage"
#include "WApplication"

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

XERCES_CPP_NAMESPACE_USE;

/*
 * On XMLCh versus wchar_t.
 *
 * XMLCh is defined as UTF-16
 *
 * The problem is as follows:
 *  sometimes, wchar_t is:
 *    - UTF-16 (windows): sizeof(wchar_t) == 2;
 *    - UCS-4 (linux/solaris): sizeof(wchar_t) == 4;
 *
 *  On Windows we can simply copy to a wchar_t.
 *
 *  if wchar_t == UCS-4, we should in principle transcode.
 *  Currently, we don't !
 */

namespace {

using namespace Wt;

class WMessageHandler : public DefaultHandler
{
public:
  WMessageHandler(WMessageResources::KeyValueMap& valueMap);

  virtual void characters(const XMLCh *const chars, const unsigned int length);
  virtual void startElement(const XMLCh *const uri,
			    const XMLCh *const localname,
			    const XMLCh *const qname,
			    const Attributes &attrs);
  virtual void endElement(const XMLCh *const uri,
			  const XMLCh *const localname,
			  const XMLCh *const qname);

  virtual void warning(const SAXParseException& exc);
  virtual void error(const SAXParseException& exc);
  virtual void fatalError(const SAXParseException& exc);

private:
  WMessageResources::KeyValueMap& valueMap_;

  int depth_;
  std::string currentKey_;
  std::wstring currentValue_;
  bool inMessage_;

  void report(const SAXParseException& exc, const char *kind);
  void currentValueAppend(const XMLCh *const s, bool attrEscapes);
};

WMessageHandler::WMessageHandler(WMessageResources::KeyValueMap& valueMap)
  : valueMap_(valueMap),
    depth_(0),
    inMessage_(false)
{ }

void WMessageHandler::currentValueAppend(const XMLCh *const s,
					 bool attrEscapes)
{
  for (unsigned i = 0; s[i] != 0; ++i) {
    if (attrEscapes) {
      if (s[i] == chAmpersand) {
	currentValue_ += L"&amp;";
      } else if (s[i] == chOpenAngle) {
	currentValue_ += L"&lt;";
      } else if (s[i] == chCloseAngle) {
	currentValue_ += L"&gt;";
      } else if (s[i] == chSingleQuote) {
	currentValue_ += L"&apos;";
      } else if (s[i] == chDoubleQuote) {
	currentValue_ += L"&quot;";
      } else
	currentValue_.push_back((wchar_t)s[i]);
    } else
      currentValue_.push_back((wchar_t)s[i]);
  }
}

void WMessageHandler::characters(const XMLCh *const chars,
				 const unsigned int length)
{
  if (inMessage_) {
    for (unsigned i = 0; i < length; ++i)
      if (chars[i] == chAmpersand) {
	currentValue_ += L"&amp;";
      } else if (chars[i] == chOpenAngle) {
	currentValue_ += L"&lt;";
      } else if (chars[i] == chCloseAngle) {
	currentValue_ += L"&gt;";
      } else
	currentValue_.push_back((wchar_t)chars[i]);
  }
}

std::wstring toWString(const XMLCh *s)
{
  std::wstring result;

  for (unsigned i = 0; s[i] != 0; ++i)
    result.push_back((wchar_t)s[i]);

  return result;
}

void WMessageHandler::startElement(const XMLCh *const uri,
				   const XMLCh *const localname,
				   const XMLCh *const qname,
				   const Attributes& attributes)
{
  ++depth_;

  if (inMessage_) {
    // The name has to be representable without any escapes

    currentValue_ += L'<';
    currentValueAppend(qname, false);

    unsigned int len = attributes.getLength();
    for (unsigned int index = 0; index < len; index++) {
      //
      //  Again the name has to be completely representable. But the
      //  attribute can have refs and requires the attribute style
      //  escaping.
      //
      currentValue_ += L' ';
      currentValueAppend(attributes.getQName(index), false);
      currentValue_ += L"=\"";
      currentValueAppend(attributes.getValue(index), true);
      currentValue_ += L'"';
    }

    currentValue_ += L'>';
  } else {
    if (depth_ == 2) {
      std::wstring name = toWString(qname);
      if (name == L"message") {
	inMessage_ = true;

	unsigned int len = attributes.getLength();

	bool haveKey = false;
	for (unsigned int index = 0; index < len; index++) {
	  std::wstring aname = toWString(attributes.getQName(index));
	  if (aname == L"id") {
	    const char *value 
	      = XMLString::transcode(attributes.getValue(index));

	    currentKey_ = value;

	    XMLString::release((char **) &value);

	    haveKey = true;
	  }

	  if (haveKey)
	    break;
	}

	if (!haveKey) {
	  std::cerr << "Missing id for message!" << std::endl;
	}
      }
    }
  }
}

void WMessageHandler::endElement(const XMLCh *const uri,
				 const XMLCh *const localname,
				 const XMLCh *const qname)
{
  if (depth_ == 2) {
    std::wstring name = toWString(qname);
    if (name == L"message") {
      inMessage_ = false;

      valueMap_[currentKey_] = currentValue_;
      currentValue_.clear();
    }
  }

  if (inMessage_) {
    // No escapes are legal here
    currentValue_ += L"</";
    currentValueAppend(qname, false);
    currentValue_ += L'>';
  }

  --depth_;
}

void WMessageHandler::warning(const SAXParseException& e)
{ 
  report(e, "Warning");
}

void WMessageHandler::error(const SAXParseException& e)
{
  report(e, "Error");
}

void WMessageHandler::fatalError(const SAXParseException& e)
{
  report(e, "Fatal error");
}

void WMessageHandler::report(const SAXParseException& e, const char *kind)
{
  const char *message = XMLString::transcode(e.getMessage());
  const char *file = XMLString::transcode(e.getSystemId());

  std::cerr << kind << " reading XML file: ";

  if (strlen(file) != 0) {
    std::cerr << "file " << file
	      << " line " << e.getLineNumber()
	      << ", char " << e.getColumnNumber()
	      << ": ";
  }

  std::cerr << message << std::endl;

  XMLString::release((char **) &file);
  XMLString::release((char **) &message);
}

} // end namespace

namespace Wt {

WMessageResources::WMessageResources(const std::string path,
				     bool loadInMemory)
  : loadInMemory_(loadInMemory),
    loaded_(false),
    path_(path)
{ }

void WMessageResources::refresh()
{
  defaults_.clear();
  readResourceFile("", defaults_);

  local_.clear();
  std::string locale = wApp->locale();

  for(;;) {
    if (readResourceFile(locale, local_))
      break;

    /* try a lesser specified variant */
    std::string::size_type l = locale.rfind('-');
    if (l != std::string::npos)
      locale.erase(l);
    else
      break;
  }

  loaded_ = true;
}

void WMessageResources::hibernate()
{
  if (!loadInMemory_) {
    defaults_.clear();
    local_.clear();
    loaded_ = false;
  }
}

bool WMessageResources::getValue(const WMessage& message,
				 std::wstring& result)
{
  if (!loaded_)
    refresh();

  KeyValueMap::const_iterator j;

  j = local_.find(message.key());
  if (j != local_.end()) {
    result = j->second;
    substitute(result, message.args());
    return true;
  }

  j = defaults_.find(message.key());
  if (j != defaults_.end()) {
    result = j->second;
    substitute(result, message.args());
    return true;
  }

  return false;
}

static void replace(std::wstring& s, std::wstring k, std::wstring r)
{
  std::wstring::size_type p = 0;

  while ((p = s.find(k, p)) != std::wstring::npos) {
    s.replace(p, k.length(), r);
    p += r.length();
  }
}

void WMessageResources::substitute(std::wstring& result,
				   const std::vector<std::wstring>& args)
{
  for (unsigned i = 0; i < args.size(); ++i) {
    std::wstring key = L'{' + boost::lexical_cast<std::wstring>(i+1) + L'}';

    replace(result, key, args[i]);
  }
}

bool WMessageResources::readResourceFile(std::string locale,
					 KeyValueMap& valueMap)
{
  std::string fileName
    = path_ + (locale.length() > 0 ? "_" : "") + locale + ".xml";

  {
    std::ifstream test(fileName.c_str());
    if (!test)
      return false;
  }

  std::cerr << "Reading resource file: " << fileName << std::endl;

  try {
    SAX2XMLReader* reader = XMLReaderFactory::createXMLReader();
    WMessageHandler handler(valueMap);
    reader->setContentHandler(&handler);
    reader->setErrorHandler(&handler);
    reader->parse(fileName.c_str());
    delete reader;

    return true;

  } catch (const std::exception& e) {
    std::cerr << "Exception caught reading XML file: "
	      << fileName << ": " << e.what() << std::endl;

    return false;
  }
}

}
