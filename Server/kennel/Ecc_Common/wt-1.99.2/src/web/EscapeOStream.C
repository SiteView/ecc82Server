/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "EscapeOStream.h"

namespace Wt {

EscapeOStreamScope::EscapeOStreamScope(EscapeOStream& s, int level)
  : s_(s),
    level_(level)
{ }

void EscapeOStreamScope::pop()
{
  s_.popUntil(level_);
}

EscapeOStream::EscapeOStream(std::ostream& sink)
  : sink_(sink)
{ }

EscapeOStreamScope EscapeOStream::scope()
{
  return EscapeOStreamScope(*this, escapers_.size());
}

void EscapeOStream::replace(char c, const char *s)
{
  escapers_.push_back(std::make_pair(c, s));
}

void EscapeOStream::replace(char c, std::string s)
{
  escapers_.push_back(std::make_pair(c, s));
}

void EscapeOStream::popUntil(int push)
{
  escapers_.erase(escapers_.begin() + push, escapers_.end());
}

EscapeOStream& EscapeOStream::operator<< (const char c)
{
  escape(c, escapers_.size() - 1);
  return *this;
}

EscapeOStream& EscapeOStream::operator<< (const std::string s)
{
  escape(s, escapers_.size() - 1);
  return *this;
}

void EscapeOStream::escape(const std::string s, int level)
{
  for (unsigned i = 0; i < s.length(); ++i) {
    escape(s[i], level);
  }
}

void EscapeOStream::escape(char c, int level)
{
  for (int i = level; i >= 0; --i) {
    if (c == escapers_[i].first) {
      escape(escapers_[i].second, i - 1);
      return;
    }
  }

  //std::cerr << c;
  sink_ << c;
}

}
