#ifndef QUERYWMI_91FCEF3D1A0E4e16AE0664D0D11B4E78
#define QUERYWMI_91FCEF3D1A0E4e16AE0664D0D11B4E78

#include <string>
#include <map>
#include <vector>
#import "progid:WbemScripting.SWbemLocator" named_guids

class QueryWMI
{
public:
	QueryWMI(std::string const & svrName = ".", 
				std::string const & name = "", 
				std::string const & password = "", 
				std::string const & nameSp = "root\\cimv2", 
				std::string const & queryLanguage = "WQL");
	~QueryWMI();
	inline std::string setQueryLang(std::string const & queryLanguage);
	unsigned int execQuery(std::string strQuery);
	bool beginRetrieve();
	bool next(std::map<std::string, std::string> & returnMap, std::vector<std::string> const & paramVec);
private:
	WbemScripting::ISWbemServicesPtr i_services;
	WbemScripting::ISWbemObjectSetPtr i_objSet;
	std::string i_queryLang;
	IEnumVARIANTPtr i_enum;
};
#endif