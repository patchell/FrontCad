#pragma once

class CCadPolygon :	public CCadObject
{
	inline static int m_PolygonCount;
	inline static SPolyAttributes m_LastAttributes;
	inline static SPolyAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CDoublePolygon m_Poly;
	SPolyAttributes m_Attrib;
	UINT m_PolyID;
	CPen *m_pPenLine;
public:
	CCadPolygon();
	CCadPolygon(int nVertex);
	CCadPolygon(CCadPolygon &v);
	virtual ~CCadPolygon();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int PointInObjectAndSelect(
		CDoublePoint p, 
		CCadObject ** ppSelList = 0, 
		int index = 0, 
		int n = 0, 
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual CDoublePoint GetReference();
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadPolygon operator=(CCadPolygon &v);
	virtual CCadObject * CopyObject(void);
	virtual void Copy(CCadObject* pObj) {
		CCadPolygon* CadPoly = (CCadPolygon*)pObj;
		m_Poly.Copy(CadPoly->GetPoly());
		m_Attrib.CopyFrom(CadPoly->GetPtrToAttributes());
		m_PolyID = GETAPP.GetUniqueID();
	}
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	virtual void RenderEnable(int e);
	virtual CDoublePoint GetCenter();
	virtual CDoubleSize& GetSize(CDoubleSize& size);
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	//---------------------------------------
	// Other methodes
	//---------------------------------------
	CDoublePolygon* GetPoly() { return &m_Poly; }
	void Reset(void);
	BOOL CompareToLast(CDoublePoint nP);
	UINT DeleteLastPoint(void);
	int GetCount(void);
	BOOL AddPoint(CDoublePoint nP);
	BOOL PointEnclosed(CDoublePoint p);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual void Flip(CDoublePoint Pivot, Axis Direction);
	virtual int EditProperties();
	void SetPolySize(int sz) { m_Poly.SetSize( sz); }
	int GetPolySize(void) { return m_Poly.GetSize(); }
	void CopyAttributesFrom(SPolyAttributes *pPA);
	void CopyAttributesTo(SPolyAttributes *pA);
	void SetCurrentVertex(CDoublePoint p) { m_Poly.SetPoint(p); }
	//------------------------------------------
	// Attribute Methods
	//-----------------------------------------
	SPolyAttributes* GetPtrToAttributes() { return &m_Attrib; }
	SPolyAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor(void) { GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor(void) { GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	UINT GetTransparent(){ return GetAttributes().m_TransparentFill; }
	void SetTransparent(UINT t) { GetAttributes().m_TransparentFill = t; }
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SPolyAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

