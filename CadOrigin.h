#pragma once

class CCadOrigin:public CCadObject
{
	inline static BOOL m_AttributesDirty = FALSE;
	inline static int m_OriginCount;
	inline static SOriginAttributes m_LastAttributes;
	inline static SOriginAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CDoublePoint m_Origin;
	SOriginAttributes m_Attrib;
public:
	CCadOrigin();
	virtual ~CCadOrigin();
	void Create(double x, double y, CString& csName);
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CPoint p);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	virtual BOOL PointInObjectAndSelect(CPoint p, CSize Offest = CSize(0, 0), CCadObject ** ppSelList = 0, int index = 0, int n = 0, DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL);
	virtual CDoublePoint GetReference();
	virtual CString& GetTypeString(void);
	CCadOrigin operator=(CCadOrigin &v);
	virtual CPoint ScalePoint(CPoint p, int Scale, int ScaleDiv);
	virtual CCadObject * CopyObject(void);
	virtual void RenderEnable(int e);
	virtual CDoublePoint& GetCenter(CDoublePoint& pointCent) {
		pointCent = m_Origin;
		return pointCent;
	}
	virtual CDoubleSize GetSize();
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	//---------------------------------------------
	// Draw Object Methodes
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
	COLORREF GetLineColor(void) { return GetAttributes().m_colorLine; }
	void SetLineColor(COLORREF c) { GetAttributes().m_colorLine = c; }
	double GetLineWidth(void) { return GetAttributes().m_LineWidth; }
	void SetLineWidth(double v) { GetAttributes().m_LineWidth = v; }
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SOriginAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

