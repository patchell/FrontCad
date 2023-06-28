#pragma once

constexpr auto LINE_KIND_NORMAL = 0;
constexpr auto LINE_KIND_FIXED = 1;


class CCadLine :public CCadObject
{
protected:
	inline static BOOL m_SavedSnapEnable = FALSE;
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_LineCount;
	inline static SLineAttributes m_LastAttributes;
	inline static SLineAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	double m_Length;
	SLineAttributes m_Attrib;
public://public methodes
	CCadLine();
	CCadLine(CCadLine &line);
	virtual ~CCadLine();
	virtual BOOL Create(CCadObject* pParent, SubTypes type);
	virtual CLexer::Tokens GetDefaultToken() { return CLexer::Tokens::LINE; }
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, CLexer::Tokens Token, int Indent = 0, int flags = 0);
	virtual void Draw(CDC* pDC, MODE mode, DOUBLEPOINT& LLHC, CScale& Scale);
	virtual BOOL PointInThisObject(
		DOUBLEPOINT point
	);
	virtual int PointInObjectAndSelect(
		DOUBLEPOINT p,
		CCadObject* pExcludeObject,
		CCadObject ** ppSelList,
		int index, 
		int n,
		UINT nKinds
	);
	virtual CString& GetTypeString(void);
	virtual CString& GetObjDescription();
	virtual CCadObject * CopyObject(void);
	virtual CDoubleSize GetSize();
	virtual CLexer::Tokens Parse(
		CLexer::Tokens Token,	// Lookahead Token
		CFileParser* pParser,	// pointer to parser
		CLexer::Tokens TypeToken = CLexer::Tokens::DEFAULT // Token type to save object as
	);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	BOOL CalcFixedPoint(
		DOUBLEPOINT MousePos, 
		CCadPoint* pPtRtAgl, 
		CCadPoint* pPtP1, 
		CCadPoint* P2
	);
	virtual void ProcessZoom(CScale& InchesPerPixel);
	virtual int EditProperties(void);
	void CreateThePen(MODE mode, CPen* pen, int Lw);
	//-------------------------------------
	//attribute Methods
	//-------------------------------------
	SLineAttributes* GePtrTotAttributes() { return &m_Attrib; }
	SLineAttributes& GetAttributes() { return m_Attrib; }
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth; }
	void CopyAttributesTo(SLineAttributes* pAtr);
	void CopyAttributesFrom(SLineAttributes* pAtr);
	//--------------------------------------------------
	// Line Length
	//--------------------------------------------------
	double GetLength() { return m_Length; }
	void SetLength(double l) { m_Length = l; }
	//--------------------------------------------------
	// Static Functions
	//--------------------------------------------------
	static void RenderEnable(int e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	static BOOL NeedsAttributes() {
		return (m_AttributesGood == FALSE);
	}
	static void ClearNeedsAttributes() {
		m_AttributesGood = TRUE;
	}
	static SLineAttributes* GetLastAttributes()  {
		return &m_LastAttributes;
	}
};

