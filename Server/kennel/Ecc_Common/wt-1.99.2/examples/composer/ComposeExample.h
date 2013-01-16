// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef COMPOSE_EXAMPLE_H_
#define COMPOSE_EXAMPLE_H_

#include <WContainerWidget>

class Composer;
class WTreeNode;

/**
 * \defgroup composerexample Composer example
 */
/*@{*/

/*! \brief Main widget of the %Composer example.
 */
class ComposeExample : public WContainerWidget
{
public:
  /*! \brief create a new Composer example.
   */
  ComposeExample(WContainerWidget *parent = 0);

private:
  Composer *composer_;
  WContainerWidget *details_;

private slots:
  void send();
  void discard();
};

/*@}*/

#endif // COMPOSE_EXAMPLE_H_
