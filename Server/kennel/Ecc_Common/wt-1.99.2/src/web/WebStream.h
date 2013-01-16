// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef WEB_STREAM_H_
#define WEB_STREAM_H_

#include <WDllDefs.h>

namespace Wt {

class WebRequest;

class WT_API WebStream
{
public:
  WebStream(bool multiThreaded);

  virtual ~WebStream();

  /*
   * Can we get a new WebRequest before the previous one
   * has been deleted ?
   */
  bool multiThreaded() const { return multiThreaded_; }

  /*
   * Get the next request, return 0 when a timeout occurs.
   * otherwise throws an exception.
   */
  virtual WebRequest *getNextRequest(int timeoutsec) = 0;

private:
  bool multiThreaded_;
};

}

#endif // WEB_STREAM_H_
