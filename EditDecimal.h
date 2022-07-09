#pragma once
//------------------------------------------
// EditDecimal.h : header file
//------------------------------------------

//--------------------------------
// CEditDecimal window
//--------------------------------

class CEditDecimal : public CEdit
{
	BOOL m_Dirty;
	int m_DecimalPlaces;
	WMarrowSubMsg m_ObjMsg;
	WindowsMsg m_WMsg;
	CWnd *m_pW;
public:
	CEditDecimal();
	virtual ~CEditDecimal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetupMessaging(CWnd* pWdest, WindowsMsg WMsg, WMarrowSubMsg SubMsg);
	void SetDecimalPlaces(int p);
	int GetDecimalPlaces(void);
	void SetValue(int v);
	int GetValue(void);
	void SetDoubleValue(double v);
	double GetDoubleValue();
protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

