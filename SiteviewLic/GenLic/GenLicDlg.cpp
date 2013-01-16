// GenLicDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GenLic.h"
#include "GenLicDlg.h"

#include "DllCall.h"
//#include "constdata.h"
#include "LogDlg.h"

#include"iphlpapi.h"
#include ".\genlicdlg.h"


#pragma comment(lib,"Iphlpapi.lib")
//#pragma comment(lib,"../release/IODll.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

// static char* MachineID="001320A50A22";//tgf wuxian
//static char* MachineID="000D602F6199";//oldhand
static char* MachineID="0x0018d990";//oldhand






static char* psw = "siteview";



// CGenLicDlg �Ի���


CGenLicDlg::CGenLicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGenLicDlg::IDD, pParent)
	, m_RdProduct(0)
{

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGenLicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_SELECT_ALL, m_CheckBox);
	DDX_Control(pDX, IDC_EDT_POINTS, m_EditPoints);
	DDX_Control(pDX, IDC_SPIN_POINTS, m_SpinCtrl_Points);
	DDX_Control(pDX, IDC_SPIN_DEVICES, m_SpinCtrl_Devices);
	DDX_Control(pDX, IDC_SPIN_DELAYDAYS, m_SpinCtrl_DelayDays);
	DDX_Control(pDX, IDC_EDT_DEVICES, m_EditDevices);
	DDX_Control(pDX, IDC_EDT_DEPLAYDAYS, m_EditDelayDays);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_StartDate);
	DDX_Control(pDX, IDC_EDT_USERNAME, m_EditUser);
	DDX_Control(pDX, IDC_EDT_SUPPORT, m_EditSupport);
	DDX_Control(pDX, IDC_CHK_DEFAULT, m_CheckDefault);
	DDX_Control(pDX, IDC_ED_PSW, m_EditPsw);
	DDX_Control(pDX, ED_MachineCode, m_MachineCode);
	DDX_Control(pDX, IDC_CHK_SELECT_ALL, m_CheckBox);
	DDX_Control(pDX, IDC_FORMALVER, m_FormalVer);
}

BEGIN_MESSAGE_MAP(CGenLicDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RD_ECC, &CGenLicDlg::OnBnClickedRdEcc)
	ON_BN_CLICKED(IDC_RD_NNM, &CGenLicDlg::OnBnClickedRdNnm)
	ON_BN_CLICKED(IDC_RD_DM, &CGenLicDlg::OnBnClickedRdDm)
	ON_BN_CLICKED(IDC_CHK_SELECT_ALL, &CGenLicDlg::OnBnClickedChkSelectAll)
	ON_BN_CLICKED(IDOK, &CGenLicDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BT_GETOLDLIC, &CGenLicDlg::OnBnClickedBtGetoldlic)
	//ON_BN_CLICKED(IDC_BT_TEST, &CGenLicDlg::OnBnClickedBtTest)
	//ON_BN_CLICKED(IDC_BT_INCTIME, &CGenLicDlg::OnBnClickedBtInctime)
	ON_BN_CLICKED(IDC_ITSM, OnBnClickedItsm)
	ON_BN_CLICKED(IDC_LOG, OnBnClickedLog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()
// CGenLicDlg ��Ϣ�������

BOOL CGenLicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	bool bCanRun = false;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *) GlobalAlloc(GMEM_FIXED, sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	
	// Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) {
		GlobalFree (pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) GlobalAlloc(GMEM_FIXED, ulOutBufLen);
	}
	
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		pAdapter = pAdapterInfo;
		while (pAdapter) 
		{
			char mac[18]={0};
			sprintf(mac, "%02X%02X%02X%02X%02X%02X",
				pAdapter->Address[0],
				pAdapter->Address[1],
				pAdapter->Address[2],
				pAdapter->Address[3],
				pAdapter->Address[4],
				pAdapter->Address[5]);
			if(strcmp(mac, MachineID) == 0)
			{
				bCanRun = true;
				break;
			}
			pAdapter = pAdapter->Next;
		}
	}
	GlobalFree (pAdapterInfo);

	if(bCanRun)
	{
		this->m_EditPsw.ShowWindow(SW_SHOW);
	}


	m_RdProduct=0;
	m_EditPoints.SetWindowText(TEXT("200")); 
	m_SpinCtrl_Points.SetRange(0,500);   
	m_SpinCtrl_Points.SetBuddy((CWnd*) &m_EditPoints);   

	m_EditDevices.SetWindowText(TEXT("50")); 
	m_SpinCtrl_Devices.SetRange(0,2000);   
	m_SpinCtrl_Devices.SetBuddy((CWnd*) &m_EditDevices);   

	m_EditDelayDays.SetWindowText(TEXT("30")); 
	m_SpinCtrl_DelayDays.SetRange(0,60);   
	m_SpinCtrl_DelayDays.SetBuddy((CWnd*) &m_EditDelayDays);   

	m_CheckList.SubclassDlgItem(IDC_LIST_MODULES, this); // IDC_LIST1��ListBox�ؼ�����ԴID
    m_CheckList.SetCheckStyle(BS_AUTOCHECKBOX);

	m_StartDate.SetFormat(_T("yyyy-MM-dd"));

	m_EditUser.SetWindowText("trial");
	m_EditSupport.SetWindowText("support");
	m_MachineCode.SetWindowText("c13810e87266a6789c5984231c4d820bfccff82ef555522e5a0e9bad61e52285f0d7ef32ffeb3d23");

	//m_CheckList.AddString((LPCTSTR)"Tuopu");
	//m_CheckList.AddString((LPCTSTR)"ITOSS");

	//int   iIndex   =   0;
	//int state = m_CheckList.GetCheck(iIndex);
	//m_CheckList.SetCheck(1, 1);
	//m_CheckList.SetCheck(0, 1);

	//CString str;
	//str.Format("%d", state);
