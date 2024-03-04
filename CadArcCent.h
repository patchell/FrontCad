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
	BOOL Create(CCadObject* pParent, SubTypes type);
	virtual int GetDefaultToken() { return TOKEN_ARC_CENTER; }
	virtual CString& GetObjDescription();
	virtual void Move(CDoubleSize Diff);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	virtual BOOL IsEnclosedShapeIntrinsic() { return FALSE; }
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject** ppSelList,
		int index,
		int n,
		UINT nKinds
	);
	CString& GetTypeString();
	//--------------------------------------------
	// Parse (LoaD) and Save Methods
	//--------------------------------------------
	virtual int Parse(
		CFile* pcfInFile,
		int Token,	// Lookahead Token
		CFileParser* pParser,	// pointer to parser
		int TypeToken = TOKEN_DEFAULT // Token type to save object as
	);
	virtual void Save(
		CFile* pcfFile,
		int Indent,
		int flags
	);
	//--------------------------------------------
	// Copy Object Methods
	//--------------------------------------------
	virtual CCadObject* Copy();
	virtual void CopyAttributes(CCadObject* pToObj);
	//---------------------------------------------
	// Draw Object Methods
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//------------------------------------------
	// object geometry
	//------------------------------------------
	DOUBLEPOINT CalculateP2(CCadPoint* pPC, CCadPoint* pP1);
//	void DrawArc(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
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
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
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

