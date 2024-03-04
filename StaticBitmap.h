#pragma once


//-----------------------------------------------------
// CStaticBitmap window
//-----------------------------------------------------
//
class CStaticBitmap : public CStatic
{
	COLORREF m_Color;
	// Operations
public:
	CStaticBitmap();
	virtual ~CStaticBitmap();
	void SetColor(COLORREF c){m_Color = c;Invalidate();}
	COLORREF GetColor(){return m_Color;}
	// Generated message map Methods
protected:
	afx_msg void OnPaint();
	afx_msg void OnStnClicked();
	DECLARE_MESSAGE_MAP()
};
