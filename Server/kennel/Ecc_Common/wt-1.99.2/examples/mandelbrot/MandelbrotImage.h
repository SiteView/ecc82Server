// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef MANDELBROT_IMAGE_H_
#define MANDELBROT_IMAGE_H_

#include <WVirtualImage>

using namespace Wt;

class MandelbrotImage : public WVirtualImage
{
public:
  MandelbrotImage(int width, int height,
		  int virtualWidth, int virtualHeight,
		  double bx1, double by1,
		  double bx2, double by2,
		  WContainerWidget *parent = 0);

  void generate(int x, int y, int w, int h, std::ostream& out);

private:
  double bx1_, by1_, bwidth_, bheight_;
  int maxDepth_;
  double bailOut2_;

  virtual WResource *render(int x, int y, int w, int h);
  int calcPixel(double x, double y);
};

#endif // MANDELBROT_IMAGE_H_
