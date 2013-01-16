/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <fstream>
#include <iostream>

#include "WFileResource"

namespace Wt {

WFileResource::WFileResource(const std::string mimeType,
			     const std::string fileName)
  : WResource(),
    mimeType_(mimeType),
    fileName_(fileName)
{
  setReentrant(true);
}

WFileResource::~WFileResource()
{ }

void WFileResource::setFileName(const std::string fileName)
{
  fileName_ = fileName;
  dataChanged.emit();
}

void WFileResource::setMimeType(const std::string mimeType)
{
  mimeType_ = mimeType;
  dataChanged.emit();
}

const std::string WFileResource::resourceMimeType() const
{
  return mimeType_;
}

void WFileResource::streamResourceData(std::ostream& stream)
{
  std::ifstream r(fileName_.c_str());

  int count = 0;

  char c;
  while (r.get(c)) {
    stream.put(c);
    ++count;
  }
}

}
