#pragma once

enum class Intercept {
	LEFTUPPER,
	LEFTLOWER,
	RIGHTUPPER,
	RIGHTLOWER
};

class CCadHoleRnd2Flat:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RoundHole2FlatCount;
	inline static SRndHole2FlatAttributes m_LastAttributes;
	inline static SRndHole2FlatAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SRndHole2FlatAttributes m_Attrib;
public:
	CCadHoleRnd2Flat();
	virtual ~CCadHoleRnd2Flat();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual int GetDefaultToken() { return TOKEN_HOLE_2FLAT; }
	virtual void Move(CDoubleSize Diff);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	void SolveIntersection();
	virtual BOOL IsEnclosedShapeIntrinsic() { return TRUE; }
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
	virtual CString& GetTypeString();
	virtual CString& GetObjDescription();
	virtual CDoubleSize GetSize();
	void CopyAttributesTo(SRndHole2FlatAttributes *pAttrb);
	void CopyAttributesFrom(SRndHole2FlatAttributes *pAttrb);
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
	//---------------------------------------------
	// Object Attributes
	//---------------------------------------------
	virtual int EditProperties();
	void CreateThePen(MODE mode, CPen* pen, int Lw);
	SRndHole2FlatAttributes& GetAttributes() { return m_Attrib; }
	SRndHole2FlatAttributes* GetPtrToAttributes() { return &m_Attrib; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF color) { GetAttributes().m_colorLine = color; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth; }
	double GetHoleRadius() { return GetAttributes().m_HoleRadius; }
	void SetHoleRadius(double d) { GetAttributes().m_HoleRadius = d; }
	double GetDistanceFromCenterToFlat() { return GetAttributes().m_FlatDistanceFromCenter; }
	void SetDistanceFromCenterToFlat(double dist) { GetAttributes().m_FlatDistanceFromCenter = dist; }
	//--------------------------------------------
	static void RenderEnable(int e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static SRndHole2FlatAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

