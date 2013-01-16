/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <fstream>
#include <iostream>

#define NO_FCGI_DEFINES
#include <fcgi_stdio.h>

#include "WFileResource"

#include <iostream>
//#include "WFileResource"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WFileResource::slots_[] = { WSlot_() };
WSignal_ WFileResource::signals_[] = { WSignal_() };

void WFileResource::buildSlotMap(WSlotMap& result)
{
  WResource::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WFileResource::buildSignalMap(WSignalMap& result)
{
  WResource::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WFileResource::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WResource::triggerSlot(sender, slot, args);
}
bool WFileResource::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WResource::undoTriggerSlot(slot, args);
}


WFileResource::WFileResource(const std::string mimeType,
			     const std::string fileName)
  : WResource(),
    mimeType_(mimeType),
    fileName_(fileName)
{ }

WFileResource::~WFileResource()
{ }

void WFileResource::setFileName(const std::string fileName)
{
  fileName_ = fileName;
  emit dataChanged();
}

void WFileResource::setMimeType(const std::string mimeType)
{
  mimeType_ = mimeType;
  emit dataChanged();
}

std::string WFileResource::resourceMimeType() const
{
  return mimeType_;
}

void WFileResource::streamResourceData(FCGI_FILE *stream)
{
  std::ifstream r(fileName_.c_str());

  int count = 0;

  char c;
  while (r.get(c)) {
    FCGI_fputc(c, stream);
    ++count;
  }

  std::cerr << "WFileResource sent " << count << " bytes from "
	    << fileName_ << std::endl;
}
