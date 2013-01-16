// SvExcelPlugin.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "SvExcelPlugin.h"
#include "svdbapi.h"

#include <xlw/pragmas.h>
#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include <xlw/xlw.h>
#include <xlw/XlFunctionRegistration.h>
#include <xlw/XlOpenClose.h>
#include <xlw/ArgList.h>
#include <xlw/ArgListFactory.h>
#include <xlw/xlarray.h>
#include <sstream>
#include <map>

using namespace svutil;
using std::istringstream;
using std::string;
using std::map;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CSvExcelPluginApp

BEGIN_MESSAGE_MAP(CSvExcelPluginApp, CWinApp)
END_MESSAGE_MAP()

namespace
{
	enum FazhiType_T {FZ_NORMAL, FZ_WARNING, FZ_ERROR};
	struct FazhiIndex_T
	{
		string i_address;
		string i_monitorId;
		FazhiType_T i_type;
		FazhiIndex_T(string const & address,
			string const & monitorId,
			FazhiType_T type) :
		i_address(address), i_monitorId(monitorId)
		{
			i_type = type;
		}
	};
	bool operator <(FazhiIndex_T const & fz1, FazhiIndex_T const & fz2)
	{
		if (fz1.i_address < fz2.i_address)
		{
			return true;
		} 
		else if (fz1.i_address > fz2.i_address) 
		{
			return false;
		}
		else if (fz1.i_monitorId < fz2.i_monitorId)
		{
			return true;
		}
		else if (fz1.i_monitorId > fz2.i_monitorId)
		{
			return false;
		}
		else
		{
			return fz1.i_type < fz2.i_type;
		}
	}
	map<FazhiIndex_T, string> s_fazhiMap;
}

// CSvExcelPluginApp ����

CSvExcelPluginApp::CSvExcelPluginApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSvExcelPluginApp ����

CSvExcelPluginApp theApp;


// CSvExcelPluginApp ��ʼ��

BOOL CSvExcelPluginApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

struct PercentState_T
{
	double normalPer;
	double warningPer;
	double errorPer;
} const g_psZero = {0, 0, 0};

struct MaxAvgMin_T
{
	double maxValue;
	double avgValue;
	double minValue;
} const g_mamZero = {0, 0, 0};

string getParentString(string const & id)
{
	static const basic_string <char>::size_type npos = -1;
	basic_string <char>::size_type pos;
	pos = id.find_last_of(".");
	if (pos != npos)
	{
		return id.substr(0, pos);
	}
	else
	{
		return "";
	}
}

bool getTime(string const & str, TTime & t)
{
	istringstream iss(str);

	char c = '*';      //��ʼ�ַ�������Ϊ' '��'-'��':'�⣬����ȡ
	int year;
	iss >> year >> c;
	if (c != '-')
	{
		return false;
	}

	int month;
	c = '*';
	iss >> month >> c;
	if (c != '-')
	{
		return false;
	}

	int day;
	c = '*';
	iss >> day;
	iss.get(c);
	if (c != ' ')
	{
		return false;
	}

	int hour;
	c = '*';
	iss >> hour >> c;
	if (c != ':')
	{
		return false;
	}

	int minute;
	c = '*';
	iss >> minute >> c;
	if (c != ':')
	{
		return false;
	}

	int second;
	iss >> second;
	if (!iss)
	{
		return false;
	}

	t = TTime(year, month, day, hour, minute, second);
	return true;
}

void PushMatrixMonitorData(CellMatrix & cm, 
													 unsigned long col, 
													 string const & address, 
													 string const & user, 
													 string const & monitorID);

void PushMatrixEntityData(CellMatrix & cm,
													unsigned long col, 
													string const & address, 
													string const & user, 
													string const & entityID);

void PushMatrixGroupData(CellMatrix & cm,  
												 unsigned long col, 
												 string const & address, 
												 string const & user, 
												 string const & groupID);

namespace
{
	const char* LibraryName = "Siteview";
};

namespace
{
	void DummyFunction()
	{
		xlAutoOpen();
		xlAutoClose();
	}
}

