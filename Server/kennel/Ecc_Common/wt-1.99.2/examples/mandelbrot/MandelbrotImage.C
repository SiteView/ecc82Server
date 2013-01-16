/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <stdio.h>
#include <fstream>
#include <iostream>
#include "gd.h"

#include <WResource>
#include <WImage>

#include "MandelbrotImage.h"

namespace {
  class MandelbrotResource : public WResource
  {
  public:
    MandelbrotResource(MandelbrotImage *img,
		       int x, int y, int w, int h)
      : img_(img),
	x_(x), y_(y), w_(w), h_(h)
    { 
      setReentrant(true);
    }

    const std::string resourceMimeType() const { return "image/png"; }

    void streamResourceData(std::ostream& stream) {
      img_->generate(x_, y_, w_, h_, stream);
    }

  private:
    MandelbrotImage *img_;
    int x_, y_, w_, h_;
  };
}

MandelbrotImage::MandelbrotImage(int width, int height,
				 int virtualWidth, int virtualHeight,
				 double bx1, double by1,
				 double bx2, double by2,
				 WContainerWidget *parent)
  : WVirtualImage(width, height, virtualWidth, virtualHeight, parent),
    bx1_(bx1), by1_(by1),
    bwidth_(bx2 - bx1), bheight_(by2 - by1),
    maxDepth_(1000),
    bailOut2_(1000*1000)
{
  redrawAll();
  scroll(virtualWidth/2, virtualHeight/2);
}

WResource *MandelbrotImage::render(int x, int y, int w, int h)
{
  return new MandelbrotResource(this, x, y, w, h);
}

void MandelbrotImage::generate(int x, int y, int w, int h, std::ostream& out)
{
  gdImagePtr im = gdImageCreateTrueColor(w, h);

  std::cerr << "rendering: (" << x << "," << y << ") (" 
	    << x+w << "," << y+h << ")" << std::endl;

  for (int i = 0; i < w; ++i)
    for (int j = 0; j < h; ++j) {
      double bx = (bx1_ + ((double) (x + i)) / virtualWidth()*bwidth_);
      double by = (by1_ + ((double) (y + j)) / virtualHeight()*bheight_);
      int d = calcPixel(bx, by);
      int col = d % 256;
      //std::cerr << col << std::endl;
      gdImageSetPixel(im, i, j, gdImageColorAllocate(im, col, col, col));
    }

  char tmpFile[20];
  strcpy(tmpFile, "/tmp/wt.mb.XXXXXX");
  int fd = mkstemp(tmpFile);
  FILE *pngout = fdopen(fd, "wb");

  gdImagePng(im, pngout);

  fclose(pngout);

  gdImageDestroy(im);

  std::ifstream r(tmpFile);
  char c;
  while (r.get(c))
    out.put(c);

  unlink(tmpFile);
}

int MandelbrotImage::calcPixel(double x, double y)
{
#define IMPL1
#ifdef IMPL1
  double x1 = x;
  double y1 = y;

  for (int i = 0; i < maxDepth_; ++i) {
    double xs = x1 * x1;
    double ys = y1 * y1;
    double x2 = xs - ys + x;
    double y2 = x1 * y1 * 2 + y;
    x1 = x2;
    y1 = y2;

    if (xs + ys > bailOut2_)
      return i;
  }

  return maxDepth_;

#else
  double aold = 0, bold = 0, a2old = 1E10, b2old=1E10,
    zsquared, a=0, b=0, asquared=0, bsquared=0;

  a=x;
  b=y;

  zsquared=asquared+bsquared;
  int iter = 0;
  for (; iter < maxDepth_; ++iter) {
    a = asquared - bsquared + x;
    asquared = a*a;
    b = 2*aold*b+y;
    if (bold==b && aold==a) {
      iter=maxDepth_ - 1;
    }
    bsquared=b*b;
    zsquared=asquared+bsquared;
    if (zsquared > bailOut2_)
      break;
    
    bold=b; aold=a;
  }

  return iter;
#endif
}
