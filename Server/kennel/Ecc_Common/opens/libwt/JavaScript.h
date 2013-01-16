/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
// This may look like C code, but it's really -*- C++ -*-
#ifndef QWEB_JAVASCRIPT_H_
#define QWEB_JAVASCRIPT_H_

#include <string>
#include <vector>

class JavaScript;
class DomElement;
class QObject;

class JSVar
{
public:
  JSVar(const JSVar& other);
  const std::string name() { return name_; }

private:
  JSVar();
  const std::string name_;

  static int varId_;

  friend class JavaScript;
  friend class DomElement;
};

class __declspec(dllexport) JavaScript
{
public:
  JavaScript();

  JSVar declareVar(const std::string value);

  void addStatement(const std::string statement);
  void createQueryString(const std::vector<QObject *>& formObjects);

  const std::string statements() const { return statements_; }

  const std::string getTempFunctionName();

private:
  std::string url_;
  std::string statements_;

  static int currentFunctionId_;
};

#endif // QWEB_JAVASCRIPT_H_
