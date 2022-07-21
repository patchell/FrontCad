#pragma once

class CCadHoleRect:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RectHoleCount;
	inline static SRectHoleAttributes m_LastAttributes;
	inline static SRectHoleAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	inline static UINT m_PlaceCount = 0;
	SRectHoleAttributes m_Attrib;
public:
	CCadHoleRect();
	virtual ~CCadHoleRect();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale);
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
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	void IncrementPlaceCount() { m_PlaceCount++; }
	void ClearPlaceCount() { m_PlaceCount = 0;}
	BOOL HasObjectBeenPlaced() { return (m_PlaceCount > 0); }
	//---------------------------------------------
	// Object Attributes
	//---------------------------------------------
	SRectHoleAttributes& GetAttributes() { return m_Attrib; }
	SRectHoleAttributes* GetPtrToAttributes() { return &m_Attrib; }
	virtual int EditProperties();
	void CopyAttributesTo(SRectHoleAttributes* pAttrb);
	void CopyAttributesFrom(SRectHoleAttributes* pAttrb);
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF color) { GetAttributes().m_colorLine = color; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double w) { GetAttributes().m_LineWidth; }
	double GetHoleWidth() {return GetAttributes().m_HoleWidth; }
	void SetHoleRadius(double d) { GetAttributes().m_HoleWidth = d; }
	//--------------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SRectHoleAttributes* GetLastAttributes() {
		return &m_LastAttributes;
	}
};

