#pragma once

/////////////////////////////////////////////////////////////////////////////
// CComboBoxFontWeight window

class CComboBoxFontWeight : public CComboBox
{
	int m_ObjMsg;
	int m_WMsg;
	CWnd* m_pW;
	// Construction
public:
	CComboBoxFontWeight();
	virtual ~CComboBoxFontWeight();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int GetFontWeight();
	void Init();
	void SetFontWeight(int w);
	void SetupMessaging(CWnd *pW,int Wmsg,int Smsg);
protected:
	afx_msg void OnSelchange();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

