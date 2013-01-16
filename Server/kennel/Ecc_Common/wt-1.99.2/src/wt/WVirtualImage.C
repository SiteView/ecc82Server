/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <iostream>

#include "WContainerWidget"
#include "WCssDecorationStyle"
#include "WImage"
#include "WScrollArea"
#include "WVirtualImage"

/*
 * Implementation strategy:
 *
 * We keep track of a neighbourhood around the current position
 * that is at least three time the size. We work with images of fixed
 * size.
 */

namespace Wt {

WVirtualImage::WVirtualImage(int width, int height,
			     int virtualWidth, int virtualHeight,
			     WContainerWidget *parent)
  : WCompositeWidget(parent),
    width_(width),
    height_(height),
    virtualWidth_(virtualWidth),
    virtualHeight_(virtualHeight),
    currentX_(0),
    currentY_(0)
{
  setImplementation(impl_ = new WContainerWidget());

  impl_->resize(width_, height_);
  impl_->setPositionScheme(Relative);
  impl_->decorationStyle().setBorder(WBorder(WBorder::Solid, WBorder::Medium,
					     Wt::red));
  impl_->mouseWentUp.connect(SLOT(this, WVirtualImage::mouseUp));

  WScrollArea *scrollArea = new WScrollArea(impl_);
  scrollArea->resize(WLength(100, WLength::Percentage),
		     WLength(100, WLength::Percentage));
  scrollArea->setScrollBarPolicy(WScrollArea::ScrollBarAlwaysOff);
  scrollArea->setPositionScheme(Absolute);

  contents_ = new WContainerWidget();
  contents_->setPositionScheme(Absolute);

  scrollArea->setWidget(contents_);
}

WVirtualImage::~WVirtualImage()
{
  for (GridMap::iterator it = grid_.begin(); it != grid_.end(); ++it) {
    delete it->second->resource();
    delete it->second;
  }
}

/*
 * Does not work until Abdiel fixes the signal/slot stuff
 */
void WVirtualImage::mouseUp(WMouseEvent e)
{
  std::cerr << "mouse up" << std::endl;
}

void WVirtualImage::redrawAll()
{
  for (unsigned i = 0; i < grid_.size(); ++i)
    delete grid_[i];

  contents_->clear();

  generateGridItems(currentX_, currentY_);
}

void WVirtualImage::scroll(int dx, int dy)
{
  int newX = std::min(virtualWidth_ - width_, std::max(0, currentX_ + dx));
  int newY = std::min(virtualHeight_ - height_, std::max(0, currentY_ + dy));

  contents_->setOffset(Left, WLength(-newX));
  contents_->setOffset(Top, WLength(-newY));

  generateGridItems(newX, newY);
}

void WVirtualImage::generateGridItems(int newX, int newY)
{
  /*
   * The coordinates of the two extreme corners of the new rendered
   * neighbourhood
   */
  Rect newNb = neighbourhood(newX, newY, width_, height_);  

  int i1 = newNb.x1 / GRID;
  int j1 = newNb.y1 / GRID;
  int i2 = newNb.x2 / GRID + 1;
  int j2 = newNb.y2 / GRID + 1;

  for (int i = i1; i < i2; ++i)
    for (int j = j1; j < j2; ++j) {
      int key = gridKey(i, j);
      GridMap::iterator it = grid_.find(key);
      if (it == grid_.end()) {
	WResource *r = render(i * GRID, j * GRID, GRID, GRID);
	WImage *img = new WImage(r, L"");
	contents_->addWidget(img);
	img->setPositionScheme(Absolute);
	img->setOffset(Left, WLength(i * GRID));
	img->setOffset(Top, WLength(j * GRID));

	grid_[key] = img;
      }
    }

  currentX_ = newX;
  currentY_ = newY;

  cleanGrid();
}

int WVirtualImage::gridKey(int i, int j)
{
  return i * 1000 + j;
}

void WVirtualImage::decodeKey(int key, int& i, int& j)
{
  i = key / 1000;
  j = key % 1000;
}

void WVirtualImage::cleanGrid()
{
  Rect cleanNb = neighbourhood(currentX_, currentY_, width_ * 3, height_ * 3);

  int i1 = cleanNb.x1 / GRID;
  int j1 = cleanNb.y1 / GRID;
  int i2 = cleanNb.x2 / GRID + 1;
  int j2 = cleanNb.y2 / GRID + 1;

  for (GridMap::iterator it = grid_.begin(); it != grid_.end(); ++it) {
    int i, j;
    decodeKey(it->first, i, j);

    if (i < i1 || i > i2 || j < j1 || j > j2) {
      delete it->second->resource();
      delete it->second;
      grid_.erase(it);
    }
  }
}

WVirtualImage::Rect 
WVirtualImage::neighbourhood(int x, int y, int marginX, int marginY)
{
  int x1 = std::max(0, x - marginX);
  int y1 = std::max(0, y - marginY);
  int x2 = std::min(virtualWidth_, x + width_ + marginX);
  int y2 = std::min(virtualHeight_, y + height_ + marginY);

  return Rect(x1, y1, x2, y2);
}

}
