
#include "Websphere.h"
#include "Utils.h"

#include <math.h>
/*
#include "..\..\base\funcGeneral.h"
#include "..\..\base\XMLFunction.h"

HRESULT WalkTree(MSXML2::IXMLDOMNode* node, int level)
{
    MSXML2::IXMLDOMNode* pChild, *pNext;
    BSTR nodeName;
    MSXML2::IXMLDOMNamedNodeMap* pattrs;

    node->get_nodeName(&nodeName);
    for (int i = 0; i < level; i++)
        printf(" ");
    printf("%s",nodeName);
    SysFreeString(nodeName);


    if (SUCCEEDED(node->get_attributes(&pattrs)) && pattrs != NULL)
    {
        pattrs->nextNode(&pChild);
        while (pChild)
        {
            BSTR name;
            pChild->get_nodeName(&name);
            printf(" %s='", name);
            ::SysFreeString(name);

            VARIANT value;
            pChild->get_nodeValue(&value);
            if (value.vt == VT_BSTR)
            {
                printf("%s", V_BSTR(&value));
            }
            printf("'");
            VariantClear(&value);
            pChild->Release();
            pattrs->nextNode(&pChild);
        }
        pattrs->Release();
    }
    printf("\n");

    node->get_firstChild(&pChild);
    while (pChild)
    {
        WalkTree(pChild, level+1);
        pChild->get_nextSibling(&pNext);
        pChild->Release();
        pChild = pNext;
    }

    return S_OK;
}
*/

BOOL WEBSPHERE_MONITOR(char *server, char *servlet, int port, char *servername, 
					   char *uid, char *pwd, char *pserver, int pport, char *puid, 
					   char *ppwd, int ngid, int nmid, char *custpath, 
					   char *szReturn)
{
//	FILE *fp = NULL;
//	char szUrl[256] = {0};
//	sprintf(szUrl, "http://%s:%d%s", server, port, servlet);
//
//	char msg[100 * 1024] = {0};
//	BOOL bResult = GetUrlData(szUrl, 20, uid, pwd, pserver, pport, puid, ppwd, 1, msg);
//	if(!bResult)
//	{
//		sprintf(szReturn, "error=%s", msg);
//		return FALSE;
//	}
//
//	CString strInfo(msg);
//	int index = strInfo.Find("<!DOCTYPE", 0);
//	if(index >= 0)
//	{
//		int index2 = strInfo.Find("<!--", 0);
//		if(index2 >= 0)
//		{
//			strInfo.Delete(index, index2 - index);
//		}
//	}
//
//	char szWpsFile[256] = {0};
//	sprintf(szWpsFile, "%s\\Wps_%d_%d.xml", custpath, ngid, nmid);
//	if((fp = fopen(szWpsFile, "w")) == NULL)
//	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Websphere_1%>"));//<%IDS_Websphere_1%>
//		return FALSE;
//	}
//
//	fprintf(fp, "%s", strInfo);
//	fclose(fp);
//
//	memset(msg, 0, 100 * 1024);
//	if((fp = fopen(szWpsFile, "r")) == NULL)
//	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Websphere_2%>"));//<%IDS_Websphere_2%>
//		return FALSE;
//	}
//
//	fread(msg, sizeof(char), sizeof(msg), fp);
//	fclose(fp);
//
//	strInfo = msg;
//
//	CoInitialize(NULL);
//
//	HRESULT hr = 0;
//	MSXML2::IXMLDOMDocument2 *pDoc = NULL;
//
//    hr = CoCreateInstance(MSXML2::CLSID_DOMDocument40, NULL, CLSCTX_INPROC_SERVER,
//                                MSXML2::IID_IXMLDOMDocument2, (void**)&pDoc);
//	if(FAILED(hr))
//	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Websphere_3%>"));//<%IDS_Websphere_3%>
//		return FALSE;
//	}
//
//	VARIANT_BOOL vb;
//	hr = pDoc->loadXML(strInfo.AllocSysString(), &vb);
//	if(FAILED(hr))
//	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Websphere_4%>"));//<%IDS_Websphere_4%>
//		return FALSE;
//	}
//
//	char szxslFile[256] = {0};
//	sprintf(szxslFile, "%s\\cgi-bin\\websphere.xsl", FuncGetInstallRootPath());
//	CString strResult = FuncGetHTML(pDoc, szxslFile);
//
//	CoUninitialize();
//
//	CStringList lstItems;
//	CreateListByStringandMark(strResult, lstItems, "$$$");
//	
//	if(lstItems.GetCount() == 0)
//	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Websphere_5%>"));//<%IDS_Websphere_5%>
//		return FALSE;
//	}
//
//	bResult = FALSE;
//	POSITION pos = lstItems.FindIndex(0);
//	while(pos)
//	{
//		CString strItem = lstItems.GetNext(pos);
//		strItem.Replace("#", "&");
//
//		CString strTMem = FuncGetParamValue(strItem, "totalMemory");
//		CString strFMem = FuncGetParamValue(strItem, "freeMemory");
//		CString strUMem = FuncGetParamValue(strItem, "usedMemory");
//		CString strCThr = FuncGetParamValue(strItem, "threadCreates");
//		CString strDThr = FuncGetParamValue(strItem, "threadDestroys");
//		CString strAThr = FuncGetParamValue(strItem, "currentValue");
//
//		if(	strTMem.IsEmpty() || strFMem.IsEmpty() || strUMem.IsEmpty() ||
//			strCThr.IsEmpty() || strDThr.IsEmpty() || strAThr.IsEmpty())
//			continue;
//		else
//		{
//			CString strServerName = FuncGetParamValue(strItem, "Server");
//			if(strServerName.Compare(servername))
//				continue;
//
//			sprintf(szReturn, "totalMemory=%.0f$freeMemory=%.0f$usedMemory=%.0f$threadCreates=%.0f$threadDestroys=%.0f$activeThreads=%.0f$", 
//								atof(strTMem), atof(strFMem), atof(strUMem), 
//								atof(strCThr), atof(strDThr), atof(strAThr));
//			bResult = TRUE;
//			break;
//		}
//	}
//	return bResult;
return FALSE;
}

