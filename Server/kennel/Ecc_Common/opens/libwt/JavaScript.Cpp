/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "JavaScript.h"

int JSVar::varId_ = 0;

JSVar::JSVar()
  : name_("j" + boost::lexical_cast<std::string>(varId_++))
{ }

JSVar::JSVar(const JSVar& other)
  : name_(other.name_)
{ }

JavaScript::JavaScript()
{ }

JSVar JavaScript::declareVar(const std::string value)
{
  JSVar result;
  addStatement("var " + result.name() + "=" + value + ";");

  return result;
}

void JavaScript::addStatement(const std::string s)
{
  statements_ += s;
}

const std::string JavaScript::getTempFunctionName()
{
  return "f" + boost::lexical_cast<std::string>(currentFunctionId_++);
}

int JavaScript::currentFunctionId_ = 0;