//	MessageBox(str);
    UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGenLicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{	
	CDialog::OnSysCommand(nID, lParam);	
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGenLicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CGenLicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGenLicDlg::OnBnClickedRdEcc()
{
	m_RdProduct = 1;

	m_CheckList.ResetContent();
	for(int i = 0; i < sizeof(modules_ecc)/sizeof(MODULEITEM); i++)
	{
		m_CheckList.InsertString(modules_ecc[i].Num, (LPCTSTR)modules_ecc[i].name);
		if(modules_ecc[i].bChecked)
			m_CheckList.SetCheck(i, true);
	}
}

void CGenLicDlg::OnBnClickedRdNnm()
{
	m_RdProduct = 2;
	m_CheckList.ResetContent();
	for(int i = 0; i < sizeof(modules_nnm)/sizeof(MODULEITEM); i++)
	{
		m_CheckList.InsertString(modules_nnm[i].Num, (LPCTSTR)modules_nnm[i].name);
		if(modules_nnm[i].bChecked)
			m_CheckList.SetCheck(i, true);
	}
}

void CGenLicDlg::OnBnClickedRdDm()
{
	m_RdProduct = 3;

	m_CheckList.ResetContent();
	for(int i = 0; i < sizeof(modules_dm)/sizeof(MODULEITEM); i++)
	{
		m_CheckList.InsertString(modules_dm[i].Num, (LPCTSTR)modules_dm[i].name);
		if(modules_dm[i].bChecked)
			m_CheckList.SetCheck(i, true);
	}
}


void CGenLicDlg::OnBnClickedItsm()
{
	m_RdProduct = 4;

	m_CheckList.ResetContent();
	//for(int i = 0; i < sizeof(modules_dm)/sizeof(MODULEITEM); i++)
	//{
	//	m_CheckList.InsertString(modules_dm[i].Num, (LPCTSTR)modules_dm[i].name);
	//	if(modules_dm[i].bChecked)
	//		m_CheckList.SetCheck(i, true);
	//}
}

void CGenLicDlg::OnBnClickedChkSelectAll()
{
	int nCount = m_CheckList.GetCount();
	if(this->m_CheckBox.GetCheck())
	{
		for(int i = 0; i < nCount; i++)
		{
			this->m_CheckList.SetCheck(i, true);
		}
	}
	else
	{
		for(int i = 0; i < nCount; i++)
		{
			this->m_CheckList.SetCheck(i, false);
		}
	}
}


void CGenLicDlg::OnBnClickedOk()
{


	bool bCanRun = false;

    //try
    //{
    //    __asm
    //    {
    //        push edx
    //        push ecx
    //        push ebx
    //        mov eax, 'VMXh'
    //        mov ebx, 0
    //        mov ecx, 10
    //        mov edx, 'VX'
    //        in eax, dx
    //        cmp ebx, 'VMXh'
    //        setz [bCanRun]
    //        pop ebx
    //        pop ecx
    //        pop edx
    //    }
    //}
    //catch(...)//(EXCEPTION_EXECUTE_HANDLER)
    //{
    //    bCanRun = false;
    //}

	//if(!bCanRun)
	//{
	//	MessageBox("��������������ϴ���License!");
	//	return;
	//}


	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *) GlobalAlloc(GMEM_FIXED, sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	
	// Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) {
		GlobalFree (pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) GlobalAlloc(GMEM_FIXED, ulOutBufLen);
	}
	
	bCanRun = false;
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		pAdapter = pAdapterInfo;
		while (pAdapter) 
		{
			char mac[18]={0};
			sprintf(mac, "%02X%02X%02X%02X%02X%02X",
				pAdapter->Address[0],
				pAdapter->Address[1],
				pAdapter->Address[2],
				pAdapter->Address[3],
				pAdapter->Address[4],
				pAdapter->Address[5]);
			if(strcmp(mac, MachineID) == 0)
			{
				bCanRun = true;
				break;
			}
			pAdapter = pAdapter->Next;
		}
	}
	GlobalFree (pAdapterInfo);

	//cxy 2012/08/15
	/*if(!bCanRun)
	{
		MessageBox("��������̨�����ϴ���License!");
		return;
	}*/

	/*CString strpsw = "";
	m_EditPsw.GetWindowText(strpsw);
	if(strpsw.Compare(psw) != 0)
	{
		MessageBox("�������!");
		return;
	}*/

	CString strMsg = GetData();
	if(!strMsg.IsEmpty())
	{
		MessageBox(strMsg);
	}
	else
	{		
		CDllCall dllinvoke;
		if(m_CheckDefault.GetCheck())
		{
			if(dllinvoke.InitLic(srcData))
			{
				MessageBox("�ɹ�����ȱʡLicesne!");
			}
			else
				MessageBox("����ȱʡLicenseʧ��!");
		}
		else
		{
			try
			{
				if(dllinvoke.GenLic(srcData))
				{
					MessageBox("�ɹ�����Licesne!");
				}
				else
					MessageBox("����Licenseʧ��!");
			}
			catch(...)
			{
				MessageBox("����Licenseʧ��!");
			}
		}
	}
}