BOOL WEBSPHERES_MONITOR(char *server, char *servlet, int port, char *uid, char *pwd, 
					    char *pserver, int pport, char *puid, char *ppwd, 
					    int ngid, int nmid, char *custpath, 
						CStringList &lstTexts, CStringList &lstValues)
{
//
//	FILE *fp = NULL;
//	char szUrl[256] = {0};
//	sprintf(szUrl, "http://%s:%d%s", server, port, servlet);
//
//	char msg[100 * 1024] = {0};
//	BOOL bResult = GetUrlData(szUrl, 20, uid, pwd, pserver, pport, puid, ppwd, 1, msg);
//	if(!bResult)
//	{
//		return FALSE;
//	}
//
//	CString strInfo(msg);
//	int index = strInfo.Find("<!DOCTYPE", 0);
//	if(index >= 0)
//	{
//		int index2 = strInfo.Find("<!--", 0);
//		if(index2 >= 0)
//		{
//			strInfo.Delete(index, index2 - index);
//		}
//	}
//
//	char szWpsFile[256] = {0};
//	sprintf(szWpsFile, "%s\\Wps_%d_%d.xml", custpath, ngid, nmid);
//	if((fp = fopen(szWpsFile, "w")) == NULL)
//	{
//		return FALSE;
//	}
//
//	fprintf(fp, "%s", strInfo);
//	fclose(fp);
//
//	memset(msg, 0, 100 * 1024);
//	if((fp = fopen(szWpsFile, "r")) == NULL)
//	{
//		return FALSE;
//	}
//
//	fread(msg, sizeof(char), sizeof(msg), fp);
//	fclose(fp);
//
//	strInfo = msg;
//
//	CoInitialize(NULL);
//
//	HRESULT hr = 0;
//	MSXML2::IXMLDOMDocument2 *pDoc = NULL;
//
//    hr = CoCreateInstance(MSXML2::CLSID_DOMDocument40, NULL, CLSCTX_INPROC_SERVER,
//                                MSXML2::IID_IXMLDOMDocument2, (void**)&pDoc);
//	if(FAILED(hr))
//	{
//		return FALSE;
//	}
//
//	VARIANT_BOOL vb;
//	hr = pDoc->loadXML(strInfo.AllocSysString(), &vb);
//	if(FAILED(hr))
//	{
//		return FALSE;
//	}
//
//	char szxslFile[256] = {0};
//	sprintf(szxslFile, "%s\\cgi-bin\\websphere.xsl", FuncGetInstallRootPath());
//	CString strResult = FuncGetHTML(pDoc, szxslFile);
//
//	CoUninitialize();
//
//	CStringList lstItems;
//	CreateListByStringandMark(strResult, lstItems, "$$$");
//	
//	if(lstItems.GetCount() == 0)
//	{
//		return FALSE;
//	}
//
//	bResult = FALSE;
//	POSITION pos = lstItems.FindIndex(0);
//	while(pos)
//	{
//		CString strItem = lstItems.GetNext(pos);
//		strItem.Replace("#", "&");
//
//		CString strTMem = FuncGetParamValue(strItem, "totalMemory");
//		CString strFMem = FuncGetParamValue(strItem, "freeMemory");
//		CString strUMem = FuncGetParamValue(strItem, "usedMemory");
//		CString strCThr = FuncGetParamValue(strItem, "threadCreates");
//		CString strDThr = FuncGetParamValue(strItem, "threadDestroys");
//		CString strAThr = FuncGetParamValue(strItem, "currentValue");
//
//		if(	strTMem.IsEmpty() || strFMem.IsEmpty() || strUMem.IsEmpty() ||
//			strCThr.IsEmpty() || strDThr.IsEmpty() || strAThr.IsEmpty())
//			continue;
//		else
//		{
//			lstTexts.AddTail(FuncGetParamValue(strItem, "Server"));
//			lstValues.AddTail(FuncGetParamValue(strItem, "Server"));
//
//			bResult = TRUE;
//		}
//	}
//
//	return bResult;
return FALSE;
}
/*
CString FuncGetHTML(MSXML2::IXMLDOMDocument2 * pDoc, CString strXSLFileName)
{
	HRESULT hr = 0;
	MSXML2::IXMLDOMDocument2 *pXSLDoc = NULL;
	BSTR bstrStyle;

	bstrStyle = FuncAsciiToBSTR(strXSLFileName);
	
	VARIANT_BOOL vb;
	VARIANT vURL;
	
	hr = CoCreateInstance(MSXML2::CLSID_DOMDocument40, NULL, CLSCTX_INPROC_SERVER,
		MSXML2::IID_IXMLDOMDocument, (void**)&pXSLDoc);

	if(FAILED(hr))
	{
		SAFERELEASE(pXSLDoc);
		return "";
	}
	
	hr = pXSLDoc->put_async(VARIANT_TRUE);
	if(FAILED(hr))
		return "";
	
	// Load xml document from the given URL
	vURL.vt = VT_BSTR;
	V_BSTR(&vURL) = bstrStyle;
	pXSLDoc->load(vURL, &vb);
	
	// Now wait for download to complete !
	FuncWaitForCompletion(pXSLDoc);
	
	hr = FuncCheckLoad(pXSLDoc);
	
	// display
	BSTR xmlString;
	pDoc->transformNode(pXSLDoc, &xmlString);
	CString strxml(xmlString);

	SAFERELEASE(pXSLDoc);
    SysFreeString(bstrStyle);

	return strxml;
}

BOOL GetAttributeFromPage(CString &strMessage, CString strAttrName, CStringList &lstAttrValues)
{
	int		index = 0, 
			index2 = 0;
	CString	strNodeName = _T("");
	CString strFindNode = _T("");

	index = strAttrName.Find('.', 0);

	strNodeName = strAttrName.Left(index);
	strAttrName.Delete(0, index + 1);

	while(1)
	{
		strFindNode.Format("<%s", strNodeName);
		index = strMessage.Find(strFindNode, 0);
		if(index >= 0)
		{
			strMessage.Delete(0, index);
			index = strMessage.Find("\r\n", 0);
			CString strThisLine = strMessage.Left(index);
			index2 = strThisLine.Find("/>", 0);
			if(index2 >= 0)
			{
				if(strAttrName.Find('.', 0) >= 0)
					strMessage.Delete(0, index + 2);
				else
				{
					index = strThisLine.Find(strAttrName + "=\"", 0);
					if(index >= 0)
					{
						strThisLine.Delete(0, index + strAttrName.GetLength() + 2);
						index = strThisLine.Find("\"", 0);
						if(index > 0)
						{
							lstAttrValues.AddTail(strThisLine.Left(index));
						}
					}
					break;
				}
			}
			else
			{
				GetAttributeFromPage(strMessage, strAttrName, lstAttrValues);
				strFindNode.Format("</%s>", strNodeName);
				index2 = strMessage.Find(strFindNode, 0);
				if(index2 >= 0)
					strMessage.Delete(0, index2 + strNodeName.GetLength() + 5);
				else
					strMessage.Delete(0, index + 2);
			}
		}
		else
		{
			break;
		}
	}

	return TRUE;
}
*/