// VCDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCDemo.h"
#include "VCDemoDlg.h"
#include "DlgProxy.h"
#include <vector>

#include "../ImApi/ImApi.h"
#pragma comment(lib, "../Output/ImApi.lib")

#define BUF_SIZE 256
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCDemoDlg dialog

IMPLEMENT_DYNAMIC(CVCDemoDlg, CDialog);

CVCDemoDlg::CVCDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVCDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVCDemoDlg)
	m_ip = _T("192.168.0.52");
	m_apiCode = _T("dpi");
	m_username = _T("dpi");
	m_pwd = _T("dpi");
	m_mobile = _T("13516871762");
	m_strContent = _T("VC Demo使用API发送短信");
	m_id=_T("10");
	m_url = _T("");
	m_wap_push = -1;
	m_strDBName = _T("mas");
	m_sendTime = _T("2007-05-24 12:00:00");
	m_srcid = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	m_bInit = FALSE;
	m_wap_push = 0;
}

CVCDemoDlg::~CVCDemoDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CVCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVCDemoDlg)
	DDX_Control(pDX, IDC_EDIT_URL, m_edit_url);
	DDX_Text(pDX, IDC_EDIT_IP, m_ip);
	DDX_Text(pDX, IDC_EDIT_apiCode, m_apiCode);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_username);
	DDX_Text(pDX, IDC_EDIT_PWD, m_pwd);
	DDX_Text(pDX, IDC_EDIT_MOBILE, m_mobile);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_strContent);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_URL, m_url);
	DDX_Radio(pDX, IDC_RADIO_NO_WAPPUSH, m_wap_push);
	DDX_Text(pDX, IDC_EDIT_DBNAME, m_strDBName);
	DDV_MaxChars(pDX, m_strDBName, 20);
	DDX_Text(pDX, IDC_EDIT_SENDTIME, m_sendTime);
	DDX_Text(pDX, IDC_EDIT_SRCID, m_srcid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVCDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CVCDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSend, OnSend)
	ON_BN_CLICKED(IDReceiveSM, OnReceiveSM)
	ON_BN_CLICKED(IDReceiveRPT, OnReceiveRPT)
	ON_BN_CLICKED(IDExit, OnExit)
	ON_BN_CLICKED(IDRelease, OnRelease)
	ON_BN_CLICKED(IDInit, OnInit)
	ON_BN_CLICKED(IDC_RADIO_IS_WAPPUSH, OnRadioIsWappush)
	ON_BN_CLICKED(IDC_RADIO_NO_WAPPUSH, OnRadioNoWappush)
	ON_BN_CLICKED(IDC_RADIO_NO_PDU, OnRadioPDU)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCDemoDlg message handlers

BOOL CVCDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVCDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVCDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVCDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

