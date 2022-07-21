#pragma once

class CCadRect:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RectCount;
	inline static SRectAttributes m_LastAttributes;
	inline static SRectAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SRectAttributes m_Attrib;
public:
	CCadRect();
	virtual ~CCadRect();
	virtual BOOL Destroy(CCadObject* pDependentObject);
	void Create();
	//--------------- Set Points In Rectangle --------------
	void SetPoints(DOUBLEPOINT p1, DOUBLEPOINT p2, DOUBLEPOINT RotDef);
	void SetPoints(CDoubleSize sz, DOUBLEPOINT p1, DOUBLEPOINT rotation);
	CCadRect& SetSecondPoint(DOUBLEPOINT P2);
	CCadRect& SetPointsFromCenter(DOUBLEPOINT P1);
	CCadRect& SetPointsFromCenter(double Width, double Height);
	CCadRect& SetCenterPoint(DOUBLEPOINT newCenter);
	CRect ToCRect(DOUBLEPOINT ULHC, CScale& Scale);
	//------------------------------------------------------------------
	virtual void Move(CDoubleSize Diff);
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex, DocFileParseToken TypeToken);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	void DrawRect(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale, BOOL bFill);
	//------------------------------------------------------------------
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale);
	CCadPoint* GetRectPoints(CCadPoint** pointDest, int n);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject ** ppSelList, 
		int index, 
		int n,
		UINT nKinds
	);
	//--------------------------------------------
	virtual CString& GetTypeString(void);
	virtual CString& GetObjDescription();
	virtual CCadObject * CopyObject(void);
	virtual CDoubleSize GetSize();
	double GetWidth();
	void SetWidth(double width);
	double GetHeight();
	void SetHeight(double Height);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties();
	//------------------------------------------
	// Attribute Methods
	//-----------------------------------------
	void CopyAttributesTo(SRectAttributes* pAttrb);
	void CopyAttributesFrom(SRectAttributes* pAttrb);
	SRectAttributes* GetPtrToAttributes() { return &m_Attrib; }
	SRectAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor(void) { return GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	UINT GetTransparent(){ return GetAttributes().m_TransparentFill; }
	void SetTransparent(UINT t) { GetAttributes().m_TransparentFill = t; }
	//--------------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SRectAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

