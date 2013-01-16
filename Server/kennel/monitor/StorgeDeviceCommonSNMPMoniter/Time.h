#ifndef	SVDB_TIME_
#define SVDB_TIME_
#include <ctime>
#include <iostream>
#include <string.h>



using namespace std;

namespace svutil{
	class TTimeSpan
	{
	public:
		TTimeSpan(){ m_timeSpan=0;}
		TTimeSpan(time_t time)
		{
			if(time<=0)
				time=0;
			m_timeSpan=time;
		}
		TTimeSpan(const TTimeSpan& timeSpanSrc)
		{
			this->m_timeSpan=timeSpanSrc.GetTotalSeconds();
		}
		const TTimeSpan& operator=(const TTimeSpan& timeSpanSrc)
		{
			this->m_timeSpan=timeSpanSrc.GetTotalSeconds();
			return *this;
		}


		TTimeSpan(long days,int hours,int mins,int sec)
		{
			m_timeSpan=days*24*60*60+hours*60*60+mins*60+sec;
		}

		long GetTotalSeconds()const
		{
			return (long)m_timeSpan;
		}
		long GetTotalMinutes()const
		{
			if(m_timeSpan<=0)
				return 0;
			return (long)m_timeSpan/60;
		}
		long GetDays()const
		{
			if(m_timeSpan<=0)
				return 0;
			return (long)((m_timeSpan/60)/60)/24;
		}

		int GetHours() const
		{
			return (int)((m_timeSpan%(60*60*24))/(60*60));
		}
		int GetMinutes() const
		{
			return (int)((m_timeSpan%(60*60))/60);
		}
		int GetSeconds() const
		{
			return (int)(m_timeSpan%60);

		}

		TTimeSpan operator-(TTimeSpan timeSpan) const
		{
			TTimeSpan ttm(this->m_timeSpan-timeSpan.m_timeSpan);
			return ttm;
		}
		TTimeSpan operator+(TTimeSpan timeSpan) const
		{
			TTimeSpan ttm(this->m_timeSpan+timeSpan.m_timeSpan);
			return ttm;
		}
		const TTimeSpan& operator+=(TTimeSpan timeSpan)
		{
			this->m_timeSpan+=timeSpan.m_timeSpan;
			return *this;
		}
		const TTimeSpan& operator-=(TTimeSpan timeSpan)
		{
			this->m_timeSpan-=timeSpan.m_timeSpan;
			return *this;
		}
		bool operator==(TTimeSpan timeSpan) const
		{
			return this->m_timeSpan==timeSpan.m_timeSpan;
		}
		bool operator!=(TTimeSpan timeSpan) const
		{
			return this->m_timeSpan!=timeSpan.m_timeSpan;
		}
		bool operator<(TTimeSpan timeSpan) const
		{
			return this->m_timeSpan<timeSpan.m_timeSpan;
		}
		bool operator>(TTimeSpan timeSpan) const
		{
			return this->m_timeSpan>timeSpan.m_timeSpan;
		}
		bool operator<=(TTimeSpan timeSpan) const
		{
			return this->m_timeSpan<=timeSpan.m_timeSpan;
		}
		bool operator>=(TTimeSpan timeSpan) const
		{
			return this->m_timeSpan>=timeSpan.m_timeSpan;

		}



	private:
		time_t m_timeSpan;
		friend class TTime;

	};

	class TTime
	{
	public:
		TTime(void){m_time=0;}
		~TTime(void){;}

		TTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
			int nDST = -1)
		{

			struct tm ttm;
			nYear=nYear<1970?1970:nYear;
			nYear=nYear>2036?2036:nYear;

			if(nMonth<1||nMonth>12)
				nMonth=1;
			if(nDay<1||nDay>31)
				nDay=1;
			if(nHour<0||nHour>23)
				nHour=1;
			if(nMin<0||nMin>59)
				nMin=1;
			if(nSec<0||nSec>59)
				nSec=1;

			ttm.tm_year=nYear-1900;
			ttm.tm_mon=nMonth-1;
			ttm.tm_mday=nDay;
			ttm.tm_hour=nHour;
			ttm.tm_min=nMin;
			ttm.tm_sec=nSec;
			ttm.tm_isdst=-1;
			m_time=mktime(&ttm);

		}
		TTime(time_t tt)
		{
			if(tt>0)
			{
				m_time=tt;
			}else
				m_time=0;
		}
		TTime(struct tm &ttm)
		{
			m_time=mktime(&ttm);
		}

		TTime(const TTime &ttm)
		{
			m_time=ttm.m_time;
		}

		const TTime& operator=(const TTime &srctime)
		{
			this->m_time=srctime.GetTime();
			return *this;
		}

		static TTime GetCurrentTimeEx(){
			time_t tt;
			time(&tt);
			TTime ttm(tt);
			return ttm;

		};

		time_t GetTime() const
		{
			return m_time;
		}

		TTimeSpan operator-(TTime time) const
		{
			time_t ttem=this->m_time-time.GetTime();
			TTimeSpan ts(ttem);
			return ts;
		}
		TTime operator-(TTimeSpan timeSpan) const
		{
			TTime tmm(this->m_time-timeSpan.m_timeSpan);
			return tmm;
		}

		TTime operator+(TTimeSpan timeSpan) const
		{
			TTime tmm(this->m_time+timeSpan.m_timeSpan);
			return tmm;
		}

		const TTime& operator+=(TTimeSpan timeSpan)
		{
			this->m_time+=timeSpan.m_timeSpan;
			return *this;

		}

		const TTime& operator-=(TTimeSpan timeSpan)
		{
			this->m_time-=timeSpan.m_timeSpan;
			return *this;
		}

		bool operator==(TTime time) const
		{
			return this->m_time==time.GetTime();
		}
		bool operator!=(TTime time) const
		{
			return this->m_time!=time.GetTime();
		}
		bool operator<(TTime time) const
		{
			return this->m_time<time.GetTime();
		}
		bool operator>(TTime time) const
		{
			return this->m_time>time.GetTime();
		}
		bool operator<=(TTime time) const
		{
			return this->m_time<=time.GetTime();
		}
		bool operator>=(TTime time) const
		{
			return this->m_time>=time.GetTime();
		}

		int GetYear() const
		{
			struct tm *ttm=localtime(&m_time);
			return ttm->tm_year+1900;
		}

		int GetMonth() const
		{
			struct tm *ttm=localtime(&m_time);
			return ttm->tm_mon+1;

		}

		int GetDay() const
		{
			struct tm *ttm=localtime(&m_time);
			return ttm->tm_mday;
		}

		int GetHour() const
		{
			struct tm *ttm=localtime(&m_time);
			return ttm->tm_hour;

		}
		int GetMinute() const
		{
			struct tm *ttm=localtime(&m_time);
			return ttm->tm_min;

		}
		int GetSecond() const
		{
			struct tm *ttm=localtime(&m_time);
			return ttm->tm_sec;

		}
		int GetWeekDay() const
		{
			struct tm *ttm=localtime(&m_time);
			return ttm->tm_wday;
			
		}
		int GetYearDay() const
		{
			struct tm *ttm=localtime(&m_time);
			return ttm->tm_yday;

		}

		std::string Format(void) const
		{
			struct tm *ttm=localtime(&m_time);

			char buf[100]={0};
			sprintf(buf,"%d-%02d-%02d %02d:%02d:%02d",ttm->tm_year+1900,
				ttm->tm_mon+1,
				ttm->tm_mday,
				ttm->tm_hour,
				ttm->tm_min,
				ttm->tm_sec);
			return (std::string)buf;
		}



	private:
		time_t m_time;

	};
}

#endif