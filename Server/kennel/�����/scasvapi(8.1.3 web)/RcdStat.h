
#ifndef	SITEVIEW_SCASVAPI_RCDSTAT_H
#define	SITEVIEW_SCASVAPI_RCDSTAT_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


class FieldStat
{
public:
	~FieldStat(){}

	void AddCompressValue(int value, int sum, string time, int compress)
	{
		if(bmin)
		{
			imin= value;
			bmin= false;
		}
		if(value>imax)
		{
			imax= value;
			when_max= time;
		}
		if(value<imin)
			imin= value;
		isFloat= false;

		++ftemp_count;
		ftemp += ((float)value)/compress;
		if( (sum%compress)==0 )
		{
			int tsum= sum/compress;
			if(ftemp_count<compress)
				ftemp= ftemp * compress/ftemp_count;

			int itemp(ftemp+0.5);
			if(tsum!=0)
				average= (int)((average*(tsum-1)+itemp)/tsum + 0.5);

			if( tsum<300 )
			{
				ostringstream temp; 
				if(ftemp_count>=compress)
					temp<<time.c_str()<<"="<<itemp<<",";
				else
					temp<<time.c_str()<<"="<<lastStatus.c_str()<<",";
				detail+= temp.str();
			}
			ftemp=0;
			ftemp_count=0;
		}
	}

	void AddCompressValue(float value, int sum, string time, int compress)
	{
		if(bmin)
		{
			fmin= value;
			bmin= false;
		}
		if(value>fmax)
		{
			fmax= value;
			when_max= time;
		}
		if(value<fmin)
			fmin= value;
		isFloat= true;

		++ftemp_count;
		ftemp += value/compress;
		if( (sum%compress)==0 )
		{
			int tsum= sum/compress;
			if(ftemp_count<compress)
				ftemp= ftemp * compress/ftemp_count;

			if(tsum!=0)
				average= (average*(tsum-1)+ftemp)/tsum;

			if( tsum<300 )
			{
				ostringstream temp; 
				if(ftemp_count>=compress)
					temp<<time.c_str()<<"="<<ftemp<<",";
				else
					temp<<time.c_str()<<"="<<lastStatus.c_str()<<",";
				detail+= temp.str();
			}
			ftemp=0;
			ftemp_count=0;
		}
	}

	void AddCompressBadStatu(int sum, string time, string status, int compress)
	{
		if( (sum%compress)==0 )
		{
			lastStatus= status;
			ftemp=0;
			ftemp_count=0;
			int tsum= sum/compress;

			if( tsum<300 )
			{
				ostringstream temp; 
				temp<<time.c_str()<<"="<<status.c_str()<<",";
				detail+= temp.str();
			}
		}
	}

	void AddBadStatu(int sum, string time, string status)
	{
		if(sum>50000)
			return;
		ostringstream temp; 
		temp<<time.c_str()<<"="<<status.c_str()<<",";
		detail+= temp.str();
	}

	void AddValue(int value, int sum, string time)
	{
		if(bmin)
		{
			imin= value;
			bmin= false;
		}
		if(value>imax)
		{
			imax= value;
			when_max= time;
		}
		if(value<imin)
			imin= value;
		isFloat= false;

		if(sum==0)
			return;
		average= (average*(sum-1)+value)/sum;
		
		if(sum>50000)
			return;
		ostringstream temp; 
		temp<<time.c_str()<<"="<<value<<",";
		detail+= temp.str();
	}

	void AddValue(float value, int sum, string time)
	{
		if(bmin)
		{
			fmin= value;
			bmin= false;
		}
		if(value>fmax)
		{
			fmax= value;
			when_max= time;
		}
		if(value<fmin)
			fmin= value;
		isFloat= true;

		if(sum==0)
			return;
		average= (average*(sum-1)+value)/sum;

		if(sum>50000)
			return;
		ostringstream temp; 
		temp<<time.c_str()<<"="<<value<<",";
		detail+= temp.str();
	}

	void AddLatest(int value)
	{
		isFloat= false;
		ilatest =value; 
	}

	void AddLatest(float value)
	{
		isFloat= true;
		flatest =value; 
	}

	bool GetIsFloat()
	{
		return isFloat;
	}

	void Display(bool show=false)
	{
		cout<<"average:"<<average<<"  when_max:"<<when_max.c_str()<<"  fmax:"<<fmax<<"  imax:"<<imax<<"  fmin:"<<fmin<<"  imin:"<<imin<<endl;
		string strshow(detail);
		for(string::size_type index=0; index !=strshow.size(); ++index)
		{
			if(strshow[index]==',')
				strshow[index]='\n';
		}
		if(show)
			cout<<"sv_name:"<<sv_name.c_str()<<"  sv_label:"<<sv_label.c_str()<<"  \ndetail:\n"<<strshow.c_str()<<endl;
		else
			cout<<"sv_name:"<<sv_name.c_str()<<"  sv_label:"<<sv_label.c_str()<<endl;
	}

	void Get(string & inaverage,string & out_when_max,string & max,string & min,string & latest,string & indetail)
	{
		ostringstream t1,t2,t3,t4,t5, t6,t7; 
		t1<<average;
		inaverage= t1.str();

		if(isFloat)
		{
			t2<<fmax;
			max= t2.str();
			t3<<fmin;
			min= t3.str();
			t6<<flatest;
			latest= t6.str();
		}
		else
		{
			t4<<imax;
			max= t4.str();
			t5<<imin;
			min= t5.str();
			t7<<ilatest;
			latest= t7.str();
		}
		indetail= detail;
		out_when_max= when_max;
	}

