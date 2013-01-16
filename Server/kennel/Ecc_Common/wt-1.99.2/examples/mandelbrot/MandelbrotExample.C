/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <WApplication>
#include <WPushButton>

#include "MandelbrotExample.h"
#include "MandelbrotImage.h"

MandelbrotExample::MandelbrotExample(WContainerWidget *parent)
  : WContainerWidget(parent)
{
  mandelbrot_ = new MandelbrotImage(400, 240,
				    1000, 750,
				    -2,
				    -2,
				    1,
				    2, this);

  (new WPushButton(L"Left", this))
    ->clicked.connect(SLOT(this, MandelbrotExample::moveLeft));
  (new WPushButton(L"Right", this))
    ->clicked.connect(SLOT(this, MandelbrotExample::moveRight));
  (new WPushButton(L"Up", this))
    ->clicked.connect(SLOT(this, MandelbrotExample::moveUp));
  (new WPushButton(L"Down", this))
    ->clicked.connect(SLOT(this, MandelbrotExample::moveDown));
}

void MandelbrotExample::moveLeft()
{
  mandelbrot_->scroll(-10, 0);
}

void MandelbrotExample::moveRight()
{
  mandelbrot_->scroll(10, 0);
}

void MandelbrotExample::moveUp()
{
  mandelbrot_->scroll(0, -10);
}

void MandelbrotExample::moveDown()
{
  mandelbrot_->scroll(0, 10);
}

WApplication *createApplication(const WEnvironment& env)
{
  WApplication *app = new WApplication(env);
  app->setTitle(L"Mandelbrot example");

  app->root()->addWidget(new MandelbrotExample());
  return app;
}

int main(int argc, char **argv)
{
   return WRun(argc, argv, &createApplication);
}
