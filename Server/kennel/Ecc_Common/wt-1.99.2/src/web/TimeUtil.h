// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef TIMEUTIL_H_
#define TIMEUTIL_H_

struct timeval;

namespace Wt {

class Time
{
public:
  Time(); // now
  ~Time(); // now
  Time(const Time &other);

  Time operator+ (int msec) const;
  Time& operator+= (int msec);
  Time& operator= (const Time &other);

  int operator- (const Time& other) const;

private:
  // Pointer to avoid inclusion of windows header files in wt
  struct timeval *t_;
};

}

#endif // TIMEUTIL_H_
