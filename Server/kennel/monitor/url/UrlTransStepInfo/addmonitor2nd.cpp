/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// addmonitor2nd.cpp
// 添加监测器的第二步，根据监测器类型展示数据
// 编辑监测器
// 设置默认报警条件
// 添加并继续等
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
// 构造函数
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
// 通过传入参数（监测器模板ID、设备节点ID， 设备是否是网络设备）展示添加监测器页面
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::AddMonitorByMTID(int nMTID, const string &szParentID, const string &szNetworkset)
{
    // 显示 上一步 按钮
    if(m_pBack)
        m_pBack->show();

    // 显示 添加并继续 按钮
    if(m_pContinue) 
        m_pContinue->show();

    // 显示 批量添加 按钮前的分隔线
    if(m_pBatchSep)
        m_pBatchSep->show();

    // 显示 添加并继续 按钮前的分隔线
    if(m_pContinueSep)
        m_pContinueSep->show();

    // 显示批量添加
    if(m_pBatch)
        m_pBatch->show();
	
	//cxy add 07/10/17...
	//if(m_nMTID == 9)
		m_pUrlStepSetup->hide();

    // 待编辑 索引置空
    m_szEditIndex = "";

    // 设置设备名称 
    if(m_pDevName)
    {
        const CEccTreeNode *pNode = CEccTreeView::getECCObject(szParentID);
        if(pNode)
            m_pDevName->setText(pNode->getName());
    }

    // 重置标题栏
    if(m_pTitle)
        m_pTitle->setText(SVResString::getResString("IDS_Add_Title") + m_szMonitorName + SVResString::getResString("IDS_Monitor_Title"));

    // 判读 待添加监测器模板ID是否和当前展示ID相等
    if(m_nMTID == nMTID)
    {// 等于
        // 重置数据
        resetData();

        // 判断待添加设备ID 是否等于 当前已缓存的设备ID
        // 判断动态参数是否为空
        // 隐藏按钮是否被正确创建
        if(szParentID != m_szDeviceID && !m_lsDyn.empty() && m_pHideButton)
        {// 满足所有条件
            // 重置 缓存的设备ID
            m_szDeviceID = szParentID;

            // 重新获得动态数据
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
            // 隐藏分隔线
            // 隐藏批量添加 按钮
            if(m_lsDyn.empty() && m_pBatch && m_pContinueSep)
            {
                m_pContinueSep->hide();
                m_pBatch->hide();
            }

            // 直接退出
            WebSession::js_af_up = "hiddenbar();";
        }

        if(szParentID != m_szDeviceID)
            m_szDeviceID = szParentID;
    }
    else
    {// 不等于
        // 重置数据
        m_nMTID = nMTID;
        m_szDeviceID = szParentID;
        m_szNetworkSet = szNetworkset;
        m_szDynName = "";

        // 清除已枚举数据
        clearData();

        // 打开设备模板
        OBJECT objMT = GetMonitorTemplet(nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
        if(objMT != INVALID_VALUE)
        {
            // 枚举基础参数
            enumBaseParam(objMT);
            // 枚举高级参数
            enumAdvParam(objMT);
            // 枚举返回值
            enumReturnItem(objMT);
            // 枚举报警条件
            enumAlert(objMT);

            // 得到主节点
            MAPNODE objNode = GetMTMainAttribNode(objMT);
            if(objNode != INVALID_VALUE)
            {
                // 得到监测器显示名称
                if(FindNodeValue(objNode, svLabel, m_szMonitorName))
                    m_szMonitorName = SVResString::getResString(m_szMonitorName.c_str());

                // 重置监测器点数
                FindNodeValue(objNode, svIntPos, m_szPoint);
                if(m_szPoint.empty())
                    m_szPoint = "1";

                // 重置标题栏
                if(m_pTitle)
                    m_pTitle->setText(SVResString::getResString("IDS_Add_Title") + m_szMonitorName + SVResString::getResString("IDS_Monitor_Title"));
            }
            // 关闭监测器模板
            CloseMonitorTemplet(objMT);
        }

        // 创建基础参数
        createBaseParam();
        // 创建高级参数
        createAdvParam();

        // 设置监测器名称
        if(m_pName)
            m_pName->setText(m_szMonitorName);

        // 判断动态参数是否为空
        // 隐藏按钮是否被正确创建
        if(!m_lsDyn.empty() && m_pHideButton)
        {// 满足条件
            // 枚举动态参数
            string szCmd("hiddenbar();");
            if(!m_pHideButton->getEncodeCmd("xclicked()").empty())
                szCmd = "update('" + m_pHideButton->getEncodeCmd("xclicked()") + "');";
            WebSession::js_af_up = szCmd;
        }
        else
        {
            // 不能批量添加数据

            // 隐藏分隔线
            if(m_pContinueSep)
                m_pContinueSep->hide();

            // 隐藏批量添加 按钮
            if(m_pBatch)
                m_pBatch->hide();

            WebSession::js_af_up = "hiddenbar();";
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 批量添加
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::BatchAdd()
{
    list<ecc_value_list> lsValue;

    // 得到动态参数 结果
    list<CEccParamItem*>::iterator lsItem;
    for(lsItem = m_lsDyn.begin(); lsItem != m_lsDyn.end(); lsItem ++)
    {
        (*lsItem)->MakeValueList(lsValue);
        m_szDynName = (*lsItem)->getName();

        break;
    }

    // 显示批量添加 界面
    CEccRightView::showBatchAddForm(lsValue, m_szMonitorName);
    WebSession::js_af_up = "hiddenbar();";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 取消添加
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::Cancel()
{
    CEccRightView::showMainForm();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 校验高级参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::checkAdvParam()
{
    bool bError = false;
    list<CEccParamItem*>::iterator lstItem;
    // 枚举每条属性
    for(lstItem = m_lsAdvParam.begin(); lstItem != m_lsAdvParam.end(); lstItem ++)
    {
        // 校验数据
        if(!(*lstItem)->checkValue())
        {// 发生错误
            if(m_pAdvance)
                m_pAdvance->showSubTable();
            bError = true;
        }
    }
    return bError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 校验基础参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::checkBaseParam()
{
    bool bError = false;
    list<CEccParamItem*>::iterator lstItem;
    // 枚举每条属性
    for(lstItem = m_lsBasicParam.begin(); lstItem != m_lsBasicParam.end(); lstItem ++)
    {
        // 校验数据
        if(!(*lstItem)->checkValue())
        {// 发生错误
            if(m_pGeneral)
                m_pGeneral->showSubTable();
            bError = true;
        }
    }
    return bError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 校验报警条件
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::checkCondition()
{
    bool bError = false;
    // 打开监测器模板
    OBJECT objMonitor = GetMonitorTemplet(m_nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
    if(objMonitor != INVALID_VALUE)
    {// 打开模板
        MAPNODE alertnode;
        // 是否已发生错误
        // 错误阀值节点
        alertnode = GetMonitorErrorAlertCondition(objMonitor);
        // 是否成功，成功后校验数据
        if(alertnode != INVALID_VALUE && m_pErrorAlert)
            if(!m_pErrorAlert->checkCondition(alertnode))
                bError = true;

            // 警告阀值节点
            alertnode = GetMonitorWarningAlertCondition(objMonitor);
            // 是否成功，成功后校验数据
            if(alertnode != INVALID_VALUE && m_pWarnningAlert)
                if(!m_pWarnningAlert->checkCondition(alertnode))
                    bError = true;

            // 正常阀值节点
            alertnode = GetMonitorGoodAlertCondition(objMonitor);
            // 是否成功，成功后校验数据
            if(alertnode != INVALID_VALUE && m_pNormalAlert)
                if(!m_pNormalAlert->checkCondition(alertnode))
                    bError = true;

        // 关闭监测器模板
        CloseMonitorTemplet(objMonitor);
    }
    return bError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 校验错误时监测频率
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
// 校验监测器名称
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
// 添加并继续
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::ContinueAdd()
{
    // 继续
    m_bContinueAdd = true;
    // 保存当前修改到新的监测器中
    SaveMonitor();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建高级参数中的基础参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createAdvBaseParam(WTable *pTable)
{
    int nRow = pTable->numRows();

    // 校验错误
    new WText(SVResString::getResString("IDS_Check_Error"), pTable->elementAt(nRow, 0));
    // 是否校验错误
    m_AdvList.m_pCheckErr = new WCheckBox("", pTable->elementAt(nRow, 1));

    pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
    pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
    pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
    pTable->GetRow(nRow)->setStyleClass("padding_top");

    // 是否校验错误帮助
    nRow ++;
    WText * pCheckErrHelp = new WText(SVResString::getResString("IDS_Check_Err_Help"), pTable->elementAt(nRow, 1));
    if(pCheckErrHelp)
    {
        pCheckErrHelp->hide();
        pTable->elementAt(nRow, 1)->setStyleClass("table_data_input_des");
        m_lsHelp.push_back(pCheckErrHelp);
    }

    // 错误频率
    nRow ++;
    new WText(SVResString::getResString("IDS_Error_Freq"), pTable->elementAt(nRow, 0));
    // 校验错误频率
    m_AdvList.m_pMonitorFreq = new WLineEdit(pTable->elementAt(nRow, 1));
    if(m_AdvList.m_pMonitorFreq)
        m_AdvList.m_pMonitorFreq->setStyleClass("cell_10");

    pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
    pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
    pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
    pTable->GetRow(nRow)->setStyleClass("padding_top");

    new WText("&nbsp;", pTable->elementAt(nRow, 1));
    // 校验错误频率时间单位
    m_AdvList.m_pTimeUnit = new WComboBox(pTable->elementAt(nRow, 1));
    if(m_AdvList.m_pTimeUnit)
    {
        m_AdvList.m_pTimeUnit->addItem(SVResString::getResString("IDS_Minute"));
        m_AdvList.m_pTimeUnit->addItem(SVResString::getResString("IDS_Hour"));
    }

    // 错误监测频率帮助
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

    // 任务计划
    nRow ++;
    new WText(SVResString::getResString("IDS_Plan"), pTable->elementAt(nRow, 0));
    // 任务计划
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
    // 任务计划帮助
    nRow ++;
    WText * pPlanHelp = new WText(SVResString::getResString("IDS_PlanHelp"), pTable->elementAt(nRow, 1));
    if(pPlanHelp)
    {
        pPlanHelp->hide();
        pTable->elementAt(nRow, 1)->setStyleClass("table_data_input_des");
        m_lsHelp.push_back(pPlanHelp);
    }

    // 描述
    nRow ++;
    new WText(SVResString::getResString("IDS_Monitor_Desc"), pTable->elementAt(nRow, 0));
    // 描述信息
    m_AdvList.m_pDescription = new WTextArea("", pTable->elementAt(nRow, 1));
    if(m_AdvList.m_pDescription )
        m_AdvList.m_pDescription->setStyleClass("cell_98");

    pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
    pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
    pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
    pTable->GetRow(nRow)->setStyleClass("padding_top");

    // 描述信息帮助
    nRow ++;
    WText *pDescHelp = new WText(SVResString::getResString("IDS_Monitor_Desc_Help"), pTable->elementAt(nRow, 1));
    if(pDescHelp)
    {
        pDescHelp->hide();
        pTable->elementAt(nRow, 1)->setStyleClass("table_data_input_des");
        m_lsHelp.push_back(pDescHelp);
    }

    // 报告描述文本
    nRow ++;
    new WText(SVResString::getResString("IDS_Report_Desc"), pTable->elementAt(nRow, 0));
    // 报告描述
    m_AdvList.m_pReportDesc = new WTextArea("", pTable->elementAt(nRow, 1));
    if(m_AdvList.m_pReportDesc)
        m_AdvList.m_pReportDesc->setStyleClass("cell_98");

    pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
    pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
    pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
    pTable->GetRow(nRow)->setStyleClass("padding_top");

    // 报告描述帮助
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
// 创建高级参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createAdvParam()
{
    if(m_pAdvance)
    {
        WTable *pSub = m_pAdvance->getListTable();
        if(pSub)
        {// 创建子表成功
            list<CEccParamItem*>::iterator it;
            // 枚举每一条属性
            for(it = m_lsAdvParam.begin(); it != m_lsAdvParam.end(); it ++)
            {
                // 是否有跟随
                if((*it)->isHasFollow())
                {// 有跟随值
                    // 得到跟随值
                    string szFollow = (*it)->getFollow();
                    if(!szFollow.empty())
                    {
                        list<CEccParamItem*>::iterator ittmp;
                        // 枚举每条属性
                        for(ittmp = m_lsAdvParam.begin(); ittmp != m_lsAdvParam.end(); ittmp ++)
                        {
                            // 是否是跟随值
                            if(strcmp(szFollow.c_str(), (*ittmp)->getName()) == 0)
                            {
                                // 设置跟随值
                                (*it)->setFollowItem(*ittmp);
                                break;
                            }
                        }
                    }
                }
                // 创建控件
                (*it)->CreateControl(pSub, m_lsHelp, m_bShowHelp);
                // 是否使用动态数据
                if((*it)->isDynamic())
                {
                    m_lsDyn.push_back((*it));
                }
            }
            
            // 创建高级参数中的基础参数
            createAdvBaseParam(pSub);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建基础参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createBaseParam()
{
    if(m_pGeneral)
    {
        WTable *pSub = m_pGeneral->getListTable();
        if(pSub)
        {
            list<CEccParamItem*>::iterator it;
            // 枚举每个属性参数
            for(it = m_lsBasicParam.begin(); it != m_lsBasicParam.end(); it ++)
            {
                // 是否有跟随值
                if((*it)->isHasFollow())
                {// 有跟随值
                    // 得到跟随值
                    string szFollow = (*it)->getFollow();
                    if(!szFollow.empty())
                    {// 跟随值不为空
                        list<CEccParamItem*>::iterator ittmp ;
                        // 枚举每一条属性参数
                        for(ittmp = m_lsBasicParam.begin(); ittmp != m_lsBasicParam.end(); ittmp ++)
                        {
                            // 是否匹配
                            if(strcmp(szFollow.c_str(), (*ittmp)->getName()) == 0)
                            {
                                // 设置跟随值
                                (*it)->setFollowItem((*ittmp));
                                break;
                            }
                        }
                    }
                }
                // 创建控件
                (*it)->CreateControl(pSub, m_lsHelp, m_bShowHelp);
                // 是否使用动态数据
                if((*it)->isDynamic())
                {
                    m_lsDyn.push_back((*it));
                    m_bHasDynamic = true;
                }
            }

            // 创建监测器名称
            createMonitorName();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建设备名称
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createDevName()
{
    if(m_pGeneral)
    {
        WTable *pSubTable = m_pGeneral->getListTable();
        if(pSubTable)
        {
            new WText(SVResString::getResString("IDS_Device_Name"), pSubTable->elementAt(0, 0));
            // 设备名称
            m_pDevName = new WLineEdit("127.0.0.1", pSubTable->elementAt(0, 1));
            if(m_pDevName)  //wangpeng 
				strcpy(m_pDevName->contextmenu_, "class='readonly_text' readOnly=true");

                //strcpy(m_pDevName->contextmenu_, "class='cell_40' readOnly=true");

            pSubTable->elementAt(0, 0)->setStyleClass("table_list_data_input_text");
            pSubTable->elementAt(0, 1)->setStyleClass("table_data_text");
            pSubTable->elementAt(0, 1)->setContentAlignment(AlignLeft);
            pSubTable->GetRow(0)->setStyleClass("padding_top");

            // 创建设备名称帮助文本
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
// 创建报警条件
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
                // 错误
                nRow = pSubTable->numRows();
                m_pErrorAlert = new SVConditionParam(pSubTable->elementAt(nRow, 0));

                // 警告
                nRow ++;
                m_pWarnningAlert = new SVConditionParam(pSubTable->elementAt(nRow, 0));

                // 正常
                nRow ++;
                m_pNormalAlert = new SVConditionParam(pSubTable->elementAt(nRow, 0));
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建隐藏按钮（枚举动态参数时调用）
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createHidebutton()
{
    int nRow = numRows();
    // 枚举动态数据
    m_pHideButton = new WPushButton("list dyn", elementAt(nRow, 0));
    if(m_pHideButton)
    {
        m_pHideButton->hide();
        WObject::connect(m_pHideButton, SIGNAL(clicked()),"showbar();",  this, SLOT(EnumDynData()), 
            WObject::ConnectionType::JAVASCRIPTDYNAMIC);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建监测器名称
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createMonitorName()
{
    if(m_pGeneral)
    {
        WTable *pTable = m_pGeneral->getListTable();
        if(pTable)
        {
            // 得到当前行数
            int nRow = pTable->numRows();
            // 说明
            new WText(SVResString::getResString("IDS_Monitor_Label"), pTable->elementAt(nRow, 0));
            // 增加（红色*）标识为必添项
            new WText("<span class =required>*</span>", pTable->elementAt(nRow, 0));

            // 创建标题输入框
            m_pName = new WLineEdit("", pTable->elementAt(nRow, 1));
            if(m_pName)
                m_pName->setStyleClass("cell_98");

            // 创建帮助文本
            WText *pNameHelp = new WText(SVResString::getResString("IDS_Monitor_Label_Help"), pTable->elementAt(nRow + 1, 1));
            if(pNameHelp)
            {
                pNameHelp->hide();
                pTable->elementAt(nRow + 1, 1)->setStyleClass("table_data_input_des");
                m_lsHelp.push_back(pNameHelp);
            }

            // 监测器名称错误提示信息
            m_pNameErr = new WText(SVResString::getResString("IDS_Monitor_Label_Error"), pTable->elementAt(nRow + 2, 1));            
            if (m_pNameErr)
            {
                pTable->elementAt(nRow + 2, 1)->setStyleClass("table_data_input_error");
                m_pNameErr->hide();
            }

            // 这个表格单元对齐方式为左上对齐
            pTable->elementAt(nRow, 0)->setContentAlignment(AlignTop | AlignLeft);
            pTable->elementAt(nRow, 0)->setStyleClass("table_list_data_input_text");
            pTable->elementAt(nRow, 1)->setStyleClass("table_data_text");
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 创建操作按钮
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::createOperate()
{
    if(m_pOperate)
    {
        // 当前行
        int nRow = m_pOperate->numRows();

        // 返回监测器列表按钮
        m_pBack = new CEccButton(SVResString::getResString("IDS_Back_One_Step"), 
                                SVResString::getResString("IDS_Back_Monitor_List_Tip"),
                                "", 
                                m_pOperate->elementAt(nRow, 0));
        if(m_pBack)
        {
            WObject::connect(m_pBack, SIGNAL(clicked()), "showbar();", this, SLOT(Forward()), 
                WObject::ConnectionType::JAVASCRIPTDYNAMIC);
        }
        // 与上一个按钮隔开一定距离
        new WText("&nbsp;", m_pOperate->elementAt(nRow, 1));
        // 保存按钮
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
        // 取消按钮
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
        // 批量添加
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
        // 添加并继续
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
        // 设置默认报警条件
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
		m_pUrlStepSetup = new CEccButton("步数设置", 
                                        "步数设置",
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
// 编辑监测器
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::EditMonitorByID(const string &szMonitorID)
{
    // 修改设备名称
    string szDeviceID = FindParentID(szMonitorID);
    if(m_pDevName)
    {
        const CEccTreeNode *pNode = CEccTreeView::getECCObject(szDeviceID);
        if(pNode)
            m_pDevName->setText(pNode->getName());
    }

    // 打开监测
    OBJECT objMonitor = GetMonitor(szMonitorID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
    if(objMonitor != INVALID_VALUE)
    {
        // 得到主节点
        MAPNODE mainnode = GetMonitorMainAttribNode(objMonitor);
        if(mainnode != INVALID_VALUE)
        {
            // 得到监测器名
            string szName (""); FindNodeValue(mainnode, svName, szName);

            // 得到监测器模板ID
            string szMTID (""); FindNodeValue(mainnode, svMonitorType, szMTID);
            if(!szMTID.empty())
            {
                int nMTID = 0;
                nMTID = atoi(szMTID.c_str());
                
                // 判断当前缓存是否和待展示相同
                if(m_nMTID == nMTID)
                {
                    // 重置数据
                    resetData();
                    // 判断待编辑监测器的父设备ID 是否等于 当前已缓存的设备ID
                    // 判断动态参数是否为空
                    // 隐藏按钮是否被正确创建
                    if(szDeviceID != m_szDeviceID && !m_lsDyn.empty() && m_pHideButton)
                    {
                        // 枚举动态参数
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
                {// 不等于
                    // 缓存
                    m_nMTID = nMTID;
                    m_szDeviceID = szDeviceID;
                    
                    // 清理数据
                    clearData();

                    // 打开设备模板
                    OBJECT objMT = GetMonitorTemplet(nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
                    if(objMT != INVALID_VALUE)
                    {
                        // 枚举基础参数
                        enumBaseParam(objMT);
                        // 枚举高级参数
                        enumAdvParam(objMT);
                        // 枚举返回值
                        enumReturnItem(objMT);
                        // 枚举报警条件
                        enumAlert(objMT);
                        
                        // 得到主节点
                        MAPNODE objNode = GetMTMainAttribNode(objMT);
                        if(objNode != INVALID_VALUE)
                        {
                            if(FindNodeValue(objNode, svLabel, m_szMonitorName))
                                m_szMonitorName = SVResString::getResString(m_szMonitorName.c_str());

                            // 监测器所占点数
                            FindNodeValue(objNode, svIntPos, m_szPoint);
                            if(m_szPoint.empty())
                                m_szPoint = "1";
                        }
                        
                        // 关闭模板
                        CloseMonitorTemplet(objMT);
                    }

                    // 创建基础参数
                    createBaseParam();
                    // 创建高级参数
                    createAdvParam();

                    // 判断动态参数是否为空
                    // 隐藏按钮是否被正确创建
                    if(!m_lsDyn.empty() && m_pHideButton)
                    {// 满足条件

                        // 枚举动态参数
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
                
                // 重置待修改的监测器索引
                m_szEditIndex = szMonitorID; 
                // 重置监测器名称
                if(m_pName)
                    m_pName->setText(szName);
            }

            // 重置标题栏
            if(m_pTitle)
                m_pTitle->setText(SVResString::getResString("IDS_Edit") + szName + SVResString::getResString("IDS_Monitor_Title"));
        }
        
        SetBaseParam(objMonitor);                                      // 设置基础数据
        SetAdvParam(objMonitor);                                       // 设置高级数据
        SetConditionParam(objMonitor);                                 // 设置报警条件
        
        // 关闭监测器
        CloseMonitor(objMonitor);
    }

    // 隐藏 上一步 按钮
    if(m_pBack)
        m_pBack->hide();

    // 隐藏 添加并继续 按钮
    if(m_pContinue) 
        m_pContinue->hide();

    // 隐藏 批量添加 分隔符
    if(m_pBatchSep)
        m_pBatchSep->hide();

    // 隐藏 添加并继续 分隔符
    if(m_pContinueSep)
        m_pContinueSep->hide();

    // 隐藏批量添加
    if(m_pBatch)
        m_pBatch->hide();
	
	//cxy add 07/10/17...
	if(m_nMTID == 9 || m_nMTID == 514)
		m_pUrlStepSetup->show();
	else
		m_pUrlStepSetup->hide();

    // 重置 保存 按钮的显示文本和提示信息
    if(m_pSave)
    {
        m_pSave->setText(SVResString::getResString("IDS_Save"));
        m_pSave->setToolTip(SVResString::getResString("IDS_Save_Tip"));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 枚举 报警条件
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::enumAlert(OBJECT &objMT)
{
    // 错误条件
    if(m_pErrorAlert)
    {
        MAPNODE	objError = GetMTErrorAlertCondition(objMT);
        // 当前函数
        if(objError != INVALID_VALUE)
        {// 
            m_pErrorAlert->SetReturnList(m_lsReturn);
            m_pErrorAlert->SetMapNode(objError);
        }
    }

    // 警告条件
    if(m_pWarnningAlert)
    {
        MAPNODE	objError = GetMTWarningAlertCondition(objMT);
        // 当前函数
        if(objError != INVALID_VALUE)
        {// 
            m_pWarnningAlert->SetReturnList(m_lsReturn);
            m_pWarnningAlert->SetMapNode(objError);
        }
    }

    // 正常条件
    if(m_pNormalAlert)
    {
        MAPNODE	objError = GetMTGoodAlertCondition(objMT);
        // 当前函数
        if(objError != INVALID_VALUE)
        {// 
            m_pNormalAlert->SetReturnList(m_lsReturn);
            m_pNormalAlert->SetMapNode(objError);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 枚举高级参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::enumAdvParam(OBJECT &objMT)
{
    LISTITEM lsItem;
    // 得到高级属性参数第一个节点
    if( FindMTAdvanceParameterFirst(objMT, lsItem))
    {
        // 枚举每个节点
        MAPNODE objNode;
        while( (objNode = FindNext(lsItem)) != INVALID_VALUE )
        {
            CEccParamItem *param = new CEccParamItem(objNode);
            m_lsAdvParam.push_back(param);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 枚举基础参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::enumBaseParam(OBJECT &objMT)
{
    LISTITEM lsItem;
    // 得到第一个标准属性参数
    if( FindMTParameterFirst(objMT, lsItem))
    {
        MAPNODE objNode;
        // 枚举每个属性
        while( (objNode = FindNext(lsItem)) != INVALID_VALUE )
        {
            CEccParamItem *param = new CEccParamItem(objNode);
            m_lsBasicParam.push_back(param);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 枚举返回值
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::enumReturnItem(OBJECT &objMT)
{
    LISTITEM lsItem;
    // 枚举每一条返回值
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
// 枚举动态参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::EnumDynData()
{
    string szRun("");
    // 枚举每个动态属性的动态数据
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
// 上一步
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::Forward()
{
    CEccRightView::showAddMonitor1st(m_szDeviceID, "", m_szNetworkSet);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 初始化
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::initForm(bool bHasHelp)
{
    // 基础类初始化函数
    CEccBaseTable::initForm(bHasHelp);

    // 
    if(m_pContent)
    {
        // 设置样式表
        setContentCellStyle("height95p");

        int nRow = m_pContent->numRows();
        // 基础信息表
        m_pGeneral = new CEccListTable(m_pContent->elementAt(nRow, 0), false, false, false);
        if(m_pGeneral)
            m_pGeneral->setTitle(SVResString::getResString("IDS_General_Infor_Title"));

        // 报警条件
        createCondition();
        
        // 高级参数表
        nRow ++;
        m_pAdvance = new CEccListTable(m_pContent->elementAt(nRow, 0), false, false, false);
        if(m_pAdvance)
            m_pAdvance->setTitle(SVResString::getResString("IDS_Advance_Option"));
    }

    // 创建设备名
    createDevName();
    // 创建操作按钮
    createOperate();
    // 创建隐藏按钮
    createHidebutton();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 数据清理
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::clearData()
{
    // 清理基础参数
    list<CEccParamItem*>::iterator lsItem;
    for(lsItem = m_lsBasicParam.begin(); lsItem != m_lsBasicParam.end(); lsItem ++)
        delete (*lsItem);

    // 清理高级参数
    for(lsItem = m_lsAdvParam.begin(); lsItem != m_lsAdvParam.end(); lsItem ++)
        delete (*lsItem);

    // 清理返回值
    list<SVReturnItem*>::iterator lsReItem;
    for(lsReItem = m_lsReturn.begin(); lsReItem != m_lsReturn.end(); lsReItem ++)
        delete (*lsReItem);

    // 列表清空
    m_lsBasicParam.clear();
    m_lsAdvParam.clear();

    m_lsReturn.clear();
    m_lsDyn.clear();

    // 删除 监测器名称
    if(m_pGeneral && m_pGeneral->getListTable())
    {
        while(m_pGeneral->getListTable()->numRows() > 2)
            m_pGeneral->getListTable()->deleteRow(m_pGeneral->getListTable()->numRows() - 1);
    }

    // 高级参数表 清空
    if(m_pAdvance && m_pAdvance->getListTable())
        m_pAdvance->getListTable()->clear();

    // 
    m_AdvList.Reset();

    // 帮助信息清空
    m_lsHelp.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 重置
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::resetData()
{
    // 基础参数
    list<CEccParamItem*>::iterator lstItem;
    for(lstItem = m_lsBasicParam.begin(); lstItem != m_lsBasicParam.end(); lstItem ++)
    {
        if(!(*lstItem)->isDynamic())
            (*lstItem)->setDefaultValue();
        else
            (*lstItem)->resetDynData();
    }

    // 高级参数
    for(lstItem = m_lsAdvParam.begin(); lstItem != m_lsAdvParam.end(); lstItem ++)
    {
        if(!(*lstItem)->isDynamic())
            (*lstItem)->setDefaultValue();
        else
            (*lstItem)->resetDynData();
    }

    // 打开监测器模板
    OBJECT objMT = GetMonitorTemplet(m_nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
    if(objMT != INVALID_VALUE)
    {
        // 枚举 报警条件
        enumAlert(objMT);
        CloseMonitorTemplet(objMT);
    }

    // 重设 监测器 名称
    if(m_pName)
        m_pName->setText(m_szMonitorName);

    m_AdvList.ResetData();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 保存高级参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::saveAdvParam(OBJECT &objMonitor, const char *pcszValue)
{
    bool bNoError = true;
    // 高级参数节点
    MAPNODE advnode = GetMonitorAdvanceParameterNode(objMonitor);
    if(advnode != INVALID_VALUE)
    {
        list<CEccParamItem*>::iterator lstItem;
        // 名称 值 保存名称
        string szName(""), szValue(""), szSavename("");
        // 计算属性 计算公式 计算使用的数据
        string szAccount (""), szExpress (""), szAccValue ("");
        char szAfterAcc[32] = {0};
        // 枚举每条属性参数
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
            {// 需要计算
                szExpress = (*lstItem)->getExpress();
                list<CEccParamItem*>::iterator itTmp;
                // 枚举每条属性
                for(itTmp = m_lsAdvParam.begin(); itTmp != m_lsAdvParam.end(); itTmp ++)
                {
                    if(szAccount.compare((*itTmp)->getName()) == 0)
                    {
                       (*itTmp)->getStringValue(szAccValue);
                        break;
                    }
                }
                // 计算
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
                // 保存结果
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
// 保存高级参数中的基础参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::saveAdvBaseParam(MAPNODE &objNode)
{
    string szDesc (""), szReportDesc ("");
    string szPlan (""), szCheckErr ("");
    string szErrFreq (""), szErrFreqUnit ("");

    // 描述
    if(m_AdvList.m_pDescription)
        szDesc = m_AdvList.m_pDescription->text();

    // 校验错误
    if(m_AdvList.m_pCheckErr)
    {
        if(m_AdvList.m_pCheckErr->isChecked())
            szCheckErr = "true";
        else
            szCheckErr = "false";
    }

    // 错误时监测频率
    if(m_AdvList.m_pMonitorFreq)
        szErrFreq = m_AdvList.m_pMonitorFreq->text();

    // 错误时监测频率单位
    if(m_AdvList.m_pTimeUnit)
        szErrFreqUnit = m_AdvList.m_pTimeUnit->currentText();
    if(szErrFreqUnit == SVResString::getResString("IDS_Hour"))
        szErrFreqUnit = "60";
    else
        szErrFreqUnit = "1";
    
    int nFreq = atoi(szErrFreq.c_str()) * atoi(szErrFreqUnit.c_str());
    char szFreq[32] = {0};
    sprintf(szFreq , "%d", nFreq);

    // 任务计划
    if(m_AdvList.m_pPlan)
        szPlan = m_AdvList.m_pPlan->currentText();

    // 报告描述
    if(m_AdvList.m_pReportDesc)
        szReportDesc = m_AdvList.m_pReportDesc->text();

    bool bNoError = false;

    // 添加
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
// 保存基础参数
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
// 保存报警条件
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CEccAddMonitor2nd::saveCondition(OBJECT &objMonitor)
{
    bool bNoError = true;
    
    // 保存错误阀值
    MAPNODE alertnode = GetMonitorErrorAlertCondition(objMonitor);
    if(alertnode != INVALID_VALUE && m_pErrorAlert)
        bNoError = m_pErrorAlert->SaveCondition(alertnode);
    else
        bNoError = false;

    // 保存错误阀值成功
    if(bNoError)
    {
        // 保存警告阀值
        alertnode = GetMonitorWarningAlertCondition(objMonitor);
        if(alertnode != INVALID_VALUE && m_pWarnningAlert)
            bNoError = m_pWarnningAlert->SaveCondition(alertnode);
        else
            bNoError = false;
    }

    if(bNoError)
    {
        // 保存正常阀值
        alertnode = GetMonitorGoodAlertCondition(objMonitor);
        if(alertnode != INVALID_VALUE && m_pNormalAlert)
            bNoError = m_pNormalAlert->SaveCondition(alertnode);
        else
            bNoError = false;
    }

    return bNoError;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 保存监测器
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
// 保存报警条件
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::SetAlertCondition()
{
    unsigned long ulStart = GetTickCount();

    bool bSucc = false;
    // 打开监测器模板
    OBJECT objMT = GetMonitorTemplet(m_nMTID, CEccMainView::m_szIDCUser, CEccMainView::m_szAddr);
    if(objMT != INVALID_VALUE)
    {// 成功
        // 错误阀值节点
        MAPNODE	objError = GetMTErrorAlertCondition(objMT);
        if(objError != INVALID_VALUE && m_pErrorAlert)
        {
            if(m_pErrorAlert->SaveCondition(objError))
            {
                // 报警阀值
                MAPNODE	objAlert = GetMTWarningAlertCondition(objMT);
                if(objAlert != INVALID_VALUE && m_pWarnningAlert)
                {
                    if(m_pWarnningAlert->SaveCondition(objAlert))
                    {
                        // 正常阀值
                        MAPNODE	objGood = GetMTGoodAlertCondition(objMT);
                        if(objGood != INVALID_VALUE && m_pNormalAlert)
                            if(m_pNormalAlert->SaveCondition(objGood))
                                bSucc = true;
                    }
                }
            }
        }
        // 提交
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
	//写第一步及_ProxyServerPort参数等 -> urlstepinfo
	//1、主动调一次savemonitor
	//

	//2、GetMonitorPropValue...
	string 	strReference1 =  "", 	strReferenceType1 = "url" ,
	strTimeOut="180",	strProxyServerPort="",	strProxyUser="",	strProxyPass="", strLimitStep="5",
	strerrorContent1="",	strcontent1="",	strusername1="",	strpassword1="";

	//监测器名称
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

	//3、SaveTo monitorid +  + Ini
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
	

	//4、调用UrlTransactionByStep吗?...->可以不用了

	//显示步数配置界面
	WebSession::js_af_up = "hiddenbar();showtestdevice('UrlTransStepInfo.exe?monitorid=" + m_szEditIndex +  "');";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 显示/隐藏帮助信息
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CEccAddMonitor2nd::ShowHideHelp()
{
    m_bShowHelp = !m_bShowHelp;

    // 错误阀值帮助
    if(m_pErrorAlert)
        m_pErrorAlert->showHelp(m_bShowHelp);

    // 警告阀值帮助
    if(m_pWarnningAlert)
        m_pWarnningAlert->showHelp(m_bShowHelp);

    // 正常阀值帮助
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
// 函数 
// 说明 设置高级参数属性值（编辑时）
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
// 函数 
// 说明 设置高级参数中的基础参数属性值（编辑时）
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

    // 描述
    if(m_AdvList.m_pDescription)
        m_AdvList.m_pDescription->setText(szDesc);

    // 校验错误
    if(m_AdvList.m_pCheckErr)
    {
        if(szCheckErr == "true")
            m_AdvList.m_pCheckErr->setChecked(true);
        else
            m_AdvList.m_pCheckErr->setChecked(false);;
    }

    // 错误时监测频率
    if(m_AdvList.m_pMonitorFreq)
        m_AdvList.m_pMonitorFreq->setText(szErrFreq);

    if(m_AdvList.m_pTimeUnit)
    {
        if(szErrFreqUnit == "1")
            m_AdvList.m_pTimeUnit->setCurrentIndexByStr(SVResString::getResString("IDS_Minute"));
        else
            m_AdvList.m_pTimeUnit->setCurrentIndexByStr(SVResString::getResString("IDS_Hour"));
    }

    // 任务计划
    if(m_AdvList.m_pPlan)
        m_AdvList.m_pPlan->setCurrentIndexByStr(szPlan);

    // 报告描述
    if(m_AdvList.m_pReportDesc)
        m_AdvList.m_pReportDesc->setText(szReportDesc);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数 
// 说明 设置基础参数属性值
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
// 函数 
// 说明 设置报警条件
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
// 批量添加监测器
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
        {// 不是网络设备
            // 已用监测器点数
            int nMonitorCount = getUsingMonitorCount();

            int nPoint = atoi(m_szPoint.c_str());
            if(nPoint <= 0)
                nPoint = 0;
            nMonitorCount += nPoint;
            // 检测是否超点
            if(!checkMonitorsPoint(nMonitorCount))
            {
                WebSession::js_af_up = "showMonitorCountErr('" + SVResString::getResString("IDS_PointPoor") + "','" +
                    SVResString::getResString("IDS_Affirm") + "');hiddenbar();";
                return (*itvalue).m_szValue;
            }
        }
		else
		{// 网络设备

			//////////////////////begin to modify at 07/07/31 /////////////////////////////
			//#ifndef IDC_Version
			if(GetCgiVersion().compare("IDC") != 0)
			{
			//////////////////////modify end at 07/07/31 //////////////////////////////////

				// 已用网络设备
				// 网络设备已用点数
				int nNetworkCount = getUsingNetworkCount(m_szDeviceID);
				// 检测是否超点
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

				// 已用监测器点数
				int nMonitorCount = getUsingMonitorCount();

				int nPoint = atoi(m_szPoint.c_str());
				if(nPoint <= 0)
					nPoint = 0;
				nMonitorCount += nPoint;
				// 检测是否超点
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

