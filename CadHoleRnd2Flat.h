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
	void Create();
	virtual BOOL Destroy(CCadObject* pDependentObject);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale);
	void SolveIntersection();
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject** ppSelList,
		int index,
		int n
	);
	virtual CString& GetTypeString(void);
	virtual CString& GetObjDescription();
	virtual CCadObject * CopyObject(void);
	virtual CDoubleSize GetSize();
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	void CopyAttributesTo(SRndHole2FlatAttributes *pAttrb);
	void CopyAttributesFrom(SRndHole2FlatAttributes *pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//---------------------------------------------
	// Object Attributes
	//---------------------------------------------
	virtual int EditProperties();
	void CreateThePen(MODE mode, CPen* pen, int Lw);
	SRndHole2FlatAttributes& GetAttributes() { return m_Attrib; }
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

