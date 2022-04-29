#pragma once

constexpr auto DIMLINE_HORIZONTAL = 1;
constexpr auto DIMLINE_VERTICAL = 2;

class CCadDimension:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_DimensionCount;
	inline static SCadDimAttributes m_LastAttributes;
	inline static SCadDimAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CCadObject* m_pHead;
	CCadObject* m_pTail;
	UINT m_nTotalObjects;
	CDoublePoint m_P1, m_P2;
	SCadDimAttributes m_DimensionAttributes;
public:
	CCadDimension();
	virtual ~CCadDimension();
	virtual void OnCreate(void);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	virtual BOOL PointInObjectAndSelect(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList = 0, int index = 0, int n = 0, DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL);
	virtual CDoublePoint GetReference();
	virtual CDoubleRect& GetRect(CDoubleRect& rect);
	virtual CString& GetTypeString(void);
	CCadDimension operator=(CCadDimension &v);
	virtual CPoint ScalePoint(CPoint p, int Scale, int ScaleDiv);
	virtual CCadObject * CopyObject(void);
	virtual void SetRect(CRect & rect, CPoint P1, CPoint P2, CSize Lw);
	CDoublePoint GetCenter();
	virtual CDoubleSize GetSize();
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	void CopyAttributesFrom(SCadDimAttributes *pAttrib);
	void CopyAttributesTo(SCadDimAttributes* pAttrib);
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	int EditProperties();
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	void AddDimObjectToTail(CCadObject* pObj);
	void RemoveDimObject(CCadObject* pObj);
	CCadObject* GetHead() { return m_pHead; }
	void SetHead(CCadObject* pObj) { m_pHead = pObj; }
	CCadObject* GetTail() { return m_pTail; }
	void SetTail(CCadObject* pObj) { m_pTail = pObj; }
	//----------------------------------------------------
	void RenderEnable(int e) { CCadDimension::m_RenderEnable = e; }
	BOOL IsRenderDimensionEnabled() { return CCadDimension::m_RenderEnable; }
	//----------------------------------------------------
	SCadDimAttributes& GetAttributes() { return m_DimensionAttributes; }
	COLORREF GetLineColor() { return GetAttributes().m_colorLine; }
	COLORREF GetTextColor() { return GetAttributes().m_colorText; }
	COLORREF GetTextBkColor() { return GetAttributes().m_colorBKG; }
	double GetLineWidth() { return GetAttributes().m_LineWidth; }
	double GetTextHeight() { return GetAttributes().m_TextHeight; }
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SCadDimAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

class CDimLine :public CCadLine
{
	double m_Slope;
	double m_yIntercept;
	UINT m_LineType;
public:
	CDimLine() {
		m_Slope = 0.0;
		m_yIntercept = 0;
		m_LineType = 0;
	};
	void Create(CDoublePoint P1, CDoublePoint P2, UINT LineType);
	virtual void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	double Slope(CDoublePoint P1, CDoublePoint P2);
	double OrthogonalSlope(CDoublePoint P1, CDoublePoint P2);
	UINT LineIs(CDoublePoint P1, CDoublePoint P2);
	double YIntercept(double m, CDoublePoint p);
	//---------------------------------------------
	void SetSlope(double m) { m_Slope = m; }
	double GetSlope() { return m_Slope; }
	void SetYIntercept(double b) { m_yIntercept = b; }
	double GetYIntercept() { return m_yIntercept; }
	void SetLineType(UINT lt) { m_LineType = lt; }
	UINT GetLineType() { return m_LineType; }
};

