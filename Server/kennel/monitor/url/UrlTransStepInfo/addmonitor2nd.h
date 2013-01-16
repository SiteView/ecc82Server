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
    void            SetBaseParam(OBJECT &objMonitor);                                      // ���û�������
    void            SetAdvParam(OBJECT &objMonitor);                                       // ���ø߼�����
    void            SetConditionParam(OBJECT &objMonitor);                                 // ���ñ�������

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
            m_pDescription = NULL;                   // ���������
            m_pReportDesc = NULL;                    // ��������
            m_pPlan = NULL;                          // ����ƻ�
            m_pCheckErr = NULL;                      // У�����
            m_pMonitorFreq = NULL;                   // ����ʱ���Ƶ��
            m_pTimeUnit = NULL;                      // ʱ�䵥λ
        }
        void Reset()
        {
            m_pDescription = NULL;                   // ���������
            m_pReportDesc = NULL;                    // ��������
            m_pPlan = NULL;                          // ����ƻ�
            m_pCheckErr = NULL;                      // У�����
            m_pMonitorFreq = NULL;                   // ����ʱ���Ƶ��
            m_pTimeUnit = NULL;                      // ʱ�䵥λ
        };

        void ResetData();

        WTextArea * m_pDescription;                   // ���������
        WTextArea * m_pReportDesc;                    // ��������
        WComboBox * m_pPlan;                          // ����ƻ�
        WCheckBox * m_pCheckErr;                      // У�����
        WLineEdit * m_pMonitorFreq;                   // ����ʱ���Ƶ��
        WComboBox * m_pTimeUnit;                      // ʱ�䵥λ
    };

    BASE_ADV_PARAM          m_AdvList;

    list<CEccParamItem*>    m_lsBasicParam;                                          // ����ѡ������list
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
