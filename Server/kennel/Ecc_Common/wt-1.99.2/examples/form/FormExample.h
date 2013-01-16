// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef FORMEXAMPLE_H_
#define FORMEXAMPLE_H_

#include <WContainerWidget>

namespace Wt {
  class WText;
}

using namespace Wt;

/**
 * \defgroup formexample Form example
 */
/*@{*/

/*!\brief Main widget for the %Form example.
 *
 * This class demonstrates, next instantiating the form itself,
 * handling of different languages.
 */
class FormExample : public WContainerWidget
{
public:
  /*!\brief Instantiate a new form example.
   */
  FormExample(WContainerWidget *parent = 0);

private slots:
  /*!\brief Change the language.
   */
  void changeLanguage();

private:
  std::vector<WText *> languageSelects_;

  void setLanguage(const std::string lang);
};

/*@}*/

#endif // FORMEXAMPLE_H_
