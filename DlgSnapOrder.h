#pragma once
#include "afxdialogex.h"


// CDlgSnapOrder dialog

class CDlgSnapOrder : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSnapOrder)

public:
	CDlgSnapOrder(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSnapOrder();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SNAP_ORDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Static_Order_1;
	CStatic m_Static_Order_2;
	CStatic m_Static_Order_3;
	CStatic m_Static_Order_4;
	CStatic m_Static_Order_5;
	CEditDecimal m_Edit_Order_1;
	CEditDecimal m_Edit_Order_2;
	CEditDecimal m_Edit_Order_3;
	CEditDecimal m_Edit_Order_4;
	CEditDecimal m_Edit_Order_5;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
