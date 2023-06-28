#pragma once

class CCadEllipse:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_ElipseCount;
	inline static SEllipseAttributes m_LastAttributes;
	inline static SEllipseAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SEllipseAttributes m_Attrib;
public:
	CCadEllipse();
	virtual ~CCadEllipse();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual CLexer::Tokens GetDefaultToken() { return CLexer::Tokens::CIRCLE; }
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, CLexer::Tokens Token, int Indent = 0, int flags = 0);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject** ppSelList,
		int index,
		int n,
		UINT nKinds
	);
	virtual CString& GetTypeString(void);
	virtual CString& GetObjDescription();
	virtual CCadObject * CopyObject(void);
	virtual CDoubleSize GetSize();
	virtual CLexer::Tokens Parse(
		CLexer::Tokens Token, 
		CFileParser *pParser, 
		CLexer::Tokens TypeToken = CLexer::Tokens::CIRCLE
	);
	void CopyAttributesTo(SEllipseAttributes *pAttrb);
	void CopyAttributesFrom(SEllipseAttributes *pAttrb);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	//----------------------------------
	// attributes Methods
	//----------------------------------
	virtual int EditProperties();
	SEllipseAttributes& GetAttributes() { return m_Attrib; }
	SEllipseAttributes* GetPtrToAttributes() { return &m_Attrib; }
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor(void) { return GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(int v) { GetAttributes().m_LineWidth = v; }
	int GetTransparent(void) { return GetAttributes().m_TransparentFill; }
	void SetTransparent(int t) { GetAttributes().m_TransparentFill = t; }
	//--------------------------------------------
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static void RenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static SEllipseAttributes* GetLastAttributes() {
		return &m_LastAttributes;
	}
};

