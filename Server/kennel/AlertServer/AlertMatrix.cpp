#include <algorithm>

class CellValue
{

public:
    bool IsAString() const;
    bool IsANumber() const;
  	bool IsBoolean() const;
	bool IsError() const;

    bool IsEmpty() const;


    CellValue(const std::string&);
    CellValue(double Number);
	CellValue(unsigned long Code, bool Error); 
	CellValue(bool TrueFalse);
	CellValue(const char* values);
	CellValue(int i);

    CellValue();

    const std::string& StringValue() const;
    double NumericValue() const;
	bool BooleanValue() const;
	unsigned long ErrorValue() const;

	std::string StringValueLowerCase() const;

	enum ValueType 
	{
		string, number, boolean, error, empty
	};

	operator std::string() const;
	operator bool() const;
	operator double() const;
	operator unsigned long() const;

	void clear();


private:
	ValueType Type;
  
    std::string ValueAsString;
    double ValueAsNumeric;
	bool ValueAsBool;
	unsigned long ValueAsErrorCode;


};

class CellMatrix
{
public:

    CellMatrix(unsigned long rows, unsigned long columns);
	CellMatrix();
	CellMatrix(double x);
	CellMatrix(std::string x);
	CellMatrix(const char* x);
	CellMatrix(const MyArray& data);
	CellMatrix(const MyMatrix& data);
	CellMatrix(unsigned long i);
	CellMatrix(int i);
    
    const CellValue& operator()(unsigned long i, unsigned long j) const;
    CellValue& operator()(unsigned long i, unsigned long j);

    unsigned long RowsInStructure() const;
    unsigned long ColumnsInStructure() const;

	void PushBottom(const CellMatrix& newRows);



private:

	std::vector<std::vector<CellValue> > Cells;
    unsigned long Rows;
    unsigned long Columns;

};

CellMatrix MergeCellMatrices(const CellMatrix& Top, const CellMatrix& Bottom);

unsigned long maxi(unsigned long a, unsigned long b)
{
	return a > b ? a : b;
}

bool CellValue::IsAString() const
{
	return Type == string;
}


bool CellValue::IsANumber() const
{
	return Type == number;
}

bool CellValue::IsBoolean() const
{
	return Type == boolean;
}

bool CellValue::IsError() const
{
	return Type == error;
}

bool CellValue::IsEmpty() const
{
	return Type == empty;
}


CellValue::operator std::string() const
{
	if (Type != string)
		throw("non string cell asked to be a string");
	return ValueAsString;

}
	
CellValue::operator bool() const
{
	if (Type != boolean)
		throw("non boolean cell asked to be a bool");
	return ValueAsBool;
}


CellValue::operator double() const
{
	if (Type != number)
		throw("non number cell asked to be a number");
	return ValueAsNumeric;


}

CellValue::operator unsigned long() const
{
	if (Type != number)
		throw("non number cell asked to be a number");
	return static_cast<unsigned long>(ValueAsNumeric);

}

void CellValue::clear()
{
	Type = empty;
	ValueAsString="";
	ValueAsNumeric=0.0;
	ValueAsBool=false;
	ValueAsErrorCode=0;
}


CellValue::CellValue(const std::string& value) : Type(CellValue::string), 
ValueAsString(value), ValueAsNumeric(0.0), ValueAsBool(false), ValueAsErrorCode(0)
{

}




CellValue::CellValue(const char* value) : Type(CellValue::string), 
ValueAsString(value), ValueAsNumeric(0.0), ValueAsBool(false), ValueAsErrorCode(0)
{

}
CellValue::CellValue(double Number): Type(CellValue::number), 
ValueAsString(""), ValueAsNumeric(Number), ValueAsBool(false), ValueAsErrorCode(0)
{

}

CellValue::CellValue(int i): Type(CellValue::number), 
ValueAsString(""), ValueAsNumeric(i), ValueAsBool(false), ValueAsErrorCode(0)
{

}

CellValue::CellValue(unsigned long Code, bool Error): Type(error), 
ValueAsString(""), ValueAsNumeric(Code), ValueAsBool(false), ValueAsErrorCode(Code)
{
	if (!Error)
		Type = number;
}

CellValue::CellValue(bool TrueFalse)
 : Type(CellValue::boolean), 
ValueAsString(""), ValueAsNumeric(0.0), ValueAsBool(TrueFalse), ValueAsErrorCode(0)
{

}

CellValue::CellValue(): Type(CellValue::empty), 
ValueAsString(""), ValueAsNumeric(0.0), ValueAsBool(false), ValueAsErrorCode(0)
{
}

const std::string& CellValue::StringValue() const
{
	if (Type != string)
		throw("non string cell asked to be a string");
	return ValueAsString;
}
double CellValue::NumericValue() const
{
	
	if (Type != number)
		throw("non number cell asked to be a number");
	return ValueAsNumeric;
}
bool CellValue::BooleanValue() const
{
	
	if (Type != boolean)
		throw("non boolean cell asked to be a bool");

	return ValueAsBool;
}
unsigned long CellValue::ErrorValue() const
{
	if (Type != error)
		throw("non error cell asked to be an error");

	return ValueAsErrorCode;
}

