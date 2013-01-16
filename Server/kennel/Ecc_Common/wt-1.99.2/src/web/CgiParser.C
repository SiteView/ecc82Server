/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 * In addition to these terms, permission is also granted to use and
 * modify these two files (CgiParser.C and CgiParser.h) so long as the
 * copyright above is maintained, modifications are documented, and
 * credit is given for any use of the library.
 *
 * CGI parser modelled after the PERL implmentation cgi-lib.pl 2.18 by
 * Steven E. Brenner with the following original copyright:

# Perl Routines to Manipulate CGI input
# cgi-lib@pobox.com
# $Id: CgiParser.C,v 1.15 2006/12/11 09:37:07 jozef Exp $
#
# Copyright (c) 1993-1999 Steven E. Brenner  
# Unpublished work.
# Permission granted to use and modify this library so long as the
# copyright above is maintained, modifications are documented, and
# credit is given for any use of the library.
#
# Thanks are due to many people for reporting bugs and suggestions

# For more information, see:
#     http://cgi-lib.stanford.edu/cgi-lib/  

 */

//#define DEBUG

#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <stdlib.h>

#include "CgiParser.h"
#include "WebRequest.h"

using std::memmove;
using std::strcpy;
using std::strtol;

namespace {
  const boost::regex boundary_e1("\\bboundary=\"([^\"]+)\"",
				 boost::regex::perl|boost::regex::icase);
  const boost::regex boundary_e2("\\bboundary=(\\S+)",
				 boost::regex::perl|boost::regex::icase);
  const boost::regex name_e1("\\bname=\"([^\"]+)\"",
			     boost::regex::perl|boost::regex::icase);
  const boost::regex name_e2("\\bname=([^\\s:;]+)",
			     boost::regex::perl|boost::regex::icase);
  const boost::regex filename_e1("\\bfilename=\"([^\"]*)\"",
				 boost::regex::perl|boost::regex::icase);
  const boost::regex filename_e2("\\bfilename=([^\\s:;]+)",
				 boost::regex::perl|boost::regex::icase);
  const boost::regex content_e1("^\\s*Content-type:\\s*\"([^\"]+)\"",
				boost::regex::perl|boost::regex::icase);
  const boost::regex content_e2("^\\s*Content-type:\\s*([^\\s:;]+)",
				boost::regex::perl|boost::regex::icase);
  const boost::regex content_disposition_e("^\\s*Content-Disposition:",
			        boost::regex::perl|boost::regex::icase);
  const boost::regex content_type_e("^\\s*Content-Type:",
				    boost::regex::perl|boost::regex::icase);

  bool fishValue(const std::string text,
		 const boost::regex& e1, std::string& result)
  {
    boost::smatch what;

    if (boost::regex_search(text, what, e1)) {
      result = what[1];
      return true;
    } else
      return false;
  }

  bool fishValue(const std::string text,
		 const boost::regex& e1, const boost::regex& e2,
		 std::string& result)
  {
    boost::smatch what;

    if (boost::regex_search(text, what, e1)) {
      result = what[1];
      return true;
    } else if (boost::regex_search(text, what, e2)) {
      result = what[1];
      return true;
    } else
      return false;
  }
}

