/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <algorithm>
#include <cctype>
#include <errno.h>
#include <stdio.h>

#include "FCGIStream.h"

#include "fcgio.h"
#include "fcgi_config.h"  // HAVE_IOSTREAM_WITHASSIGN_STREAMBUF

using std::memset;
using std::exit;

namespace {
  using namespace Wt;

  class FCGIRequest : public WebRequest
  {
  public:
    FCGIRequest(FCGX_Request *request)
      : request_(request)
    { 
      in_streambuf_ = new fcgi_streambuf(request_->in);
      out_streambuf_ = new fcgi_streambuf(request_->out);
      err_streambuf_ = new fcgi_streambuf(request_->err);
      in_ = new std::istream(in_streambuf_);
      out_ = new std::ostream(out_streambuf_);
      err_ = new std::ostream(err_streambuf_);
    }

    virtual ~FCGIRequest() {
      out_->flush();
      delete err_;
      delete out_;
      delete in_;
      delete err_streambuf_;
      delete out_streambuf_;
      delete in_streambuf_;

      FCGX_Finish_r(request_);
      delete request_;
    }

    virtual std::istream& in() { return *in_; }
    virtual std::ostream& out() { return *out_; }
    virtual std::ostream& err() { return *err_; }

    virtual void setContentType(const std::string value)
    {
      out() << "Content-Type: " << value << "\r\n\r\n";
    }

    virtual void addHeader(const std::string name, const std::string value)
    {
      out() << name << ": " << value << "\r\n";
    }

    virtual void setRedirect(const std::string url)
    {
      out() << "Location: " << url << "\r\n\r\n";
    }

    virtual std::string headerValue(const std::string name) const {
      char *result = FCGX_GetParam(cgiEnvName(name).c_str(), request_->envp);
      if (result)
	return result;
      else
	return "";
    }

    std::string cgiEnvName(const std::string name) const {
      if (name == "Content-Type")
	return "CONTENT_TYPE";
      else if (name == "Content-Length")
	return "CONTENT_LENGTH";
      else {
	std::string result = "HTTP_" + name;
	std::string::size_type i;
	while ((i = result.find('-')) != std::string::npos)
	  result[i] = '_';

	std::transform(result.begin(), result.end(), result.begin(), toupper);

	return result;
      }
    }

    virtual std::string scriptName() const {
      if (FCGX_GetParam("SCRIPT_NAME", request_->envp) == 0)
	std::cerr << "No SCRIPT_NAME ?" << std::endl;
      return FCGX_GetParam("SCRIPT_NAME", request_->envp);
    }

    virtual std::string requestMethod() const {
      if (FCGX_GetParam("REQUEST_METHOD", request_->envp) == 0)
	std::cerr << "No REQUEST_METHOD ?" << std::endl;
      return FCGX_GetParam("REQUEST_METHOD", request_->envp);
    }

    virtual std::string queryString() const {
      if (FCGX_GetParam("QUERY_STRING", request_->envp) == 0)
	std::cerr << "No QUERY_STRING ?" << std::endl;
      return FCGX_GetParam("QUERY_STRING", request_->envp);
    }

  private:
    FCGX_Request *request_;
    fcgi_streambuf *in_streambuf_, *out_streambuf_, *err_streambuf_;
    std::istream *in_;
    std::ostream *out_, *err_;
  };
}

namespace Wt {

FCGIStream::FCGIStream()
  : WebStream(true)
{
  FCGX_Init();
}

FCGIStream::~FCGIStream()
{ }

WebRequest *FCGIStream::getNextRequest(int timeoutsec)
{
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(0, &rfds);
  struct timeval timeout;
  timeout.tv_sec = timeoutsec;
  timeout.tv_usec = 0;

  for(;;) {
    int result = select(FD_SETSIZE, &rfds, 0, 0, &timeout);

    if (result == 0)
      return 0; // timeout
    else if (result == -1) {
      if (errno != EINTR) {
	perror("select");
	exit(1); // FIXME: throw exception
      } else
	; // EINTR, try again
    } else
      break;
  }

  FCGX_Request *request = new FCGX_Request();
  FCGX_InitRequest(request, 0, 0);

  if (FCGX_Accept_r(request) == 0) {
    return new FCGIRequest(request);
  } else {
    std::cerr << "Could not FCGX_Accept ?" << std::endl;
    delete request;

    exit(1); // FIXME: throw exception
  }
}

}
