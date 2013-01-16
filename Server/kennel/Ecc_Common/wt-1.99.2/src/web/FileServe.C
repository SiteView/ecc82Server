/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <cassert>

#include "FileServe.h"

namespace Wt {

FileServe::FileServe(const char *contents)
  : template_(contents),
    currentPos_(0)
{ }

void FileServe::setVar(const std::string name, const std::string value)
{
  vars_[name] = value;
}

void FileServe::stream(std::ostream& out, const std::string until)
{
  std::string currentVar;
  bool readingVar = false;

  for (; template_[currentPos_]; ++currentPos_) {
    const char *s = template_ + currentPos_;

    if (readingVar) {
      if (*s == '}') {
	if (currentVar == until) {
	  ++currentPos_;
	  return;
	}

	std::map<std::string, std::string>::const_iterator i
	  = vars_.find(currentVar);

	if (i == vars_.end()) {
	  std::cerr << "Internal error: could not find variable: "
		    << currentVar << std::endl;
	  assert(i != vars_.end());
	}

	out << i->second;

	readingVar = false;
      } else
	currentVar += *s;
    } else {
      if (*s == '$') {
	if (*(s + 1) == '{') {
	  ++currentPos_;
	  readingVar = true;
	  currentVar.clear();
	} else
	  out << *s;
      } else
	out << *s;
    }
  }
}

}
