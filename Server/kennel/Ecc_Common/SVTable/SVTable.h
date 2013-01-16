
#ifndef _SV_TABLE_H_
#define _SV_TABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;

// 数据类型
enum SVDataType
{
    adNULL       = 0x0000,                  // NULL
    adLineEdit   = 0x0001,                  // editbox
    adCheckBox   = 0x0002,                  // checkbox
    adComboBox   = 0x0003,                  // combobox
    adText       = 0x0004,                  // text
    adImage      = 0x0005,                  // image
    adRadioBox   = 0x0006,                  // radiobox
    adPushButton = 0x0007,                  // button
    adString     = 0x0008,                  // string
    adInt        = 0x0009                   // int
};

// siteview table
class SVTable;
// siteview table row
class SVTableRow;

// siteview table cell
class SVTableCell 
{
    // 友元类
    friend class SVTableRow;
    friend class SVIntTable;
    friend class SVTable;
public:
    // 构造函数
    SVTableCell();
    // 拷贝构造函数
    SVTableCell(const SVTableCell &cell);
    // 析构函数
    ~SVTableCell();

    // operator =
    const SVTableCell &operator=(const SVTableCell &cell)
    {
        // 类型为string 且 不为空
        if(cell.m_nType == adString && cell.m_pValue)
        {
            // 保留原始数据指针
            void *pszTemp = m_pValue; 
            // 分配内存
            m_pValue = new char[strlen((char*)(cell.m_pValue)) + 1];
            // 拷贝
            strcpy((char*)m_pValue, (const char*)(cell.m_pValue));
            // 删除原始数据
            delete []pszTemp;
        }
        else if(cell.m_pValue)
            m_pValue = cell.m_pValue;
        else
        {
            if(m_nType == adString)
                delete []m_pValue;
            m_pValue = NULL;
        }

        m_nType = cell.m_nType;
        m_nTag  = cell.m_nTag;
        m_nCol = cell.m_nCol;
        m_szProperty = cell.m_szProperty;
        m_szRow      = cell.m_szRow;
        return *this;
    }
    
    const void * Value() ;                      // get pointer
    const char * Row() ;                        // get row's index (char*)
    const char * Property();                    // get cell's Property

    const int Type();                           // get data type
    const int Col() ;                           // get Col's index
    const int RowIndex();                       // get row's index (int)
    const int Tag();                            // get tag

    void setProperty(const char * szProperty);  // set cell's Property
    void setValue(const void * pVoid);          // set pointer
    void setType(int nType);                    // set data type
    void setTag(int nTag);                      // set tag
private:
    string m_szRow;                             // Row's Index(string)
    string m_szProperty;                        // Property
    
    void * m_pValue;                            // pointer
    
    int    m_nType;                             // data type
    int    m_nRow;                              // row's Index(int)
    int    m_nCol;                              // col's Index
    int    m_nTag;                              // tag
};

typedef map<int, SVTableCell, less<int> > Cells;
typedef Cells::iterator cell;

class SVTableRow
{
    friend class SVIntTable;
    friend class SVTable;
public:
    SVTableRow();
    ~SVTableRow();
    void setTag(int nTag);
    void setProperty(const char * szProperty);
    int getRowIndex() { return m_nRow;};
    int getTag();
    const char * getProperty();
    const char * getRow() { return m_szRow.c_str(); };
    SVTableCell * Cell(int nCol);
private:
    void WriteCell(int nCol, SVTableCell &cellValue);
    void DelCell(int nCol);
private:
    Cells m_Cells;
    
    int   m_nRow;
    int   m_nTag;

    string m_szRow;
    string m_szProperty;
};

typedef map<string, SVTableRow, less<string> > Rows;
typedef Rows::iterator row;

class SVTable
{
public:
    SVTable();
    ~SVTable();
    SVTableCell * Cell(string szKey, int nCol);
    void WriteCell(string szKey, int nCol, SVTableCell &cellValue);
    void DelCell(string szKey, int nCol);
    void DelRow(string szKey);
    const int RowCount();
    const int ColCount(string szKey);
    bool empty() {return m_Row.empty();};
    SVTableRow * Row(string szKey);

    row begin() { return m_Row.begin();};
    row end() { return m_Row.end();};
    void clear();
private:
    Rows m_Row;
//    row m_row;
};

// 
typedef map<int, SVTableRow, less<int> > IntRows;
typedef IntRows::iterator irow;

class SVIntTable
{
public:
    SVIntTable();
    ~SVIntTable();
    SVTableCell * Cell(int nRow, int nCol);
    void WriteCell(int nRow, int nCol, SVTableCell &cellValue);
    void DelCell(int nRow, int nCol);
    void DelRow(int nRow);
    const int RowCount();
    const int ColCount(int nRow);
    bool empty() {return m_Row.empty();};
    SVTableRow * Row(int nRow);

    irow begin() { return m_Row.begin();};
    irow end() { return m_Row.end();};
    void clear();
private:
    IntRows m_Row;
};

#endif