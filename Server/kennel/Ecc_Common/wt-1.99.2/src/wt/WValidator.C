#include "WValidator"

namespace Wt {

WValidator::WValidator(bool mandatory)
  : mandatory_(mandatory)
{ }

void WValidator::setMandatory(bool mandatory)
{
  mandatory_ = mandatory;
}

void WValidator::fixup(std::wstring& input) const
{ }

WValidator::State WValidator::validate(std::wstring& input, int& pos) const
{
  if (isMandatory()) {
    if (input.empty())
      return InvalidEmpty;
  }

  return Valid;
}

}
