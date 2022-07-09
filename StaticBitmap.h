#pragma once


//-----------------------------------------------------
// CStaticBitmap window
//-----------------------------------------------------
//
class CStaticBitmap : public CStatic
{
	COLORREF m_Color;
	BOOL m_bDirty;
	// Operations
public:
	CStaticBitmap();
	virtual ~CStaticBitmap();
	void SetColor(COLORREF c){m_Color = c;Invalidate();}
	COLORREF GetColor(void){return m_Color;}
	BOOL IsDirty() { return m_bDirty; }
	// Generated message map Methods
protected:
	afx_msg void OnPaint();
	afx_msg void OnStnClicked();
	DECLARE_MESSAGE_MAP()
};
