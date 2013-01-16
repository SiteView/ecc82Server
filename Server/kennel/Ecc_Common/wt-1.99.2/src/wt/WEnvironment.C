/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WEnvironment"
#include "CgiParser.h"
#include "WebRequest.h"

#include <stdexcept>
#include <boost/spirit/core.hpp>
#include <boost/bind.hpp>
#include <assert.h>

namespace Wt {

WEnvironment::WEnvironment(WebSession *session)
  : session_(session),
    doesJavaScript_(false),
    doesAjax_(false),
    doesCookies_(false),
    inFrame_(false)
{ }

#if WIN32
// Because class is WT_API, all methods must be defined
WEnvironment::WEnvironment(const WEnvironment &)
{
  assert(false);
}
#endif

void WEnvironment::init(const CgiParser& cgi, const WebRequest& request)
{
  for (CgiParser::EntryMap::const_iterator i = cgi.entries().begin();
       i != cgi.entries().end(); ++i) {
    ArgumentValues v;

    CgiEntry *e = i->second;

    while (e) {
      v.push_back(e->value());
      e = e->next();
    }

    arguments_[i->first] = v;
  }

  host_      = request.headerValue("Host");
  userAgent_ = request.headerValue("User-Agent");
  referer_   = request.headerValue("Referer");

  std::string cookie = request.headerValue("Cookie");
  doesCookies_ = !cookie.empty();

  if (doesCookies_)
    parseCookies(cookie);

  locale_ = parsePreferredAcceptValue(request.headerValue("Accept-Language"));
}

const WEnvironment::ArgumentValues&
WEnvironment::getArgument(const std::string argument_name) const
{
  ArgumentMap::const_iterator i = arguments_.find(argument_name);

  if (i == arguments_.end())
    throw std::runtime_error("missing argument: " + argument_name);
  else
    return i->second;
}

const std::string WEnvironment::getCookie(const std::string cookie_name) const
{
  CookieMap::const_iterator i = cookies_.find(cookie_name);

  if (i == cookies_.end())
    throw std::runtime_error("missing cookie: " + cookie_name);
  else
    return i->second;
}

namespace {
  using namespace boost::spirit;
  using namespace boost;

  /*
   * My first spirit parser -- spirit is nifty !
   *
   * Parses things like:
   *  nl-be,en-us;q=0.7,en;q=0.3
   *  ISO-8859-1,utf-8;q=0.7,*;q=0.7
   *
   * And store the values with indicated qualities.
   */
  class ValueListParser : public grammar<ValueListParser>
  {
  public:
    struct Value {
      std::string value;
      double quality;

      Value(std::string v, double q) : value(v), quality(q) { }
    };

    ValueListParser(std::vector<Value>& values)
      : values_(values)
    { }

  private:
    std::vector<Value>& values_;

    void setQuality(double v) const {
      values_.back().quality = v;
    }

    void addValue(char const* str, char const* end) const {
      values_.push_back(Value(std::string(str, end), 1.));
    }

    typedef ValueListParser self_t;

  public:
    template <typename ScannerT>
    struct definition
    {
      definition(ValueListParser const& self)
      {
	option 
	  = ((ch_p('q') | ch_p('Q'))
	     >> '=' 
	     >> ureal_p
	        [
		  bind(&self_t::setQuality, self, _1)
		]
	     )
	  | (+alpha_p >> '=' >> +alnum_p)
	  ;

	value
	  = lexeme_d[(alpha_p >> +(alpha_p | '-')) | '*']
	    [
	       bind(&self_t::addValue, self, _1, _2)
	    ]
	    >> !( ';' >> option )
	  ;

	valuelist
	  = !(value  >> *(',' >> value )) >> end_p
	  ;
      }

      rule<ScannerT> option, value, valuelist;

      rule<ScannerT> const&
      start() const { return valuelist; }
    };
  };
};

std::string WEnvironment::parsePreferredAcceptValue(const std::string& str)
{
  std::vector<ValueListParser::Value> values;

  ValueListParser valueListParser(values);

  using namespace boost::spirit;

  parse_info<> info = parse(str.c_str(), valueListParser, space_p);

  if (info.full) {
    unsigned best = 0;
    for (unsigned i = 1; i < values.size(); ++i) {
      if (values[i].quality > values[best].quality)
	best = i;
    }

    if (best < values.size())
      return values[best].value;
    else
      return "";
  } else {
    std::cerr << "Could not parse, stopped at: \"" << info.stop 
	      << "\"" << std::endl;
    return "";
  }
}

namespace {
  using namespace boost::spirit;
  using namespace boost;

  /*
   * My second spirit parser -- spirit is still nifty !
   *
   * Parses Cookies set by the browser, as defined in RFC 2965
   */
  class CookieParser : public grammar<CookieParser>
  {
    struct ParseState
    {
      std::string lastName;
    };

  public:
    CookieParser(std::map<std::string, std::string>& cookies)
      : cookies_(cookies),
	constParseState_(ParseState()),
	parseState_(constParseState_)
    { }

  private:
    std::map<std::string, std::string>& cookies_;
    ParseState  constParseState_;
    ParseState& parseState_;

    void setName(char const* str, char const* end) const {
      parseState_.lastName = std::string(str, end);
      cookies_[parseState_.lastName] = "";
    }

    void setValue(char const* str, char const* end) const {
      cookies_[parseState_.lastName] = std::string(str, end);
    }

    static std::string unescape(std::string value) {
      std::string::size_type pos;

      while ((pos = value.find('\\')) != std::string::npos) {
	value.erase(pos, 1);
      }

      return value;
    }

    void setQuotedValue(char const* str, char const* end) const {
      cookies_[parseState_.lastName] = unescape(std::string(str + 1, end - 1));
    }

    typedef CookieParser self_t;

  public:
    template <typename ScannerT>
    struct definition
    {
      definition(CookieParser const& self)
      {
	token 
	  = lexeme_d[+(alnum_p | '_' | '-' | '.' | '*' | '$' | '#')];

	quoted_string
	  = lexeme_d[ch_p('"')
		     >> *(('\\' >> anychar_p) | ~ch_p('"')) >> ch_p('"')]
	  ;

	name
	  = token
	    [
	      bind(&self_t::setName, self, _1, _2)
	    ]
	  ;

	value
	  = token
	    [
	      bind(&self_t::setValue, self, _1, _2)
	    ]
	  | quoted_string
	    [
	      bind(&self_t::setQuotedValue, self, _1, _2)
	    ]
	  ;

	cookie
	  = name >> !(ch_p('=') >> value)
	  ;

	cookielist
	  = !(cookie >> *((ch_p(',') | ch_p(';')) >> cookie)) >> end_p
	  ;
      }

      rule<ScannerT> token, quoted_string, name, value, cookie, cookielist;

      rule<ScannerT> const&
      start() const { return cookielist; }
    };
  };
};

void WEnvironment::parseCookies(const std::string& str)
{
  CookieParser cookieParser(cookies_);

  using namespace boost::spirit;

  parse_info<> info = parse(str.c_str(), cookieParser, space_p);

  if (!info.full) {
    std::cerr << "Could not parse cookies, stopped at: \"" << info.stop 
	      << "\"" << std::endl;
  }
}

bool WEnvironment::agentIsSpiderBot() const
{
  static const char *bots[] = { "Googlebot", "msnbot", "Slurp", "Crawler" };
  for (int i = 0; i < 4; ++i)
    if (userAgent_.find(bots[i]) != std::string::npos)
      return true;

  return false;
}

}
