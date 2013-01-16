/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WebStream.h"

namespace Wt {

WebStream::WebStream(bool multithreaded)
  : multiThreaded_(multithreaded)
{ }

WebStream::~WebStream()
{ }

}
