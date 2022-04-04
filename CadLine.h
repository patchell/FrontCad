#pragma once

class CCadLine :public CCadObject
{
	inline static int m_DocCount;
	inline static SLineAttributes m_LastAttributes;
	inline static SLineAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CDoubleLine m_Line;
	double m_Length;
	CDoubleRect m_doubleRectSelected;
	CPen *m_pLinePen;
	SLineAttributes m_Attrib;
	CDoubleRect& EncloseLineInRectangle(CDoubleRect& rect, int WidthInPixels);
public: public://public methodes
	CCadLine();
	CCadLine(CCadLine &line);
	virtual void OnCreate();
	virtual ~CCadLine();
	virtual void Move(CDoubleSize Diff);
	virtual void Save(FILE * pO, DocFileParseToken Token, int Indent = 0, int flags = 0);
	virtual void SetVertex(int v, CDoubleSize sz);
	virtual int GrabPoint(CDoublePoint p);
	virtual void Draw(CDC* pDC, MODE mode, CSize Offset = CSize(0, 0), CScale Scale = CScale(0.1, 0.1));
	virtual int PointInObjectAndSelect(
		CDoublePoint p, 
		CCadObject ** ppSelList = 0, 
		int index = 0, 
		int n = 0, 
		DrawingCheckSelectFlags flag = DrawingCheckSelectFlags::FLAG_ALL
	);
	virtual void AdjustReference(CDoubleSize Ref);
	virtual CString& GetTypeString(void);
	CCadLine operator=(CCadLine &v);
	virtual CCadObject * CopyObject(void);
	virtual void RenderEnable(int e);
	virtual CDoublePoint GetCenter();
	virtual CDoubleRect& GetRect(CDoubleRect& rect) { return EncloseLineInRectangle(rect, 10); };
	virtual void ChangeCenter(CDoubleSize p);
	virtual CDoubleSize GetSize();
	virtual void ChangeSize(CDoubleSize Sz);
	virtual DocFileParseToken Parse(DocFileParseToken Token, CLexer *pLex, DocFileParseToken TypeToken);
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
	virtual ObjectDrawState MouseMove(ObjectDrawState DrawState);
	virtual int EditProperties(void);
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
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
//--------------------------------------------------
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SLineAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
};