std::string CellValue::StringValueLowerCase() const
{
	if (Type != string)
		throw("non string cell asked to be a string");
	std::string tmp(ValueAsString);

    std::transform(tmp.begin(),tmp.end(),tmp.begin(),tolower);
	return tmp;
}

CellMatrix::CellMatrix() : Cells(0), Rows(0), Columns(0)
{
}

CellMatrix::CellMatrix(double x): Cells(1), Rows(1), Columns(1)
{
	Cells[0].push_back(CellValue(x));
}
CellMatrix::CellMatrix(std::string x): Cells(1), Rows(1), Columns(1)
{
	Cells[0].push_back(CellValue(x));
}

CellMatrix::CellMatrix(const char* x): Cells(1), Rows(1), Columns(1)
{
	Cells[0].push_back(CellValue(x));
}




CellMatrix::CellMatrix(const MyArray& data) : Cells(data.size()), 
	Rows(data.size()), Columns(1)
{
	for (unsigned long i=0; i < data.size(); ++i)
		Cells[i].push_back(CellValue(data[i]));
}
CellMatrix::CellMatrix(const MyMatrix& data): Cells(data.rows()), 
	Rows(data.rows()), Columns(data.columns())
{
	for (unsigned long i=0; i < data.rows(); ++i)
		for (unsigned long j=0; j < data.columns(); ++j)
			Cells[i].push_back(CellValue(Element(data,i,j)));
}

CellMatrix::CellMatrix(unsigned long i)
	: Cells(1), Rows(1), Columns(1)
{
	Cells[0].push_back(CellValue(static_cast<double>(i)));
}

CellMatrix::CellMatrix(int i): Cells(1), Rows(1), Columns(1)
{
	Cells[0].push_back(CellValue(static_cast<double>(i)));
}
    
CellMatrix::CellMatrix(unsigned long rows, unsigned long columns) 
: Cells(rows), Rows(rows), Columns(columns)
{
	for (unsigned long i=0; i < rows; i++)
		Cells[i].resize(columns);
}
    
const CellValue& CellMatrix::operator()(unsigned long i, unsigned long j) const
{
	return Cells.at(i).at(j);

}
CellValue& CellMatrix::operator()(unsigned long i, unsigned long j)
{
	return Cells.at(i).at(j);
}

unsigned long CellMatrix::RowsInStructure() const
{
	return Rows;
}
unsigned long CellMatrix::ColumnsInStructure() const
{	
	return Columns;
}

CellMatrix MergeCellMatrices(const CellMatrix& Top, const CellMatrix& Bottom)
{
	unsigned long cols = maxi(Top.ColumnsInStructure(), Bottom.ColumnsInStructure());
	unsigned long rows = Top.RowsInStructure()+Bottom.RowsInStructure();

	CellMatrix merged(rows,cols);

	{for (unsigned long i=0; i < Top.ColumnsInStructure(); i++)
		for (unsigned long j=0; j < Top.RowsInStructure(); j++)
			merged(j,i) = Top(j,i);}

	for (unsigned long i=0; i < Bottom.ColumnsInStructure(); i++)
		for (unsigned long j=0; j < Bottom.RowsInStructure(); j++)
			merged(j+Top.RowsInStructure(),i) = Bottom(j,i);


	return merged;
}

void CellMatrix::PushBottom(const CellMatrix& newRows)
{
	CellMatrix newRowsResize(newRows);
	unsigned long newColumns = maxi(newRows.ColumnsInStructure(),Columns);

	if (newColumns > Columns)
		for (unsigned long i=0; i < Rows; i++)
			Cells[i].resize(newColumns);

	if (newColumns > newRows.Columns)
		for (unsigned long i=0; i < newRowsResize.Rows; i++)
			newRowsResize.Cells[i].resize(newColumns);

	for (unsigned long i=0; i < newRowsResize.Rows; i++)
		Cells.push_back(newRowsResize.Cells[i]);

	Rows = static_cast<unsigned long>(Cells.size());
	Columns = newColumns;

}


void CDownloadAddress::doneIt() 
{
	m_Address.GetWindowText(m_strAddress);

	if (m_strAddress =="")
	{
		AfxMessageBox("\n error! ");
		return ;
	}

	CString szHTTP	= m_strAddress;
	szHTTP	= szHTTP.Left(5);
	szHTTP.MakeUpper();

	
	if(szHTTP	!=	_T("HTTP:")) 
		m_strAddress	=	_T("HTTP://") + m_strAddress;

	CDialog::OnOK();	

}


CString CDownloadAddress::GetDownloadAddress()  
{	
	return m_strAddress;
}



