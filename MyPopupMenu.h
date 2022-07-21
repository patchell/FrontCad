#pragma once


// CMyPopupMenu

class CMyPopupMenu : public CWnd
{
	CCadObject** m_ppCadObjects;
	CFont m_Font;
	int m_HighlightedItem;
	CSize m_szCell;
	COLORREF m_colorBkGnd;
	COLORREF m_colorText;
	COLORREF m_colorBorder;
	COLORREF m_colorHighLight;
	int m_NumOfItems;
	DECLARE_DYNAMIC(CMyPopupMenu)
public:
	CMyPopupMenu();
	virtual ~CMyPopupMenu();

	BOOL Create(
		CWnd* pParent, 
		CPoint pointLocation, 
		CCadObject** pObjList,
		int n,
		COLORREF BkGnd,
		COLORREF Tesxt,
		COLORREF Border,
		COLORREF Highlight
		);
	void SetObjectList(CCadObject** ppObjs) { m_ppCadObjects = ppObjs; }
	//----------------------------------------------
	// Windows Message Handlers
	//----------------------------------------------
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	DECLARE_MESSAGE_MAP()
};


