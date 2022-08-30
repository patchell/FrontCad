#pragma once
#include "afxdialogex.h"


// CDlgSnapSelector dialog

class CDlgSnapSelector : public CDialogEx
{
	SnapOrder* m_pSnapTypesPool;
	SnapOrder* m_pSnapTypesActive;
	CButton m_Button_To;
	CButton m_Button_From;
	CListBox m_Listbox_SnapList;
	CListBox m_Listbox_Snap_Pool;
	DECLARE_DYNAMIC(CDlgSnapSelector)

public:
	CDlgSnapSelector(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSnapSelector();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SNAP_TYPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonSnapListFrom();
	afx_msg void OnClickedButtonSnapSelTo();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeListsnapPool();
	afx_msg void OnSelchangeListSnapList();
	afx_msg void OnDblclkListSnapList();
	afx_msg void OnDblclkListsnapPool();
};
