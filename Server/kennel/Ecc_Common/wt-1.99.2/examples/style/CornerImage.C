/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <stdio.h>
#include "gd.h"

#include <WFileResource>

#include "CornerImage.h"

CornerImage::CornerImage(Corner c, WColor fg, WColor bg,
			 int radius, WContainerWidget *parent)
  : WImage(parent),
    corner_(c),
    fg_(fg),
    bg_(bg),
    radius_(radius),
    resource_(0)
{
  compute();
}

CornerImage::~CornerImage()
{
  if (resource_) {
    unlink(resource_->fileName().c_str());
    delete resource_;
  }
}

void CornerImage::setRadius(int radius)
{
  if (radius != radius_) {
    radius_ = radius;
    compute();
  }
}

void CornerImage::setForeground(WColor color)
{
  if (fg_ != color) {
    fg_ = color;
    compute();
  }
}

#ifdef WIN32
// Hmm. The precompiled libgd is useless for saving files in MSVC > 6.
// This is the proposed solution from the FAQ.
// TODO: sensible error handling.
static void mySavePng(gdImagePtr im, FILE *out)
{
  int size;
  char *data;
  data = (char *) gdImagePngPtr(im, &size);
  if (!data) {
    return;
    /* Error */
  }
  if (fwrite(data, 1, size, out) != size) {
    return;
    /* Error */
  }
  gdFree(data);  
}
#else
#define mySavePng gdImagePng
#endif

void CornerImage::compute()
{
  /* We want an anti-aliased image: oversample twice */
  int AA = 2;

  gdImagePtr imBig = gdImageCreate(radius_ * AA, radius_ * AA);

  /* automatically becomes the background color -- gd documentation */
  gdImageColorAllocate(imBig, bg_.red(), bg_.green(), bg_.blue());

  int fgColor
    = gdImageColorAllocate(imBig, fg_.red(), fg_.green(), fg_.blue());

  int cx, cy;

  if (corner_ & Top)
    cy = radius_ * AA - 1;
  else
    cy = 0;

  if (corner_ & Left)
    cx = radius_ * AA - 1;
  else
    cx = 0;

  gdImageFilledArc(imBig, cx, cy, (radius_*2 - 1) * AA, (radius_*2 - 1) * AA,
		   0, 360, fgColor, gdArc);

  /* now create the real image, downsampled by a factor of 2 */
  gdImagePtr im = gdImageCreateTrueColor(radius_, radius_);
  gdImageCopyResampled(im, imBig, 0, 0, 0, 0, im->sx, im->sy,
		       imBig->sx, imBig->sy);

  /* and save it in a new temporary file */
#ifndef WIN32
  char tmpFile[20];
  strcpy(tmpFile, "/tmp/wt.ci.XXXXXX");
  int fd = mkstemp(tmpFile);
  FILE *pngout = fdopen(fd, "wb");
#else
  char tmpFile[2 * L_tmpnam];
  tmpnam(tmpFile);
  FILE *pngout = fopen(tmpFile, "wb");
#endif

  mySavePng(im, pngout);

  fclose(pngout);

  gdImageDestroy(im);
  gdImageDestroy(imBig);

  if (resource_) {
    /* delete the previous temporary file */
    unlink(resource_->fileName().c_str());
    resource_->setFileName(tmpFile);
  } else {
    /* create and set the file resource that contains the image */
    resource_ = new WFileResource("image/png", tmpFile);
    setResource(resource_);
  }
}