namespace Wt {

#ifndef WIN32
const int CgiParser::BUFSIZE;
const int CgiParser::MAXBOUND;
#endif
char CgiParser::buf_[BUFSIZE + MAXBOUND];

CgiParser::CgiParser(int maxPostData)
  : maxPostData_(maxPostData)
{
}

CgiParser::~CgiParser()
{
  for (EntryMap::const_iterator i = entries_.begin();
       i != entries_.end(); ++i)
    delete i->second;
}

CgiEntry *CgiParser::getEntry(const std::string key) const
{
  EntryMap::const_iterator i = entries_.find(key);
  if (i != entries_.end())
    return i->second;
  else
    return 0;
}

void CgiParser::parse(WebRequest& request)
{
  try {

  unsigned len = request.contentLength();
  std::string type = request.contentType();
  std::string meth = request.requestMethod();

  postDataExceeded_ = (len > maxPostData_ ? len : 0);

  std::string queryString = request.queryString();

  if (!queryString.empty()) {
    if ((meth == "POST")
	&& (type.find("application/x-www-form-urlencoded") == 0)) {
      char *buf = new char[len + 1];

      request.in().read(buf, len);
      if (request.in().gcount() != (int)len) {
        delete[] buf;
	throw Exception("Unexpected short read.");
      }
      buf[len] = 0;

      // This is a special Wt feature, I do not think it standard.
      // For POST, parameters in url-encoded URL are still parsed.
      queryString += '&';
      queryString += buf;
      delete[] buf;
    }

    typedef boost::tokenizer<boost::char_separator<char> > amp_tok;
    amp_tok tok(queryString, boost::char_separator<char>("&"));

    for (amp_tok::iterator i = tok.begin(); i != tok.end(); ++i) {
      std::string pair = *i;

#ifdef DEBUG
      std::cerr << pair << std::endl;
#endif // DEBUG

      // convert plus to space
      replaceAll(pair, '+', ' ');

      // split into key and value
      std::string::size_type equalPos = pair.find('=');
      std::string key = pair.substr(0, equalPos);
      std::string value  = (equalPos != std::string::npos) 
	? pair.substr(equalPos + 1) : "";

      // convert %XX from hex numbers to alphanumeric
      replaceHexTokens(key);
      replaceHexTokens(value);

#ifdef DEBUG
      std::cerr << key << ": \"" << value << "\"" << std::endl;
#endif // DEBUG

      CgiEntry *e = createEntry(key);
      e->setValue(value);
    }
  }

  if (type.find("multipart/form-data") == 0) {
    if (meth != "POST") {
      throw Exception("Invalid method for multipart/form-data: " + meth);
    }

    readMultipartData(request, type, len);
  }

  } catch (Exception e) {
    std::cerr << "Error parsing CGI data: " << e.what() << std::endl;
    throw e;
  }
}

void CgiParser::readMultipartData(WebRequest& request,
				  const std::string type, int len)
{
  std::string boundary;
    
  if (!fishValue(type, boundary_e1, boundary_e2, boundary))
    throw Exception("Could not find a boundary for multipart data.");
    
  boundary = "--" + boundary;

  buflen_ = 0;
  left_ = len;
  spoolStream_ = 0;
  entry_ = 0;

  parseBody(request, boundary);
  for (;;) {
    if (!parseHead(request))
      break;
    if (!parseBody(request,boundary)) 
      break;
  }
}

/*
 * Read until finding the boundary, saving to resultString or
 * resultFile. The boundary itself is not consumed.
 *
 * tossAtBoundary controls how many characters extra (<0)
 * or few (>0) are saved at the start of the boundary in the result.
 */
void CgiParser::readUntilBoundary(WebRequest& request,
				  const std::string boundary,
				  int tossAtBoundary,
				  std::string *resultString,
				  std::ostream *resultFile)
{
  int bpos;

  while ((bpos = index(boundary)) == -1) {
    if ((left_ == 0) && (buflen_ == 0))
      throw Exception("CgiParser: reached end of input while seeking end of "
		      "headers or content. Format of CGI input is wrong");

    /* save BUFSIZE from buffer to file or value string*/
    int save = std::min(buflen_, BUFSIZE);
    if (resultString)
      *resultString += std::string(buf_, save);
    if (resultFile) 
      resultFile->write(buf_, save);

    /* wind buffer */
    windBuffer(save);

    unsigned amt = std::min(left_, BUFSIZE + MAXBOUND - buflen_);

    request.in().read(buf_ + buflen_, amt);    
    if (request.in().gcount() != (int)amt)
      throw Exception("CgiParser: short read");

    left_ -= amt;
    buflen_ += amt;
  }

  if (resultString)
    *resultString += std::string(buf_, bpos - tossAtBoundary);
  if (resultFile)
    resultFile->write(buf_, bpos - tossAtBoundary);

  /* wind buffer */
  windBuffer(bpos);
}

void CgiParser::windBuffer(int offset)
{
  if (offset < buflen_) {
    memmove(buf_, buf_ + offset, buflen_ - offset);
    buflen_ -= offset;
  } else
    buflen_ = 0;
}

int CgiParser::index(const std::string search)
{
  std::string bufS = std::string(buf_, buflen_);

  std::string::size_type i = bufS.find(search);

  if (i == std::string::npos)
    return -1;
  else
    return i;
}

bool CgiParser::parseHead(WebRequest& request)
{
  std::string head;
  readUntilBoundary(request, "\r\n\r\n", -2, &head, 0);

  /* split lines */
  boost::regex re("\r\n");
  boost::sregex_token_iterator i(head.begin(), head.end(), re, -1);
  boost::sregex_token_iterator j;

  std::string name;
  std::string fn;
  std::string ctype;

  while (i != j) {
    const std::string text = *i++;

    if (boost::regex_search(text, content_disposition_e)) {
      fishValue(text, name_e1, name_e2, name);
      fishValue(text, filename_e1, filename_e2, fn);
    }

    if (boost::regex_search(text, content_type_e)) {
      fishValue(text, content_e1, content_e2, ctype);
    }
  }

#ifdef DEBUG
  std::cerr << "name: " << name 
	    << " ct: " << ctype 
	    << " fn: " << fn << std::endl;
#endif

  entry_ = createEntry(name);

  if (!fn.empty()) {
    if (!postDataExceeded_) {
      /*
       * It is not easy to create a std::ostream pointing to a
       * temporary file name.
       */
#ifndef WIN32
      char spool[20];
      strcpy(spool, "/tmp/wtXXXXXX");

      int i = mkstemp(spool);
      close(i);
#else
      char spool[2 * L_tmpnam];
      // FIXME: check for error retval for tmpnam
      // FIXME: where is this tmp file created ? cwd ? is that ok ?
      tmpnam(spool);
#endif

      spoolStream_ = new std::ofstream(spool);

      entry_->setStream(spool, fn, ctype);
    } else {
      spoolStream_ = 0;
    }
  }

  windBuffer(4);

  return true;
}

bool CgiParser::parseBody(WebRequest& request, const std::string boundary)
{
  std::string value;

  readUntilBoundary(request, boundary, 2,
		    spoolStream_ ? 0 : (entry_ ? &value : 0),
		    spoolStream_);

  if (spoolStream_) {
    delete spoolStream_;
    spoolStream_ = 0;
  } else
    if (entry_) {
#ifdef DEBUG
      std::cerr << "value: \"" << value << "\"" << std::endl;
#endif
      entry_->setValue(value);
    }

  entry_ = 0;

  if (std::string(buf_ + boundary.length(), 2) == "--")
    return false;

  windBuffer(boundary.length() + 2);

  return true;
}

void CgiParser::replaceAll(std::string& v, char from, char to)
{
  for (std::string::size_type i = v.find(from);
       i != std::string::npos;
       i = v.find(from, i+1))
    v[i] = to;
}

void CgiParser::replaceHexTokens(std::string& v)
{
  for (unsigned i = 0; i < (unsigned)std::max(0, (int)v.length() - 2); ++i) {
    if (v[i] == '%') {
      std::string h = v.substr(i + 1, 2);
      char *e = 0;
      int hval = strtol(h.c_str(), &e, 16);

      if (*e != 0)
	continue; // not a proper %XX with XX hexadecimal format

      v.replace(i, 3, 1, (char)hval);
    }
  }
}

CgiEntry *CgiParser::createEntry(const std::string param)
{
  CgiEntry *result = new CgiEntry();

  if (entries_.find(param) != entries_.end()) {
    CgiEntry *l = entries_[param];

    for (; l->next_; l = l->next_)
      ;

    l->next_ = result;
  } else
    entries_[param] = result;

  return result;
}

CgiParser::Exception::Exception(const std::string what)
  : what_(what)
{ }

CgiParser::Exception::~Exception() throw()
{ }

CgiEntry::CgiEntry()
  : isFile_(false),
    fileIsStolen_(false),
    next_(0)
{ }

CgiEntry::~CgiEntry()
{
  if (next_)
    delete next_;

  if (isFile_ && !fileIsStolen_)
    unlink(value_.c_str());
}

void CgiEntry::stealFile()
{
  fileIsStolen_ = true;
}

void CgiEntry::setValue(const std::string value)
{
  value_ = value;
  isFile_ = false;
}

void CgiEntry::setStream(const std::string spoolName,
			 const std::string clientFilename,
			 const std::string clientType)
{
  value_ = spoolName;
  cfn_ = clientFilename;
  ct_ = clientType;
  isFile_ = true;
}

} // namespace Wt
