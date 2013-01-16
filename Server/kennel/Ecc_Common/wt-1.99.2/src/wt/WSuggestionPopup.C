/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/lexical_cast.hpp>

#include "WContainerWidget"
#include "WText"
#include "WFormWidget"

#include "WSuggestionPopup"

namespace Wt {

WSuggestionPopup::WSuggestionPopup(const std::string matcherJS,
				   const std::string replacerJS,
				   WContainerWidget *parent)
  : WCompositeWidget(parent),
    matcherJS_(matcherJS),
    replacerJS_(replacerJS),
    editKeyDown_(parent), // should be this, but IE hack...
    editKeyUp_(parent),
    suggestionClicked_(parent),
    delayHide_(parent)
{
  setImplementation(content_ = new WContainerWidget());

  setPopup(true);
  setPositionScheme(Absolute);

  editKeyDown_.setJavascript
    ("function(edit, event) {"
     "  var self = " + jsRef() + ";"
     "  var sel = self.getAttribute('sel');"
     "  if (sel != null) sel = getElement(sel);"
     "  if (self.style.display != 'none' && sel != null) {"
     "    if ((event.keyCode == 13) || (event.keyCode == 9)) {"
     "      sel.firstChild.onclick();"
     "      return false;"
     "    } else if (event.keyCode == 40 || event.keyCode == 38) {"
     "      if (event.type.toUpperCase() == 'KEYDOWN')"
     "        self.setAttribute('kd', 'true');"
     "      if (event.type.toUpperCase() == 'KEYPRESS'"
     "        && self.getAttribute('kd') == 'true')"
     "        return false;"
     "      var n = sel;"
     "      for (var n = (event.keyCode == 40) ? n.nextSibling : n.previousSibling; "
     "           n != null && n.nodeName == 'DIV' && n.style.display == 'none';"
     "           n = (event.keyCode == 40) ? n.nextSibling : n.previousSibling) { }"
     "      if (n != null && n.nodeName == 'DIV') {"
     "        sel.setAttribute('class', null);"
     "        n.setAttribute('class', 'sel');"
     "        self.setAttribute('sel', n.id);"
     "      }"
     "      return false;"
     "    }"
     "  }"
     "  return (event.keyCode != 13 && event.keyCode != 9);"
     "}");

  editKeyUp_.setJavascript
    ("function(edit, event) {"
     "  var self = " + jsRef() + ";"
     "  var sel = self.getAttribute('sel');"
     "  if (sel != null) sel = getElement(sel);"
     ""
     "  if (event.keyCode == 27 || event.keyCode == 37 || event.keyCode == 39) {"
     "    self.style.display = 'none';"
     "    if (event.keyCode == 27)"
     "      edit.blur();"
     "  } else {"
     "    var text = edit.value;"
     "    var matcher = " + matcherJS_ + "(edit);"
     "    var first = null;"
     "    for (var i = 0; i < self.childNodes.length; i++) {"
     "      var child = self.childNodes[i];"
     "      if (child.nodeName == 'DIV') {"
     "        var result = matcher(child.firstChild.innerHTML);"
     "        child.firstChild.innerHTML = result.suggestion;"
     "        if (result.match) {"
     "          child.style.display = 'block';"
     "          if (first == null) first = child;"
     "        } else"
     "          child.style.display = 'none';"
     "        child.className = null;"
     "      }"
     "    }"
     "    if (first == null) {"
     "      self.style.display = 'none';"
     "    } else {"
     "      if (self.style.display != 'block') {"
     "        self.style.display = 'block';"
     "        self.parentNode.removeChild(self);"
     "        edit.parentNode.appendChild(self);"
     "        self.setAttribute('sel', null);"
     "        sel = null;"
     "      }"
     "      if ((sel == null) || (sel.style.display == 'none')) {"
     "        self.setAttribute('sel', first.id);"
     "        first.className = 'sel';"
     "      } else {"
     "        sel.className = 'sel';"
     "      }"
     "    }"
     "  }"
     "}");

  suggestionClicked_.setJavascript
    ("function(suggestion, event) {"
     "  var self = " + jsRef() + ";"
     "  var edit = self.previousSibling;"
     ""
     "  var sText = suggestion.innerHTML;"
     "  var sValue = suggestion.getAttribute('sug');"
     "  var replacer = " + replacerJS_ + ";"
     "  edit.focus();"
     "  replacer(edit, sText, sValue);"
     "  self.style.display = 'none';"
     "}");

  delayHide_.setJavascript
    ("function(edit, event) {"
     "  setTimeout(\"if (" + jsRef() + ") " + jsRef() + ".style.display = 'none';\", 300);"
      "}");

  hide();
}

void WSuggestionPopup::forEdit(WFormWidget *edit)
{
  edit->keyPressed.connect(editKeyDown_);
  edit->keyWentDown.connect(editKeyDown_);
  edit->keyWentUp.connect(editKeyUp_);
  edit->blurred.connect(delayHide_);
}

void WSuggestionPopup::clearSuggestions()
{
  content_->clear();
}

void WSuggestionPopup::addSuggestion(const std::wstring suggestionText,
				     const std::wstring suggestionValue)
{
  WContainerWidget *line = new WContainerWidget(content_);

  WText *value = new WText(suggestionText);
  line->addWidget(value);
  value->setFormatting(WText::PlainFormatting);
  value->setAttributeValue("sug", suggestionValue);
  value->clicked.connect(suggestionClicked_);
}

namespace {
  std::string generateParseEditJS(const WSuggestionPopup::Options& options)
  {
    return std::string() +
      "var value = edit.value;"
      "var pos;"
      "if (edit.selectionStart) { pos = edit.selectionStart; }"
      "  else { pos = value.length; }"
      "var ws = '" + options.whitespace + "';"
      + (options.listSeparator != 0
	 ? (std::string("var start = value.lastIndexOf('")
	    + options.listSeparator + "', pos - 1) + 1;")
	 : ("var start = 0;")) +
      "while ((start < pos)"
      "  && (ws.indexOf(value.charAt(start)) != -1))"
      "  start++;"
      "var end = pos;";
  }
};

std::string WSuggestionPopup::generateMatcherJS(const Options& options)
{
  return std::string() +
    "function (edit) {"
    + generateParseEditJS(options) +
    "value = edit.value.substring(start, end).toUpperCase();"
    ""
    "deleteAll = function (s1, s2) {"
    "  var i = 0;"
    "  while ((i != -1) && (i < s1.length)) {"
    "    i = s1.toUpperCase().indexOf(s2);"
    "    if (i != -1)"
    "      s1 = s1.substring(0, i) + s1.substring(i + s2.length, s1.length);"
    "  }"
    "  return s1;"
    "};"
    ""
    "return function(suggestion) {"
    + (!options.highlightBeginTag.empty()
       ? ("suggestion = deleteAll(suggestion, '"
	  + options.highlightBeginTag + "');"
	  "suggestion = deleteAll(suggestion, '"
	  + options.highlightEndTag + "');")
       : "") +
    "var sep='" + options.wordSeparators + "';"
    "var matched = false;"
    "var i = 0;"
    "var sugup = suggestion.toUpperCase();"
    "var inserted = 0;"
    "if (value.length != 0) {"
    "while ((i != -1) && (i < sugup.length)) {"
    "  var matchpos = sugup.indexOf(value, i);"
    "  if (matchpos != -1) {"
    "    if ((matchpos == 0)"
    "       || (sep.indexOf(sugup.charAt(matchpos - 1)) != -1)) {"
    + (!options.highlightEndTag.empty()
       ? ("suggestion = suggestion.substring(0, matchpos + inserted)"
	  " + '" + options.highlightBeginTag + "'"
	  " + suggestion.substring(matchpos + inserted,"
	  "     matchpos + inserted + value.length)"
	  " + '" + options.highlightEndTag + "'"
	  " + suggestion.substring(matchpos + inserted + value.length,"
	  "     suggestion.length);"
	  " inserted += "
	  + boost::lexical_cast<std::string>(options.highlightBeginTag.length()
					     + options.highlightEndTag.length())
	  + ";")
       : "") +
    "      matched = true;"
    "    }"
    "    i = matchpos + 1;"
    "  } else"
    "    i = matchpos;"
    "}"
    "}"
    "return { match: matched,"
    "         suggestion: suggestion }"
    "}"
    "}";
}

std::string WSuggestionPopup::generateReplacerJS(const Options& options)
{
  return std::string() +
    "function (edit, suggestionText, suggestionValue) {"
    + generateParseEditJS(options) +
    "edit.value = edit.value.substring(0, start) +"
    "  suggestionValue "
    + (!options.appendReplacedText.empty()
       ? "+ '" + options.appendReplacedText + "'"
       : "") +
    " + edit.value.substring(end, edit.value.length);"
    " if (edit.selectionStart) {"
    "   edit.selectionStart = start + suggestionValue.length"
    + (!options.appendReplacedText.empty()
       ? "+ " + boost::lexical_cast<std::string>(2)
       : "") + ";"
    "   edit.selectionEnd = start + suggestionValue.length"
    + (!options.appendReplacedText.empty()
       ? "+ " + boost::lexical_cast<std::string>(2)
       : "") + ";"
    " }"
    "}";
}

}
