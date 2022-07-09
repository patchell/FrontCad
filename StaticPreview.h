#pragma once
//--------------------------------
// StaticPreview.h : header file
//--------------------------------

enum class StaticPreivewMode {
	STATICPREVIEW_OFFSET_CENTER,
	STATICPREVIEW_OFFSET_X10,
	STATICPREVIEW_OFFSET_Y10
};

/////////////////////////////////////////////////////////////////////////////
// CStaticPreview window

class CStaticPreview : public CStatic
{
	//----------------------------
	// Data
	//----------------------------
	CScale m_Scale;
	CSize m_Offset;
	int m_X, m_Y;
	StaticPreivewMode m_OffsetMode;
	CCadObject* m_pObj;	//pointer to object to display
public:
	CStaticPreview();
	virtual ~CStaticPreview();
	void EnableAxis(int x_en, int y_en) { m_X = x_en; m_Y = y_en; }
	void SetScale(CScale scale) { m_Scale = scale; }
	void SetOffsetMode(StaticPreivewMode m) { m_OffsetMode = m; }
	void Init();
	void AutoScale();
	void SetViewOffset(CPoint Point);
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void SetPreviewObject(CCadObject* pObj) { m_pObj = pObj; }
	CCadObject* GetPreviewObject() { return m_pObj; }
	CScale CalculateScale(CDoubleSize& ObjSize);
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

