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
	SArcAttributes m_Attrb;
public:
	CCadArc();
	virtual ~CCadArc();
	void Create();
	virtual BOOL Destroy(CCadObject* pDependentObjects);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void Draw(CDC* pDC, MODE mode, CCadPoint ULHC, CScale Scale);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject** ppSelList,
		int index,
		int n,
		UINT nKinds
	);
	virtual CString& GetTypeString(void);
	virtual CString& GetObjDescription();
	virtual CCadObject * CopyObject(void);
	virtual CDoubleSize GetSize();
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
//	void DrawArc(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale);
	double CalcY(double x, double A, double B);
	BadDelta DeltaIsBad(CPoint P1, CPoint P2);
	CDoubleSize SlopeIsOneAt(double Asquared, double Bsquared);
	//----------------------------------------
	// Attribute Methods
	//----------------------------------------
	virtual int EditProperties(void);
	void CopyAttributesTo(SArcAttributes* pAttrb);
	void CopyAttributesFrom(SArcAttributes* pAttrb);
	SArcAttributes& GetAttributes() { return m_Attrb; }
	SArcAttributes* GetPtrToAttributes() { return &m_Attrb; }
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth = w; }
	//--------------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SArcAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

