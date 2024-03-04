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
	virtual int GetDefaultToken() { return TOKEN_CIRCLE; }
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
	virtual CString& GetTypeString();
	virtual CString& GetObjDescription();
	virtual CDoubleSize GetSize();
	void CopyAttributesTo(SEllipseAttributes *pAttrb);
	void CopyAttributesFrom(SEllipseAttributes *pAttrb);
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
	//----------------------------------
	// attributes Methods
	//----------------------------------
	virtual int EditProperties();
	SEllipseAttributes& GetAttributes() { return m_Attrib; }
	SEllipseAttributes* GetPtrToAttributes() { return &m_Attrib; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	COLORREF GetFillColor() { return GetAttributes().m_colorFill; }
	void SetFillColor(COLORREF c) { GetAttributes().m_colorFill = c; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(int v) { GetAttributes().m_LineWidth = v; }
	int GetTransparent() { return GetAttributes().m_TransparentFill; }
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

