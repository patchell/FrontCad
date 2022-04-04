
// FrontCad.h : main header file for the FrontCad application
//
#pragma once

//-------------------------------------------------------
// CFrontCadApp:
// See FrontCad.cpp for the implementation of this class
//-------------------------------------------------------

class CFrontCadApp : public CWinApp
{
	static CString csDrawState[16];
	static UINT Id;
	FILE* pConsol;
	CMainFrame* m_pMainFrame;
	CMoveObjects m_ClipBoard;
	//-----------------------------------
	// Default Attributes
	//-----------------------------------
	SArcAttributes m_ArcAttributes;
	SArcCenterAttributes m_ArcCenterAttributes;
	SArcAngleAttributes m_ArcAngleAttributes;
	SArrowAttributes m_ArrowAttributes;
	SBitmapAttributes m_BitmapAttributes;
	SCadDimAttributes m_DimensionAttributes;
	SEllipseAttributes m_EllipseAttributes;
	SLineAttributes m_LineAttributes;
	SOriginAttributes m_OriginAttributes;
	SPolyAttributes m_PolygonAttributes;
	SRectAttributes m_RectangleAttributes;
	SRoundedRectAttributes m_RoundedRectangleAttributes;
	STextAttributes m_TextAttributes;
	SRectHoleAttributes m_RectangularHoleAtrributes;
	SRoundHoleAttributes m_RoundHoleAttributes;
	SRndHole1FlatAttributes m_RoundHole1FlatAttributes;
	SRndHole2FlatAttributes m_RoundHole2FlatsAttributes;
	SGridAttributes m_GridAttributes;
	SRullerAttributes m_RulerAttributes;
	double Pi;
public:
	CFrontCadApp() noexcept;
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	void UpdateStatusBar(CString csS) { m_pMainFrame->SetStatusText(csS); }
	afx_msg void OnAppAbout();
	CMainFrame* GetMainFrame() { return m_pMainFrame; }
	void CutCopy(CCadObject* pObj, int mode);
	void CopyToClipBoard(CCadObject* pObjList);
	void CutToClipboard(CCadObject* pSelList);
	void SetClipboardRef(CDoublePoint p);
	int IsClipboardReadyToPaste(void) { return m_ClipBoard.IsReadyToPaste(); }
	CMoveObjects* GetClipBoard(void) { return &m_ClipBoard; }
	void LoadSettings();
	void SaveSettings();
	void* GetObjectDefaultAttributes(ObjectType ObjectType);
	//------------------------------------------
	// Save Document utility methods
	//------------------------------------------
	void NewLine(FILE* pO){fputc('\n', pO);	}
	void OpenParen(FILE* pO) { fputc('(', pO); }
	void CloseParen(FILE* pO) { fputc(')', pO); }
	void Comma(FILE* pO) { fputc(',', pO); }
	void SaveColor(FILE* pO, int Indent, COLORREF color, DocFileParseToken Token);
	void SaveString(FILE* pO, int Indent, CString& csStr, DocFileParseToken Token);
	void SaveDouble(FILE* pO, int Indent, double v, DocFileParseToken Token);
	void SaveUINT(FILE* pO, int Indent, UINT v, DocFileParseToken Token);
	afx_msg void OnFileOpen();
	//------------------------------------------
	// General utility methodes
	// -----------------------------------------
	// String Methodes
	//-----------------------------------------
	char* MkIndentString(char* pDest, int count, int c = ' ');
	char* ConvertCStringToChar(char* cpDest, CString& csSource);
	//-----------------------------------------
	// Math Methods
	//-----------------------------------------
	double GetPi() { return Pi; }
	double ArcTan(double X, double Y);
	double ArcSin(double X, double Y);
	BOOL IsPositive(double X) {
		BOOL rV = FALSE;
		if (X >= 0.0) rV = TRUE;
		return rV;
	}
	BOOL IsNegative(double X) {
		BOOL rV = FALSE;
		if (X < 0)
			rV = TRUE;
		return rV;
	}
	double StringToDouble(CString& csVal) {
		return _wtof(csVal.GetString());
	}
	double RoundToNearset(double v, double roundto);
	double RoundDownToNearest(double v, double roundupto);
	double RoundUpToNearest(double v, double roundupto);
	int RoundDoubleToInt(double dVal)
	{
		return int(dVal + 0.5 - (dVal < 0.0));
	}
	BOOL CheckAngle(double Start, double End, double Angle);
	BOOL IsEqToZero(double v, double tol) {
		BOOL rV = FALSE;

		if (v < tol && v > -tol)
			rV = TRUE;
		return rV;
	}
	double Snap(double v, double SnapTo) {
		double vMod;
		double rV;

		vMod = fmod(v, SnapTo);
		if (vMod < (SnapTo / 2.0) )
			rV = v - vMod;
		else
			rV = v + SnapTo - vMod;
		return rV;
	}
	//-----------------------------------------
	// Ellipse Methods
	//-----------------------------------------
	BOOL PointInEllipse(double A, double B, CDoublePoint Point, CDoublePoint Center);
	double Ellipse(double A, double B, CDoublePoint Point, CDoublePoint Center);
	BOOL TestEllipsePoint(double A, double B, CDoublePoint pt, CDoublePoint Center, double Tolerance);
	//-----------------------------------------
	// Polygon Methods
	//-----------------------------------------
	void ShiftDoublePointArray(CDoublePoint* pdptPoints, UINT nPoints, UINT Direction);
	CPoint* MakePolygonFromDoublePolygon(
		CPoint* dest, 
		CDoublePoint* src, 
		int n,
		CSize Offset,
		CScale Scale
	);
	BOOL GeneratePointInPolygon(CDoublePoint* Poly, int n, CDoublePoint& point);
	BOOL PtEnclosedInPolygon(CDoublePoint ptPoint, CDoublePoint* ptArray, UINT nVeticies);
	CWnd* GetCurrentView();
	UINT GetUniqueID() {
		return ++Id;
	}
	//-----------------------------------
	// Default Attributes Methods
	//-----------------------------------
	SArcAttributes* GetArcAttributes(){return &m_ArcAttributes;}
	SArcCenterAttributes* GetArcCenterAttributes(){ return &m_ArcCenterAttributes;}
	SArcAngleAttributes* GetArcAngleAttributes(){ return &m_ArcAngleAttributes;}
	SArrowAttributes* GetArrowAttributes(){ return &m_ArrowAttributes;}
	SBitmapAttributes* GetBitmapAttributes() { return &m_BitmapAttributes; }
	SCadDimAttributes* GetDimensionAttributes(){ return &m_DimensionAttributes;}
	SEllipseAttributes* GetEllipseAttributes(){ return &m_EllipseAttributes;}
	SLineAttributes* GetLineAttributes(){ return &m_LineAttributes;}
	SOriginAttributes* GetOriginAttributes(){ return &m_OriginAttributes;}
	SPolyAttributes* GetPolygonAttributes(){ return &m_PolygonAttributes;}
	SRectAttributes* GetRectangleAttributes(){ return &m_RectangleAttributes;}
	SRoundedRectAttributes* GetRoundedRectangleAttributes(){ return &m_RoundedRectangleAttributes;}
	STextAttributes* GetTextAttributes(){ return &m_TextAttributes;}
	SRectHoleAttributes* GetRectHoleAttributes(){ return &m_RectangularHoleAtrributes;}
	SRoundHoleAttributes* GetRoundHoleAttributes(){ return &m_RoundHoleAttributes;}
	SRndHole1FlatAttributes* GetRoundHole1FlatAttributes(){ return &m_RoundHole1FlatAttributes;}
	SRndHole2FlatAttributes* GetRoundHole2FlatAttributes(){ return &m_RoundHole2FlatsAttributes;}
	SGridAttributes* GetGridAttributes() { return &m_GridAttributes; }
	SRullerAttributes* GetRulerAttributes() { return &m_RulerAttributes; }
	//--------------------------------------Debug---------------------------
	CString& DrawStateToString(ObjectDrawState State)
	{
		return csDrawState[int(State)];
	}

protected:
	DECLARE_MESSAGE_MAP()
};

extern CFrontCadApp theApp;	//the only "real" global variable

