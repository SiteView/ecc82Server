// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef WEB_REQUEST_H_
#define WEB_REQUEST_H_

#include <iostream>

#include <WDllDefs.h>

namespace Wt {

class WT_API WebRequest
{
public:
  virtual ~WebRequest();

  virtual std::istream& in() = 0;
  virtual std::ostream& out() = 0;
  virtual std::ostream& err() = 0;

  virtual void setRedirect(const std::string url) = 0;
  virtual void setContentType(const std::string value) = 0;
  virtual void addHeader(const std::string name, const std::string value) = 0;

  /* info that are not headers (but are in the request line itself )*/
  virtual std::string scriptName() const = 0;
  virtual std::string requestMethod() const = 0;
  virtual std::string queryString() const = 0;

  /* access to headers -- rfc2616 name*/
  virtual std::string headerValue(const std::string name) const = 0;

  std::string userAgent() const;
  std::string contentType() const;
  int         contentLength() const;
};

}

#endif // WEB_REQUEST_H_