// ��ȡ���õ�����
CString CGenLicDlg::GetData(void)
{
	UpdateData(TRUE);
	CString strProductName, strModules=CString(""), strSel;
	switch(m_RdProduct)
	{
	case 1:
		strProductName = "ECC";
		for(int m = 0; m < m_CheckList.GetCount(); m++)
		{
			if(m_CheckList.GetCheck(m))
			{
				m_CheckList.GetText(m, strSel);
				for(int n = 0; n < sizeof(modules_ecc)/ sizeof(MODULEITEM); n++)
				{
					if(strSel.CompareNoCase(modules_ecc[n].name) == 0)
					{
						strModules += CString(modules_ecc[n].shortName) + ",";
						break;
					}
				}
			}
		}
		strModules = strModules.TrimRight(",");
		break;
	case 2:
		strProductName = "NNM";
		for(int m = 0; m < m_CheckList.GetCount(); m++)
		{
			if(m_CheckList.GetCheck(m))
			{
				m_CheckList.GetText(m, strSel);
				for(int n = 0; n < sizeof(modules_nnm)/ sizeof(MODULEITEM); n++)
				{
					if(strSel.CompareNoCase(modules_nnm[n].name) == 0)
					{
						strModules += CString(modules_nnm[n].shortName) + ",";
						break;
					}
				}
			}
		}
		strModules = strModules.TrimRight(",");
		break;
	case 3:
		strProductName = "DM";
		for(int m = 0; m < m_CheckList.GetCount(); m++)
		{
			if(m_CheckList.GetCheck(m))
			{
				m_CheckList.GetText(m, strSel);
				for(int n = 0; n < sizeof(modules_dm)/ sizeof(MODULEITEM); n++)
				{
					if(strSel.CompareNoCase(modules_dm[n].name) == 0)
					{
						strModules += CString(modules_dm[n].shortName) + ",";
						break;
					}
				}
			}
		}
		strModules = strModules.TrimRight(",");
		break;
	case 4:
		strProductName = "ITSM";
		break;
	default:
		return CString("��ѡ���Ʒ!");
	}
	//strProductName = CString("SiteView") + strProductName;
	strProductName = CString("SiteView") + strProductName;
	CString str;
	strcpy(srcData.ProductName, (LPCSTR)strProductName);
	strcpy(srcData.Modules, (LPCSTR)strModules);

	m_StartDate.GetWindowText(str);
	str.Replace("-", "");
	strcpy(srcData.StartDate, (LPCSTR)str);
	//cxy 2012/08/21
	strcpy(srcData.MakeDate, (LPCSTR)str);
	srcData.DelayTimes = 2;

	m_EditPoints.GetWindowText(str);
	if(!str.IsEmpty() && isNumeric((LPCSTR)str))
	{
		srcData.Points = atoi((LPCSTR)str);
	}
	else
		return CString("����������Ϊ�գ��ұ���Ϊ����!");

	m_EditDevices.GetWindowText(str);
	if(!str.IsEmpty() && isNumeric((LPCSTR)str))
	{
		srcData.Devices = atoi((LPCSTR)str);
	}
	else
		return CString("�豸������Ϊ�գ��ұ���Ϊ����!");

	m_EditDelayDays.GetWindowText(str);
	if(!str.IsEmpty() && isNumeric((LPCSTR)str))
	{
		srcData.Delaydays = atoi((LPCSTR)str);
	}
	else
		return CString("������������Ϊ�գ��ұ���Ϊ����!");

	m_EditUser.GetWindowText(str);
	strcpy(srcData.UserName, (LPCSTR)str);
	m_EditSupport.GetWindowText(str);
	strcpy(srcData.Support, (LPCSTR)str);

	strcpy(srcData.RegCode, "NULL");

//	strcpy(srcData.SysFeatures, "NULL");
	CString strMask, strCode("NULL");
	m_MachineCode.GetWindowText(strMask);
	if( (srcData.Delaydays > 180 || strProductName == "SiteViewITSM") &&
		(strMask.IsEmpty() || strMask.GetLength() == 0 || strMask.GetLength() % 2 != 0))
	{
		//MessageBox("�������ʽ����ȷ!");
		return CString("�������ʽ����ȷ!");
	}
	unsigned char encbuff[1024] = {0};
	unsigned char plainText[1024] = {0};

	chartohex((unsigned char*)(LPCTSTR)strMask,strMask.GetLength(),(unsigned char*)&encbuff);
    int Length = des_decrypt((unsigned char*)&encbuff,strMask.GetLength()/2,(unsigned char*)&plainText);
	strCode.Format("%s",plainText);
	//MessageBox(strCode);
	if (!verifiylicense(plainText))
	{
		return CString("�������ʽ����ȷ!");
	}
	strcpy(srcData.SysFeatures, (LPCSTR)strCode);

	srcData.TotalTime = 0;

	//cxy 2012/08/16
//	strcpy(srcData.Component_MD5, "");
//	strcpy(srcData.License_MD5, "");

	//cxy 2012/08/21
	//strcpy(srcData.RegCode,  (LPCSTR)strCode);
	strcpy(srcData.Customer,  "��������");
	strcpy(srcData.Tokens, "NULL");
	
	srcData.Isfinal = 1;
	srcData.Allowmachine = 1;
	/*strcpy(srcData.ekey, "99593");
	strcpy(srcData.dkey, "284186897990237326457");
	strcpy(srcData.nkey, "363325105734158528063");*/

	return CString("");
}

