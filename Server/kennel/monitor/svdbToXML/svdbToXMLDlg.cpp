// svdbToXMLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "svdbToXML.h"
#include "svdbToXMLDlg.h"
#include <string>
#include <algorithm>
#include ".\svdbtoxmldlg.h"

using std::string;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class compare_sv_pair   
{
public:   
	bool operator()(const sv_pair & x, const sv_pair & y) const   
	{
		//return x.name < y.name;
		int nx = 0;
		int ny = 0;
		static const basic_string<char>::size_type npos = -1;

		basic_string<char>::size_type ix = 0;
		basic_string<char>::size_type iy = 0;

		while ((ix != npos) && (iy != npos))
		{
			basic_string<char>::size_type eix = x.name.find_first_of(".", ix);
			basic_string<char>::size_type eiy = y.name.find_first_of(".", iy);

			if (eix == npos)
			{
				nx = atoi(x.name.substr(ix).c_str());
			}
			else
			{
				nx = atoi(x.name.substr(ix, eix - ix).c_str());
				++eix;
			}

			if (eiy == npos)
			{
				ny = atoi(y.name.substr(iy).c_str());
			}
			else
			{			
				ny = atoi(y.name.substr(iy, eiy - iy).c_str());
				++eiy;
			}

			if (nx < ny)
			{
				return true;
			}

			if (nx > ny)
			{
				return false;
			}

			ix = eix;
			iy = eiy;

			if (ix > x.name.size())
			{
				ix = npos;
			}
			if (iy > y.name.size())
			{
				iy = npos;
			}
		}

		if (ix == npos)
		{
			return true;
		}
		else
		{
			return false;
		}	
	}              
};

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CsvdbToXMLDlg 对话框



CsvdbToXMLDlg::CsvdbToXMLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsvdbToXMLDlg::IDD, pParent)
	, m_config("config.xml")
	, m_interval(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_path = m_config.GetValue("Path").c_str();
	m_ip = m_config.GetValue("Server").c_str();
	m_interval = atoi(m_config.GetValue("Interval").c_str());
}

void CsvdbToXMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_path);
	DDX_Text(pDX, IDC_EDIT2, m_interval);
	DDX_Text(pDX, IDC_EDIT3, m_ip);
}

BEGIN_MESSAGE_MAP(CsvdbToXMLDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CsvdbToXMLDlg 消息处理程序

BOOL CsvdbToXMLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CsvdbToXMLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsvdbToXMLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CsvdbToXMLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsvdbToXMLDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int old = m_interval;
	UpdateData();

	char buffer[32] = {0};
	ultoa(m_interval, buffer, 10);
	string tmp = buffer;
	m_config.SetValue("Interval", tmp);

	if (old != m_interval)
	{
		KillTimer(1);
		TTime st = readTime();
		TTime et = TTime::GetCurrentTimeEx();
		writeTime(et);
		generateXml(st, et, m_ip.GetBuffer(m_ip.GetLength()));
		m_ip.ReleaseBuffer();

		SetTimer(1, m_interval*60*1000, 0);
	}

	tmp = m_ip.GetBuffer(m_ip.GetLength());
	m_ip.ReleaseBuffer();
	m_config.SetValue("Server", tmp);

	tmp = m_path.GetBuffer(m_path.GetLength());
	m_path.ReleaseBuffer();
	m_config.SetValue("Path", tmp, true);

	writeTime(TTime::GetCurrentTimeEx());
}

TTime CsvdbToXMLDlg::readTime(void)
{
	time_t t = atol(m_config.GetValue("LastTime").c_str());
	return TTime(t);
}

bool CsvdbToXMLDlg::writeTime(TTime const & t)
{
	char buffer[32] = {0};
	ltoa(t.GetTime(), buffer, 10);
	string tmp = buffer;	 
	return m_config.SetValue("LastTime", tmp, true);
}

void CsvdbToXMLDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TTime st = readTime();
	TTime et = TTime::GetCurrentTimeEx();
	writeTime(et);
	generateXml(st, et, m_ip.GetBuffer(m_ip.GetLength()));
	m_ip.ReleaseBuffer();
	CDialog::OnTimer(nIDEvent);
}

