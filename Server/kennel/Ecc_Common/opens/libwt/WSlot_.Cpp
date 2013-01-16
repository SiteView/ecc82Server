/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WSlot_"

WSlot_::WSlot_()
{ }

WSlot_::WSlot_(const std::string name, bool canStatic)
  : name_(name),
    canStatic_(canStatic)
{ }