BOOL CVCDemoDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CVCDemoDlg::OnSend() 
{
	if(!UpdateData())
		return;

	char szBuf[1240] = {0};

	int n_Mobile = 0;
	long id = atoi((LPCTSTR)m_id);
	if(id < 0)
	{		
		sprintf(szBuf, "短信ID只能为整数");
		MessageBox(szBuf, "信息", MB_OK|MB_ICONINFORMATION);
		return;
	}

	long ret = -1;

	if(m_wap_push == 0)
	{
		//ret = sendSM(m_mobile,m_strContent,id);
		long srcID =0;
		if(m_srcid == "")
		{
			srcID = id;	
		}else
		{
			srcID = atol(m_srcid);
		}
		ret  = sendSMAdvTime(m_mobile, m_strContent, m_sendTime, id,srcID );
	}
	else if(m_wap_push == 1)
	{
		if(strlen(m_url) == 0)
		{
			sprintf(szBuf, "请输入WAP Push的链接地址！");
			MessageBox(szBuf, "信息", MB_OK|MB_ICONINFORMATION);
			m_edit_url.SetFocus();
			return;
		}
			const char* s;
			s = m_url;
			int iBeTrimLeftLen,iBeTrimRightLen;
			char* p = (char*)s;
			char* str = (char*)s;
			int len = strlen(str);

			while(*p==' '||*p=='\r'||*p=='\n')
			{
				p++;
			}

			iBeTrimLeftLen = p-str;
			p = str + len-1;

			while(*p==' '||*p=='\r'||*p=='\n') 
			{
				p--;
			}
			iBeTrimRightLen = str + len - p - 1;
			len -= iBeTrimLeftLen + iBeTrimRightLen;
			len = len < 0 ? 0 : len;
		
			if(len == 0)
			{
				sprintf(szBuf, "URL参数不合法，请重新输入！");
				MessageBox(szBuf, "信息", MB_OK|MB_ICONINFORMATION);
				return;
			}
			long srcID =0;
			if(m_srcid == "")
			{
				srcID = id;	
			}else
			{
				srcID = atol(m_srcid);
			}
		ret = sendWapPushSMAdvTime( m_mobile, m_strContent, id, srcID, m_url, m_sendTime);
	}
	else if(m_wap_push == 2)
	{
		char aaa [] = {(char) 0X61, (char)0X62, (char)0X63,0};
		ret = sendAdvPDU("13800138000",aaa,88,89,0,0,0,"","","",-1);
	}
	if(ret == 0)//成功
	{
		sprintf(szBuf, "提交发送短信成功！");
		MessageBox(szBuf, "信息", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		sprintf(szBuf, "发送失败， 错误代码：%d\n", ret );
		MessageBox(szBuf, "错误", MB_OK|MB_ICONERROR);
	}
}

int strTrimLen(const char* s)
{
	int iBeTrimLeftLen,iBeTrimRightLen;
	char* p = (char*)s;
	char* str = (char*)s;
	int len = strlen(str);

	while(*p==' '||*p=='\r'||*p=='\n')
	{
		p++;
	}

	iBeTrimLeftLen = p-str;
	p = str + len-1;

	while(*p==' '||*p=='\r'||*p=='\n') 
	{
		p--;
	}
	iBeTrimRightLen = str + len - p - 1;
	len -= iBeTrimLeftLen + iBeTrimRightLen;

	return len < 0 ? 0 : len;
}

void CVCDemoDlg::OnReceiveSM() 
{
	if(!UpdateData())
		return;

	int nCount = 0;
	char szBuf[BUF_SIZE*640] = {0};
	char szBufItem[BUF_SIZE * 2];


	MOItem moBody[100];

	memset(&moBody, 0,  sizeof(moBody));
	nCount = receiveSMAdv(moBody, 10, 100);
	//nCount = receiveSM(moBody,20);
	if ( nCount > 0)
	{
		sprintf(szBuf, "共收到 %d 条短信\n", nCount);
		
		for(int i = 0; i < nCount; i++)
		{
			sprintf(szBufItem, "短信ID：%ld\t 手机号码：%s\t 内容：%s\t  mo时间：%s\t msgFmt:%ld\t\n\n",
					moBody[i].smId,moBody[i].mobile, moBody[i].smContent,moBody[i].moTime,moBody[i].msgFmt);
			strcat(szBuf, szBufItem);
		}
		
		MessageBox(szBuf, "信息", MB_OK|MB_ICONINFORMATION);
	}
	else if(nCount == 0)
	{
		MessageBox("没有短信可接收！", "信息", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		sprintf(szBuf, "接收失败，错误代码：%d", nCount);
		MessageBox(szBuf, "错误", MB_OK|MB_ICONERROR);
	}	
}

void CVCDemoDlg::OnReceiveRPT() 
{
	if(!UpdateData())
		return;

	int nCount = 0;
	unsigned long ulSmId = 0;
	char szBuf[BUF_SIZE*640] = {0};
	char szBufItem[BUF_SIZE * 2];

	RptItem rptBody[20];

	memset(&rptBody, 0, sizeof(rptBody));
	nCount = receiveRPT(rptBody, 20);
	if ( nCount > 0)
	{
		sprintf(szBuf, "共收到 %d 条回执\n", nCount);
		for(int i = 0; i< nCount; i++)
		{
				sprintf(szBufItem, "手机号码: %s\t短信ID: %d\t回执Id: %d\t回执描述: %s\t 回执时间：%s\n\n",
					 rptBody[i].mobile, rptBody[i].smId, rptBody[i].rptId, rptBody[i].rptDesc,rptBody[i].rptTime);
				strcat(szBuf, szBufItem);
		}
		MessageBox(szBuf, "信息", MB_OK|MB_ICONINFORMATION);
	}
	else if(nCount == 0)
	{
		MessageBox("没有回执可接收！", "信息", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		sprintf(szBuf, "接收失败，错误代码：%d", nCount);
		MessageBox(szBuf, "错误", MB_OK|MB_ICONERROR);
	}	
}

void CVCDemoDlg::OnExit() 
{	
	if(CanExit())
	{
		release();
		exit(0);
	}
}

void CVCDemoDlg::OnRelease() 
{
	int nRet = release();
	if (nRet==0)
	{
		MessageBox("释放成功", "信息", MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		char szBuf[BUF_SIZE*640] = {0};
		sprintf(szBuf, "释放失败，错误代码：%d", nRet);
		MessageBox(szBuf, "错误", MB_OK|MB_ICONINFORMATION);
	}
	m_bInit = FALSE;
	
}

void CVCDemoDlg::OnInit() 
{
	if(!UpdateData())
		return;

	if (m_bInit)
	{
		release();
	}

	int nResult = initWithDB(m_ip, m_username, m_pwd, m_strDBName, m_apiCode);

	char szBuf[BUF_SIZE] = {0};
	if(nResult == 0)
	 {
		sprintf(szBuf, " 初始化成功");
		MessageBox(szBuf, "信息", MB_OK|MB_ICONINFORMATION);
		m_bInit = TRUE;
		return;
	 } else
	 {
		sprintf(szBuf, " 初始化失败，错误代码：%d",nResult);
		MessageBox(szBuf, "错误", MB_OK|MB_ICONERROR);
		m_bInit = FALSE;
		return;
	 }  
}


void CVCDemoDlg::OnRadioIsWappush() 
{
	// TODO: Add your control notification handler code here
	m_wap_push = 0;
	m_edit_url.EnableWindow(TRUE);
}

void CVCDemoDlg::OnRadioNoWappush() 
{
	// TODO: Add your control notification handler code here
	m_wap_push = 1;
	m_edit_url.EnableWindow(FALSE);
}

void CVCDemoDlg::OnRadioPDU() 
{
	// TODO: Add your control notification handler code here
	m_wap_push = 2;
	m_edit_url.EnableWindow(FALSE);
}
