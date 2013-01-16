
#include <string>
#include <map>
#include <list>
#include <vector>
#include "addonapi.h"

using std::string;
using std::map;
using std::vector;

typedef std::map<string,string> StringMap;
typedef std::map< string, StringMap > ForestMap;
typedef std::vector< StringMap >	ForestVector;


namespace addon_func{

	void SetSvdbAddr();
	std::string GetSvdbAddr();
}

