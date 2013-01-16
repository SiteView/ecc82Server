/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WSignal_"
#include "WConnection_"

WSignal_::WSignal_()
{ }

WSignal_::WSignal_(const std::string name, bool event)
  : name_(name),
    event_(event)
{ }

