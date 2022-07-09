#pragma once
#include "afxdialogex.h"


// CDlgDimensionPropertiex dialog

class CDlgDimensionPropertiex : public CDialogEx
{
	BOOL m_bDirty;
	CCadDimension* m_pDimension;
	//---------------------------------
	// Controls
	//---------------------------------
	CStaticBitmap m_Static_LineColor;
	CStaticBitmap m_Static_TextColor;
	CStaticBitmap m_Static_BackgroundColor;
	CEditDecimal m_Edit_TextHeight;
	CEditDecimal m_Edit_LineWidth;
	CEditText m_Edit_DimensionText;
	CEditDecimal m_Edit_ExtensionLength;
	CStaticPreview m_Static_DimPreview;
	DECLARE_DYNAMIC(CDlgDimensionPropertiex)

public:
	CDlgDimensionPropertiex(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgDimensionPropertiex();
	void SetDimension(CCadDimension* pCD) { m_pDimension = pCD; }
	BOOL IsDirty() { return m_bDirty; }
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DIMENSION_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditTextHeight();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickedStaticBackgroundColor();
	afx_msg void OnClickedStaticDimPreviw();
	afx_msg void OnClickedStaticLineColor();
protected:
	afx_msg LRESULT OnDlgControlDirty(WPARAM wParam, LPARAM lParam);
};
