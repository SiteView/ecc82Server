
#ifndef _SV_SPLIT_STRING_H_
#define _SV_SPLIT_STRING_H_

#pragma once

#include "../opens/boost/regex.hpp"

#include <string>
#include <list>

using namespace std;

namespace SV_Split
{
    inline unsigned SplitString(list<string>&l, string &szValue, char chSeparator = '\v')
    {
        static boost::regex e;
        if(chSeparator == '$' || chSeparator == '.' || chSeparator == '?' ||
            chSeparator == '\\' || chSeparator == '^' || chSeparator == '*' ||
            chSeparator == '+' || chSeparator == '|' || chSeparator == '%' ||
            chSeparator == '[' || chSeparator == ']')
            e = string("\\") + chSeparator;
        else
            e = string ("") + chSeparator;
        
        return static_cast<int>(boost::regex_split(std::back_inserter(l), szValue, e));
    };

    inline bool isEmail(std::string & szValue)
    {
        static const boost::regex e("\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*");
        return regex_match(szValue, e);
    };

    inline bool isNumeric(std::string & szValue)
    {
        static const boost::regex e("\\d*");
        return regex_match(szValue, e);
    };
};

#endif