void CGenLicDlg::OnBnClickedBtGetoldlic()
{
	CFileDialog fileDlg(true, "lic");
	if( fileDlg.DoModal() == IDCANCEL )
	{
		return;
	}
	CString pathName = fileDlg.GetPathName();
	if(pathName.IsEmpty())
	{
		MessageBox("��ѡ��ԭ����License�ļ�!");
		return;
	}

	CDllCall dllInvoke;

	LICENSEDATA licData;

	if(!dllInvoke.GetOldLic((char*)pathName.GetBuffer(0), &licData))
	{
		MessageBox("License�ļ����ݲ���ȷ!");
		return;
	}
	
	//��ʾԭ����License
	vector <string> mdls = tokenize(licData.Modules, ",", true);

	CString str, str01;
	str.Format("%s",licData.ProductName);
	if(str.Mid(8, 3).Compare("ECC") == 0)
	{
		m_RdProduct = 1;
		SendDlgItemMessage(IDC_RD_ECC, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(IDC_RD_NNM, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(IDC_RD_DM, BM_SETCHECK, BST_UNCHECKED, 0);

		m_CheckList.ResetContent();
		for(int i = 0; i < sizeof(modules_ecc)/sizeof(MODULEITEM); i++)
		{
			m_CheckList.InsertString(modules_ecc[i].Num, (LPCTSTR)modules_ecc[i].name);
			for(int j = 0; j < mdls.size(); j++)
			{
				if(mdls[j].compare(modules_ecc[i].shortName) == 0)
				{
					m_CheckList.SetCheck(i, true);
					break;
				}
			}
		}
	}
	else if(str.Mid(8, 3).Compare("NNM") == 0)
	{
		m_RdProduct = 2;
		SendDlgItemMessage(IDC_RD_NNM, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(IDC_RD_ECC, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(IDC_RD_DM, BM_SETCHECK, BST_UNCHECKED, 0);

		m_CheckList.ResetContent();
		for(int i = 0; i < sizeof(modules_nnm)/sizeof(MODULEITEM); i++)
		{
			m_CheckList.InsertString(modules_nnm[i].Num, (LPCTSTR)modules_nnm[i].name);
			for(int j = 0; j < mdls.size(); j++)
			{
				if(mdls[j].compare(modules_nnm[i].shortName) == 0)
				{
					m_CheckList.SetCheck(i, true);
					break;
				}
			}
		}
	}
	else if(str.Mid(8, 2).Compare("DM") == 0)
	{
		m_RdProduct = 3;
		SendDlgItemMessage(IDC_RD_DM, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(IDC_RD_NNM, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(IDC_RD_ECC, BM_SETCHECK, BST_UNCHECKED, 0);

		m_CheckList.ResetContent();
		for(int i = 0; i < sizeof(modules_dm)/sizeof(MODULEITEM); i++)
		{
			m_CheckList.InsertString(modules_dm[i].Num, (LPCTSTR)modules_dm[i].name);
			for(int j = 0; j < mdls.size(); j++)
			{
				if(mdls[j].compare(modules_dm[i].shortName) == 0)
				{
					m_CheckList.SetCheck(i, true);
					break;
				}
			}
		}
	}
	else if(str.Mid(8, 4).Compare("ITSM") == 0)
	{
		m_RdProduct = 4;
		SendDlgItemMessage(IDC_RD_DM, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(IDC_RD_NNM, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(IDC_RD_ECC, BM_SETCHECK, BST_UNCHECKED, 0);
		SendDlgItemMessage(IDC_ITSM, BM_SETCHECK, BST_CHECKED, 0);
		m_CheckList.ResetContent();
	}

	str.Format("%d", licData.Points);
	m_EditPoints.SetWindowText(str);
	str.Format("%d", licData.Devices);
	m_EditDevices.SetWindowText(str);
	str.Format("%d", licData.Delaydays);
	m_EditDelayDays.SetWindowText(str);

	m_EditUser.SetWindowText(licData.UserName);
	m_EditSupport.SetWindowText("");//licData.Support);

	CString strMask;
	char strCode[1024] = {0};
	des_hex_encrypt((unsigned char *)&licData.SysFeatures,strlen(licData.SysFeatures),(unsigned char *)&strCode);
	strMask.Format("%s",strCode);

	m_MachineCode.SetWindowText(strMask);

	SYSTEMTIME sysTime;
	memset(&sysTime, 0, sizeof(sysTime));
	str = licData.StartDate;
	sysTime.wYear = atoi((LPCSTR)str.Mid(0,4));
	sysTime.wMonth = atoi((LPCSTR)str.Mid(4,2));
	sysTime.wDay = atoi((LPCSTR)str.Mid(6,2));

//	m_StartDate.SetWindowText(licData.StartDate);

	m_StartDate.SetTime(&sysTime);

	UpdateData(FALSE);
}

void CGenLicDlg::OnBnClickedBtTest()
{
	CDllCall dllInvoke;

	char szLic[20480]={0};
	dllInvoke.GetLicString("DM2.0.lic", (char*)&szLic);

	if(dllInvoke.IsValidLic(NULL, (char*)&szLic, 1000, true))
	{
		MessageBox("����ͨ��!");
	}
	else
	{
		MessageBox("���Բ�ͨ��!");
	}
}

void CGenLicDlg::OnBnClickedBtInctime()
{
	CDllCall dllInvoke;

	char szLic[20480]={0};
	dllInvoke.GetLicString("DM2.0.lic", (char*)&szLic);

	if(dllInvoke.IncreaseTime((char*)&szLic, 1000))
	{
		MessageBox("OK!");
	}
	else
	{
		MessageBox("Error!");
	}
}

void CGenLicDlg::OnBnClickedBtCpuid()
{
	unsigned long s1,s2;   
   unsigned char vendor_id[]="------------";//CPU�ṩ��ID   
   CString str1,str2,str3;   
   // ����Ϊ���CPU ID�Ļ������ָ��   
   _asm    // �õ�CPU�ṩ����Ϣ    
   {     
       xor eax,eax   // ��eax��0   
       cpuid         // ��ȡCPUID��ָ��   
       mov dword ptr vendor_id,ebx   
       mov dword ptr vendor_id[+4],edx   
       mov dword ptr vendor_id[+8],ecx     
   }   
   str1.Format("%s",vendor_id);   
     
   _asm    // �õ�CPU ID�ĸ�32λ    
   {    
       mov eax,01h       
       xor edx,edx   
       cpuid   
       mov s2,eax   
   }   
   str2.Format("%08X-",s2);   
      
   _asm    // �õ�CPU ID�ĵ�64λ   
   {    
       mov eax,03h   
       xor ecx,ecx   
       xor edx,edx   
       cpuid   
       mov s1,edx   
       mov s2,ecx   
   }   
   str3.Format("%08X-%08X\n",s1,s2);   
    
   str2=str2+str3;   
   MessageBox(str1);
   MessageBox(str2);
   //m_editVendor.SetWindowText(str1);   
   //m_editCPUID.SetWindowText(str2);     

}
void CGenLicDlg::OnBnClickedLog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLogDlg *m_LogDlg;
	m_LogDlg = new CLogDlg(this);
	
	m_LogDlg->DoModal();
	delete m_LogDlg;
}

void CGenLicDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
   DestroyWindow(); 
}














//trimָʾ�Ƿ����մ���Ĭ��Ϊ������
vector<string> tokenize(const string& src, string tok, bool trim, string null_subst)
{
	vector<string> v;
	if( src.empty() || tok.empty() )
	{
		return v;
//		throw "tokenize: empty string\0";
	}
	S_T pre_index = 0, index = 0, len = 0;
	while( (index = src.find(tok, pre_index)) != npos )
//	while( (index = src.find_first_of(tok, pre_index)) != npos )
	{
		if( (len = index-pre_index)!=0 )
		v.push_back(src.substr(pre_index, len));
		else if(trim==false)
		v.push_back(null_subst);
		pre_index = index+ tok.length();
//		pre_index = index+1;
	}
	string endstr = src.substr(pre_index);
	if( trim==false ) v.push_back( endstr.empty()? null_subst:endstr );
	else if( !endstr.empty() ) v.push_back(endstr);
	return v;
}
