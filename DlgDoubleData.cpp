// DlgDoubleData.cpp : implementation file
//

#include "pch.h"

// CDlgDoubleData dialog

IMPLEMENT_DYNAMIC(CDlgDoubleData, CDialogEx)

CDlgDoubleData::CDlgDoubleData(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DOUBLE_VALUE, pParent)
{
	m_bFloatMode = FALSE;
	m_doubleValue = 0.0;
	m_intValue = 0;
	m_DecimalPlaces = 0;
}

CDlgDoubleData::~CDlgDoubleData()
{
}

void CDlgDoubleData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DATA, m_EditValue);
	DDX_Control(pDX, IDC_STATIC_DATA_LABEL, m_Static_Label);
}


BEGIN_MESSAGE_MAP(CDlgDoubleData, CDialogEx)
END_MESSAGE_MAP()


// CDlgDoubleData message handlers


BOOL CDlgDoubleData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_EditValue.SetDecimalPlaces(m_DecimalPlaces);
	if (m_bFloatMode)
		m_EditValue.SetDoubleValue(m_doubleValue);
	else
		m_EditValue.SetValue(m_intValue);
	SetWindowTextW(m_csDialogTitle);
	m_Static_Label.SetWindowTextW(m_csLabel);
	m_EditValue.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
}


void CDlgDoubleData::OnOK()
{
	if (m_bFloatMode)
		m_doubleValue = m_EditValue.GetDoubleValue();
	else
		m_intValue = m_EditValue.GetValue();
	CDialogEx::OnOK();
}
