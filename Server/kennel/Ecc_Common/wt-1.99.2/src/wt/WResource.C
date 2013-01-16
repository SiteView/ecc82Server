/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "WResource"
#include "WApplication"
#include "WebSession.h"
#include "WtRandom.h"

namespace Wt {

WResource::WResource(WObject* parent)
  : WObject(parent),
    dataChanged(this),
    reentrant_(false)
{ }

WResource::~WResource()
{ 
  if (wApp)
    wApp->removeExposedResource(this);
}

void WResource::setReentrant(bool how)
{
  reentrant_ = how;
}

void WResource::suggestFilename(const std::string name)
{
  fileName_ = name;
}

const std::string WResource::generateUrl() const
{
  wApp->addExposedResource(const_cast<WResource *>(this));

  std::string url;

  if (!fileName_.empty())
    url = WebSession::instance()->appBinaryname()
      + "/" + fileName_;

  url += WebSession::instance()->relativeUrl()
    + "&resource=" + formName()
    + "&rand=" + boost::lexical_cast<std::string>((int)(drand48() * 100000));

  return url;
}

}
