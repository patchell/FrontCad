#pragma once

class CCadRndRect:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_RoundedRectCount;
	inline static SRoundedRectAttributes m_LastAttributes;
	inline static SRoundedRectAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SRoundedRectAttributes m_Attrib;
public:
	CCadRndRect();
	virtual ~CCadRndRect();
	void Create();
	virtual BOOL Destroy(CCadObject* pDependentObjects);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT ULHC, CScale& Scale);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject ** ppSelList, 
		int index, 
		int n
	);
	virtual CString& GetTypeString(void);
	virtual CString& GetObjDescription();
	virtual CCadObject * CopyObject(void);
	virtual CDoubleSize GetSize();
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	void CopyAttributesTo(SRoundedRectAttributes *pAttrb);
	void CopyAttributesFrom(SRoundedRectAttributes*pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties();
	//------------------------------------------
	// Attribute Methods
	//-----------------------------------------
	SRoundedRectAttributes* GetPtrToAttributes() { return &m_Attrib; }
	SRoundedRectAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor() { return GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	UINT GetTransparent() { return GetAttributes().m_TransparentFill; }
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
	static SRoundedRectAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

