#pragma once

class CCadElispe:public CCadObject
{
	inline static int m_DocCount;
	inline static SEllipseAttributes m_LastAttributes;
	inline static SEllipseAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CDoublePoint m_P1;
	CDoublePoint m_P2;
	CPen *	m_pPenLine;
	CBrush *m_pBrFill;
	SEllipseAttributes m_Attrib;
public:
	CCadElispe();
	CCadElispe(CCadElispe &e);
	virtual void OnCreate();
	virtual ~CCadElispe();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int PointInObjectAndSelect(
		CDoublePoint p,
		CCadObject** ppSelList = 0,
		int index = 0,
		int n = 0,
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual void AdjustReference(CDoubleSize Ref);
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadElispe operator=(CCadElispe &v);
	virtual CCadObject * CopyObject(void);
	virtual CDoublePoint& GetCenter(CDoublePoint& Center);
	virtual void ChangeCenter(CSize p);
	virtual CDoubleSize& GetSize(CDoubleSize& size);
	virtual void ChangeSize(CDoubleSize Sz);
	virtual DocFileParseToken Parse(
		DocFileParseToken Token, 
		CLexer *pLex, 
		DocFileParseToken TypeToken = DocFileParseToken::ELIPSE
	);
	void CopyAttributesTo(SEllipseAttributes *pAttrb);
	void CopyAttributesFrom(SEllipseAttributes *pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	BOOL PtInElipse(CDoublePoint p);
	//----------------------------------
	// attributes Methods
	//----------------------------------
	virtual int EditProperties();
	SEllipseAttributes& GetAttributes() { return m_Attrib; }
	SEllipseAttributes* GetPtrToAttributes() { return &m_Attrib; }
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor(void) { return GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(int v) { GetAttributes().m_LineWidth = v; }
	int GetTransparent(void) { return GetAttributes().m_TransparentFill; }
	void SetTransparent(int t) { GetAttributes().m_TransparentFill = t; }
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SEllipseAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

