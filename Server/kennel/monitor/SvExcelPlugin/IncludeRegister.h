#ifndef INCLUDE_REGISTER
#define INCLUDE_REGISTER

#include <set>
#include <map>
#include <string>

class IncludeRegistry : public Singleton<IncludeRegistry>
{
public: 
    friend class Singleton<IncludeRegistry>;
	
	void Register(const std::string& arg, const std::string& include);
	void UseArg(const std::string& arg);

	std::set<std::string> GetIncludes() const; 

private:
	IncludeRegistry()
	{}
	IncludeRegistry(const IncludeRegistry&)
	{}

	std::map<std::string,std::string> ArgInclude;
	std::map<std::string, bool> ArgUsed;

};
#endif
