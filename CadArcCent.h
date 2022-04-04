#pragma once

class CCadArcCent :public CCadObject
{
	inline static UINT m_ArcCentCount;
	inline static SArcCenterAttributes m_LastAttributes;
	inline static SArcCenterAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CPen *m_pPenLine;
	//-------------------------------------
	// Things that define the Arc
	//-------------------------------------
	CDoublePoint m_pointCenter;
	CDoubleRect m_rectShape;
	CDoublePoint m_pointStart;
	CDoublePoint m_pointEnd;
	//-------------------------------------
	SArcCenterAttributes m_Attrb;
public:
	CCadArcCent();
	CCadArcCent(CCadArcCent &arc);
	virtual ~CCadArcCent();
	void Create(void);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1,0.1));
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
	CCadArcCent operator=(CCadArcCent &v);
	virtual CCadObject * CopyObject(void);
	virtual void Copy(CCadObject* pObj);
	virtual CDoublePoint GetCenter() { return m_pointCenter; }
	virtual void SetCenter(CDoublePoint cp) { m_pointCenter = cp; }
	virtual void ChangeCenter(CSize p);
	virtual CDoubleSize& GetSize(CDoubleSize& size);
	virtual void ChangeSize(CSize Sz);
	virtual DocFileParseToken Parse(
		DocFileParseToken Token, 
		CLexer *pLex, 
		DocFileParseToken TypeToken = DocFileParseToken::ARCCENTERED
	);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	BOOL PtInArc(CDoublePoint p);
	double Evaluate(CDoublePoint p);
	//------------------------------------------
	// Attribute Methods
	//-------------------------------------------
	virtual int EditProperties();
	void CopyAttributesTo(SArcCenterAttributes* pAttrb);
	void CopyAttributesFrom(SArcCenterAttributes* pAttrb);
	SArcCenterAttributes& GetAttributes() { return m_Attrb; }
	SArcCenterAttributes* GetAttributesPointer() { return &m_Attrb; }
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(int w) { GetAttributes().m_LineWidth; }
	CDoublePoint GetStartPoint(void) { return m_pointStart.ToCPoint(); }
	void SetStartPoint(CDoublePoint p) { m_pointStart = p; }
	CDoublePoint GetEndPoint(void) { return m_pointEnd.ToCPoint(); }
	void SetEndPoint(CDoublePoint p){m_pointEnd = p;}
	CDoubleRect& GetShapeRect(){ return m_rectShape; };
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();

	double GetA() {
		return GetShapeRect().GetWidth()/2.0;
	}
	double GetB() {
		return GetShapeRect().GetHeight()/2.0;
	}
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SArcCenterAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

