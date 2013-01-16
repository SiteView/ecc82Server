// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 * $Id: WebRenderer.h,v 1.8 2006/12/31 08:56:09 jozef Exp $
 */
#ifndef WEBRENDERER_H_
#define WEBRENDERER_H_

#include <string>
#include <sstream>
#include <vector>
#include "WEnvironment"
#include "WStatelessSlot"

namespace Wt {

class WebRequest;
class WebStream;

class WApplication;
class WWidget;
class WObject;
class WResource;
class WStatelessSlot;
class WWebWidget;

class WebRenderer : public Wt::SlotLearnerInterface
{
public:
  WebRenderer(WebSession& session);

  void setTwoPhaseThreshold(int bytes);

  bool visibleOnly() const { return visibleOnly_; }

  void needUpdate(WWebWidget *w);
  void doneUpdate(WWebWidget *w);

  enum ResponseType { ResponseEval, ResponseHTML };

  const std::vector<WObject *>& formObjects() const;

  void saveChanges();
  void discardChanges();
  void letReload(WebRequest& request, bool parentFrame);

  void serveMainWidget(WebRequest& request, ResponseType responseType);
  void serveBootstrap(WebRequest& request);
  void serveError(WebRequest& request, const std::string message,
		  ResponseType responseType);
  void serveError(WebRequest& request, const std::exception& error,
		  ResponseType responseType);

  void setCookie(const std::string name, const std::string value,
		 int maxAge, const std::string domain,
		 const std::string path);

  static std::string appSessionCookie(std::string url);

private:
  struct Cookie {
    std::string name;
    std::string value;
    std::string path;
    std::string domain;
    int maxAge;

    Cookie(std::string n, std::string v, std::string p, std::string d, int m)
      : name(n), value(v), path(p), domain(d), maxAge(m) { }
  };

  WebSession& session_;
  bool        visibleOnly_;
  int         twoPhaseThreshold_;
  std::string charEncoding_;

  std::vector<Cookie> cookiesToSet_;

  std::vector<WObject *> currentFormObjects_;
  std::string		 currentFormObjectsVar_;

  void setHeaders(WebRequest& request, const std::string mimeType);

  void serveJavascriptUpdate(WebRequest& request, WWidget *topWidget);
  void serveMainpage(WebRequest& request, WWidget *topWidget);
  void serveJavascriptUpdate(WebRequest& request, const std::string js);

  void collectChanges(WWebWidget *topWebWidget,
		      std::vector<DomElement *>& changes);

  void collectJavascriptUpdate(WWidget *topWidget, std::ostream& out);

  std::string createFormsVar(const std::vector<WObject *>& formObjects);

  std::string       learn(WStatelessSlot* slot);
  void              preLearnStateless();
  std::stringstream collectedChanges_;
  void              collectJS(WWebWidget *mainWebWidget, std::ostream *js);

  typedef std::map<std::string, WWebWidget *> UpdateMap;
  UpdateMap updateMap_;
  bool      learning_;
};

}

#endif // WEBRENDERER_H_