void CsvdbToXMLDlg::generateXml(TTime const & begTime, TTime const & endTime, string const & address)
{
	TiXmlDocument doc; 

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "GB2312", "" ); 
	doc.LinkEndChild( decl ); 

	TiXmlElement * root = new TiXmlElement( "SiteView" ); 
	doc.LinkEndChild( root );
	root->SetAttribute("BeginTime", begTime.Format());
	root->SetAttribute("EndTime", endTime.Format());

	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue(" SiteView数据库转成XML格式 " ); 
	root->LinkEndChild( comment );

	PAIRLIST selist;
	PAIRLIST::iterator iSe;
	GetAllMonitorsInfo(selist, "sv_name", "default", address);
	
	selist.sort(compare_sv_pair());

	
	for(iSe = selist.begin(); iSe != selist.end(); iSe++)
	{
		TiXmlElement * monitor = new TiXmlElement("Monitor");
		monitor->SetAttribute("ID", iSe->name);
		root->LinkEndChild(monitor);

		RECORDSET rs = QueryRecords(iSe->name, begTime, endTime, "default", address);

		list<string> fieldlist;
		GetReocrdSetField(rs, fieldlist);
		list<string>::size_type flsize = fieldlist.size();
		list<string>::iterator fl_beg_it = fieldlist.begin();
		list<string>::iterator fl_end_it = fieldlist.end();

		TTime tmpTime;
		int tmpState = 0;
		bool getRecord = false;

		LISTITEM item;
		if (!FindRecordFirst(rs, item))
		{
			monitor->SetAttribute("error", "no data");
		}
		else
		{		
			RECORD rd;
			while((rd = FindNextRecord(item))!=INVALID_VALUE)
			{
				TiXmlElement * xItem = new TiXmlElement("Item");
				monitor->LinkEndChild(xItem);

				if (GetRecordCreateTime(rd, tmpTime))
				{
					xItem->SetAttribute("time", tmpTime.Format());
				}
				else
				{
					xItem->SetAttribute("error", "time error");
				}

				string dstr;
//				if (GetRecordState(rd, tmpState))
				if (GetRecordDisplayString(rd, tmpState, dstr))				
				{
					switch(tmpState)
					{
					case 1:
						xItem->SetAttribute("state", "正常");
						getRecord = true;
						break;
					case 2:
						xItem->SetAttribute("state", "危险");
						getRecord = true;
						break;
					case 3:
						xItem->SetAttribute("state", "错误");
						getRecord = true;
						break;
					case 4:
						xItem->SetAttribute("state", "禁止");
						getRecord = false;
						break;
					case 5:
						xItem->SetAttribute("state", "坏数据");
						getRecord = false;
						break;
					default:
						xItem->SetAttribute("state", "空");
						getRecord = false;
					}
					xItem->LinkEndChild(new TiXmlText(dstr));
				}
				else
				{
					xItem->SetAttribute("error", "state error");
					getRecord = false;
				}

				if (getRecord)
				{
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
								xItem->SetAttribute(it->c_str(), iv);
								break;
							case 2:
								xItem->SetDoubleAttribute(it->c_str(), static_cast<double>(fv));
								break;
							case 3:
								xItem->SetAttribute(it->c_str(), sv);
								break;
							default:
								break;
							}
						} 
						else
						{
							xItem->SetAttribute("error", *it + " error");
						}				
					}
				} 
			}
		}
	}
	

	string tmpPath = m_path.GetBuffer(m_path.GetLength());
	m_path.ReleaseBuffer();

	doc.SaveFile(tmpPath + endTime.FormatForName() + ".xml");
}
void CsvdbToXMLDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	TTime st = readTime();
	TTime et = TTime::GetCurrentTimeEx();
	writeTime(et);
	generateXml(st, et, m_ip.GetBuffer(m_ip.GetLength()));
	m_ip.ReleaseBuffer();

	SetTimer(1, m_interval*60*1000, 0);

// 	TTime st = readTime();
// 	char buf[30] = {0};
// 	ltoa(static_cast<long>(st.GetTime()), buf, 10);
// 	MessageBox(buf);
// 	TTime et = TTime::GetCurrentTimeEx();
// 	writeTime(et);
// 	generateXml(st, et, m_ip.GetBuffer(m_ip.GetLength()));
// 	m_ip.ReleaseBuffer();
}

void CsvdbToXMLDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	OnCancel();
}
