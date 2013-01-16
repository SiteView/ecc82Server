/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// addmonitor2nd.cpp
// ��Ӽ�����ĵڶ��������ݼ��������չʾ����
// �༭�����
// ����Ĭ�ϱ�������
// ��Ӳ�������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "addmonitor2nd.h"
#include "paramitem.h"
#include "rightview.h"
#include "mainview.h"
#include "listtable.h"
#include "resstring.h"
#include "basedefine.h"
#include "eccobjfunc.h"
#include "basefunc.h"
#include "debuginfor.h"
#include "pushbutton.h"
#include "returnitem.h"
#include "conditionparam.h"
#include "treeview.h"

#include "../userright/User.h"

#include "../base/OperateLog.h"

#include "../../opens/libwt/WText"
#include "../../opens/libwt/WLineEdit"
#include "../../opens/libwt/WCheckbox"
#include "../../opens/libwt/WComboBox"
#include "../../opens/libwt/WPushButton"
#include "../../opens/libwt/WTableCell"
#include "../../opens/libwt/WebSession.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::BASE_ADV_PARAM::ResetData()
{
    if(m_pDescription)  m_pDescription->setText("");
    if(m_pReportDesc)   m_pReportDesc->setText("");
    if(m_pPlan)         m_pPlan->setCurrentIndex(0);
    if(m_pCheckErr)     m_pCheckErr->setChecked(false);
    if(m_pMonitorFreq)  m_pMonitorFreq->setText("");
    if(m_pTimeUnit)     m_pTimeUnit->setCurrentIndex(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���캯��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CEccAddMonitor2nd::CEccAddMonitor2nd(WContainerWidget *parent):
CEccBaseTable(parent),
m_pGeneral(NULL),
m_pAdvance(NULL),
m_pBatch(NULL),
m_pSave(NULL),
m_pContinue(NULL),
m_pCancel(NULL),
m_pBack(NULL),
m_pContinueSep(NULL),
m_pBatchSep(NULL),
m_pErrorAlert(NULL),
m_pWarnningAlert(NULL),
m_pNormalAlert(NULL),
m_pHideButton(NULL),
m_pName(NULL),
m_pNameErr(NULL),
m_pFreqErr(NULL),
m_pDevName(NULL),
m_szDeviceID(""),
m_szNetworkSet(""),
m_szEditIndex(""),
m_szMonitorName(""),
m_szPoint("")
{
    m_nMTID = 0;
    setStyleClass("panel");
    initForm(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ͨ����������������ģ��ID���豸�ڵ�ID�� �豸�Ƿ��������豸��չʾ��Ӽ����ҳ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::AddMonitorByMTID(int nMTID, const string &szParentID, const string &szNetworkset)
{
    // ��ʾ ��һ�� ��ť
    if(m_pBack)
        m_pBack->show();

    // ��ʾ ��Ӳ����� ��ť
    if(m_pContinue) 
        m_pContinue->show();

    // ��ʾ ������� ��ťǰ�ķָ���
    if(m_pBatchSep)
        m_pBatchSep->show();

    // ��ʾ ��Ӳ����� ��ťǰ�ķָ���
    if(m_pContinueSep)
        m_pContinueSep->show();

    // ��ʾ�������
    if(m_pBatch)
        m_pBatch->show();
	
	//cxy add 07/10/17...
	//if(m_nMTID == 9)
		m_pUrlStepSetup->hide();

    // ���༭ �����ÿ�
    m_szEditIndex = "";

    // �����豸���� 
    if(m_pDevName)
    {
        const CEccTreeNode *pNode = CEccTreeView::getECCObject(szParentID);
        if(pNode)
            m_pDevName->setText(pNode->getName());
    }

    // ���ñ�����
    if(m_pTitle)
        m_pTitle->setText(SVResString::getResString("IDS_Add_Title") + m_szMonitorName + SVResString::getResString("IDS_Monitor_Title"));

    // �ж� ����Ӽ����ģ��ID�Ƿ�͵�ǰչʾID���
    if(m_nMTID == nMTID)
    {// ����
        // ��������
        resetData();

        // �жϴ�����豸ID �Ƿ���� ��ǰ�ѻ�����豸ID
        // �ж϶�̬�����Ƿ�Ϊ��
        // ���ذ�ť�Ƿ���ȷ����
        if(szParentID != m_szDeviceID && !m_lsDyn.empty() && m_pHideButton)
        {// ������������
            // ���� ������豸ID
            m_szDeviceID = szParentID;

            // ���»�ö�̬����
            list<CEccParamItem*>::iterator lstItem;
            for(lstItem = m_lsDyn.begin(); lstItem != m_lsDyn.end(); lstItem ++)
                (*lstItem)->clearDynData();

            string szCmd("hiddenbar();");
            if(!m_pHideButton->getEncodeCmd("xclicked()").empty())
                szCmd = "update('" + m_pHideButton->getEncodeCmd("xclicked()") + "');";
            WebSession::js_af_up = szCmd;
        }
        else
        {
            // ���طָ���
            // ����������� ��ť
            if(m_lsDyn.empty() && m_pBatch && m_pContinueSep)
            {
                m_pContinueSep->hide();
                m_pBatch->hide();
            }

            // ֱ���˳�
            WebSession::js_af_up = "hiddenbar();";
        }

        if(szParentID != m_szDeviceID)
            m_szDeviceID = szParentID;
    }
    else
    {// ������
        // ��������
        m_nMTID = nMTID;
        m_szDeviceID = szParentID;
        m_szNetworkSet = szNetworkset;
        m_szDynName = "";

        // �����ö������
        clearData();

        // ���豸ģ��
        OBJECT objMT = GetMonitorTemplet(nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
        if(objMT != INVALID_VALUE)
        {
            // ö�ٻ�������
            enumBaseParam(objMT);
            // ö�ٸ߼�����
            enumAdvParam(objMT);
            // ö�ٷ���ֵ
            enumReturnItem(objMT);
            // ö�ٱ�������
            enumAlert(objMT);

            // �õ����ڵ�
            MAPNODE objNode = GetMTMainAttribNode(objMT);
            if(objNode != INVALID_VALUE)
            {
                // �õ��������ʾ����
                if(FindNodeValue(objNode, svLabel, m_szMonitorName))
                    m_szMonitorName = SVResString::getResString(m_szMonitorName.c_str());

                // ���ü��������
                FindNodeValue(objNode, svIntPos, m_szPoint);
                if(m_szPoint.empty())
                    m_szPoint = "1";

                // ���ñ�����
                if(m_pTitle)
                    m_pTitle->setText(SVResString::getResString("IDS_Add_Title") + m_szMonitorName + SVResString::getResString("IDS_Monitor_Title"));
            }
            // �رռ����ģ��
            CloseMonitorTemplet(objMT);
        }

        // ������������
        createBaseParam();
        // �����߼�����
        createAdvParam();

        // ���ü��������
        if(m_pName)
            m_pName->setText(m_szMonitorName);

        // �ж϶�̬�����Ƿ�Ϊ��
        // ���ذ�ť�Ƿ���ȷ����
        if(!m_lsDyn.empty() && m_pHideButton)
        {// ��������
            // ö�ٶ�̬����
            string szCmd("hiddenbar();");
            if(!m_pHideButton->getEncodeCmd("xclicked()").empty())
                szCmd = "update('" + m_pHideButton->getEncodeCmd("xclicked()") + "');";
            WebSession::js_af_up = szCmd;
        }
        else
        {
            // ���������������

            // ���طָ���
            if(m_pContinueSep)
                m_pContinueSep->hide();

            // ����������� ��ť
            if(m_pBatch)
                m_pBatch->hide();

            WebSession::js_af_up = "hiddenbar();";
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::BatchAdd()
{
    list<ecc_value_list> lsValue;

    // �õ���̬���� ���
    list<CEccParamItem*>::iterator lsItem;
    for(lsItem = m_lsDyn.begin(); lsItem != m_lsDyn.end(); lsItem ++)
    {
        (*lsItem)->MakeValueList(lsValue);
        m_szDynName = (*lsItem)->getName();

        break;
    }

    // ��ʾ������� ����
    CEccRightView::showBatchAddForm(lsValue, m_szMonitorName);
    WebSession::js_af_up = "hiddenbar();";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ȡ�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::Cancel()
{
    CEccRightView::showMainForm();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// У��߼�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::checkAdvParam()
{
    bool bError = false;
    list<CEccParamItem*>::iterator lstItem;
    // ö��ÿ������
    for(lstItem = m_lsAdvParam.begin(); lstItem != m_lsAdvParam.end(); lstItem ++)
    {
        // У������
        if(!(*lstItem)->checkValue())
        {// ��������
            if(m_pAdvance)
                m_pAdvance->showSubTable();
            bError = true;
        }
    }
    return bError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// У���������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::checkBaseParam()
{
    bool bError = false;
    list<CEccParamItem*>::iterator lstItem;
    // ö��ÿ������
    for(lstItem = m_lsBasicParam.begin(); lstItem != m_lsBasicParam.end(); lstItem ++)
    {
        // У������
        if(!(*lstItem)->checkValue())
        {// ��������
            if(m_pGeneral)
                m_pGeneral->showSubTable();
            bError = true;
        }
    }
    return bError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// У�鱨������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::checkCondition()
{
    bool bError = false;
    // �򿪼����ģ��
    OBJECT objMonitor = GetMonitorTemplet(m_nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
    if(objMonitor != INVALID_VALUE)
    {// ��ģ��
        MAPNODE alertnode;
        // �Ƿ��ѷ�������
        // ����ֵ�ڵ�
        alertnode = GetMonitorErrorAlertCondition(objMonitor);
        // �Ƿ�ɹ����ɹ���У������
        if(alertnode != INVALID_VALUE && m_pErrorAlert)
            if(!m_pErrorAlert->checkCondition(alertnode))
                bError = true;

            // ���淧ֵ�ڵ�
            alertnode = GetMonitorWarningAlertCondition(objMonitor);
            // �Ƿ�ɹ����ɹ���У������
            if(alertnode != INVALID_VALUE && m_pWarnningAlert)
                if(!m_pWarnningAlert->checkCondition(alertnode))
                    bError = true;

            // ������ֵ�ڵ�
            alertnode = GetMonitorGoodAlertCondition(objMonitor);
            // �Ƿ�ɹ����ɹ���У������
            if(alertnode != INVALID_VALUE && m_pNormalAlert)
                if(!m_pNormalAlert->checkCondition(alertnode))
                    bError = true;

        // �رռ����ģ��
        CloseMonitorTemplet(objMonitor);
    }
    return bError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// У�����ʱ���Ƶ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::checkMonitorFreq(string &szValue)
{
    if(m_pFreqErr)
        m_pFreqErr->hide();

    if(m_AdvList.m_pMonitorFreq)
    {
        szValue = m_AdvList.m_pMonitorFreq->text();
        szValue = strtriml(szValue.c_str());
        szValue = strtrimr(szValue.c_str());

        if((!szValue.empty() && SV_IsNumeric(szValue)) || szValue.empty())
            return true;
    }

    if(m_pFreqErr)
        m_pFreqErr->show();

    if(m_pAdvance)
        m_pAdvance->showSubTable();

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// У����������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::checkName(string &szName)
{
    if(m_pNameErr)
        m_pNameErr->hide();

    if(m_pName)
    {
        szName = m_pName->text();
        szName = strtriml(szName.c_str());
        szName = strtrimr(szName.c_str());

        if(!szName.empty())
            return true;
    }

    if(m_pNameErr)
        m_pNameErr->show();

    if(m_pGeneral)
        m_pGeneral->showSubTable();

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��Ӳ�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::ContinueAdd()
{
    // ����
    m_bContinueAdd = true;
    // ���浱ǰ�޸ĵ��µļ������
    SaveMonitor();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����߼������еĻ�������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createAdvBaseParam(WTable *pTable)
{
    int nRow = pTable->numRows();

    // У�����
    new WText(SVResString::getResString("IDS_Check_Error"), pTable->elementAt(nRow, 0));
    // �Ƿ�У�����
    m_AdvList.m_pCheckErr = new WCheckBox("", pTable->elementAt(nRow, 1));

    pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
    pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
    pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
    pTable->GetRow(nRow)->setStyleClass("padding_top");

    // �Ƿ�У��������
    nRow ++;
    WText * pCheckErrHelp = new WText(SVResString::getResString("IDS_Check_Err_Help"), pTable->elementAt(nRow, 1));
    if(pCheckErrHelp)
    {
        pCheckErrHelp->hide();
        pTable->elementAt(nRow, 1)->setStyleClass("table_data_input_des");
        m_lsHelp.push_back(pCheckErrHelp);
    }

    // ����Ƶ��
    nRow ++;
    new WText(SVResString::getResString("IDS_Error_Freq"), pTable->elementAt(nRow, 0));
    // У�����Ƶ��
    m_AdvList.m_pMonitorFreq = new WLineEdit(pTable->elementAt(nRow, 1));
    if(m_AdvList.m_pMonitorFreq)
        m_AdvList.m_pMonitorFreq->setStyleClass("cell_10");

    pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
    pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
    pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
    pTable->GetRow(nRow)->setStyleClass("padding_top");

    new WText("&nbsp;", pTable->elementAt(nRow, 1));
    // У�����Ƶ��ʱ�䵥λ
    m_AdvList.m_pTimeUnit = new WComboBox(pTable->elementAt(nRow, 1));
    if(m_AdvList.m_pTimeUnit)
    {
        m_AdvList.m_pTimeUnit->addItem(SVResString::getResString("IDS_Minute"));
        m_AdvList.m_pTimeUnit->addItem(SVResString::getResString("IDS_Hour"));
    }

    // ������Ƶ�ʰ���
    nRow ++;
    WText *pFreqHelp = new WText(SVResString::getResString("IDS_Error_Freq_Help"), pTable->elementAt(nRow, 1));
    if(pFreqHelp)
    {
        pFreqHelp->hide();
        pTable->elementAt(nRow, 1)->setStyleClass("table_data_input_des");
        m_lsHelp.push_back(pFreqHelp);
    }

    nRow ++;
    m_pFreqErr = new WText(SVResString::getResString("IDS_Numbic_Error"), pTable->elementAt(nRow, 1));
    if(m_pFreqErr)
    {
        m_pFreqErr->hide();
        pTable->elementAt(nRow, 1)->setStyleClass("table_data_input_error");
    }

    // ����ƻ�
    nRow ++;
    new WText(SVResString::getResString("IDS_Plan"), pTable->elementAt(nRow, 0));
    // ����ƻ�
    m_AdvList.m_pPlan = new WComboBox(pTable->elementAt(nRow, 1));
    pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
    pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
    pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
    pTable->GetRow(nRow)->setStyleClass("padding_top");

    if(m_AdvList.m_pPlan)
    {
        m_AdvList.m_pPlan->setStyleClass("cell_40");
        AddTaskList(m_AdvList.m_pPlan);
    }
    // ����ƻ�����
    nRow ++;
    WText * pPlanHelp = new WText(SVResString::getResString("IDS_PlanHelp"), pTable->elementAt(nRow, 1));
    if(pPlanHelp)
    {
        pPlanHelp->hide();
        pTable->elementAt(nRow, 1)->setStyleClass("table_data_input_des");
        m_lsHelp.push_back(pPlanHelp);
    }

    // ����
    nRow ++;
    new WText(SVResString::getResString("IDS_Monitor_Desc"), pTable->elementAt(nRow, 0));
    // ������Ϣ
    m_AdvList.m_pDescription = new WTextArea("", pTable->elementAt(nRow, 1));
    if(m_AdvList.m_pDescription )
        m_AdvList.m_pDescription->setStyleClass("cell_98");

    pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
    pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
    pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
    pTable->GetRow(nRow)->setStyleClass("padding_top");

    // ������Ϣ����
    nRow ++;
    WText *pDescHelp = new WText(SVResString::getResString("IDS_Monitor_Desc_Help"), pTable->elementAt(nRow, 1));
    if(pDescHelp)
    {
        pDescHelp->hide();
        pTable->elementAt(nRow, 1)->setStyleClass("table_data_input_des");
        m_lsHelp.push_back(pDescHelp);
    }

    // ���������ı�
    nRow ++;
    new WText(SVResString::getResString("IDS_Report_Desc"), pTable->elementAt(nRow, 0));
    // ��������
    m_AdvList.m_pReportDesc = new WTextArea("", pTable->elementAt(nRow, 1));
    if(m_AdvList.m_pReportDesc)
        m_AdvList.m_pReportDesc->setStyleClass("cell_98");

    pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
    pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
    pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
    pTable->GetRow(nRow)->setStyleClass("padding_top");

    // ������������
    nRow ++;
    WText * pReportHelp = new WText(SVResString::getResString("IDS_Report_Desc_Help"), pTable->elementAt(nRow, 1));
    if(pReportHelp)
    {
        pReportHelp->hide();
        pTable->elementAt(nRow, 1)->setStyleClass("table_data_input_des");
        m_lsHelp.push_back(pReportHelp);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����߼�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createAdvParam()
{
    if(m_pAdvance)
    {
        WTable *pSub = m_pAdvance->getListTable();
        if(pSub)
        {// �����ӱ�ɹ�
            list<CEccParamItem*>::iterator it;
            // ö��ÿһ������
            for(it = m_lsAdvParam.begin(); it != m_lsAdvParam.end(); it ++)
            {
                // �Ƿ��и���
                if((*it)->isHasFollow())
                {// �и���ֵ
                    // �õ�����ֵ
                    string szFollow = (*it)->getFollow();
                    if(!szFollow.empty())
                    {
                        list<CEccParamItem*>::iterator ittmp;
                        // ö��ÿ������
                        for(ittmp = m_lsAdvParam.begin(); ittmp != m_lsAdvParam.end(); ittmp ++)
                        {
                            // �Ƿ��Ǹ���ֵ
                            if(strcmp(szFollow.c_str(), (*ittmp)->getName()) == 0)
                            {
                                // ���ø���ֵ
                                (*it)->setFollowItem(*ittmp);
                                break;
                            }
                        }
                    }
                }
                // �����ؼ�
                (*it)->CreateControl(pSub, m_lsHelp, m_bShowHelp);
                // �Ƿ�ʹ�ö�̬����
                if((*it)->isDynamic())
                {
                    m_lsDyn.push_back((*it));
                }
            }
            
            // �����߼������еĻ�������
            createAdvBaseParam(pSub);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createBaseParam()
{
    if(m_pGeneral)
    {
        WTable *pSub = m_pGeneral->getListTable();
        if(pSub)
        {
            list<CEccParamItem*>::iterator it;
            // ö��ÿ�����Բ���
            for(it = m_lsBasicParam.begin(); it != m_lsBasicParam.end(); it ++)
            {
                // �Ƿ��и���ֵ
                if((*it)->isHasFollow())
                {// �и���ֵ
                    // �õ�����ֵ
                    string szFollow = (*it)->getFollow();
                    if(!szFollow.empty())
                    {// ����ֵ��Ϊ��
                        list<CEccParamItem*>::iterator ittmp ;
                        // ö��ÿһ�����Բ���
                        for(ittmp = m_lsBasicParam.begin(); ittmp != m_lsBasicParam.end(); ittmp ++)
                        {
                            // �Ƿ�ƥ��
                            if(strcmp(szFollow.c_str(), (*ittmp)->getName()) == 0)
                            {
                                // ���ø���ֵ
                                (*it)->setFollowItem((*ittmp));
                                break;
                            }
                        }
                    }
                }
                // �����ؼ�
                (*it)->CreateControl(pSub, m_lsHelp, m_bShowHelp);
                // �Ƿ�ʹ�ö�̬����
                if((*it)->isDynamic())
                {
                    m_lsDyn.push_back((*it));
                    m_bHasDynamic = true;
                }
            }

            // �������������
            createMonitorName();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����豸����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createDevName()
{
    if(m_pGeneral)
    {
        WTable *pSubTable = m_pGeneral->getListTable();
        if(pSubTable)
        {
            new WText(SVResString::getResString("IDS_Device_Name"), pSubTable->elementAt(0, 0));
            // �豸����
            m_pDevName = new WLineEdit("127.0.0.1", pSubTable->elementAt(0, 1));
            if(m_pDevName)  //wangpeng 
				strcpy(m_pDevName->contextmenu_, "class='readonly_text' readOnly=true");

                //strcpy(m_pDevName->contextmenu_, "class='cell_40' readOnly=true");

            pSubTable->elementAt(0, 0)->setStyleClass("table_list_data_input_text");
            pSubTable->elementAt(0, 1)->setStyleClass("table_data_text");
            pSubTable->elementAt(0, 1)->setContentAlignment(AlignLeft);
            pSubTable->GetRow(0)->setStyleClass("padding_top");

            // �����豸���ư����ı�
            WText * pDeviceHelp = new WText(SVResString::getResString("IDS_Device_Name_Help"), 
                pSubTable->elementAt(1, 1));
            if(pDeviceHelp)
            {
                pDeviceHelp->hide();
                pSubTable->elementAt(1, 1)->setStyleClass("table_data_input_des");
                m_lsHelp.push_back(pDeviceHelp);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createCondition()
{
    if(m_pContent)
    {
        int nRow = m_pContent->numRows();
        CEccListTable *pCondition = new CEccListTable(m_pContent->elementAt(nRow, 0), false, false, false);
        if(pCondition)
        {
            pCondition->setTitle(SVResString::getResString("IDS_Alert_Condition_Title"));
            WTable *pSubTable = pCondition->getListTable();
            if(pSubTable)
            {
                // ����
                nRow = pSubTable->numRows();
                m_pErrorAlert = new SVConditionParam(pSubTable->elementAt(nRow, 0));

                // ����
                nRow ++;
                m_pWarnningAlert = new SVConditionParam(pSubTable->elementAt(nRow, 0));

                // ����
                nRow ++;
                m_pNormalAlert = new SVConditionParam(pSubTable->elementAt(nRow, 0));
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������ذ�ť��ö�ٶ�̬����ʱ���ã�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createHidebutton()
{
    int nRow = numRows();
    // ö�ٶ�̬����
    m_pHideButton = new WPushButton("list dyn", elementAt(nRow, 0));
    if(m_pHideButton)
    {
        m_pHideButton->hide();
        WObject::connect(m_pHideButton, SIGNAL(clicked()),"showbar();",  this, SLOT(EnumDynData()), 
            WObject::ConnectionType::JAVASCRIPTDYNAMIC);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createMonitorName()
{
    if(m_pGeneral)
    {
        WTable *pTable = m_pGeneral->getListTable();
        if(pTable)
        {
            // �õ���ǰ����
            int nRow = pTable->numRows();
            // ˵��
            new WText(SVResString::getResString("IDS_Monitor_Label"), pTable->elementAt(nRow, 0));
            // ���ӣ���ɫ*����ʶΪ������
            new WText("<span class =required>*</span>", pTable->elementAt(nRow, 0));

            // �������������
            m_pName = new WLineEdit("", pTable->elementAt(nRow, 1));
            if(m_pName)
                m_pName->setStyleClass("cell_98");

            // ���������ı�
            WText *pNameHelp = new WText(SVResString::getResString("IDS_Monitor_Label_Help"), pTable->elementAt(nRow + 1, 1));
            if(pNameHelp)
            {
                pNameHelp->hide();
                pTable->elementAt(nRow + 1, 1)->setStyleClass("table_data_input_des");
                m_lsHelp.push_back(pNameHelp);
            }

            // ��������ƴ�����ʾ��Ϣ
            m_pNameErr = new WText(SVResString::getResString("IDS_Monitor_Label_Error"), pTable->elementAt(nRow + 2, 1));            
            if (m_pNameErr)
            {
                pTable->elementAt(nRow + 2, 1)->setStyleClass("table_data_input_error");
                m_pNameErr->hide();
            }

            // ������Ԫ���뷽ʽΪ���϶���
            pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
            pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
            pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����������ť
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createOperate()
{
    if(m_pOperate)
    {
        // ��ǰ��
        int nRow = m_pOperate->numRows();

        // ���ؼ�����б�ť
        m_pBack = new CEccButton(SVResString::getResString("IDS_Back_One_Step"), 
                                SVResString::getResString("IDS_Back_Monitor_List_Tip"),
                                "", 
                                m_pOperate->elementAt(nRow, 0));
        if(m_pBack)
        {
            WObject::connect(m_pBack, SIGNAL(clicked()), "showbar();", this, SLOT(Forward()), 
                WObject::ConnectionType::JAVASCRIPTDYNAMIC);
        }
        // ����һ����ť����һ������
        new WText("&nbsp;", m_pOperate->elementAt(nRow, 1));
        // ���水ť
        m_pSave = new CEccImportButton(SVResString::getResString("IDS_Add"), 
                                    SVResString::getResString("IDS_Add_Monitor_Tip"),
                                    "",
                                    m_pOperate->elementAt(nRow, 3));
        if(m_pSave)
        {
            WObject::connect(m_pSave, SIGNAL(clicked()), "showbar();", this, SLOT(SaveMonitor()), 
                WObject::ConnectionType::JAVASCRIPTDYNAMIC);
        }

        new WText("&nbsp;", m_pOperate->elementAt(nRow, 4));
        // ȡ����ť
        m_pCancel = new CEccButton(SVResString::getResString("IDS_Cancel"), 
                                        SVResString::getResString("IDS_Cancel_Current_Edit_Tip"),
                                        "",
                                        m_pOperate->elementAt(nRow, 5));
        if(m_pCancel)
        {
            WObject::connect(m_pCancel, SIGNAL(clicked()), "showbar();", this, SLOT(Cancel()), 
                WObject::ConnectionType::JAVASCRIPTDYNAMIC);
        }

        m_pBatchSep = new WText("&nbsp;", m_pOperate->elementAt(nRow, 6));
        // �������
        m_pBatch = new CEccButton(SVResString::getResString("IDS_Batch_Add"), 
                                    SVResString::getResString("IDS_Batch_Add_Tip"),
                                    "",
                                    m_pOperate->elementAt(nRow, 7));
        if(m_pBatch)
        {
            WObject::connect(m_pBatch, SIGNAL(clicked()), "showbar();", this, SLOT(BatchAdd()), 
                WObject::ConnectionType::JAVASCRIPTDYNAMIC);
        }

        m_pContinueSep = new WText("&nbsp;", m_pOperate->elementAt(nRow, 8));
        // ��Ӳ�����
        m_pContinue = new CEccButton(SVResString::getResString("IDS_Continue_Add"), 
                                        SVResString::getResString("IDS_Continue_Add_Tip"),
                                        "",
                                        m_pOperate->elementAt(nRow, 9));
        if(m_pContinue)
        {
            WObject::connect(m_pContinue, SIGNAL(clicked()), "showbar();", this, SLOT(ContinueAdd()), 
                WObject::ConnectionType::JAVASCRIPTDYNAMIC);
        }

        new WText("&nbsp;", m_pOperate->elementAt(nRow, 10));
        // ����Ĭ�ϱ�������
        CEccButton *pSetAlert = new CEccButton(SVResString::getResString("IDS_Set_Default_Alert"), 
                                        SVResString::getResString("IDS_Set_Default_Alert_Tip"),
                                        "",
                                        m_pOperate->elementAt(nRow, 11));
        if(pSetAlert)
        {
            WObject::connect(pSetAlert, SIGNAL(clicked()), "showbar();", this, SLOT(SetAlertCondition()), 
                WObject::ConnectionType::JAVASCRIPTDYNAMIC);
        }
		
		//cxy add 07/10/17...
		new WText("&nbsp;", m_pOperate->elementAt(nRow, 12));
		m_pUrlStepSetup = new CEccButton("��������", 
                                        "��������",
                                        "",
                                        m_pOperate->elementAt(nRow, 13));
        if(m_pUrlStepSetup)
        {            
			WObject::connect(m_pUrlStepSetup, SIGNAL(clicked()), "showbar();", this, SLOT(UrlStepSetup()), 
                WObject::ConnectionType::JAVASCRIPTDYNAMIC);
			m_pUrlStepSetup->hide();
        }

        nRow = static_cast<WTableCell*>(m_pOperate->parent())->row();
        elementAt(nRow, 0)->setContentAlignment(AlignBottom | AlignCenter);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �༭�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::EditMonitorByID(const string &szMonitorID)
{
    // �޸��豸����
    string szDeviceID = FindParentID(szMonitorID);
    if(m_pDevName)
    {
        const CEccTreeNode *pNode = CEccTreeView::getECCObject(szDeviceID);
        if(pNode)
            m_pDevName->setText(pNode->getName());
    }

    // �򿪼��
    OBJECT objMonitor = GetMonitor(szMonitorID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
    if(objMonitor != INVALID_VALUE)
    {
        // �õ����ڵ�
        MAPNODE mainnode = GetMonitorMainAttribNode(objMonitor);
        if(mainnode != INVALID_VALUE)
        {
            // �õ��������
            string szName (""); FindNodeValue(mainnode, svName, szName);

            // �õ������ģ��ID
            string szMTID (""); FindNodeValue(mainnode, svMonitorType, szMTID);
            if(!szMTID.empty())
            {
                int nMTID = 0;
                nMTID = atoi(szMTID.c_str());
                
                // �жϵ�ǰ�����Ƿ�ʹ�չʾ��ͬ
                if(m_nMTID == nMTID)
                {
                    // ��������
                    resetData();
                    // �жϴ��༭������ĸ��豸ID �Ƿ���� ��ǰ�ѻ�����豸ID
                    // �ж϶�̬�����Ƿ�Ϊ��
                    // ���ذ�ť�Ƿ���ȷ����
                    if(szDeviceID != m_szDeviceID && !m_lsDyn.empty() && m_pHideButton)
                    {
                        // ö�ٶ�̬����
                        m_szDeviceID = szDeviceID;
                        list<CEccParamItem*>::iterator lstItem;
                        for(lstItem = m_lsDyn.begin(); lstItem != m_lsDyn.end(); lstItem ++)
                            (*lstItem)->clearDynData();

                        string szCmd("hiddenbar();");
                        if(!m_pHideButton->getEncodeCmd("xclicked()").empty())
                            szCmd = "update('" + m_pHideButton->getEncodeCmd("xclicked()") + "');";
                        WebSession::js_af_up = szCmd;
                    }
                    else
                    {
                        WebSession::js_af_up = "hiddenbar();";
                    }
                }
                else
                {// ������
                    // ����
                    m_nMTID = nMTID;
                    m_szDeviceID = szDeviceID;
                    
                    // ��������
                    clearData();

                    // ���豸ģ��
                    OBJECT objMT = GetMonitorTemplet(nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
                    if(objMT != INVALID_VALUE)
                    {
                        // ö�ٻ�������
                        enumBaseParam(objMT);
                        // ö�ٸ߼�����
                        enumAdvParam(objMT);
                        // ö�ٷ���ֵ
                        enumReturnItem(objMT);
                        // ö�ٱ�������
                        enumAlert(objMT);
                        
                        // �õ����ڵ�
                        MAPNODE objNode = GetMTMainAttribNode(objMT);
                        if(objNode != INVALID_VALUE)
                        {
                            if(FindNodeValue(objNode, svLabel, m_szMonitorName))
                                m_szMonitorName = SVResString::getResString(m_szMonitorName.c_str());

                            // �������ռ����
                            FindNodeValue(objNode, svIntPos, m_szPoint);
                            if(m_szPoint.empty())
                                m_szPoint = "1";
                        }
                        
                        // �ر�ģ��
                        CloseMonitorTemplet(objMT);
                    }

                    // ������������
                    createBaseParam();
                    // �����߼�����
                    createAdvParam();

                    // �ж϶�̬�����Ƿ�Ϊ��
                    // ���ذ�ť�Ƿ���ȷ����
                    if(!m_lsDyn.empty() && m_pHideButton)
                    {// ��������

                        // ö�ٶ�̬����
                        string szCmd("hiddenbar();");
                        if(!m_pHideButton->getEncodeCmd("xclicked()").empty())
                            szCmd = "update('" + m_pHideButton->getEncodeCmd("xclicked()") + "');";
                        WebSession::js_af_up = szCmd;
                    }
                    else
                    {
                        WebSession::js_af_up = "hiddenbar();";
                    }
                }
                
                // ���ô��޸ĵļ��������
                m_szEditIndex = szMonitorID; 
                // ���ü��������
                if(m_pName)
                    m_pName->setText(szName);
            }

            // ���ñ�����
            if(m_pTitle)
                m_pTitle->setText(SVResString::getResString("IDS_Edit") + szName + SVResString::getResString("IDS_Monitor_Title"));
        }
        
        SetBaseParam(objMonitor);                                      // ���û�������
        SetAdvParam(objMonitor);                                       // ���ø߼�����
        SetConditionParam(objMonitor);                                 // ���ñ�������
        
        // �رռ����
        CloseMonitor(objMonitor);
    }

    // ���� ��һ�� ��ť
    if(m_pBack)
        m_pBack->hide();

    // ���� ��Ӳ����� ��ť
    if(m_pContinue) 
        m_pContinue->hide();

    // ���� ������� �ָ���
    if(m_pBatchSep)
        m_pBatchSep->hide();

    // ���� ��Ӳ����� �ָ���
    if(m_pContinueSep)
        m_pContinueSep->hide();

    // �����������
    if(m_pBatch)
        m_pBatch->hide();
	
	//cxy add 07/10/17...
	if(m_nMTID == 9 || m_nMTID == 514)
		m_pUrlStepSetup->show();
	else
		m_pUrlStepSetup->hide();

    // ���� ���� ��ť����ʾ�ı�����ʾ��Ϣ
    if(m_pSave)
    {
        m_pSave->setText(SVResString::getResString("IDS_Save"));
        m_pSave->setToolTip(SVResString::getResString("IDS_Save_Tip"));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ö�� ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::enumAlert(OBJECT &objMT)
{
    // ��������
    if(m_pErrorAlert)
    {
        MAPNODE	objError = GetMTErrorAlertCondition(objMT);
        // ��ǰ����
        if(objError != INVALID_VALUE)
        {// 
            m_pErrorAlert->SetReturnList(m_lsReturn);
            m_pErrorAlert->SetMapNode(objError);
        }
    }

    // ��������
    if(m_pWarnningAlert)
    {
        MAPNODE	objError = GetMTWarningAlertCondition(objMT);
        // ��ǰ����
        if(objError != INVALID_VALUE)
        {// 
            m_pWarnningAlert->SetReturnList(m_lsReturn);
            m_pWarnningAlert->SetMapNode(objError);
        }
    }

    // ��������
    if(m_pNormalAlert)
    {
        MAPNODE	objError = GetMTGoodAlertCondition(objMT);
        // ��ǰ����
        if(objError != INVALID_VALUE)
        {// 
            m_pNormalAlert->SetReturnList(m_lsReturn);
            m_pNormalAlert->SetMapNode(objError);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ö�ٸ߼�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::enumAdvParam(OBJECT &objMT)
{
    LISTITEM lsItem;
    // �õ��߼����Բ�����һ���ڵ�
    if( FindMTAdvanceParameterFirst(objMT, lsItem))
    {
        // ö��ÿ���ڵ�
        MAPNODE objNode;
        while( (objNode = FindNext(lsItem)) != INVALID_VALUE )
        {
            CEccParamItem *param = new CEccParamItem(objNode);
            m_lsAdvParam.push_back(param);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ö�ٻ�������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::enumBaseParam(OBJECT &objMT)
{
    LISTITEM lsItem;
    // �õ���һ����׼���Բ���
    if( FindMTParameterFirst(objMT, lsItem))
    {
        MAPNODE objNode;
        // ö��ÿ������
        while( (objNode = FindNext(lsItem)) != INVALID_VALUE )
        {
            CEccParamItem *param = new CEccParamItem(objNode);
            m_lsBasicParam.push_back(param);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ö�ٷ���ֵ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::enumReturnItem(OBJECT &objMT)
{
    LISTITEM lsItem;
    // ö��ÿһ������ֵ
    if( FindMTReturnFirst(objMT, lsItem))
    {
        MAPNODE objNode;
        while( (objNode = FindNext(lsItem)) != INVALID_VALUE )
        {
            SVReturnItem *reItem = new SVReturnItem(objNode);
            m_lsReturn.push_back(reItem);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ö�ٶ�̬����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::EnumDynData()
{
    string szRun("");
    // ö��ÿ����̬���ԵĶ�̬����
    list<CEccParamItem*>::iterator lstItem;
    for(lstItem = m_lsDyn.begin(); lstItem != m_lsDyn.end(); lstItem ++)
    {
        if(m_szEditIndex.empty())
        {
            szRun = MakeRunParamStringByID(m_szDeviceID, m_nMTID);
            (*lstItem)->enumDynValue(szRun, m_szDeviceID);
        }
        else
        {
            szRun = MakeRunParamStringByID(FindParentID(m_szEditIndex), m_nMTID);
            (*lstItem)->enumDynValue(szRun, m_szEditIndex);
        }
    }
    WebSession::js_af_up = "hiddenbar();";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��һ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::Forward()
{
    CEccRightView::showAddMonitor1st(m_szDeviceID, "", m_szNetworkSet);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʼ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::initForm(bool bHasHelp)
{
    // �������ʼ������
    CEccBaseTable::initForm(bHasHelp);

    // 
    if(m_pContent)
    {
        // ������ʽ��
        setContentCellStyle("height95p");

        int nRow = m_pContent->numRows();
        // ������Ϣ��
        m_pGeneral = new CEccListTable(m_pContent->elementAt(nRow, 0), false, false, false);
        if(m_pGeneral)
            m_pGeneral->setTitle(SVResString::getResString("IDS_General_Infor_Title"));

        // ��������
        createCondition();
        
        // �߼�������
        nRow ++;
        m_pAdvance = new CEccListTable(m_pContent->elementAt(nRow, 0), false, false, false);
        if(m_pAdvance)
            m_pAdvance->setTitle(SVResString::getResString("IDS_Advance_Option"));
    }

    // �����豸��
    createDevName();
    // ����������ť
    createOperate();
    // �������ذ�ť
    createHidebutton();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::clearData()
{
    // �����������
    list<CEccParamItem*>::iterator lsItem;
    for(lsItem = m_lsBasicParam.begin(); lsItem != m_lsBasicParam.end(); lsItem ++)
        delete (*lsItem);

    // ����߼�����
    for(lsItem = m_lsAdvParam.begin(); lsItem != m_lsAdvParam.end(); lsItem ++)
        delete (*lsItem);

    // ������ֵ
    list<SVReturnItem*>::iterator lsReItem;
    for(lsReItem = m_lsReturn.begin(); lsReItem != m_lsReturn.end(); lsReItem ++)
        delete (*lsReItem);

    // �б����
    m_lsBasicParam.clear();
    m_lsAdvParam.clear();

    m_lsReturn.clear();
    m_lsDyn.clear();

    // ɾ�� ���������
    if(m_pGeneral && m_pGeneral->getListTable())
    {
        while(m_pGeneral->getListTable()->numRows() > 2)
            m_pGeneral->getListTable()->deleteRow(m_pGeneral->getListTable()->numRows() - 1);
    }

    // �߼������� ���
    if(m_pAdvance && m_pAdvance->getListTable())
        m_pAdvance->getListTable()->clear();

    // 
    m_AdvList.Reset();

    // ������Ϣ���
    m_lsHelp.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::resetData()
{
    // ��������
    list<CEccParamItem*>::iterator lstItem;
    for(lstItem = m_lsBasicParam.begin(); lstItem != m_lsBasicParam.end(); lstItem ++)
    {
        if(!(*lstItem)->isDynamic())
            (*lstItem)->setDefaultValue();
        else
            (*lstItem)->resetDynData();
    }

    // �߼�����
    for(lstItem = m_lsAdvParam.begin(); lstItem != m_lsAdvParam.end(); lstItem ++)
    {
        if(!(*lstItem)->isDynamic())
            (*lstItem)->setDefaultValue();
        else
            (*lstItem)->resetDynData();
    }

    // �򿪼����ģ��
    OBJECT objMT = GetMonitorTemplet(m_nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
    if(objMT != INVALID_VALUE)
    {
        // ö�� ��������
        enumAlert(objMT);
        CloseMonitorTemplet(objMT);
    }

    // ���� ����� ����
    if(m_pName)
        m_pName->setText(m_szMonitorName);

    m_AdvList.ResetData();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����߼�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::saveAdvParam(OBJECT &objMonitor, const char *pcszValue)
{
    bool bNoError = true;
    // �߼������ڵ�
    MAPNODE advnode = GetMonitorAdvanceParameterNode(objMonitor);
    if(advnode != INVALID_VALUE)
    {
        list<CEccParamItem*>::iterator lstItem;
        // ���� ֵ ��������
        string szName(""), szValue(""), szSavename("");
        // �������� ���㹫ʽ ����ʹ�õ�����
        string szAccount (""), szExpress (""), szAccValue ("");
        char szAfterAcc[32] = {0};
        // ö��ÿ�����Բ���
        for(lstItem = m_lsAdvParam.begin(); lstItem != m_lsAdvParam.end(); lstItem ++)
        {
            szName = "";
            szValue = "";
            szSavename = "";
            szAccount = "";
            szExpress = "";
            szAccValue = "";

            (*lstItem)->getName(szName);
            if(m_szDynName == szName)
            {
                if(pcszValue)
                    szValue = pcszValue;
            }
            else
            {
                (*lstItem)->getStringValue(szValue);
            }

            (*lstItem)->getStringValue(szValue);
            szSavename = (*lstItem)->getSaveName();
            szAccount = (*lstItem)->getAccount();
            if(!szAccount.empty())
            {// ��Ҫ����
                szExpress = (*lstItem)->getExpress();
                list<CEccParamItem*>::iterator itTmp;
                // ö��ÿ������
                for(itTmp = m_lsAdvParam.begin(); itTmp != m_lsAdvParam.end(); itTmp ++)
                {
                    if(szAccount.compare((*itTmp)->getName()) == 0)
                    {
                       (*itTmp)->getStringValue(szAccValue);
                        break;
                    }
                }
                // ����
                int nAccValue = 0;
                switch(szExpress.c_str()[0])
                {
                case '+':
                    nAccValue = atoi(szValue.c_str()) + atoi(szAccValue.c_str());
                    break;
                case '-':
                    nAccValue = atoi(szValue.c_str()) - atoi(szAccValue.c_str());
                    break;
                case '*':
                    nAccValue = atoi(szValue.c_str()) * atoi(szAccValue.c_str());
                    break;
                case '/':
                    if(atoi(szAccValue.c_str()) != 0)
                        nAccValue = atoi(szValue.c_str()) * atoi(szAccValue.c_str());
                    else
                        nAccValue = atoi(szValue.c_str()) ;
                    break;
                }
                // ������
                sprintf(szAfterAcc, "%d", nAccValue);
                AddNodeAttrib(advnode, szName + "1", szValue);
            }
            if(strlen(szAfterAcc) > 0)
                szValue = szAfterAcc;

            if(!szSavename.empty())
            {
                szName = szSavename;
                if(!szValue.empty() && !AddNodeAttrib(advnode, szName, szValue))
                {
                    bNoError = false;
                    break;
                }
            }
            else if(!AddNodeAttrib(advnode, szName, szValue))
            {
                bNoError = false;
                break;
            }
        }
    }

    return bNoError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����߼������еĻ�������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::saveAdvBaseParam(MAPNODE &objNode)
{
    string szDesc (""), szReportDesc ("");
    string szPlan (""), szCheckErr ("");
    string szErrFreq (""), szErrFreqUnit ("");

    // ����
    if(m_AdvList.m_pDescription)
        szDesc = m_AdvList.m_pDescription->text();

    // У�����
    if(m_AdvList.m_pCheckErr)
    {
        if(m_AdvList.m_pCheckErr->isChecked())
            szCheckErr = "true";
        else
            szCheckErr = "false";
    }

    // ����ʱ���Ƶ��
    if(m_AdvList.m_pMonitorFreq)
        szErrFreq = m_AdvList.m_pMonitorFreq->text();

    // ����ʱ���Ƶ�ʵ�λ
    if(m_AdvList.m_pTimeUnit)
        szErrFreqUnit = m_AdvList.m_pTimeUnit->currentText();
    if(szErrFreqUnit == SVResString::getResString("IDS_Hour"))
        szErrFreqUnit = "60";
    else
        szErrFreqUnit = "1";
    
    int nFreq = atoi(szErrFreq.c_str()) * atoi(szErrFreqUnit.c_str());
    char szFreq[32] = {0};
    sprintf(szFreq , "%d", nFreq);

    // ����ƻ�
    if(m_AdvList.m_pPlan)
        szPlan = m_AdvList.m_pPlan->currentText();

    // ��������
    if(m_AdvList.m_pReportDesc)
        szReportDesc = m_AdvList.m_pReportDesc->text();

    bool bNoError = false;

    // ���
    if((bNoError = AddNodeAttrib(objNode, svDescription, szDesc)))
    {
        if((bNoError = AddNodeAttrib(objNode, svReportDesc, szReportDesc)))
        {
            if((bNoError = AddNodeAttrib(objNode, svPlan, szPlan)))
            {
                if((bNoError = AddNodeAttrib(objNode, svCheckErr, szCheckErr)))
                {
                    if((bNoError = AddNodeAttrib(objNode, svErrFreq, szFreq)))
                    {
                        if((bNoError = AddNodeAttrib(objNode, svErrFreqUnit, szErrFreqUnit)))
                            bNoError = AddNodeAttrib(objNode, svErrFreqSave, szErrFreq);
                    }
                }
            }
        }
    }
    return bNoError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::saveBaseParam(OBJECT &objMonitor, const char *pcszValue)
{
    bool bNoError = true;
    list<CEccParamItem*>::iterator lstItem;

    MAPNODE basenode = GetMonitorParameter(objMonitor);
    if(basenode != INVALID_VALUE)
    {
        string szName (""), szValue (""), szSavename ("");
        string szAccount (""), szExpress (""), szAccValue ("");
        char szAfterAcc[32] = {0};
        for(lstItem = m_lsBasicParam.begin(); lstItem != m_lsBasicParam.end(); lstItem ++)
        {
            szName = "";
            szValue = "";
            szSavename = "";
            szAccount = "";
            szExpress = "";
            szAccValue = "";

            (*lstItem)->getName(szName);
            if(m_szDynName == szName)
            {
                if(pcszValue)
                    szValue = pcszValue;
            }
            else
            {
                (*lstItem)->getStringValue(szValue);
            }

            szSavename = (*lstItem)->getSaveName();
            szAccount = (*lstItem)->getAccount();
            if(!szAccount.empty())
            {
                szExpress = (*lstItem)->getExpress();
                list<CEccParamItem*>::iterator itTmp;
                for(itTmp = m_lsBasicParam.begin(); itTmp != m_lsBasicParam.end(); itTmp ++)
                {
                    if(szAccount.compare((*itTmp)->getName()) == 0)
                    {
                       (*itTmp)->getStringValue(szAccValue);
                        break;
                    }
                }
                int nAccValue = 0;
                switch(szExpress.c_str()[0])
                {
                case '+':
                    nAccValue = atoi(szValue.c_str()) + atoi(szAccValue.c_str());
                    break;
                case '-':
                    nAccValue = atoi(szValue.c_str()) - atoi(szAccValue.c_str());
                    break;
                case '*':
                    nAccValue = atoi(szValue.c_str()) * atoi(szAccValue.c_str());
                    break;
                case '/':
                    if(atoi(szAccValue.c_str()) != 0)
                        nAccValue = atoi(szValue.c_str()) * atoi(szAccValue.c_str());
                    else
                        nAccValue = atoi(szValue.c_str()) ;
                    break;
                }
                sprintf(szAfterAcc, "%d", nAccValue);
                AddNodeAttrib(basenode, szName + "1", szValue);
            }
            if(strlen(szAfterAcc) > 0)
                szValue = szAfterAcc;

            if(!szSavename.empty())
            {
                szName = szSavename;
                if(!szValue.empty() && !AddNodeAttrib(basenode, szName, szValue))
                {
                    bNoError = false;
                    break;
                }
            }
            else if(!AddNodeAttrib(basenode, szName, szValue))
            {
                bNoError = false;
                break;
            }
        }
        
        if(bNoError)
            saveAdvBaseParam(basenode);
    }
    return bNoError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���汨������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::saveCondition(OBJECT &objMonitor)
{
    bool bNoError = true;
    
    // �������ֵ
    MAPNODE alertnode = GetMonitorErrorAlertCondition(objMonitor);
    if(alertnode != INVALID_VALUE && m_pErrorAlert)
        bNoError = m_pErrorAlert->SaveCondition(alertnode);
    else
        bNoError = false;

    // �������ֵ�ɹ�
    if(bNoError)
    {
        // ���澯�淧ֵ
        alertnode = GetMonitorWarningAlertCondition(objMonitor);
        if(alertnode != INVALID_VALUE && m_pWarnningAlert)
            bNoError = m_pWarnningAlert->SaveCondition(alertnode);
        else
            bNoError = false;
    }

    if(bNoError)
    {
        // ����������ֵ
        alertnode = GetMonitorGoodAlertCondition(objMonitor);
        if(alertnode != INVALID_VALUE && m_pNormalAlert)
            bNoError = m_pNormalAlert->SaveCondition(alertnode);
        else
            bNoError = false;
    }

    return bNoError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::SaveMonitor()
{
    unsigned long ulStart = GetTickCount();

    string szName(""), szRealIndex(""), szErrFreq("");
    bool bError = false;

    if(!checkName(szName))
        bError = true;

    if(checkBaseParam())
        bError = true;

    if(checkAdvParam())
        bError = true;

    if(!checkMonitorFreq(szErrFreq))
        bError = true;

    if(checkCondition())
        bError = true;    

    if(bError)
    {
        WebSession::js_af_up = "hiddenbar();";
        return;
    }


    string szOpt("Add New Monitor");

    CEccTreeDevice *pDevice = NULL;
    const CEccTreeNode *pNode = CEccMainView::m_pTreeView->getECCObject(m_szDeviceID);
    if(pNode)
        pDevice = static_cast<CEccTreeDevice*>(const_cast<CEccTreeNode*>(pNode));

    OBJECT objMonitor; 

    if(m_szEditIndex.empty()) 
    { 
        if(m_szNetworkSet != "true")
        {
            int nMonitorCount = getUsingMonitorCount();

            int nPoint = atoi(m_szPoint.c_str());
            if(nPoint <= 0)
                nPoint = 0;
            nMonitorCount += nPoint;

            if(!checkMonitorsPoint(nMonitorCount))
            {
                WebSession::js_af_up = "showMonitorCountErr('" + SVResString::getResString("IDS_PointPoor") + "','" +
                    SVResString::getResString("IDS_Affirm") + "');hiddenbar();";
                return;
            }
        }
		else
		{

			//////////////////////begin to modify at 07/07/31 /////////////////////////////
			//#ifndef IDC_Version
			if(GetCgiVersion().compare("IDC") != 0)
			{
			//////////////////////modify end at 07/07/31 //////////////////////////////////

				int nNetworkCount = getUsingNetworkCount(m_szDeviceID); 

				if(!checkNetworkPoint(nNetworkCount))
				{
					WebSession::js_af_up = "showMonitorCountErr('" + SVResString::getResString("IDS_PointPoor") + "','" +
						SVResString::getResString("IDS_Affirm") + "');hiddenbar();";
					return;
				}

			//////////////////////begin to modify at 07/07/31 /////////////////////////////
			//#else
			}
			else
			{
			//////////////////////modify end at 07/07/31 //////////////////////////////////

				int nMonitorCount = getUsingMonitorCount();

				int nPoint = atoi(m_szPoint.c_str());
				if(nPoint <= 0)
					nPoint = 0;
				nMonitorCount += nPoint;

				if(!checkMonitorsPoint(nMonitorCount))
				{
					WebSession::js_af_up = "showMonitorCountErr('" + SVResString::getResString("IDS_PointPoor") + "','" +
						SVResString::getResString("IDS_Affirm") + "');hiddenbar();";
					return;
				}

			//////////////////////begin to modify at 07/07/31 /////////////////////////////
			//#endif
			}
			//////////////////////modify end at 07/07/31 //////////////////////////////////

		}
        objMonitor = CreateMonitor();
    }
    else
    {
        szOpt = "Edit Monitor: " + m_szEditIndex;
        objMonitor = GetMonitor(m_szEditIndex, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
    }

    if(objMonitor != INVALID_VALUE)
    {
        MAPNODE mainnode;
        mainnode  = GetMonitorMainAttribNode(objMonitor);
        if(mainnode != INVALID_VALUE)
        {
            char chMTID [8] = {0};
            sprintf(chMTID, "%d", m_nMTID);
            //if(m_szNetworkSet == "true")
            //    m_szPoint = "0";

			//////////////////////begin to modify at 07/07/31 /////////////////////////////
			//#ifdef IDC_Version
			//	if(m_szNetworkSet == "true")
			//		m_szPoint = "1";
			//#else
			//	if(m_szNetworkSet == "true")
			//		m_szPoint = "0";
			//#endif

			if(GetCgiVersion().compare("IDC") == 0)
			{
				if(m_szNetworkSet == "true")
					m_szPoint = "1";
			}
			else
			{
				if(m_szNetworkSet == "true")
					m_szPoint = "0";
			}
			//////////////////////modify end at 07/07/31 //////////////////////////////////


            if(AddNodeAttrib(mainnode, svName, szName) && AddNodeAttrib(mainnode, svIntPos, m_szPoint)
                 && AddNodeAttrib(mainnode, svMonitorType, chMTID))
            {
                if(saveBaseParam(objMonitor) && saveAdvParam(objMonitor) && saveCondition(objMonitor))
                {
                    if(m_szEditIndex.empty())
                    {
                        saveDisableByParent(mainnode, SiteView_ECC_Device, m_szDeviceID);
                        szRealIndex = AddNewMonitor(objMonitor, m_szDeviceID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
                        if(!szRealIndex.empty())
                        {
                            int nIndex = FindIndexByID(szRealIndex);            
                            if(pDevice)
                                nIndex = pDevice->AppendMonitor(szRealIndex, szName, m_nMTID);

                            char szIndex[16] = {0};
                            sprintf(szIndex, "%d", nIndex);
                            AddNodeAttrib(mainnode, svShowIndex, szIndex);

                            InsertTable(szRealIndex, m_nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
                            CEccMainView::m_pTreeView->m_SVSEUser.AddUserScopeAllRight(szRealIndex, SiteView_ECC_Monitor);
                        }
                    }
                    else
                    {// Submit
                        if(SubmitMonitor(objMonitor, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr))
                            if(pDevice)
                                pDevice->EditMonitor(m_szEditIndex, szName);
                    }
                }
            }
        }
        CloseMonitor(objMonitor);
    }

    if(m_szEditIndex.empty() && m_bContinueAdd)
    {
        m_bContinueAdd = false;
        szOpt += " && continue add";

        CEccRightView::showAddMonitor1st(m_szDeviceID, "", m_szNetworkSet);
    }
    else
    {
        CEccRightView::showMainForm();
    }

    m_szEditIndex = "";

    InsertHitRecord(CEccTreeView::m_SVSEUser.getUserID(), "treeview", "CEccAddMonitor2nd.SaveMonitor", szOpt, 
        0, 0);

    InsertHitRecord(CEccTreeView::m_SVSEUser.getUserID(), "treeview", "CEccAddMonitor2nd.SaveMonitor", szOpt, 
        0, GetTickCount() - ulStart);
    //CEccTreeView::ReCreateTreeNode(this->m_szDeviceID);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���汨������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::SetAlertCondition()
{
    unsigned long ulStart = GetTickCount();

    bool bSucc = false;
    // �򿪼����ģ��
    OBJECT objMT = GetMonitorTemplet(m_nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
    if(objMT != INVALID_VALUE)
    {// �ɹ�
        // ����ֵ�ڵ�
        MAPNODE	objError = GetMTErrorAlertCondition(objMT);
        if(objError != INVALID_VALUE && m_pErrorAlert)
        {
            if(m_pErrorAlert->SaveCondition(objError))
            {
                // ������ֵ
                MAPNODE	objAlert = GetMTWarningAlertCondition(objMT);
                if(objAlert != INVALID_VALUE && m_pWarnningAlert)
                {
                    if(m_pWarnningAlert->SaveCondition(objAlert))
                    {
                        // ������ֵ
                        MAPNODE	objGood = GetMTGoodAlertCondition(objMT);
                        if(objGood != INVALID_VALUE && m_pNormalAlert)
                            if(m_pNormalAlert->SaveCondition(objGood))
                                bSucc = true;
                    }
                }
            }
        }
        // �ύ
        SubmitMonitorTemplet(objMT, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
        CloseMonitorTemplet(objMT);  
    }
	
	if(bSucc)
        WebSession::js_af_up = "hiddenbar();showAlertMsg('" + SVResString::getResString("IDS_SetAlertConditionSuccess")
            + "','" + SVResString::getResString("IDS_Affirm") + "');";
    else
        WebSession::js_af_up = "hiddenbar();";
	
    InsertHitRecord(CEccTreeView::m_SVSEUser.getUserID(), "treeview", "CEccAddMonitor2nd.SetAlertCondition", SVResString::getResString("IDS_Set_Default_Alert"), 
        0, 0);

    InsertHitRecord(CEccTreeView::m_SVSEUser.getUserID(), "treeview", "CEccAddMonitor2nd.SetAlertCondition", SVResString::getResString("IDS_Set_Default_Alert"), 
        0, GetTickCount() - ulStart);
}

//cxy add 07/10/17...
void CEccAddMonitor2nd::UrlStepSetup()
{
	//д��һ����_ProxyServerPort������ -> urlstepinfo
	//1��������һ��savemonitor
	//

	//2��GetMonitorPropValue...
	string 	strReference1 =  "", 	strReferenceType1 = "url" ,
	strTimeOut="180",	strProxyServerPort="",	strProxyUser="",	strProxyPass="", strLimitStep="5",
	strerrorContent1="",	strcontent1="",	strusername1="",	strpassword1="";

	//���������
	OBJECT objMonitor = GetMonitor(m_szEditIndex);
	if(objMonitor != INVALID_VALUE)
    {
		MAPNODE motnitorBasenode = GetMonitorParameter(objMonitor);
		if(motnitorBasenode != INVALID_VALUE)
		{
			//
			FindNodeValue(motnitorBasenode, "_reference1", strReference1);
			//FindNodeValue(motnitorBasenode, "_urlReference", strReference1);
			//_urlReferenceType1 =  "url" 
		}

		MAPNODE motnitorAdvnode = GetMonitorAdvanceParameterNode(objMonitor);
        if(motnitorAdvnode != INVALID_VALUE)
        {
			//
			FindNodeValue(motnitorAdvnode, "_TimeOut", strTimeOut);
			FindNodeValue(motnitorAdvnode, "_ProxyServerPort", strProxyServerPort);
			FindNodeValue(motnitorAdvnode, "_ProxyUser", strProxyUser);
			FindNodeValue(motnitorAdvnode, "_ProxyPass", strProxyPass);
			FindNodeValue(motnitorAdvnode, "_limitStep", strLimitStep);

			FindNodeValue(motnitorAdvnode, "_errorContent1", strerrorContent1);
			FindNodeValue(motnitorAdvnode, "_content1", strcontent1);
			FindNodeValue(motnitorAdvnode, "_username1", strusername1);
			FindNodeValue(motnitorAdvnode, "_password1", strpassword1);
		}

		CloseMonitor(objMonitor);
	}

	//3��SaveTo monitorid +  + Ini
	string strIniFileName = m_szEditIndex;
	strIniFileName += "_UrlStepInfo.ini";
	
	WriteIniFileString("1", "nIndex", "1", strIniFileName);
	WriteIniFileString("1", "StepName", strReference1, strIniFileName);
	WriteIniFileString("1", "StepType", strReferenceType1, strIniFileName);
	WriteIniFileString("1", "StepPostData", "", strIniFileName);
	WriteIniFileString("1", "StepContent", strcontent1, strIniFileName);
	WriteIniFileString("1", "StepErrorContent", strerrorContent1, strIniFileName);
	WriteIniFileString("1", "StepUserName", strusername1, strIniFileName);
	WriteIniFileString("1", "StepUserPwd", strpassword1, strIniFileName);

	WriteIniFileString("1", "ProxyUser", strProxyUser, strIniFileName);
	WriteIniFileString("1", "ProxyPass", strProxyPass, strIniFileName);
	WriteIniFileString("1", "ProxyServerPort", strProxyServerPort, strIniFileName);
	WriteIniFileString("1", "TimeOut", strTimeOut, strIniFileName);
	WriteIniFileString("1", "limitStep", strLimitStep, strIniFileName);
	

	//4������UrlTransactionByStep��?...->���Բ�����

	//��ʾ�������ý���
	WebSession::js_af_up = "hiddenbar();showtestdevice('UrlTransStepInfo.exe?monitorid=" + m_szEditIndex +  "');";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʾ/���ذ�����Ϣ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::ShowHideHelp()
{
    m_bShowHelp = !m_bShowHelp;

    // ����ֵ����
    if(m_pErrorAlert)
        m_pErrorAlert->showHelp(m_bShowHelp);

    // ���淧ֵ����
    if(m_pWarnningAlert)
        m_pWarnningAlert->showHelp(m_bShowHelp);

    // ������ֵ����
    if(m_pNormalAlert)
        m_pNormalAlert->showHelp(m_bShowHelp);

    list<WText*>::iterator it;
    for(it = this->m_lsHelp.begin(); it != m_lsHelp.end(); it++)
    {
        if(m_bShowHelp)
            (*it)->show();
        else
            (*it)->hide();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� 
// ˵�� ���ø߼���������ֵ���༭ʱ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::SetAdvParam(OBJECT &objMonitor)
{
    MAPNODE basenode = GetMonitorAdvanceParameterNode(objMonitor);
    if(basenode != INVALID_VALUE)
    {
        list<CEccParamItem*>::iterator it;
        string szValue(""), szName(""), szSaveName("");
        for(it = m_lsAdvParam.begin(); it != m_lsAdvParam.end(); it ++)
        {
            szValue = "";
            szName = "";
            szSaveName = "";
            (*it)->getName(szName);
            szSaveName = (*it)->getSaveName();
            if(!szSaveName.empty())
                szName = szSaveName;
            if(!(*it)->getAccount().empty())
                szName += "1";
            FindNodeValue(basenode, szName, szValue);
            (*it)->setStringValue(szValue);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� 
// ˵�� ���ø߼������еĻ�����������ֵ���༭ʱ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::SetBaseAdvParam(MAPNODE &mainnode)
{
    string szDesc (""), szReportDesc ("");
    string szPlan (""), szCheckErr ("");
    string szErrFreq (""), szErrFreqUnit ("");

    FindNodeValue(mainnode, svDescription, szDesc);
    FindNodeValue(mainnode, svReportDesc, szReportDesc);
    FindNodeValue(mainnode, svPlan, szPlan);
    FindNodeValue(mainnode, svCheckErr, szCheckErr);
    FindNodeValue(mainnode, svErrFreqSave, szErrFreq);
    FindNodeValue(mainnode, svErrFreqUnit, szErrFreqUnit);

    // ����
    if(m_AdvList.m_pDescription)
        m_AdvList.m_pDescription->setText(szDesc);

    // У�����
    if(m_AdvList.m_pCheckErr)
    {
        if(szCheckErr == "true")
            m_AdvList.m_pCheckErr->setChecked(true);
        else
            m_AdvList.m_pCheckErr->setChecked(false);;
    }

    // ����ʱ���Ƶ��
    if(m_AdvList.m_pMonitorFreq)
        m_AdvList.m_pMonitorFreq->setText(szErrFreq);

    if(m_AdvList.m_pTimeUnit)
    {
        if(szErrFreqUnit == "1")
            m_AdvList.m_pTimeUnit->setCurrentIndexByStr(SVResString::getResString("IDS_Minute"));
        else
            m_AdvList.m_pTimeUnit->setCurrentIndexByStr(SVResString::getResString("IDS_Hour"));
    }

    // ����ƻ�
    if(m_AdvList.m_pPlan)
        m_AdvList.m_pPlan->setCurrentIndexByStr(szPlan);

    // ��������
    if(m_AdvList.m_pReportDesc)
        m_AdvList.m_pReportDesc->setText(szReportDesc);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� 
// ˵�� ���û�����������ֵ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::SetBaseParam(OBJECT &objMonitor)
{
    MAPNODE basenode = GetMonitorParameter(objMonitor);
    if(basenode != INVALID_VALUE)
    {
        list<CEccParamItem*>::iterator it;
        string szValue(""), szName(""), szSaveName("");
        for(it = m_lsBasicParam.begin(); it != m_lsBasicParam.end(); it ++)
        {
            szValue = "";
            szName = "";
            szSaveName = "";
            (*it)->getName(szName);
            
            szSaveName = (*it)->getSaveName();
            if(!szSaveName.empty())
                szName = szSaveName;

            if(!(*it)->getAccount().empty())
                szName += "1";

            FindNodeValue(basenode, szName, szValue);
            (*it)->setStringValue(szValue);
        }
        SetBaseAdvParam(basenode);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� 
// ˵�� ���ñ�������
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::SetConditionParam(OBJECT &objMonitor)
{
    MAPNODE alertnode = GetMonitorErrorAlertCondition(objMonitor);
    if(alertnode != INVALID_VALUE && m_pErrorAlert)
        m_pErrorAlert->SetCondition(alertnode);

    alertnode = GetMonitorWarningAlertCondition(objMonitor);
    if(alertnode != INVALID_VALUE && m_pWarnningAlert)
        m_pWarnningAlert->SetCondition(alertnode);

    alertnode = GetMonitorGoodAlertCondition(objMonitor);
    if(alertnode != INVALID_VALUE && m_pNormalAlert)
        m_pNormalAlert->SetCondition(alertnode);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������Ӽ����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string CEccAddMonitor2nd::BatchAddMonitor(list<ecc_value_list> &lsValue)
{
    list<ecc_value_list>::iterator itvalue;

    string szName(""), szRealIndex("");

    char chMTID [8] = {0};
    sprintf(chMTID, "%d", m_nMTID);

    for(itvalue = lsValue.begin(); itvalue != lsValue.end(); itvalue ++)
    {
        if(m_szNetworkSet != "true")
        {// ���������豸
            // ���ü��������
            int nMonitorCount = getUsingMonitorCount();

            int nPoint = atoi(m_szPoint.c_str());
            if(nPoint <= 0)
                nPoint = 0;
            nMonitorCount += nPoint;
            // ����Ƿ񳬵�
            if(!checkMonitorsPoint(nMonitorCount))
            {
                WebSession::js_af_up = "showMonitorCountErr('" + SVResString::getResString("IDS_PointPoor") + "','" +
                    SVResString::getResString("IDS_Affirm") + "');hiddenbar();";
                return (*itvalue).m_szValue;
            }
        }
		else
		{// �����豸

			//////////////////////begin to modify at 07/07/31 /////////////////////////////
			//#ifndef IDC_Version
			if(GetCgiVersion().compare("IDC") != 0)
			{
			//////////////////////modify end at 07/07/31 //////////////////////////////////

				// ���������豸
				// �����豸���õ���
				int nNetworkCount = getUsingNetworkCount(m_szDeviceID);
				// ����Ƿ񳬵�
				if(!checkNetworkPoint(nNetworkCount))
				{
					WebSession::js_af_up = "showMonitorCountErr('" + SVResString::getResString("IDS_PointPoor") + "','" +
						SVResString::getResString("IDS_Affirm") + "');hiddenbar();";
					return (*itvalue).m_szValue;
				}

			//////////////////////begin to modify at 07/07/31 /////////////////////////////
			//#else
			}
			else
			{
			//////////////////////modify end at 07/07/31 //////////////////////////////////

				// ���ü��������
				int nMonitorCount = getUsingMonitorCount();

				int nPoint = atoi(m_szPoint.c_str());
				if(nPoint <= 0)
					nPoint = 0;
				nMonitorCount += nPoint;
				// ����Ƿ񳬵�
				if(!checkMonitorsPoint(nMonitorCount))
				{
					WebSession::js_af_up = "showMonitorCountErr('" + SVResString::getResString("IDS_PointPoor") + "','" +
						SVResString::getResString("IDS_Affirm") + "');hiddenbar();";
					return (*itvalue).m_szValue;
				}

			//////////////////////begin to modify at 07/07/31 /////////////////////////////
			//#endif
			}
			//////////////////////modify end at 07/07/31 //////////////////////////////////

		}

        OBJECT objMonitor = CreateMonitor();
        if(objMonitor != INVALID_VALUE)
        {
            MAPNODE mainnode;
            mainnode  = GetMonitorMainAttribNode(objMonitor);
            if(mainnode != INVALID_VALUE)
            {

                szName = m_szMonitorName + ":" + (*itvalue).m_szLabel;

                char chMTID [8] = {0};
                sprintf(chMTID, "%d", m_nMTID);
                //if(m_szNetworkSet == "true")
                //    m_szPoint = "0";

				//////////////////////begin to modify at 07/07/31 /////////////////////////////
				//#ifdef IDC_Version
				//	if(m_szNetworkSet == "true")
				//		m_szPoint = "1";
				//#else
				//	if(m_szNetworkSet == "true")
				//		m_szPoint = "0";
				//#endif

				if(GetCgiVersion().compare("IDC") == 0)
				{
					if(m_szNetworkSet == "true")
						m_szPoint = "1";
				}
				else
				{
					if(m_szNetworkSet == "true")
						m_szPoint = "0";
				}
				//////////////////////modify end at 07/07/31 //////////////////////////////////


                if(AddNodeAttrib(mainnode, svName, szName) && AddNodeAttrib(mainnode, svIntPos, m_szPoint)
                    && AddNodeAttrib(mainnode, svMonitorType, chMTID))
                {
                    if(saveBaseParam(objMonitor, (*itvalue).m_szValue.c_str()) && saveAdvParam(objMonitor, (*itvalue).m_szValue.c_str()) 
                        && saveCondition(objMonitor))
                    {
                        saveDisableByParent(mainnode, SiteView_ECC_Device, m_szDeviceID);
                        szRealIndex = AddNewMonitor(objMonitor, m_szDeviceID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
                        if(!szRealIndex.empty())
                        {
                            int nIndex = FindIndexByID(szRealIndex);
                            char szIndex[16] = {0};
                            sprintf(szIndex, "%d", nIndex);
                            AddNodeAttrib(mainnode, svShowIndex, szIndex);

                            InsertTable(szRealIndex, m_nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
                            CEccMainView::m_pTreeView->m_SVSEUser.AddUserScopeAllRight(szRealIndex, SiteView_ECC_Monitor);
                            CEccMainView::m_pRightView->InsertNewMonitorList(szRealIndex);
                        }
                    }
                }
            }
            CloseMonitor(objMonitor);
        }
    }
    CEccTreeView::ReCreateTreeNode(m_szDeviceID);
    return "";
}

