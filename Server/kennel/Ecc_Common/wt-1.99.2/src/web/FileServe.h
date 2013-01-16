// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 * $Id: FileServe.h,v 1.3 2006/12/26 13:08:38 jozef Exp $
 */
#ifndef FILE_SERVE_H_
#define FILE_SERVE_H_

#include <string>
#include <iostream>
#include <map>

namespace Wt {

class FileServe
{
public:
  FileServe(const char *contents);

  void setVar(const std::string name, const std::string value);
  void stream(std::ostream& out, const std::string until = std::string());

private:
  const char *template_;
  int currentPos_;
  std::map<std::string, std::string> vars_;
};

}

#endif // FILE_SERVE_H_
