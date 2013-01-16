// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef STYLEEXAMPLE_H_
#define STYLEEXAMPLE_H_

#include <WContainerWidget>

namespace Wt {
  class WLineEdit;
  class WText;
}

using namespace Wt;

class RoundedWidget;

/**
 * \defgroup styleexample Style example
 */
/*@{*/

/*! \brief A demonstration of the RoundedWidget.
 *
 * This is the main class for the style example.
 */
class StyleExample : public WContainerWidget
{
public:
  /*! \brief Create a StyleExample.
   */
  StyleExample(WContainerWidget *parent = 0);

private:
  RoundedWidget *w_;
  WText *error_;

  WLineEdit *radius_, *r_, *g_, *b_;

  WLineEdit *createColorLineEdit(int value);

private slots:
  void updateStyle();
};

/*@}*/

#endif // STYLEEXAMPLE_H_
