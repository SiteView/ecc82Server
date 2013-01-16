// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef FCGI_STREAM_H_
#define FCGI_STREAM_H_

#include "WebStream.h"
#include "WebRequest.h"

class FCGX_Request;

namespace Wt {

class FCGIStream : public WebStream
{
public:
  FCGIStream();
  ~FCGIStream();

  virtual WebRequest *getNextRequest(int timeoutsec);
};

}

#endif // FCGI_STREAM_H_
