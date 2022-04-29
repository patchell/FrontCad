#pragma once

class CCadArc :public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_ArcCount;
	inline static SArcAttributes m_LastAttributes;
	inline static SArcAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	//----------- Atributes ------------------
	CDoubleRect m_rectShape;
	CDoublePoint m_pointStart;
	CDoublePoint m_pointEnd;
	SArcAttributes m_Attrb;
	CPen *m_pPenLine;
public:
	CCadArc();
	virtual ~CCadArc();
	void Create(CRect Shape, CPoint ptStart, CPoint ptEnd);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CDoublePoint ULHC, CScale Scale);
	virtual int PointInObjectAndSelect(
		CDoublePoint p,
		CCadObject** ppSelList = 0,
		int index = 0,
		int n = 0,
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadArc operator=(CCadArc &v);
	virtual CCadObject * CopyObject(void);
	virtual CDoublePoint& GetCenter(CDoublePoint& Center);
	virtual CDoubleSize& GetSize(CDoubleSize& size);
	virtual DocFileParseToken Parse(
		DocFileParseToken Token, 
		CLexer *pLex, 
		DocFileParseToken TypeToken = DocFileParseToken::ARC
	);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties(void);
	BOOL PtInArc(CDoublePoint p);
	//----------------------------------------
	// Attribute Methods
	//----------------------------------------
	void CopyAttributesTo(SArcAttributes* pAttrb);
	void CopyAttributesFrom(SArcAttributes* pAttrb);
	SArcAttributes& GetAttributes() { return m_Attrb; }
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth = w; }
	CDoublePoint GetStartPoint(void) { return m_pointStart; }
	void SetStartPoint(CDoublePoint p) { m_pointStart = p; }
	CDoublePoint GetEndPoint(void) { return m_pointEnd; }
	void SetEndPoint(CDoublePoint p) { m_pointEnd = p; };
	virtual BOOL NeedsAttributes(); 
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SArcAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

