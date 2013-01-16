#include "strkey.h"

using namespace svutil;

word::word(const char* wd){
	if(wd)
		m_word=strdup(wd);
	else
		m_word=strdup("");
}
word::word(const word& wd)
{
	if(wd.m_word!=NULL)
		m_word=strdup(wd.m_word);
	else
		m_word=strdup("");

}
word::word(const std::string &str)
{
	const char *p=str.c_str();
	if(p)
		m_word=strdup(p);
	else
		m_word=strdup("");
}


word& word::operator=(const char *p)
{
	if(m_word)
		free(m_word);

	if(p)
		m_word=strdup(p);
	else
		m_word=strdup("");
	return *this;
}

word& word::operator=(const word& wd)
{
	if(m_word)
		free(m_word);

	if(wd.m_word!=NULL)
		m_word=strdup(wd.m_word);
	else
		m_word=strdup("");
	return *this;
}

bool word::operator==(const word &wd) const
{
	if(m_word==NULL||wd.m_word==NULL)
		return false;
	return strcmp(m_word,wd.m_word)==0;
}

bool word::operator<(const word &wd) const
{
	if(m_word==NULL||wd.m_word==NULL)
		return false;
	int count=strlen(m_word);
	int index=strlen(wd.m_word);
	count=(count<=index) ? count : index;
	for(int i=0;i<=count;i++)
	{
		if(m_word[i]<wd.m_word[i])
			return true;
		else if(m_word[i]==wd.m_word[i])
			continue;
		else
			return false;
	}

	if(count>=index)
		return false;
	else
		return true;

}

bool word::operator>(const word &wd) const
{
	if(m_word==NULL||wd.m_word==NULL)
		return false;

	int count=strlen(m_word);
	int index=strlen(wd.m_word);
	count=(count<=index) ? count : index;
	for(int i=0;i<=count;i++)
	{
		if(m_word[i]>wd.m_word[i])
			return true;
		else if(m_word[i]==wd.m_word[i])
			continue;
		else
			return false;
	}

	if(count<=index)
		return false;
	else
		return true;

}

bool word::operator>=(const word &wd) const
{
	if(m_word==NULL||wd.m_word==NULL)
		return false;

	int count=strlen(m_word);
	int index=strlen(wd.m_word);
	count=(count<=index) ? count : index;
	for(int i=0;i<=count;i++)
	{
		if(m_word[i]>wd.m_word[i])
			return true;
		else if(m_word[i]==wd.m_word[i])
			continue;
		else
			return false;
	}

	if(count<index)
		return false;
	else
		return true;


}

bool word::operator<=(const word &wd) const
{
	if(m_word==NULL||wd.m_word==NULL)
		return false;
	int count=strlen(m_word);
	int index=strlen(wd.m_word);
	count=(count<=index) ? count : index;
	for(int i=0;i<=count;i++)
	{
		if(m_word[i]<wd.m_word[i])
			return true;
		else if(m_word[i]==wd.m_word[i])
			continue;
		else
			return false;
	}

	if(count>index)
		return false;
	else
		return true;

}






