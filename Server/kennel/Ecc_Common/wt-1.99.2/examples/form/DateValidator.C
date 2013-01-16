#include "DateValidator.h"

#include <WStringUtil>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::gregorian;

/*
 * Disclaimer: I am clueless how to use boost::gregorian in a sensible way.
 *
 * That, together with the fact that I wanted to test WRegExpValiator
 * is the reason why I use a regular expression to get the
 * day/month/year fields, and boost::gregorian to check that the date
 * is a valid date.
 */

DateValidator::DateValidator(const date& bottom, const date& top)
  : WRegExpValidator(L"(\\d{1,2})/(\\d{1,2})/(\\d{4})"),
    bottom_(bottom),
    top_(top)
{ }

WValidator::State DateValidator::validate(std::wstring& input, int& pos) const
{
  WValidator::State state = WRegExpValidator::validate(input, pos);

  if ((state == Valid) && !input.empty()) {
    boost::wsmatch what;
    boost::regex_match(input, what, regExp());

    try {
      date d
	= from_string(Wt::narrow(what[3] + L"/" + what[2] + L"/" + what[1]));

      if ((d >= bottom_) && (d <= top_))
	return Valid;
      else
	return Invalid;

    } catch (std::exception& e) {
      return Invalid;
    }
  } else
    return state;
}