//��ʼע��
namespace
{
	XLRegistration::Arg QueryRecordsDataByTimeArgs[]=
	{
		{"monitorID", "�������ڵ��ID"},
		{"startTime", "��ʼʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"endTime", "����ʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryRecordsDataByTime
		(
		"xlQueryRecordsDataByTime",
		"QueryRecordsDataByTime",
		"������ʼʱ�䣬����ָ���������ļ������ݣ����ж��У�",
		LibraryName,
		QueryRecordsDataByTimeArgs,
		"RRRRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryRecordsDataByTime
		(
		LPXLOPER monitorIDa,
		LPXLOPER startTimea,
		LPXLOPER endTimea,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDb(monitorIDa);
		XlfOper startTimeb(startTimea);
		XlfOper endTimeb(endTimea);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string monitorID(monitorIDb.AsString("monitorID"));
		std::string startTime(startTimeb.AsString("startTime"));
		std::string endTime(endTimeb.AsString("endTime"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if ((monitorID == "") || (startTime == "") || (endTime == ""))
		{
			return XlfOper("������ȱʡֵ�Ĳ�������Ϊ�գ�");
		}
		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		TTime sTime, eTime;
		if (!(getTime(startTime, sTime)) || !(getTime(endTime, eTime)))
		{
			return XlfOper("�����밴��ʽ����ʱ�������");
		}

		RECORDSET rs = QueryRecords(monitorID, sTime, eTime, user, address);

		list<string> fieldlist;
		GetReocrdSetField(rs, fieldlist);
		list<string>::size_type flsize = fieldlist.size();
		list<string>::iterator fl_beg_it = fieldlist.begin();
		list<string>::iterator fl_end_it = fieldlist.end();

		CellMatrix resultCells(0, static_cast<unsigned long>(flsize + 2));
		CellMatrix tmpCells(1, static_cast<unsigned long>(flsize + 2));
		TTime tmpTime;
		int tmpState = 0;
		bool getRecord = false;

		LISTITEM item;
		if (!FindRecordFirst(rs, item))
		{
			return XlfOper("����ȡ��¼ʧ�ܣ�");
		}

		RECORD rd;
		while((rd = FindNextRecord(item))!=INVALID_VALUE)
		{
			if (GetRecordCreateTime(rd, tmpTime))
			{
				tmpCells(0, 0) = tmpTime.Format();
			}
			else
			{
				tmpCells(0, 0) = "ȡʱ��ʧ��";
			}

			if (GetRecordState(rd, tmpState))
			{
				switch(tmpState)
				{
				case 1:
					tmpCells(0, 1) = "����";
					getRecord = true;
					break;
				case 2:
					tmpCells(0, 1) = "Σ��";
					getRecord = true;
					break;
				case 3:
					tmpCells(0, 1) = "����";
					getRecord = true;
					break;
				case 4:
					tmpCells(0, 1) = "��ֹ";
					getRecord = false;
					break;
				case 5:
					tmpCells(0, 1) = "������";
					getRecord = false;
					break;
				default:
					tmpCells(0, 1) = "��";
				}
			}
			else
			{
				tmpCells(0, 1) = "ȡ״̬ʧ��";
				getRecord = false;
			}

			if (getRecord)
			{
				unsigned long i = 2;
				int type;
				int state;
				int iv;
				float fv;
				string sv;
				for (list<string>::iterator it = fl_beg_it; it != fl_end_it; it++)
				{
					if (GetRecordValueByField(rd, *it, type, state, iv, fv, sv))
					{
						switch(type)
						{
						case 1:
							tmpCells(0, i) = static_cast<double>(iv);
							break;
						case 2:
							tmpCells(0, i) = static_cast<double>(fv);
							break;
						case 3:
							tmpCells(0, i) = sv;
							break;
						default:
							tmpCells(0, i) = "";
							break;
						}
					} 
					else
					{
						tmpCells(0, i) = "";
					}				
					++i;
				}
			} 
			else
			{
				for (list<string>::size_type i = 0; i < flsize; ++i)
				{
					tmpCells(0, static_cast<unsigned long>(i+2)) = "";
				}
			}
			resultCells.PushBottom(tmpCells);
		}

		ReleaseRecordList(item);
		CloseRecordSet(rs);

		return XlfOper(resultCells);
		EXCEL_END
	}
}

/////////////////////////////////////////////////////
namespace
{
	XLRegistration::Arg QueryCountByTimeArgs[]=
	{
		{"monitorID", "�������ڵ��ID"},
		{"startTime", "��ʼʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"endTime", "����ʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryCountByTime
		(
		"xlQueryCountByTime",
		"QueryCountByTime",
		"������ʼʱ�䣬����ָ���������ļ������ݼ�¼����",
		LibraryName,
		QueryCountByTimeArgs,
		"RRRRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryCountByTime
		(
		LPXLOPER monitorIDa,
		LPXLOPER startTimea,
		LPXLOPER endTimea,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDb(monitorIDa);
		XlfOper startTimeb(startTimea);
		XlfOper endTimeb(endTimea);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string monitorID(monitorIDb.AsString("monitorID"));
		std::string startTime(startTimeb.AsString("startTime"));
		std::string endTime(endTimeb.AsString("endTime"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if ((monitorID == "") || (startTime == "") || (endTime == ""))
		{
			return XlfOper("������ȱʡֵ�Ĳ�������Ϊ�գ�");
		}
		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		TTime sTime, eTime;
		if (!(getTime(startTime, sTime)) || !(getTime(endTime, eTime)))
		{
			return XlfOper("�����밴��ʽ����ʱ�������");
		}


		RECORDSET rs = QueryRecords(monitorID, sTime, eTime, user, address);
		size_t count = 0;
		if (!GetRecordCount(rs, count))
		{
			return XlfOper("����ȡ��¼����ʧ�ܣ�");
		}
		CloseRecordSet(rs);

		CellMatrix resultCells(static_cast<double>(count));
		return XlfOper(resultCells);
		EXCEL_END
	}
}

/////////////////////////////////////////////////////
namespace
{
	XLRegistration::Arg QueryHeadArgs[]=
	{
		{"monitorID", "�������ڵ��ID"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryHead
		(
		"xlQueryHead",
		"QueryHead",
		"����ָ���������ļ������ݵı�ͷ��һ�ж��У�",
		LibraryName,
		QueryHeadArgs,
		"RRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryHead
		(
		LPXLOPER monitorIDa,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDb(monitorIDa);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string monitorID(monitorIDb.AsString("monitorID"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (monitorID == "")
		{
			return XlfOper("������ȱʡֵ�Ĳ�������Ϊ�գ�");
		}
		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		TTime sTime(2100, 1, 1, 1, 1, 1);


		RECORDSET rs = QueryRecords(monitorID, sTime, sTime, user, address);
		list<string> fieldlist;
		GetReocrdSetField(rs, fieldlist);

		list<string>::size_type flsize = fieldlist.size();
		list<string>::iterator fl_beg_it = fieldlist.begin();
		list<string>::iterator fl_end_it = fieldlist.end();

		CellMatrix resultCells(1, static_cast<unsigned long>(flsize));

		unsigned long i = 0;
		for (list<string>::iterator it = fieldlist.begin(); it != fieldlist.end(); it++)
		{
			resultCells(0, i) = *it;
			i++;
		}

		CloseRecordSet(rs);

		return XlfOper(resultCells);
		EXCEL_END
	}
}

/////////////////////////////////////////////////////
namespace
{
	XLRegistration::Arg GetMonitorTreeArgs[]=
	{
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerGetMonitorTree
		(
		"xlGetMonitorTree",
		"GetMonitorTree",
		"����������ʾ�ļ������б����ж��У�",
		LibraryName,
		GetMonitorTreeArgs,
		"RR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlGetMonitorTree
		(
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		CellMatrix resultCells;
		PAIRLIST selist;
		PAIRLIST::iterator iSe;
		GetAllSVSEInfo(selist, user, address);
		OBJECT root = NULL;
		unsigned long curCol = 0;
		for(iSe = selist.begin(); iSe != selist.end(); iSe++)
		{
			CellMatrix tmpCells(1, curCol+1);
			tmpCells(0, curCol) = iSe->name + " - " + iSe->value;
			resultCells.PushBottom(tmpCells);

			root = GetSVSE(iSe->name, user, address);

			list<string> lsGroupID;
			list<string> lsEntityID;
			list<string>::iterator lstItem;

			if (root != INVALID_VALUE)
			{			
				if(GetSubGroupsIDBySE(root, lsGroupID))
				{
					for(lstItem = lsGroupID.begin(); lstItem != lsGroupID.end(); ++lstItem)
					{
						PushMatrixGroupData(resultCells, curCol+1, address, user, *lstItem);
					}
				}
				if(GetSubEntitysIDBySE(root, lsEntityID))
				{
					for(lstItem = lsEntityID.begin(); lstItem != lsEntityID.end(); ++lstItem)
					{
						PushMatrixEntityData(resultCells, curCol+1, address, user, *lstItem);
					}
				}
				CloseSVSE(root);
				root = NULL;
			}		
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

void
PushMatrixMonitorData(CellMatrix & cm, 
											unsigned long col, 
											string const & address, 
											string const & user, 
											string const & monitorID)
{
	CellMatrix tmpCells(1, col+2);
	OBJECT mobj = GetMonitor(monitorID, user, address);
	MAPNODE mn = GetMonitorMainAttribNode(mobj);
	string value;
	if (!FindNodeValue(mn, "sv_name", value)) value = "ȡ��������ʧ�ܣ�";
	tmpCells(0, col) = monitorID;
	tmpCells(0, col+1) = value;
	cm.PushBottom(tmpCells);
	CloseMonitor(mobj);
}

void
PushMatrixEntityData(CellMatrix & cm,
										 unsigned long col, 
										 string const & address, 
										 string const & user, 
										 string const & entityID)
{
	CellMatrix tmpCells(1, col+1);
	OBJECT eobj = GetEntity(entityID, user, address);
	MAPNODE mn = GetEntityMainAttribNode(eobj);
	string value;
	if (!FindNodeValue(mn, "sv_name", value)) value = "";
	tmpCells(0, col) = entityID + " - " + value;
	cm.PushBottom(tmpCells);

	list<string> lstMonitor;
	list<string>::iterator lstItem;

	if (eobj != INVALID_VALUE)
	{
		if(GetSubMonitorsIDByEntity(eobj, lstMonitor))
		{
			for(lstItem = lstMonitor.begin(); lstItem != lstMonitor.end(); ++lstItem)
			{
				PushMatrixMonitorData(cm, col+1, address, user, *lstItem);
			}
		}
		CloseEntity(eobj);
	}
}

void
PushMatrixGroupData(CellMatrix & cm,  
										unsigned long col, 
										string const & address, 
										string const & user, 
										string const & groupID)
{
	CellMatrix tmpCells(1, col+1);
	OBJECT gobj = GetGroup(groupID, user, address);
	MAPNODE mn = GetGroupMainAttribNode(gobj);
	string value;
	if (!FindNodeValue(mn, "sv_name", value)) value = "";
	tmpCells(0, col) = groupID + " - " + value;
	cm.PushBottom(tmpCells);

	list<string> lstMonitor;
	list<string> lstGroup;
	list<string>::iterator lstItem;

	if (gobj != INVALID_VALUE)
	{
		if(GetSubGroupsIDByGroup(gobj, lstGroup))
		{
			for(lstItem = lstGroup.begin(); lstItem != lstGroup.end(); ++lstItem)
			{
				PushMatrixGroupData(cm, col+1, address, user, *lstItem);
			}
		}
		if(GetSubEntitysIDByGroup(gobj, lstMonitor))
		{
			for(lstItem = lstMonitor.begin(); lstItem != lstMonitor.end(); ++lstItem)
			{
				PushMatrixEntityData(cm, col+1, address, user, *lstItem);
			}
		}
		CloseGroup(gobj);
	}
}

/////////////////////////////////////////////////////
namespace
{
	XLRegistration::Arg GetAllMonitorArgs[]=
	{
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"},
		{"infoName", "ȡ��������ʲô��Ϣ��ȱʡֵΪ��sv_name"},		
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerGetAllMonitor
		(
		"xlGetAllMonitor",
		"GetAllMonitor",
		"�������м�������Ϣ������2�У�",
		LibraryName,
		GetAllMonitorArgs,
		"RRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlGetAllMonitor
		(
		LPXLOPER addressa,
		LPXLOPER usera,
		LPXLOPER infoNamea
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper addressb(addressa);
		XlfOper userb(usera);
		XlfOper infoNameb(infoNamea);

		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));
		std::string infoName(infoNameb.AsString("infoName"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}
		if (infoName == "")
		{
			infoName = "sv_name";
		}

		CellMatrix resultCells;
		PAIRLIST selist;
		PAIRLIST::iterator iSe;
		GetAllMonitorsInfo(selist, infoName, user, address);
		for(iSe = selist.begin(); iSe != selist.end(); iSe++)
		{
			CellMatrix tmpCells(1, 2);
			tmpCells(0, 0) = iSe->name;
			tmpCells(0, 1) = iSe->value;
			resultCells.PushBottom(tmpCells);				
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

/////////////////////////////////////////////////////
namespace
{
	XLRegistration::Arg QueryMachineNamesArgs[]=
	{
		{"monitorIDs", "�������ڵ��ID(����1��)"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryMachineNames
		(
		"xlQueryMachineNames",
		"QueryMachineNames",
		"������������ļ������ڵ�Ļ�������IP��ַ������1�У�",
		LibraryName,
		QueryMachineNamesArgs,
		"PRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryMachineNames
		(
		LPXLOPER monitorIDsa,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDsb(monitorIDsa);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		CellMatrix monitorIDs(monitorIDsb.AsCellMatrix("monitorIDs"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		long row = monitorIDs.RowsInStructure();
		CellMatrix resultCells(row, 1);

		for (long i = 0; i < row; ++i)
		{
			OBJECT eobj = GetEntity(getParentString(monitorIDs(i, 0).StringValue()), user, address);
			MAPNODE en = GetEntityMainAttribNode(eobj);
			string value;
			if (!FindNodeValue(en, "_MachineName", value)) value = "ȡ������������ʧ�ܣ�";
			resultCells(i, 0) = value;
			CloseEntity(eobj);
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

namespace
{
	XLRegistration::Arg QueryMachineNameArgs[]=
	{
		{"monitorID", "�������ڵ��ID"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryMachineName
		(
		"xlQueryMachineName",
		"QueryMachineName",
		"���ؼ������ڵ�Ļ�������IP��ַ",
		LibraryName,
		QueryMachineNameArgs,
		"RRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryMachineName
		(
		LPXLOPER monitorIDa,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDb(monitorIDa);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string monitorID(monitorIDb.AsString("monitorID"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		OBJECT eobj = GetEntity(getParentString(monitorID), user, address);
		MAPNODE en = GetEntityMainAttribNode(eobj);
		string value;
		if (!FindNodeValue(en, "_MachineName", value)) value = "ȡ������������ʧ�ܣ�";
		CloseEntity(eobj);

		CellMatrix resultCells(value);
		return XlfOper(resultCells);
		EXCEL_END
	}
}

/////////////////////////////////////////////////////
namespace
{
	XLRegistration::Arg QueryMonitorNamesArgs[]=
	{
		{"monitorIDs", "�������ڵ��ID(����1��)"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryMonitorNames
		(
		"xlQueryMonitorNames",
		"QueryMonitorNames",
		"������������ļ������ڵ�����ƣ�����1�У�",
		LibraryName,
		QueryMonitorNamesArgs,
		"PRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryMonitorNames
		(
		LPXLOPER monitorIDsa,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDsb(monitorIDsa);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		CellMatrix monitorIDs(monitorIDsb.AsCellMatrix("monitorIDs"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		long row = monitorIDs.RowsInStructure();
		CellMatrix resultCells(row, 1);

		for (long i = 0; i < row; ++i)
		{
			OBJECT mobj = GetMonitor(monitorIDs(i, 0).StringValue(), user, address);
			MAPNODE mn = GetMonitorMainAttribNode(mobj);
			string value;
			if (!FindNodeValue(mn, "sv_name", value)) value = "ȡ����������ʧ�ܣ�";
			resultCells(i, 0) = value;
			CloseMonitor(mobj);
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

namespace
{
	XLRegistration::Arg QueryMonitorNameArgs[]=
	{
		{"monitorID", "�������ڵ��ID"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryMonitorName
		(
		"xlQueryMonitorName",
		"QueryMonitorName",
		"���ؼ������ڵ������",
		LibraryName,
		QueryMonitorNameArgs,
		"RRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryMonitorName
		(
		LPXLOPER monitorIDa,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDb(monitorIDa);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string monitorID(monitorIDb.AsString("monitorID"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		OBJECT mobj = GetMonitor(monitorID, user, address);
		MAPNODE mn = GetMonitorMainAttribNode(mobj);
		string value;
		if (!FindNodeValue(mn, "sv_name", value)) value = "ȡ������������ʧ�ܣ�";
		CloseMonitor(mobj);

		CellMatrix resultCells(value);
		return XlfOper(resultCells);
		EXCEL_END
	}
}

/////////////////////////////////////////////////////
namespace
{
	XLRegistration::Arg QueryEntityNamesArgs[]=
	{
		{"monitorIDs", "�������ڵ��ID(����1��)"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryEntityNames
		(
		"xlQueryEntityNames",
		"QueryEntityNames",
		"������������ļ������ڵ������豸�����⣨����1�У�",
		LibraryName,
		QueryEntityNamesArgs,
		"PRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryEntityNames
		(
		LPXLOPER monitorIDsa,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDsb(monitorIDsa);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		CellMatrix monitorIDs(monitorIDsb.AsCellMatrix("monitorIDs"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		long row = monitorIDs.RowsInStructure();
		CellMatrix resultCells(row, 1);

		for (long i = 0; i < row; ++i)
		{
			OBJECT eobj = GetEntity(getParentString(monitorIDs(i, 0).StringValue()), user, address);
			MAPNODE en = GetEntityMainAttribNode(eobj);
			string value;
			if (!FindNodeValue(en, "sv_name", value)) value = "ȡ������������������ʧ�ܣ�";
			resultCells(i, 0) = value;
			CloseEntity(eobj);
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

namespace
{
	XLRegistration::Arg QueryEntityNameArgs[]=
	{
		{"monitorID", "�������ڵ��ID"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryEntityName
		(
		"xlQueryEntityName",
		"QueryEntityName",
		"���ؼ������ڵ������豸������",
		LibraryName,
		QueryEntityNameArgs,
		"RRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryEntityName
		(
		LPXLOPER monitorIDa,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDb(monitorIDa);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string monitorID(monitorIDb.AsString("monitorID"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		OBJECT eobj = GetEntity(getParentString(monitorID), user, address);
		MAPNODE en = GetEntityMainAttribNode(eobj);
		string value;
		if (!FindNodeValue(en, "sv_name", value)) value = "ȡ�����������豸������ʧ�ܣ�";
		CloseEntity(eobj);

		CellMatrix resultCells(value);
		return XlfOper(resultCells);
		EXCEL_END
	}
}

/////////////////////////////////////////////////////
bool
getPercentStateData(PercentState_T & percentState,
										string const & address,
										TTime & sTime,
										TTime & eTime, 
										string const & user, 
										string const & monitorID)
{
	RECORDSET rs = QueryRecords(monitorID, sTime, eTime, user, address);

	int normalNum = 0;
	int warningNum = 0;
	int errorNum = 0;
	
	int tmpState = 0;

	LISTITEM item;
	if (!FindRecordFirst(rs, item))
	{
		percentState = g_psZero;
		return false;
	}

	RECORD rd;
	while((rd = FindNextRecord(item))!=INVALID_VALUE)
	{
		if (GetRecordState(rd, tmpState))
		{
			switch(tmpState)
			{
			case 1:
				//pass through
			case 4:
				++normalNum;
				break;
			case 2:
				++warningNum;
				break;
			case 3:
				//pass through
			case 5:
				++errorNum;
				break;
			default:
				;
			}
		}
	}

	ReleaseRecordList(item);
	CloseRecordSet(rs);

	double count = static_cast<double>(normalNum + warningNum + errorNum);
	percentState.normalPer = normalNum * 100 / count;
	percentState.warningPer = warningNum * 100 / count;
	percentState.errorPer = errorNum * 100 / count;
	return true;
}


namespace
{
	XLRegistration::Arg QueryPercentStatesArgs[]=
	{
		{"monitorIDs", "�������ڵ��ID(����1��)"},
		{"startTime", "��ʼʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"endTime", "����ʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryPercentStates
		(
		"xlQueryPercentStates",
		"QueryPercentStates",
		"������������ļ������ڵ���ָ��ʱ������������Σ�պʹ�������ʱ��İٷֱȣ�����3�У�",
		LibraryName,
		QueryPercentStatesArgs,
		"PRRRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryPercentStates
		(
		LPXLOPER monitorIDsa,
		LPXLOPER startTimea,
		LPXLOPER endTimea,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDsb(monitorIDsa);
		XlfOper startTimeb(startTimea);
		XlfOper endTimeb(endTimea);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		CellMatrix monitorIDs(monitorIDsb.AsCellMatrix("monitorIDs"));
		std::string startTime(startTimeb.AsString("startTime"));
		std::string endTime(endTimeb.AsString("endTime"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if ((startTime == "") || (endTime == ""))
		{
			return XlfOper("������ȱʡֵ�Ĳ�������Ϊ�գ�");
		}
		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		TTime sTime, eTime;
		if (!(getTime(startTime, sTime)) || !(getTime(endTime, eTime)))
		{
			return XlfOper("�����밴��ʽ����ʱ�������");
		}

		CellMatrix resultCells(0, 3);
		CellMatrix tmpCells(1, 3);

		PercentState_T ps = g_psZero;
		bool hasGet = false;

		long row = monitorIDs.RowsInStructure();
		for (long i = 0; i < row; ++i)
		{
			hasGet = getPercentStateData(ps, address, sTime, eTime, user, monitorIDs(i, 0).StringValue());
			if (hasGet)
			{
				tmpCells(0, 0) = ps.normalPer;
				tmpCells(0, 1) = ps.warningPer;
				tmpCells(0, 2) = ps.errorPer;
			}
			else
			{
				for (int j = 0; j < 3; ++j)
				{
					tmpCells(0, j) = "������";
				}
			}
			resultCells.PushBottom(tmpCells);
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

namespace
{
	XLRegistration::Arg QueryPercentStateArgs[]=
	{
		{"monitorID", "�������ڵ��ID"},
		{"startTime", "��ʼʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"endTime", "����ʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryPercentState
		(
		"xlQueryPercentState",
		"QueryPercentState",
		"���ؼ������ڵ������豸��ָ��ʱ������������Σ�պʹ�������ʱ��İٷֱȣ�һ��3�У�",
		LibraryName,
		QueryPercentStateArgs,
		"RRRRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryPercentState
		(
		LPXLOPER monitorIDa,
		LPXLOPER startTimea,
		LPXLOPER endTimea,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDb(monitorIDa);
		XlfOper startTimeb(startTimea);
		XlfOper endTimeb(endTimea);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string monitorID(monitorIDb.AsString("monitorID"));
		std::string startTime(startTimeb.AsString("startTime"));
		std::string endTime(endTimeb.AsString("endTime"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if ((monitorID == "") || (startTime == "") || (endTime == ""))
		{
			return XlfOper("������ȱʡֵ�Ĳ�������Ϊ�գ�");
		}
		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		TTime sTime, eTime;
		if (!(getTime(startTime, sTime)) || !(getTime(endTime, eTime)))
		{
			return XlfOper("�����밴��ʽ����ʱ�������");
		}

		CellMatrix resultCells(1, 3);

		PercentState_T ps = g_psZero;
		bool hasGet = false;

		hasGet = getPercentStateData(ps, address, sTime, eTime, user, monitorID);
		if (hasGet)
		{
			resultCells(0, 0) = ps.normalPer;
			resultCells(0, 1) = ps.warningPer;
			resultCells(0, 2) = ps.errorPer;
		}
		else
		{
			for (int j = 0; j < 3; ++j)
			{
				resultCells(0, j) = "������";
			}
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

////////////////////////////////////////////////////////////////////////////
namespace
{
	XLRegistration::Arg GetFirstColumnNamesArgs[]=
	{
		{"monitorIDs", "�������ڵ��ID(����1��)"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerGetFirstColumnNames
		(
		"xlGetFirstColumnNames",
		"GetFirstColumnNames",
		"������������ļ������ڵ�ĵ�һ������������������1�У�",
		LibraryName,
		GetFirstColumnNamesArgs,
		"PRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlGetFirstColumnNames
		(
		LPXLOPER monitorIDsa,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDsb(monitorIDsa);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		CellMatrix monitorIDs(monitorIDsb.AsCellMatrix("monitorIDs"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		long row = monitorIDs.RowsInStructure();
		CellMatrix resultCells(row, 1);

		MAPNODE ResNode=GetResourceNode(LoadResource("default", address));
		for (long i = 0; i < row; ++i)
		{
			OBJECT mobj = GetMonitor(monitorIDs(i, 0).StringValue(), user, address);
			MAPNODE mn = GetMonitorMainAttribNode(mobj);
			string typeID;
			FindNodeValue(mn, "sv_monitortype", typeID);
			int iType = atoi(typeID.c_str());

			OBJECT mt = GetMonitorTemplet(iType, user, address);
			LISTITEM item;
			FindMTReturnFirst(mt, item);
			MAPNODE objNode = FindNext(item);
			string fieldLable, fieldLableName;
			FindNodeValue(objNode, "sv_label", fieldLable);
			
			if( ResNode != INVALID_VALUE )
				FindNodeValue(ResNode, fieldLable, fieldLableName);
			if(fieldLableName=="")
				fieldLableName = fieldLable;
			MAPNODE templetMapNode = GetMTMainAttribNode(mt);
			resultCells(i, 0) = fieldLableName;
			CloseMonitorTemplet(mt);
			CloseMonitor(mobj);
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

namespace
{
	XLRegistration::Arg GetFirstColumnNameArgs[]=
	{
		{"monitorID", "�������ڵ��ID"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerGetFirstColumnName
		(
		"xlGetFirstColumnName",
		"GetFirstColumnName",
		"���ؼ������ڵ�ĵ�һ������������",
		LibraryName,
		GetFirstColumnNameArgs,
		"RRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlGetFirstColumnName
		(
		LPXLOPER monitorIDa,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDb(monitorIDa);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string monitorID(monitorIDb.AsString("monitorID"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		OBJECT mobj = GetMonitor(monitorID, user, address);
		MAPNODE mn = GetMonitorMainAttribNode(mobj);
		string typeID;
		FindNodeValue(mn, "sv_monitortype", typeID);
		int iType = atoi(typeID.c_str());

		OBJECT mt = GetMonitorTemplet(iType, user, address);
		LISTITEM item;
		FindMTReturnFirst(mt, item);
		MAPNODE objNode = FindNext(item);
		string fieldLable, fieldLableName;
		FindNodeValue(objNode, "sv_label", fieldLable);
		MAPNODE ResNode=GetResourceNode(LoadResource("default", address));
		if( ResNode != INVALID_VALUE )
			FindNodeValue(ResNode, fieldLable, fieldLableName);
		if(fieldLableName=="")
			fieldLableName = fieldLable;
		MAPNODE templetMapNode = GetMTMainAttribNode(mt);
		CloseMonitorTemplet(mt);

		CloseMonitor(mobj);

		CellMatrix resultCells(fieldLableName);
		return XlfOper(resultCells);
		EXCEL_END
	}
}

//////////////////////////////////////////////////////////////////////
bool
getMaxAvgMinData(MaxAvgMin_T & maxAvgMin,
								 string const & address,
								 TTime & sTime,
								 TTime & eTime, 
								 string const & user, 
								 string const & monitorID)
{
	RECORDSET rs = QueryRecords(monitorID, sTime, eTime, user, address);
	list<string> fieldlist;
	GetReocrdSetField(rs, fieldlist);

	double maxValue = 0;
	double minValue = 0;
	double sum = 0.0;
	int count = 0;

	LISTITEM item;
	if (!FindRecordFirst(rs, item))
	{
		maxAvgMin = g_mamZero;
		return false;
	}

	RECORD rd;
	while((rd = FindNextRecord(item))!=INVALID_VALUE)
	{
		int type;
		int state = 0;
		int iv;
		float fv;
		string sv;

		if ((GetRecordValueByField(rd, *(fieldlist.begin()), type, state, iv, fv, sv)) && 
			(state == 1 || state == 2 || state == 3))
		{
			double v = 0;
			switch(type)
			{
			case 1:
				v = static_cast<double>(iv);			
				break;
			case 2:
				v = static_cast<double>(fv);
				break;
			default:
				return false;
				break;
			}
			if (count == 0)
			{
				maxValue = minValue = v;
			}
			else if (maxValue < v)
			{
				maxValue = v;
			}
			else if (minValue > v)
			{
				minValue = v;
			}
			sum += v;
			++count;
		} 
	}

	ReleaseRecordList(item);
	CloseRecordSet(rs);

	maxAvgMin.maxValue = maxValue;
	if (count != 0)
	{
		maxAvgMin.avgValue = sum / count;
	}
	else
	{
		maxAvgMin.avgValue = 0;
	}
	maxAvgMin.minValue = minValue;
	return true;
}

namespace
{
	XLRegistration::Arg QueryMaxAvgMinsArgs[]=
	{
		{"monitorIDs", "�������ڵ��ID(����1��)"},
		{"startTime", "��ʼʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"endTime", "����ʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryMaxAvgMins
		(
		"xlQueryMaxAvgMins",
		"QueryMaxAvgMins",
		"������������ļ������ڵ���ָ��ʱ�����ڵ�һ���������Ե����ƽ������Сֵ������3�У�",
		LibraryName,
		QueryMaxAvgMinsArgs,
		"PRRRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryMaxAvgMins
		(
		LPXLOPER monitorIDsa,
		LPXLOPER startTimea,
		LPXLOPER endTimea,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDsb(monitorIDsa);
		XlfOper startTimeb(startTimea);
		XlfOper endTimeb(endTimea);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		CellMatrix monitorIDs(monitorIDsb.AsCellMatrix("monitorIDs"));
		std::string startTime(startTimeb.AsString("startTime"));
		std::string endTime(endTimeb.AsString("endTime"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if ((startTime == "") || (endTime == ""))
		{
			return XlfOper("������ȱʡֵ�Ĳ�������Ϊ�գ�");
		}
		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		TTime sTime, eTime;
		if (!(getTime(startTime, sTime)) || !(getTime(endTime, eTime)))
		{
			return XlfOper("�����밴��ʽ����ʱ�������");
		}

		long row = monitorIDs.RowsInStructure();
		CellMatrix resultCells(row, 3);

		MaxAvgMin_T mam = g_mamZero;
		bool hasGet = false;

		
		for (long i = 0; i < row; ++i)
		{
			hasGet = getMaxAvgMinData(mam, address, sTime, eTime, user, monitorIDs(i, 0).StringValue());
			if (hasGet)
			{
				resultCells(i, 0) = mam.maxValue;
				resultCells(i, 1) = mam.avgValue;
				resultCells(i, 2) = mam.minValue;
			}
			else
			{
				for (int j = 0; j < 3; ++j)
				{
					resultCells(i, j) = "������";
				}
			}
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

namespace
{
	XLRegistration::Arg QueryMaxAvgMinArgs[]=
	{
		{"monitorID", "�������ڵ��ID"},
		{"startTime", "��ʼʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"endTime", "����ʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryMaxAvgMin
		(
		"xlQueryMaxAvgMin",
		"QueryMaxAvgMin",
		"���ؼ������ڵ������豸��ָ��ʱ�����ڵ�һ���������Ե����ƽ������Сֵ��һ��3�У�",
		LibraryName,
		QueryMaxAvgMinArgs,
		"RRRRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryMaxAvgMin
		(
		LPXLOPER monitorIDa,
		LPXLOPER startTimea,
		LPXLOPER endTimea,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper monitorIDb(monitorIDa);
		XlfOper startTimeb(startTimea);
		XlfOper endTimeb(endTimea);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string monitorID(monitorIDb.AsString("monitorID"));
		std::string startTime(startTimeb.AsString("startTime"));
		std::string endTime(endTimeb.AsString("endTime"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if ((monitorID == "") || (startTime == "") || (endTime == ""))
		{
			return XlfOper("������ȱʡֵ�Ĳ�������Ϊ�գ�");
		}
		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		TTime sTime, eTime;
		if (!(getTime(startTime, sTime)) || !(getTime(endTime, eTime)))
		{
			return XlfOper("�����밴��ʽ����ʱ�������");
		}

		CellMatrix resultCells(1, 3);

		MaxAvgMin_T mam = g_mamZero;
		bool hasGet = false;

		hasGet = getMaxAvgMinData(mam, address, sTime, eTime, user, monitorID);
		if (hasGet)
		{
			resultCells(0, 0) = mam.maxValue;
			resultCells(0, 1) = mam.avgValue;
			resultCells(0, 2) = mam.minValue;
		}
		else
		{
			for (int j = 0; j < 3; ++j)
			{
				resultCells(0, j) = "������";
			}
		}

		return XlfOper(resultCells);
		EXCEL_END
	}
}

////////////////////////////////////////////////////////////////////////////////
void getAlertMonitors(string const & address,
											string const & user,
											string const & index, 
											list<string> & lsMonitorID)
{
	string m = GetIniFileString(index, "AlertTarget", "", "alert.ini", address, user);

	basic_string <char>::size_type bpos = 0, epos = 0;
	static const basic_string <char>::size_type npos = -1;
	while ( ((bpos = epos +1) < m.size()) 
		&& ((epos = m.find(",", bpos)) != npos) )
	{
		lsMonitorID.push_back(m.substr(bpos, epos - bpos));
	}
}

inline string getDeviceName(string const & address,
									 string const & user,
									 string const & monitorID)
{
	OBJECT eobj = GetEntity(getParentString(monitorID), user, address);
	MAPNODE en = GetEntityMainAttribNode(eobj);
	string value;
	FindNodeValue(en, "sv_name", value);
	CloseEntity(eobj);
	return value;
}

inline string getMonitorName(string const & address,
														 string const & user,
														 string const & monitorID)
{
	OBJECT mobj = GetMonitor(monitorID, user, address);
	MAPNODE mn = GetMonitorMainAttribNode(mobj);
	string value;
	FindNodeValue(mn, "sv_name", value);
	CloseMonitor(mobj);
	return value;
}

string findAertMonitor(list<string> const & lsMonitorID,
											 string const & address,
											 string const & user,
											 string const & deviceName,
											 string const & monitorName)
{
	list<string>::const_iterator it;
	list<string>::const_iterator lsBegin = lsMonitorID.begin();
	list<string>::const_iterator lsEnd = lsMonitorID.end();
	for (it = lsBegin; it != lsEnd; ++it)
	{
		if (deviceName == getDeviceName(address, user, *it)
			&& monitorName == getMonitorName(address, user, *it))
		{
			return *it;
		}
	}
	return "";
}

//��map�в�ѯָ�����Ե�ֵ
inline string getMapPropValue(MAPNODE const & aMapNaode, string strPropName)
{
	string strTmp = "";
	FindNodeValue(aMapNaode, strPropName, strTmp);
	return strTmp;
}

#if 0
enum FazhiType_T {FZ_NORMAL, FZ_WARNING, FZ_ERROR};
#endif

string getFazhi(string const & address,
								string const & monitorId,
								FazhiType_T type,
								string const & user,
								MAPNODE const & fazhiMap,
								OBJECT const & objTempMonitor)
{
	FazhiIndex_T fz(address, monitorId, type);
	if (s_fazhiMap.find(fz) != s_fazhiMap.end())
	{
		OutputDebugString("get from map");
		return s_fazhiMap[fz];	
	}
	OutputDebugString("begin get fazhi");
	string strFazhi;
	OBJECT objRes = LoadResource("default", address);  
	MAPNODE ResNode = GetResourceNode(objRes);

	MAPNODE monitorTempNode;

	PAIRLIST ParamList;
	LISTITEM ParamItem;
	string strTemp, strTempCon;

	string strCount, strParamName, strParamValue, strParamOperate, strParamRelation, strReturn;
	bool bFind = FindNodeValue(fazhiMap, "sv_conditioncount", strCount);  //��ȡ��ֵ��������Ŀ
	int iCount = atoi(strCount.c_str());
	for(int i=1; i<= iCount; i++)
	{
		strTempCon = "[";

		string paraIndex;
		char buf[65];
		itoa(i, buf, 10);
		paraIndex = buf;
		strTemp = "sv_paramname" + paraIndex;
		bFind = FindNodeValue(fazhiMap, strTemp, strReturn);

		//�ڼ�����ģ��ķ���ֵ�в��ҷ�ֵ����������
		if(FindMTReturnFirst(objTempMonitor, ParamItem))
		{
			while((monitorTempNode = FindNext(ParamItem)) != INVALID_VALUE )
			{ 
				string sReturnName = "", MonitorIDSName ="";
				FindNodeValue(monitorTempNode, "sv_name", sReturnName);
				if(sReturnName == strReturn)
				{
					FindNodeValue(monitorTempNode, "sv_label", MonitorIDSName);
					if (FindNodeValue(ResNode, MonitorIDSName, strParamName))
					{
						strTempCon += strParamName;
					} 
					else
					{
						strTempCon += MonitorIDSName;
					}
#if 0
					strTempCon += MonitorIDSName;
#endif				
				}
			}
		}

		strTemp = "sv_operate" + paraIndex;
		bFind = FindNodeValue(fazhiMap, strTemp, strParamOperate); //��ȡ��ֵ�������ţ����硰>����<=����

		strTempCon += strParamOperate;

		strTemp = "sv_paramvalue" + paraIndex;
		bFind = FindNodeValue(fazhiMap, strTemp, strParamValue);	//��ȡ��ֵ

		strTempCon += strParamValue;
		strTempCon += "]";

		strFazhi += strTempCon; //����һ��

		if(i<iCount)
		{
			strTemp = "sv_relation" + paraIndex;
			bFind = FindNodeValue(fazhiMap, strTemp, strParamRelation); //��ȡ��ϵ��

			strFazhi = strFazhi + " " + strParamRelation + " "; //����ո�ʹ��ʾ������										
		}
	}
	CloseResource(objRes);
	s_fazhiMap[fz] = strFazhi;
	return strFazhi;
}

bool getAlertData(string const & address,
									string const & user,
									string const & index,
									string const & deviceName,
									string const & monitorName,
									TTime const & time,
									CellMatrix & matrix)
{
#if 0
	static int i = 0;
	char buf[10] = {0};
	itoa(i, buf, 10);
	OutputDebugString("getalertdata call number is ");
	OutputDebugString(buf);
	i++;
#endif
	list<string> lsMonitorID;
	getAlertMonitors(address, user, index, lsMonitorID);
	string monitorID = findAertMonitor(lsMonitorID, address, user, deviceName, monitorName);
	
	TTime bTime = time - TTimeSpan(60);
	RECORDSET rs = QueryRecords(monitorID, bTime, time, user, address);
	list<string> fieldlist;
	GetReocrdSetField(rs, fieldlist);
	LISTITEM item;
	if (!FindRecordFirst(rs, item))
	{
		CloseRecordSet(rs);
		return false;
	}
	RECORD rd;
	int type;
	int state;
	int iv;
	float fv;
	string sv;
	if((rd = FindNextRecord(item))!=INVALID_VALUE
		&& GetRecordValueByField(rd, *(fieldlist.begin()), type, state, iv, fv, sv))
	{
		if (state == 2)
		{
			matrix(0, 2) = "����";
		}
		else if (state == 3)
		{
			matrix(0, 2) = "����";
		} 
		else
		{
			CloseRecordSet(rs);
			return false;
		}

		switch(type)
		{
		case 1:
			matrix(0, 3) = static_cast<double>(iv);
			break;
		case 2:
			matrix(0, 3) = static_cast<double>(fv);
			break;
		case 3:
			matrix(0, 3) = sv;
			break;
		default:
			matrix(0, 3) = "";
			break;
		}		
	}
	else
	{
		CloseRecordSet(rs);
		return false;
	}
	CloseRecordSet(rs);

	OBJECT hMon = GetMonitor( monitorID, user, address );
	MAPNODE monitorNode;
	if (hMon == INVALID_VALUE
		|| (monitorNode = GetMonitorMainAttribNode(hMon)) == INVALID_VALUE)   //ȡ���������ֵ��ȫ��Ϊ��
	{
		matrix(0, 5) = "";
		matrix(0, 6) = "";
		matrix(0, 7) = "";
		matrix(0, 8) = "";
		CloseMonitor(hMon);
		return true;
	}  //end "if (hMon == INVALID_VALUE)"

	MAPNODE paramMn = GetMonitorParameter(hMon);
	string strUnit = "����";
	if (getMapPropValue(paramMn, "_frequencyUnit") == "60")   //Ƶ�ʵ�λΪ���ӣ�����ڡ�60����ΪСʱ
	{
		strUnit = "Сʱ";
	}
	matrix(0, 5) = getMapPropValue(paramMn, "_frequency") + strUnit;

	OBJECT objTempMonitor = INVALID_VALUE;	
	objTempMonitor = GetMonitorTemplet(atoi(getMapPropValue(monitorNode, "sv_monitortype").c_str()),
		user, address);               //������ģ�����
	if (objTempMonitor == INVALID_VALUE)
	{
		matrix(0, 6) = "";
		matrix(0, 7) = "";
		matrix(0, 8) = "";
		CloseMonitor(hMon);
		return true;
	}
	MAPNODE templetMainMap = GetMTMainAttribNode(objTempMonitor);
	matrix(0, 1) = getMapPropValue(templetMainMap, "sv_name");

	///////////////////begin get fazhi///////////////////////
#if 0
	MAPNODE ma = INVALID_VALUE;  //��ma�Ƶ����涨�壬�����ں���û����
	// ȡ�����������󱨾����ò�������
	int nEventType = 3;
	switch(nEventType)
	{
	case 1: // ��������
		ma = GetMonitorGoodAlertCondition(hMon);
		break;
	case 2: // Σ�ձ���
		ma = GetMonitorWarningAlertCondition(hMon);
		break;
	case 3: // ���󱨾�
		ma = GetMonitorErrorAlertCondition(hMon);
		break;
	default:
		break;
	}
#endif	
	////////////////////end get fazhi////////////////////////
	MAPNODE ma = INVALID_VALUE;
	ma = GetMonitorGoodAlertCondition(hMon);
	matrix(0, 6) = getFazhi(address, monitorID, FZ_NORMAL, user, ma, objTempMonitor);
	ma = GetMonitorWarningAlertCondition(hMon);
	matrix(0, 7) = getFazhi(address, monitorID, FZ_WARNING, user, ma, objTempMonitor);
	ma = GetMonitorErrorAlertCondition(hMon);
	matrix(0, 8) = getFazhi(address, monitorID, FZ_ERROR, user, ma, objTempMonitor);
	CloseMonitorTemplet(objTempMonitor);
	CloseMonitor(hMon);
	return true;
}

namespace
{
	XLRegistration::Arg QueryAlertDataByTimeArgs[]=
	{
		{"startTime", "��ʼʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"endTime", "����ʱ�䣨��ʽ����-��-�� ʱ:��:�룩"},
		{"address", "������IP��ַ��ȱʡֵΪ��localhost"},
		{"user", "�û�����ȱʡֵΪ��default"}
	};
	XLRegistration::XLFunctionRegistrationHelper
		registerQueryAlertDataByTime
		(
		"xlQueryAlertDataByTime",
		"QueryAlertDataByTime",
		"������ʼʱ�䣬����ECC����������ʷ������Ϣ",
		LibraryName,
		QueryAlertDataByTimeArgs,
		"RRRR"
		);
}

extern "C"
{
	LPXLOPER EXCEL_EXPORT
		xlQueryAlertDataByTime
		(
		LPXLOPER startTimea,
		LPXLOPER endTimea,
		LPXLOPER addressa,
		LPXLOPER usera
		)
	{
		EXCEL_BEGIN;

		if (XlfExcel::Instance().IsCalledByFuncWiz())
			return XlfOper(true);

		XlfOper startTimeb(startTimea);
		XlfOper endTimeb(endTimea);
		XlfOper addressb(addressa);
		XlfOper userb(usera);

		std::string startTime(startTimeb.AsString("startTime"));
		std::string endTime(endTimeb.AsString("endTime"));
		std::string address(addressb.AsString("address"));
		std::string user(userb.AsString("user"));

		if ((startTime == "") || (endTime == ""))
		{
			return XlfOper("������ȱʡֵ�Ĳ�������Ϊ�գ�");
		}
		if (address == "")
		{
			address = "localhost";				
		}
		if (user == "")
		{
			user = "default";
		}

		TTime sTime, eTime;
		if (!(getTime(startTime, sTime)) || !(getTime(endTime, eTime)))
		{
			return XlfOper("�����밴��ʽ����ʱ�������");
		}

		RECORDSET rs = QueryRecords("alertlogs", sTime, eTime, user, address);

		long const COLUMN_NUM = 9;
		CellMatrix resultCells(0, COLUMN_NUM);
		CellMatrix tmpCells(1, COLUMN_NUM);
		TTime tmpTime;
		int tmpState = 0;
		bool getRecord = false;

		LISTITEM item;
		if (!FindRecordFirst(rs, item))
		{
			return XlfOper("����ȡ��¼ʧ�ܣ�");
		}

		RECORD rd;
		string const & DEVICE_FIELD_NAME = "_DeviceName";
		string const & MONITOR_FIELD_NAME = "_MonitorName";
		string const & INDEX_FIELD_NAME = "_AlertIndex";
		string strIndex;
		string strDeviceName;
		string strMonitorName;

		while((rd = FindNextRecord(item))!=INVALID_VALUE)
		{
			if (GetRecordCreateTime(rd, tmpTime))
			{
				tmpCells(0, 4) = tmpTime.Format();
			}
			else
			{
				tmpCells(0, 4) = "ȡʱ��ʧ��";
			}

			int type;
			int state;
			int iv;
			float fv;		

			if (GetRecordValueByField(rd, INDEX_FIELD_NAME, type, state, iv, fv, strIndex)
				&& state == 1 && type == 3
				&& GetRecordValueByField(rd, DEVICE_FIELD_NAME, type, state, iv, fv, strDeviceName)
				&& state == 1 && type == 3
				&& GetRecordValueByField(rd, MONITOR_FIELD_NAME, type, state, iv, fv, strMonitorName)
				&& state == 1 && type == 3
				&& getAlertData(address, user, strIndex, strDeviceName, strMonitorName, tmpTime, tmpCells))
			{
				tmpCells(0, 0) = strDeviceName + ":" + strMonitorName;
				resultCells.PushBottom(tmpCells);
			} 
		}

		ReleaseRecordList(item);
		CloseRecordSet(rs);

		return XlfOper(resultCells);
		EXCEL_END
	}
}