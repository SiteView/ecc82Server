#ifndef _SiteView_Ecc_Add_Monitor_2nd_H_
#define _SiteView_Ecc_Add_Monitor_2nd_H_

#include "ecctable.h"

class WPushButton;
class WLineEdit;
class WTextArea;
class WComboBox;

class CEccListTable;

class SVConditionParam;
class CEccParamItem;
class SVReturnItem;

#include "../../kennel/svdb/svapi/svapi.h"

#include "rightview.h"

class CEccAddMonitor2nd : public CEccBaseTable
{
    //MOC: W_OBJECT CEccAddMonitor2nd:CEccBaseTable
    W_OBJECT;
public:
    CEccAddMonitor2nd(WContainerWidget *parent = NULL);
    string  BatchAddMonitor(list<ecc_value_list> &lsValue);
    void    AddMonitorByMTID(int nMTID, const string &szParentID, const string &szNetworkset);
    void    EditMonitorByID(const string &szMonitorID);
private://slots
    //MOC: SLOT CEccAddMonitor2nd::Forward()
    void            Forward();
    //MOC: SLOT CEccAddMonitor2nd::SaveMonitor()
    void            SaveMonitor();
    //MOC: SLOT CEccAddMonitor2nd::Cancel()
    void            Cancel();
    //MOC: SLOT CEccAddMonitor2nd::BatchAdd()
    void            BatchAdd();
    //MOC: SLOT CEccAddMonitor2nd::EnumDynData()
    void            EnumDynData();
    //MOC: SLOT CEccAddMonitor2nd::SetAlertCondition()
    void            SetAlertCondition();
    //MOC: SLOT CEccAddMonitor2nd::ContinueAdd()
    void            ContinueAdd();
    //MOC: SLOT CEccAddMonitor2nd::UrlStepSetup()
    void            UrlStepSetup();
    
    virtual void    ShowHideHelp();
private:
    virtual void    initForm(bool bHasHelp);

    void            createAdvParam();
    void            createAdvBaseParam(WTable *pTable);
    void            createBaseParam();
    void            createCondition();
    void            createDevName();
    void            createHidebutton();
    void            createMonitorName();
    void            createOperate();

    void            enumAdvParam(OBJECT &objMT);
    void            enumBaseParam(OBJECT &objMT);
    void            enumAlert(OBJECT &objMT);
    void            enumReturnItem(OBJECT &objMT);

    bool            checkName(string &szName);
    bool            checkBaseParam();
    bool            checkAdvParam();
    bool            checkCondition();
    bool            checkMonitorFreq(string &szValue);

    void            clearData();

    void            resetData();

    void            SetBaseAdvParam(MAPNODE &mainnode);
    void            SetBaseParam(OBJECT &objMonitor);                                      // 设置基础数据
    void            SetAdvParam(OBJECT &objMonitor);                                       // 设置高级数据
    void            SetConditionParam(OBJECT &objMonitor);                                 // 设置报警条件

    bool            saveAdvParam(OBJECT &objMonitor, const char *pcszValue = NULL);
    bool            saveAdvBaseParam(MAPNODE &objNode);
    bool            saveBaseParam(OBJECT &objMonitor, const char *pcszValue = NULL);
    bool            saveCondition(OBJECT &objMonitor);

    bool                    m_bHasDynamic;
    bool                    m_bContinueAdd;

    CEccListTable           *m_pGeneral;
    CEccListTable           *m_pAdvance;

    CEccButton              *m_pBatch;
    CEccImportButton        *m_pSave;
    CEccButton              *m_pContinue;
    CEccButton              *m_pCancel;
    CEccButton              *m_pBack;
	
	//cxy add 07/10/17...
	CEccButton              *m_pUrlStepSetup;

    WText                   *m_pContinueSep;
    WText                   *m_pBatchSep;

    SVConditionParam        *m_pErrorAlert;
    SVConditionParam        *m_pWarnningAlert;
    SVConditionParam        *m_pNormalAlert;

    WPushButton             *m_pHideButton;

    WLineEdit               *m_pName;
    WText                   *m_pNameErr;
    WText                   *m_pFreqErr;
    WLineEdit               *m_pDevName;

    struct BASE_ADV_PARAM
    {
        BASE_ADV_PARAM()
        {
            m_pDescription = NULL;                   // 监测器描述
            m_pReportDesc = NULL;                    // 报告描述
            m_pPlan = NULL;                          // 任务计划
            m_pCheckErr = NULL;                      // 校验错误
            m_pMonitorFreq = NULL;                   // 错误时监测频率
            m_pTimeUnit = NULL;                      // 时间单位
        }
        void Reset()
        {
            m_pDescription = NULL;                   // 监测器描述
            m_pReportDesc = NULL;                    // 报告描述
            m_pPlan = NULL;                          // 任务计划
            m_pCheckErr = NULL;                      // 校验错误
            m_pMonitorFreq = NULL;                   // 错误时监测频率
            m_pTimeUnit = NULL;                      // 时间单位
        };

        void ResetData();

        WTextArea * m_pDescription;                   // 监测器描述
        WTextArea * m_pReportDesc;                    // 报告描述
        WComboBox * m_pPlan;                          // 任务计划
        WCheckBox * m_pCheckErr;                      // 校验错误
        WLineEdit * m_pMonitorFreq;                   // 错误时监测频率
        WComboBox * m_pTimeUnit;                      // 时间单位
    };

    BASE_ADV_PARAM          m_AdvList;

    list<CEccParamItem*>    m_lsBasicParam;                                          // 基础选项数据list
    list<CEccParamItem*>    m_lsAdvParam;
    list<CEccParamItem*>    m_lsDyn;
    list<SVReturnItem*>     m_lsReturn;

    int                     m_nMTID;
    string                  m_szDeviceID;
    string                  m_szNetworkSet;
    string                  m_szEditIndex;
    string                  m_szMonitorName;
    string                  m_szPoint;
    string                  m_szDynName;
};

#endif
