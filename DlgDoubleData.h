#pragma once
#include "afxdialogex.h"


// CDlgDoubleData dialog

class CDlgDoubleData : public CDialogEx
{
	CEditDecimal m_EditValue;
	CStatic m_Static_Label;
	CString m_csDialogTitle;
	CString m_csLabel;
	double m_doubleValue;
	int m_intValue;
	int m_DecimalPlaces;
	BOOL m_bFloatMode;
	DECLARE_DYNAMIC(CDlgDoubleData)
public:
	CDlgDoubleData(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgDoubleData();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DOUBLE_VALUE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void SetDoubleValue(double Value)
	{
		m_doubleValue = Value;
	}
	void SetIntValue(int Value)
	{
		m_intValue = Value;
	}
	void SetDoubleMode()
	{
		m_bFloatMode = TRUE;
	}
	void SetDecimalPlaces(int DecimalPlaces)
	{
		m_DecimalPlaces = DecimalPlaces;
	}

	double GetDoubleValue()
	{
		return m_doubleValue;
	}

	int GetIntValue()
	{
		return m_intValue;
	}
	void SetDialogTitle(CString& csTit)
	{
		m_csDialogTitle = csTit;
	}
	void SetLabel(CString& csLab)
	{
		m_csLabel = csLab;
	}
};
