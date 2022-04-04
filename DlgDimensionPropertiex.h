#pragma once
#include "afxdialogex.h"


// CDlgDimensionPropertiex dialog

class CDlgDimensionPropertiex : public CDialogEx
{
	CCadDimension* m_pDimension;
	CStaticBitmap m_Static_LineColor;
	CStaticBitmap m_Static_TextColor;
	CStaticBitmap m_Static_BackgroundColor;
	CEdit m_Edit_TextHeight;
	CEdit m_Edit_LineWidth;
	CEdit m_Edit_DimensionText;
	CEdit m_Edit_ExtensionLength;
	CStaticPreview m_Static_DimPreview;
	DECLARE_DYNAMIC(CDlgDimensionPropertiex)

public:
	CDlgDimensionPropertiex(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgDimensionPropertiex();
	void SetDimension(CCadDimension* pCD) { m_pDimension = pCD; }
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
};
