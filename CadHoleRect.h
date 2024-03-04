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
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual int GetDefaultToken() { return TOKEN_HOLERECT; }
	virtual void Move(CDoubleSize Diff);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
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
	//-------------------- Static Methods ------------------------
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

