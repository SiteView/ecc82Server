/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WebRequest.h"

using std::atoi;

namespace Wt {

WebRequest::~WebRequest()
{ }

std::string WebRequest::userAgent() const
{
  return headerValue("User-Agent");
}

std::string WebRequest::contentType() const
{
  return headerValue("Content-Type");
}

int WebRequest::contentLength() const
{
  std::string lenstr = headerValue("Content-Length");
  if (lenstr.empty())
    return 0;
  else
    return atoi(lenstr.c_str());
}

}