CellMatrix::CellMatrix()
{
	
	m_clrHilite = GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow = GetSysColor(COLOR_BTNSHADOW);
	m_clrDkShad = GetSysColor(COLOR_3DDKSHADOW);

	m_clrButton = RGB(160,180,220); 
	m_bDrawGripper = false;
	m_bDrawSeperator = false;
	m_bFlatStyle = TRUE;

}

CellMatrix::~CellMatrix()
{
}


void CellMatrix::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(1, 10, NULL);
	CEdit::OnMouseMove(nFlags, point);
}

void CellMatrix::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = true;
	CEdit::OnLButtonDown(nFlags, point);
}

void CellMatrix::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown =false;
	CEdit::OnLButtonUp(nFlags, point);
}

void CellMatrix::OnTimer(UINT nIDEvent) 
{
	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	static bool bPainted = false;

	if(!rcItem.PtInRect(pt)) {
		KillTimer(1);
		DrawEdit(FE_MOUSEOUT, m_clrDkShad, m_clrHilite);
		return;
	}
	else {
		DrawEdit(FE_MOUSEOVER, m_clrShadow, m_clrHilite);
	}



	CEdit::OnTimer(nIDEvent);
}

void CellMatrix::OnPaint() 
{
	Default();
	DrawEdit(FE_MOUSEOUT, m_clrDkShad, m_clrButton);

}



void CellMatrix::OnSysColorChange() 
{
	CEdit::OnSysColorChange();
	m_clrHilite = GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow = GetSysColor(COLOR_BTNSHADOW);
	m_clrDkShad = GetSysColor(COLOR_3DDKSHADOW);
	m_clrButton = RGB(160,180,220); 
	
}

void CellMatrix::DrawEdit(int style, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	CRect rcItem;
	CRect rcGripper;
	CRect rcSeperator;

	GetClientRect(&rcItem);
	GetClientRect(&rcGripper);
	GetClientRect(&rcSeperator);

	CDC* pDC = GetDC();
	COLORREF clrBlack;
	clrBlack = RGB(0, 0, 0);

	if(m_bFlatStyle)
	{
		pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
		rcItem.InflateRect(1,1);
		
		if (!IsWindowEnabled()) {
			pDC->Draw3dRect(rcItem, m_clrHilite, m_clrHilite);
		}
		
		else {
			pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		}

		rcItem.InflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		rcItem.InflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		rcItem.InflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
	
		rcItem.DeflateRect(4, 4);

		if(m_bDrawGripper)
		{
			rcGripper.top = rcItem.top;
			rcGripper.bottom = rcItem.bottom;
			rcGripper.left -= 4;
			rcGripper.right = rcGripper.left +2;
			pDC->Draw3dRect(rcGripper, m_clrHilite, m_clrShadow);
			rcGripper.left -= 3;
			rcGripper.right = rcGripper.left +2;
			pDC->Draw3dRect(rcGripper, m_clrHilite, m_clrShadow);
		}
		if(m_bDrawSeperator)
		{
			rcSeperator.top = rcItem.top;
			rcSeperator.bottom = rcItem.bottom;
			rcSeperator.left = rcItem.right + 2;
			rcSeperator.right = rcSeperator.left + 2;
			pDC->Draw3dRect(rcSeperator, m_clrShadow, m_clrHilite);
		}
	
	
		if (!IsWindowEnabled()) {
			return;
		}

	
	
		if(style == FE_MOUSEOVER)
		{
			pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton); 
			rcItem.top -= 1; 
			rcItem.left -= 1;
			rcItem.bottom += 1;
			rcItem.right += 1; 
			pDC->Draw3dRect(rcItem, m_clrShadow, m_clrHilite); 
		}
	}
	else {

		pDC->Draw3dRect(rcItem, clrBlack, clrBlack);
		rcItem.InflateRect(1, 1);
		pDC->Draw3dRect(rcItem, clrBlack, clrBlack);
		pDC->Draw3dRect(rcItem, clrBlack, clrBlack);
		rcItem.InflateRect(1,1);

		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		rcItem.InflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		rcItem.DeflateRect(4, 4);
		
		if(m_bDrawGripper)
		{
			rcGripper.top = rcItem.top - 2;
			rcGripper.bottom = rcItem.bottom + 2;
			rcGripper.left -= 6;
			rcGripper.right = rcGripper.left + 2;
			pDC->Draw3dRect(rcGripper, clrBlack, clrBlack);
			rcGripper.left -= 3;
			rcGripper.right -= 3;
			pDC->Draw3dRect(rcGripper, clrBlack, clrBlack);
		}

		if(m_bDrawSeperator)
		{
			rcSeperator.top = rcItem.top - 2;
			rcSeperator.bottom = rcItem.bottom + 2;
			rcSeperator.left = rcItem.right + 4;
			rcSeperator.right = rcSeperator.left + 2;
			pDC->Draw3dRect(rcSeperator, clrBlack, clrBlack);
		}
	}

	ReleaseDC(pDC);
}

void CellMatrix::SetIndent(int margin)
{
	SetMargins(margin, 1);
}