#pragma once


//-----------------------------------------------------
// CStaticBitmap window
//-----------------------------------------------------
//
class CStaticBitmap : public CStatic
{
	COLORREF m_Color;
	CWnd* m_pMaster;
	int m_Msg;
	int m_WMsg;
	// Operations
public:
	CStaticBitmap();
	virtual ~CStaticBitmap();
	void SetColor(COLORREF c){m_Color = c;Invalidate();}
	COLORREF GetColor(void){return m_Color;}

	void SetupMessaging(CWnd *pW,int Wmsg,int Smsg);
	void SendChange(void);
	void SetTheMaster(CWnd *W){m_pMaster = W;}
	void SetMsgValue(int m);
	void SetWMsgValue(int m){m_WMsg = m;}

	// Generated message map Methods
protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
