// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef MANDELBROT_EXAMPLE_H_
#define MANDELBROT_EXAMPLE_H_

#include <WContainerWidget>

using namespace Wt;

class MandelbrotImage;

class MandelbrotExample : public WContainerWidget
{
public:
  MandelbrotExample(WContainerWidget *parent = 0);

private:
  MandelbrotImage *mandelbrot_;

private slots:
  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();
};

#endif // MANDELBROT_EXAMPLE_H_
