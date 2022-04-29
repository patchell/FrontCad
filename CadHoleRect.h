#pragma once

class CCadHoleRect:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RectHoleCount;
	inline static SRectHoleAttributes m_LastAttributes;
	inline static SRectHoleAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	inline static UINT m_PlaceCount = 0;
	CPen *m_pPenLine;
	SRectHoleAttributes m_Attrib;
	CDoublePoint m_Center;
public:
	CCadHoleRect();
	virtual ~CCadHoleRect();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	virtual int PointInObjectAndSelect(
		CDoublePoint p,
		CCadObject** ppSelList = 0,
		int index = 0,
		int n = 0,
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual void AddObject(CCadObject *pO);
	virtual void RemoveObject(CCadObject *pO);
	virtual CCadObject *GetHead(void);
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadHoleRect operator=(CCadHoleRect &v);
	virtual CPoint ScalePoint(CPoint p, int Scale, int ScaleDiv);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual CDoublePoint GetCenter();
	virtual void SetCenter(CDoublePoint Center) { m_Center = Center; }
	virtual CDoubleSize GetSize();
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	void IncrementPlaceCount() { m_PlaceCount++; }
	void ClearPlaceCount() { m_PlaceCount = 0;}
	BOOL HasObjectBeenPlaced() { return (m_PlaceCount > 0); }
	//---------------------------------------------
	// Object Attributes
	//---------------------------------------------
	virtual int EditProperties();
	void CopyAttributesTo(SRectHoleAttributes* pAttrb);
	void CopyAttributesFrom(SRectHoleAttributes* pAttrb);
	SRectHoleAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF color) { GetAttributes().m_colorLine = color; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth; }
	double GetHoleWidth() {return GetAttributes().m_HoleWidth; }
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();

	void SetHoleRadius(double d) { GetAttributes().m_HoleWidth = d; }
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SRectHoleAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

