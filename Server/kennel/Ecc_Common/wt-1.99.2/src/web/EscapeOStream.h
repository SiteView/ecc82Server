// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef ESCAPE_OSTREAM_H_
#define ESCAPE_OSTREAM_H_

#include <iostream>
#include <string>
#include <vector>

namespace Wt {

class EscapeOStream;

struct EscapeOStreamScope {
  void pop();

private:
  EscapeOStreamScope(EscapeOStream& s, int level);

  EscapeOStream& s_;
  int            level_;

  friend class EscapeOStream;
};

class EscapeOStream
{
public:
  EscapeOStream(std::ostream& sink);

  EscapeOStreamScope scope();
  void replace(char c, const char *s);
  void replace(char c, std::string s);

  EscapeOStream& operator<< (char);
  EscapeOStream& operator<< (const std::string s);

private:
  std::ostream& sink_;
  std::vector<std::pair<char, std::string> > escapers_;

  void popUntil(int push);
  void escape(const std::string s, int level);
  void escape(char c, int level);

  friend class EscapeOStreamScope;
};

}

#endif // ESCAPE_OSTREAM_H_
