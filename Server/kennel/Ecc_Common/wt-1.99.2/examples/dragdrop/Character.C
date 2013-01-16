#include <iostream>
#include <boost/lexical_cast.hpp>

#include "Character.h"

Character::Character(const std::wstring name, WContainerWidget *parent)
  : WText(parent),
    name_(name),
    redDrops_(0),
    blueDrops_(0)
{
  setText(name_ + L" got no pills");

  setStyleClass(L"character");

  /*
   * Accept drops, and indicate this with a change in CSS style class.
   */
  acceptDrops("red-pill", L"red-drop-site");
  acceptDrops("blue-pill", L"blue-drop-site");

  setInline(false);
}

void Character::dropEvent(WDropEvent event)
{
  if (event.mimeType() == "red-pill")
    ++redDrops_;
  if (event.mimeType() == "blue-pill")
    ++blueDrops_;

  std::wstring text = name_ + L" got ";

  if (redDrops_ != 0)
    text += boost::lexical_cast<std::wstring>(redDrops_) + L" red pill";
  if (redDrops_ > 1)
    text += L"s";

  if (redDrops_ != 0 && blueDrops_ != 0)
    text += L" and ";

  if (blueDrops_ != 0)
    text += boost::lexical_cast<std::wstring>(blueDrops_) + L" blue pill";
  if (blueDrops_ > 1)
    text += L"s";

  setText(text);
}
