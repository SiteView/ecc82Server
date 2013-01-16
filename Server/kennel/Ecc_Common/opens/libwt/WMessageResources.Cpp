/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>
//#include <libxml++/libxml++.h>
//#include <libxml++/parsers/textreader.h>

#include "WMessageResources"
#include "WMessage"
#include "WApplication"

WMessageResources::WMessageResources(const std::string path)
  : path_(path)
{
  refresh();
}

void WMessageResources::refresh()
{
  readResourceFile("", defaults_);
  readResourceFile(wApp->locale(), local_);
}

bool WMessageResources::getValue(const WMessage& message,
				 std::string& result) const
{
  std::string locale = wApp->locale();

  KeyValueMap::const_iterator j;

  j = local_.find(message.key());
  if (j != local_.end()) {
    result = j->second;
    return true;
  }

  j = defaults_.find(message.key());
  if (j != defaults_.end()) {
    result = j->second;
    return true;
  }

  return false;
}

void WMessageResources::readResourceFile(std::string locale,
					 KeyValueMap& valueMap)
{
/*  std::string fileName
    = path_ + (locale.length() > 0 ? "_" : "") + locale + ".xml";

  valueMap.clear();

  try {
    xmlpp::TextReader reader(fileName);

    while (reader.read()) {
      int depth = reader.get_depth();
      if ((depth == 1) && (reader.get_name() == "message")) {
	std::string key;
	if (reader.has_attributes()) {
	  reader.move_to_first_attribute();
	  if (reader.get_name() == "id")
	    key = reader.get_value();
	  else {
	    std::cerr << "Error in resource file " << fileName
		      << ":" << reader.get_current_node()->get_line()
		      << ": need id for message node." << std::endl;
	    return;
	  }
	  reader.move_to_element();
	} else {
	  std::cerr << "Error in resource file " << fileName
		    << ":" << reader.get_current_node()->get_line()
		    << ": need id for message node." << std::endl;
	  return;
	}

	if (valueMap.find(key) == valueMap.end())
	  valueMap[key] = reader.read_inner_xml();
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }
  */
}
