// -*- C++ -*-
/***************************************************************************
 *   Copyright (C) 2004 by Abdiel Janulgue                                 *
 *   xynopsis at users dot sourceforge dot net                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <boost/lexical_cast.hpp>

#include "WJavascriptSlot"
#include "WStatelessSlot"
#include "WWidget"
#include "WWebWidget"

namespace Wt {

int WJavascriptSlot::nextFid_ = 0;

WJavascriptSlot::WJavascriptSlot(WWidget *parent)
  : widget_(parent),
    fid_(nextFid_++)
{
  imp_ = new WStatelessSlot(parent, widget_ ? "return " + jsFunctionName()
			    + "(this, event);" : "return false;");
}

WJavascriptSlot::~WJavascriptSlot()
{
  delete imp_;
}

std::string WJavascriptSlot::jsFunctionName() const
{
  return "sf" + boost::lexical_cast<std::string>(fid_);
}

void WJavascriptSlot::setJavascript(const std::string& imp)
{
  if (widget_)
    widget_->webWidget()->setScript(jsFunctionName(), imp);
  else
    imp_->setJavaScript(imp);
}

WStatelessSlot* WJavascriptSlot::slotimp()
{
  return imp_;
}

}
