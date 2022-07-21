#pragma once

class CCadArcCent :public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static UINT m_ArcCentCount;
	inline static SArcCenterAttributes m_LastAttributes;
	inline static SArcCenterAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SArcCenterAttributes m_Attrb;
public:
	CCadArcCent();
	virtual ~CCadArcCent();
	void Create(void);
	virtual BOOL Destroy(CCadObject* pDepndentObjects);
	virtual CString& GetObjDescription();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE* pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void Draw(CDC* pDC, MODE mode, CCadPoint ULHC, CScale Scale);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject** ppSelList,
		int index,
		int n,
		UINT nKinds
	);
	CString& GetTypeString(void);
	virtual CCadObject* CopyObject(void);
	virtual DocFileParseToken Parse(
		DocFileParseToken Token,
		CLexer* pLex,
		DocFileParseToken TypeToken = DocFileParseToken::ARCCENTERED
	);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//------------------------------------------
	// object geometry
	//------------------------------------------
	DOUBLEPOINT CalculateP2(CCadPoint* pPC, CCadPoint* pP1);
//	void DrawArc(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale);
//	double CalcY(double x, double A, double B);
//	BadDelta DeltaIsBad(CPoint P1, CPoint P2);
//	CDoubleSize SlopeIsOneAt(double Asquared, double Bsquared);
	//------------------------------------------
	// Attribute Methods
	//-------------------------------------------
	virtual int EditProperties();
	void CopyAttributesTo(SArcCenterAttributes* pAttrb);
	void CopyAttributesFrom(SArcCenterAttributes* pAttrb);
	SArcCenterAttributes& GetAttributes() { return m_Attrb; }
	SArcCenterAttributes* GetPtrToAttributes() { return &m_Attrb; }
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(int w) { GetAttributes().m_LineWidth; }
	//--------------------------------------------
	//  Static Methods
	//--------------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SArcCenterAttributes* GetLastAttributes() {
		return &m_LastAttributes;
	}
};