	FieldStat()
	{
		average=0;
		fmax=fmin=0;
		imax=imin=0;
		bmin=true;

		isFloat= false;
		ftemp=0;
		ftemp_count=0;

		ilatest=0;
		flatest=0;

	}
	string sv_label, sv_name;

private:
	string detail;
	float average;
	float fmax, fmin;
	int   imax, imin;
	bool  bmin;

	string when_max;

	bool  isFloat;
	float ftemp;
	string lastStatus;
	int   ftemp_count;

	int ilatest;
	float flatest;
};


class MonitorStat
{
public:
	MonitorStat(string id)
	{
		m_id= id;
		okP=warnP=errorP=0;
		sumForSatus=sum=0;
		compress=1;
	}
	MonitorStat(string id,int fieldCount)
	{
		m_id= id;
		okP=warnP=errorP=0;
		sumForSatus=sum=0;

		for(int i=1; i<=fieldCount; ++i)
		{
			FieldStat vs;
			fields.push_back(vs);
		}
	}
	~MonitorStat(){}

	void SetCompress(int in)
	{
		compress= in;
	}
	
	void SetFieldCount(int fieldCount)
	{
		if(!fields.empty())
			return;

		for(int i=1; i<=fieldCount; ++i)
		{
			FieldStat vs;
			fields.push_back(vs);
		}
	}

	void GetStat(string & id, float & ok, float & warn, float & error)
	{
		id=m_id;  ok=okP;  warn=warnP;  error=errorP;
	}

	vector<FieldStat> & GetFields()
	{
		return fields;
	}

	FieldStat & GetValue(int index)
	{
		return fields[index];
	}

	bool AddState(int rstate, string time, int & outsum)
	{
		nowTime= time;
		//if( rstate==0 )		  status= "null";
		//else if( rstate==1 )   status= "ok";
		//else if( rstate==2 )   status= "warning";
		//else if( rstate==3 )   status= "error";
		//else if( rstate==4 )   status= "disable";
		//else if( rstate==5 )   status= "bad";
		//else	              status= "GetRecordState failed.";
		++sumForSatus;
		outsum= sumForSatus;
		if(sumForSatus==0)
			return false;

		if( rstate==0 )
		{
			SetBadStatus("null");
			return false;
		}
		if( rstate==4 )
		{
			SetBadStatus("disable");
			return false;
		}
		if( rstate>=5 || rstate<1 )
		{
			SetBadStatus("bad");
			return false;
		}

		//没数据时不参与平均值计算

		++sum;
		if(sum==0)
			return false;

		if( rstate==1 )
		{
			okP= (okP*(sum-1)+1)/sum;
			warnP= warnP*(sum-1)/sum;
			errorP= errorP*(sum-1)/sum;
			return true;
		}
		if( rstate==2 )
		{
			warnP= (warnP*(sum-1)+1)/sum;
			okP=   okP*(sum-1)/sum;
			errorP= errorP*(sum-1)/sum;
			return true;
		}
		errorP= (errorP*(sum-1)+1)/sum;
		okP= okP*(sum-1)/sum;
		warnP= warnP*(sum-1)/sum;
		return true;
	}

	void AddValue(int index, int value)
	{
		if(compress<2)
			fields[index].AddValue(value, sum, nowTime); 
		else
			fields[index].AddCompressValue(value, sum, nowTime, compress);
	}

	void AddValue(int index, float value)
	{
		if(compress<2)
			fields[index].AddValue(value, sum, nowTime);
		else
			fields[index].AddCompressValue(value, sum, nowTime, compress);
	}

	void SetFieldKey(int index, string sv_name)
	{
		fields[index].sv_name= sv_name;
	}

	void AddLatest(int index, int value)
	{
		fields[index].AddLatest(value); 
	}

	void AddLatest(int index, float value)
	{
		fields[index].AddLatest(value); 
	}

	void Display(bool show=false)
	{
		cout<<"id: "<<m_id.c_str()<<"  ok:"<<okP<<"  warn:"<<warnP<<"  error:"<<errorP<<"  record_count:"<<sum<<"("<<sumForSatus<<")  field_count:"<<fields.size()<<"  compress_ratio:"<<compress<<endl;
		if(!show)
			return;

		int index(0);
		for(vector<FieldStat>::iterator it= fields.begin(); it!=fields.end(); ++it)
		{
			cout<<endl<<"index: "<<index++<<"  ";
			(*it).Display(true);
		}
	}

private:
	void SetBadStatus(string status)
	{
		int index(0);
		for(vector<FieldStat>::iterator it= fields.begin(); it!=fields.end(); ++it, index++)
		{
			if(compress<2)
				fields[index].AddBadStatu(sumForSatus, nowTime, "(status)"+status);
			else
				fields[index].AddCompressBadStatu(sumForSatus, nowTime, "(status)"+status, compress);
		}
	}


	int compress;
	string m_id, nowTime;
	float okP,warnP,errorP;
	int sum,sumForSatus;

	vector<FieldStat> fields;

};


#endif