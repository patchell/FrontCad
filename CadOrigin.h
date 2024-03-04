#pragma once

class CCadOrigin:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_OriginCount;
	inline static SOriginAttributes m_LastAttributes;
	inline static SOriginAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	SOriginAttributes m_Attrib;
public:
	CCadOrigin();
	virtual ~CCadOrigin();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual int GetDefaultToken() { return TOKEN_ORG; }
	virtual void Move(CDoubleSize Diff);
	virtual DOUBLEPOINT GetCenterPoint();
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	virtual BOOL IsEnclosedShapeIntrinsic() { return TRUE; }
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual BOOL PointInThisObject(DOUBLEPOINT point);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject ** ppSelList,
		int index, 
		int n,
		UINT nKinds
	);
	virtual CString& GetTypeString();
	virtual CString& GetObjDescription();
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
	//-------------------------------------
	//attribute Methods
	//-------------------------------------
	SOriginAttributes& GetAttributes() { return m_Attrib; }
	SOriginAttributes* GetPtrToAttributes() { return &m_Attrib; }
	void CopyAttributesTo(SOriginAttributes* pAttrb);
	void CopyAttributesFrom(SOriginAttributes* pAttrb);
	virtual int EditProperties();
	void CreateThePen(MODE mode, CPen* pen, int Lw);
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	//--------------------------------------------
	static void RenderEnable(int e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static SOriginAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

