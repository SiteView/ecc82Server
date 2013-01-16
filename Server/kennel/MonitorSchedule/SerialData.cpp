#include "SerialData.h"


SerialData::~SerialData(void)
{
}
bool SerialData::SerialStringMap(STRINGMAP &strmap,FILE *pf)
{
	STRINGMAP::iterator it;
	while(strmap.findnext(it))
	{
		fprintf(pf,"%s=%s\r\n",(*it).getkey().getword(),(*it).getvalue().getword());
	}

	return true;
}
bool SerialData::CovertStringToMap(string str,STRINGMAP &smap)
{
	int pos=0;
	if((pos=(int)str.find("//"))==0)
		return false;

	pos=(int)str.rfind("\r\n");
	str.replace(pos,2,"");

	if((pos=(int)str.find("="))<0)
		return false;



	string name=str.substr(0,pos);
	if(name.empty())
		return false;
	string value=str.substr(pos+1,str.size());
	smap[name.c_str()]=value.c_str();
		
	return true;
}
int SerialData::Serial(FILE *pf,CMonitorList &MonitorList,Groups *pGroups,Subsequent *pSubsequent,TASKMAP &TaskMap,bool out)
{
	if(pf==NULL)
		return -2;

	STRINGMAP strmap;

	if(out)
	{
		fputs("//****SiteView ECC serial data file****\r\n",pf);
		fputs("//****Data version 7.01****\r\n",pf);
		fputs("[MonitorList_Begin]\r\n",pf);

		CMonitorList::iterator it;
		for(it=MonitorList.begin();it!=MonitorList.end();it++)
		{
			strmap.clear();
			if(!(*it)->ToTextMap(strmap))
				continue;
			fputs("[Monitor_Begin]\r\n",pf);
			SerialStringMap(strmap,pf);
			fputs("[Monitor_End]\r\n",pf);
		}
		fputs("[MonitorList_End]\r\n",pf);

		strmap.clear();
		if(!pGroups->ToTextMap(strmap))
		{
			fclose(pf);
			return -3;
		}

		fputs("[Groups_Begin]\r\n",pf);
		SerialStringMap(strmap,pf);
		fputs("[Groups_End]\r\n",pf);

		strmap.clear();
		if(!pSubsequent->ToTextMap(strmap))
		{
			fclose(pf);
			return -4;
		}

		fputs("[Subsequent_Begin]\r\n",pf);
		SerialStringMap(strmap,pf);
		fputs("[Subsequent_End]\r\n",pf);

		fputs("[TaskMap_Begin]\r\n",pf);

		TASKMAP::iterator tit;
		Task *ptask=NULL;
		while(TaskMap.findnext(tit))
		{
	    	strmap.clear();
			if((ptask=(*tit).getvalue())==NULL)
				continue;
			if(!ptask->ToTextMap(strmap))
				continue;
			fputs("[Task_Begin]\r\n",pf);
			SerialStringMap(strmap,pf);
			fputs("[Task_End]\r\n",pf);

		}

		fputs("[TaskMap_End]\r\n",pf);

		fclose(pf);


	}else
	{
		string sline;
		char line[2048]={0};
		char *pc=NULL;
		pc=fgets(line,2048,pf);
		if(pc==NULL)
			return -5;
		sline=line;
		int pos=0;
		if((pos=(int)sline.find("//"))!=0)
			return -6;
		if((pos=(int)sline.find("SiteView ECC serial data file"))<0)
			return -7;
		pc=fgets(line,2048,pf);
		if(pc==NULL)
			return -10;

		sline=line;
		if((pos=(int)sline.find("//"))!=0)
			return -8;
		if((pos=(int)sline.find("Data version 7.01"))<0)
			return -9;

		int i=0;
		bool flag=false;

		while(fgets(line,2048,pf)!=NULL)
		{
			sline=line;
			switch(i)
			{
			case	0:
				if((pos=(int)sline.find("[MonitorList_Begin]"))!=0)
				{
					continue;
				}
				else
				{
					flag=true;
					while(true)
					{
						bool mflag=false;
						strmap.clear();
						while((pc=fgets(line,2048,pf))!=NULL)
						{
							sline=line;
							if((pos=(int)sline.find("[Monitor_Begin]"))!=0)
							{
								if((pos=(int)sline.find("[MonitorList_End]"))==0)
								{
									flag=false;
									i++;
									break;
								}
								continue;
							}
							else
								break;
						}
						if(pc==NULL)
							return -11;
						if(!flag)
							break;

						while((pc=fgets(line,2048,pf))!=NULL)
						{
							sline=line;
							if((pos=(int)sline.find("[Monitor_End]"))==0)
							{
								mflag=true;
								break;
							}
							CovertStringToMap(sline,strmap);
						}
						if(pc==NULL)
							return -12;
						if(mflag)
						{
							Monitor *pm=new Monitor();
							if(Univ::seid!=1)
								pm->m_isRefresh= true;
							if(pm->FromTextMap(strmap))
								MonitorList.push_back(pm);
						}
					}
				}
				break;
			case		1:
				if((pos=(int)sline.find("[Groups_Begin]"))!=0)
				{
					continue;
				}else
				{
					strmap.clear();
					flag=false;
					while((pc=fgets(line,2048,pf))!=NULL)
					{
						sline=line;
						if((pos=(int)sline.find("[Groups_End]"))==0)
						{
							i++;
							flag=true;
							break;
						}
						CovertStringToMap(sline,strmap);
					}
					if(pc==NULL)
						return -13;
					if(flag)
						pGroups->FromTextMap(strmap);

				}
				break;
			case	2:
				if((pos=(int)sline.find("[Subsequent_Begin]"))!=0)
				{
					continue;
				}else
				{
					strmap.clear();
					flag=false;
					while((pc=fgets(line,2048,pf))!=NULL)
					{
						sline=line;
						if((pos=(int)sline.find("[Subsequent_End]"))==0)
						{
							i++;
							flag=true;
							break;
						}
						CovertStringToMap(sline,strmap);
					}
					if(pc==NULL)
						return -13;
					if(flag)
						pSubsequent->FromTextMap(strmap);
				}
				break;
			case	3:
				if((pos=(int)sline.find("[TaskMap_Begin]"))!=0)
				{
					continue;
				}else
				{
					flag=true;
					while(true)
					{
						bool mflag=false;
						strmap.clear();
						while((pc=fgets(line,2048,pf))!=NULL)
						{
							sline=line;
							if((pos=(int)sline.find("[Task_Begin]"))!=0)
							{
								if((pos=(int)sline.find("[TaskMap_End]"))==0)
								{
									flag=false;
									i++;
									break;
								}
								continue;
							}
							else
								break;
						}
						if(pc==NULL)
							return -11;
						if(!flag)
						{
							return 0;
						}

						while((pc=fgets(line,2048,pf))!=NULL)
						{
							sline=line;
							if((pos=(int)sline.find("[Task_End]"))==0)
							{
								mflag=true;
								break;
							}
							CovertStringToMap(sline,strmap);
						}
						if(pc==NULL)
							return -12;
						if(mflag)
						{
							Task *pm=new Task();
							if(pm->FromTextMap(strmap))
								TaskMap[pm->m_taskname.c_str()]=pm;
						}


					}
				}
				break;
			case	4:
				return 0;
			default :break;
			}

		}

	}

	return 0;

}
int SerialData::Serial(CMonitorList &MonitorList,Groups *pGroups,Subsequent *pSubsequent,TASKMAP &TaskMap,bool out)
{
	if(m_filename.empty())
		return -1;

	FILE *pf=NULL;
	if(out)
		pf=fopen(m_filename.c_str(),"w");
	else
		pf=fopen(m_filename.c_str(),"r");

	if(pf==NULL)
		return -2;

	int nret=Serial(pf,MonitorList,pGroups,pSubsequent,TaskMap,out);

	fclose(pf);

	return nret;
}
