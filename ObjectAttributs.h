#pragma once

class CLexer;
//------------- Arc Attributes -----------------

struct SArcAttributes
{
	double m_LineWidth;
	COLORREF m_colorLine;
	SArcAttributes() {
		m_LineWidth = 0.01;	//default line width
		m_colorLine = RGB(0, 0, 0);	//default color
	}
	void CopyFrom(SArcAttributes* pAttributes) {
		m_LineWidth = pAttributes->m_LineWidth;
		m_colorLine = pAttributes->m_colorLine;
	}
	void CopyTo(SArcAttributes* pAttributes) {
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_colorLine = m_colorLine;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SArcAttributes* pAttrb);
	static void SaveSettings(SArcAttributes* pAttrb);
};

// Arc Centered Attributes

struct SArcCenterAttributes
{
	double m_LineWidth;
	COLORREF m_colorLine;
	SArcCenterAttributes() {
		m_LineWidth = 0.01;	//default line width
		m_colorLine = RGB(0, 0, 0);	//default color
	}
	void CopyFrom(SArcCenterAttributes* pAttributes) {
		m_LineWidth = pAttributes->m_LineWidth;
		m_colorLine = pAttributes->m_colorLine;
	}
	void CopyTo(SArcCenterAttributes* pAttributes) {
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_colorLine = m_colorLine;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SArcCenterAttributes* pAttrb);
	static void SaveSettings(SArcCenterAttributes* pAttrb);
};

// Arc Angle Attributes

struct SArcAngleAttributes
{
	double m_LineWidth;
	COLORREF m_colorLine;
	SArcAngleAttributes() {
		m_LineWidth = 0.01;	//default line width
		m_colorLine = RGB(0, 0, 0);	//default color
	}
	void CopyFrom(SArcAngleAttributes* pAttributes) {
		m_LineWidth = pAttributes->m_LineWidth;
		m_colorLine = pAttributes->m_colorLine;
	}
	void CopyTo(SArcAngleAttributes* pAttributes) {
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_colorLine = m_colorLine;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SArcAngleAttributes* pAttrb);
	static void SaveSettings(SArcAngleAttributes* pAttrb);
};

//---------- Arrow Attributes ---------

struct SArrowAttributes {
	COLORREF m_colorLine;
	COLORREF m_colorFill;
	double m_LineWidth;
	CDoublePolygon m_PolyPointsShape;  /*m_ArrowPoly*/;;
	SArrowAttributes() {
		m_colorLine = RGB(255, 0, 0);
		m_colorFill = RGB(0, 0, 255);
		m_LineWidth = 0.01;
		m_PolyPointsShape.SetSize(4);
		m_PolyPointsShape.AddPoint(CDoublePoint(0.0, 0.0));
		m_PolyPointsShape.AddPoint(CDoublePoint(0.4, 0.1));
		m_PolyPointsShape.AddPoint(CDoublePoint(0.3, 0.0));
		m_PolyPointsShape.AddPoint(CDoublePoint(0.4, -0.1));
	}
	~SArrowAttributes() {}
	void CopyFrom(SArrowAttributes* pAttributes) {
		m_LineWidth = pAttributes->m_LineWidth;
		m_colorLine = pAttributes->m_colorLine;
		m_colorFill = pAttributes->m_colorFill;
		for(int i = 0;i<4;++i)
			m_PolyPointsShape.SetPoint(i, pAttributes->m_PolyPointsShape.GetPoint(i));
	}
	void CopyTo(SArrowAttributes* pAttributes) {
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_colorFill = m_colorFill;
		for (int i = 0; i < 4; ++i)
			pAttributes->m_PolyPointsShape.SetPoint(i, m_PolyPointsShape.GetPoint(i));
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, DocFileParseToken TypeToken, int Indent, int flags) {

	}
	static void LoadSettings(SArrowAttributes* pAttrb);
	static void SaveSettings(SArrowAttributes* pAttrb);
};

//------------ Bitmap Attributes -------------------------

struct SBitmapAttributes {
	BOOL m_MaintainAspectRatio;
	CDoubleSize m_BitmapSize;
	SBitmapAttributes() {
		m_MaintainAspectRatio = TRUE;
	}
	void CopyFrom(SBitmapAttributes* pAttrib) {
		m_MaintainAspectRatio = pAttrib->m_MaintainAspectRatio;
		m_BitmapSize = pAttrib->m_BitmapSize;
	}
	void CopyTo(SBitmapAttributes* pAttrib) {
		pAttrib->m_MaintainAspectRatio = m_MaintainAspectRatio;
		pAttrib->m_BitmapSize = m_BitmapSize;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, DocFileParseToken TypeToken, int Indent, int flags);
	static void LoadSettings(SBitmapAttributes* pAttrb);
	static void SaveSettings(SBitmapAttributes* pAttrb);
};
//----------- Dimension Attributes -----------------------
struct SCadDimAttributes {
	inline static CDoublePoint m_aDefaultArrow[4] = {
		{0.0,0.0},
		{0.20,0.05},
		{0.175,0.0},
		{0.20,-0.05},
	};
	COLORREF m_colorLine;
	COLORREF m_colorText;
	COLORREF m_colorBKG;	//text backhround color
	double m_TextHeight;
	double m_LineWidth;
	double m_ExtLineGap;
	double m_DimLineInset;
	CDoublePoint m_aArrowShape[4];
	SCadDimAttributes() {
		m_colorLine = RGB(0,0,0);
		m_colorText = RGB(0,0,0);
		m_TextHeight = 0.06;
		m_colorBKG = RGB(255,255,255);
		m_LineWidth = 0.010;
		m_ExtLineGap = 0.075;
		m_DimLineInset = 0.125;
		m_aArrowShape[0] = m_aDefaultArrow[0];
		m_aArrowShape[1] = m_aDefaultArrow[1];
		m_aArrowShape[2] = m_aDefaultArrow[2];
		m_aArrowShape[3] = m_aDefaultArrow[3];
	}
	void CopyFrom(SCadDimAttributes* pAttributes) {
		m_LineWidth = pAttributes->m_LineWidth;
		m_colorLine = pAttributes->m_colorLine;
		m_colorText = pAttributes->m_colorText;
		m_TextHeight = pAttributes->m_TextHeight;
		m_colorBKG = pAttributes->m_colorBKG;
		m_ExtLineGap = pAttributes->m_ExtLineGap;
		m_DimLineInset = pAttributes->m_DimLineInset;
		memcpy_s(
			m_aArrowShape, 
			sizeof(CDoublePoint) * 4, 
			pAttributes->m_aArrowShape, 
			sizeof(CDoublePoint) * 4
		);
	}
	void CopyTo(SCadDimAttributes* pAttributes) {
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_colorText = m_colorText;
		pAttributes->m_TextHeight = m_TextHeight;
		pAttributes->m_colorBKG = m_colorBKG;
		pAttributes->m_ExtLineGap = m_ExtLineGap;
		pAttributes->m_DimLineInset = m_DimLineInset;
		memcpy_s(
			pAttributes->m_aArrowShape,
			sizeof(CDoublePoint) * 4,
			m_aArrowShape,
			sizeof(CDoublePoint) * 4
		);
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, DocFileParseToken TypeToken, int Indent, int flags) {

	}
	static void LoadSettings(SCadDimAttributes* pAttrb);
	static void SaveSettings(SCadDimAttributes* pAttrb);
};

//------------ Elipse Attributes -----------------------

struct SEllipseAttributes {

	COLORREF m_colorLine;
	COLORREF m_colorFill;
	double m_LineWidth;
	UINT m_TransparentFill;
	SEllipseAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_colorFill = RGB(255, 0, 0);
		m_LineWidth = 0.01;
		m_TransparentFill = 0;
	}
	void CopyFrom(SEllipseAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_colorFill = pAttributes->m_colorFill;
		m_LineWidth = pAttributes->m_LineWidth;
		m_TransparentFill = pAttributes->m_TransparentFill;
	}
	void CopyTo(SEllipseAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_colorFill = m_colorFill;
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_TransparentFill = m_TransparentFill;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SEllipseAttributes* pAttrb);
	static void SaveSettings(SEllipseAttributes* pAttrb);
};

//--------------  Line Attributes ---------------------

struct SLineAttributes {
	COLORREF m_colorLine;
	double m_LineWidth;
	BOOL m_LockLength;
	SLineAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_LineWidth = 0.01;
		m_LockLength = FALSE;
	}
	void CopyFrom(SLineAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_LineWidth = pAttributes->m_LineWidth;
		m_LockLength = pAttributes->m_LockLength;
	}
	void CopyTo(SLineAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_LockLength = m_LockLength;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SLineAttributes* pAttrib);
	static void SaveSettings(SLineAttributes* pAttrib);
};

//---------------  Origin Attributes  -------------------------

struct SOriginAttributes {
	COLORREF m_colorLine;
	double m_LineWidth;
	SOriginAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_LineWidth = 0.01;
	}
	void CopyFrom(SOriginAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_LineWidth = pAttributes->m_LineWidth;
	}
	void CopyTo(SOriginAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_LineWidth = m_LineWidth;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SOriginAttributes* pAttrb);
	static void SaveSettings(SOriginAttributes* pAttrb);
};

//---------- Polygon Attributes -------------------

struct SPolyAttributes {
	COLORREF m_colorLine;
	COLORREF m_colorFill;
	double m_LineWidth;
	UINT m_TransparentFill;
	SPolyAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_colorFill = RGB(255, 0, 0);
		m_LineWidth = 0.01;
		m_TransparentFill = 0;
	}
	void CopyFrom(SPolyAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_colorFill = pAttributes->m_colorFill;
		m_LineWidth = pAttributes->m_LineWidth;
		m_TransparentFill = pAttributes->m_TransparentFill;
	}
	void CopyTo(SPolyAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_colorFill = m_colorFill;
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_TransparentFill = m_TransparentFill;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SPolyAttributes* pAttrib);
	static void SaveSettings(SPolyAttributes* pAttrib);
};

//--------- Rectangle Attributes ------------------
struct SRectAttributes {
	COLORREF m_colorLine;
	COLORREF m_colorFill;
	double m_LineWidth;
	UINT m_TransparentFill;
	SRectAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_colorFill = RGB(255, 0, 0);
		m_LineWidth = 0.01;
		m_TransparentFill = 0;
	}
	void CopyFrom(SRectAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_colorFill = pAttributes->m_colorFill;
		m_LineWidth = pAttributes->m_LineWidth;
		m_TransparentFill = pAttributes->m_TransparentFill;
	}
	void CopyTo(SRectAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_colorFill = m_colorFill;
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_TransparentFill = m_TransparentFill;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SRectAttributes* pAttrb);
	static void SaveSettings(SRectAttributes* pAttrb);
};

//----- Rounded Rectangle Attributes --------------

struct SRoundedRectAttributes {
	COLORREF m_colorLine;
	COLORREF m_colorFill;
	double m_LineWidth;
	UINT m_TransparentFill;
	CDoubleSize m_dszCornerRadius;
	SRoundedRectAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_colorFill = RGB(255, 0, 0);
		m_LineWidth = 0.01;
		m_TransparentFill = 0;
		m_dszCornerRadius = CDoubleSize(0.1, 0.1);
	}
	void CopyFrom(SRoundedRectAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_colorFill = pAttributes->m_colorFill;
		m_LineWidth = pAttributes->m_LineWidth;
		m_TransparentFill = pAttributes->m_TransparentFill;
		m_dszCornerRadius = pAttributes->m_dszCornerRadius;
	}
	void CopyTo(SRoundedRectAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_colorFill = m_colorFill;
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_TransparentFill = m_TransparentFill;
		pAttributes->m_dszCornerRadius = m_dszCornerRadius;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SRoundedRectAttributes* pAttrb);
	static void SaveSettings(SRoundedRectAttributes* pAttrb);
};
//----------- Text Attributes ---------------------

struct STextAttributes {
	CString m_csFontName;
	COLORREF m_colorText;
	COLORREF m_colorBK;
	double m_Angle;
	double m_fontHeight;
	double m_fontWidth;
	UINT m_Format;
	UINT m_Transparent;
	UINT m_fontWeight;
	STextAttributes() {
		m_csFontName = CString(_T("ARIAL"));
		m_colorText = RGB(255,255,255);
		m_colorBK = RGB(0,0,0);
		m_Angle = 0.0;
		m_fontHeight = 0.060;
		m_fontWidth = 0.0;
		m_Format = DT_BOTTOM | DT_SINGLELINE;
		m_Transparent = 1;
		m_fontWeight = FW_DEMIBOLD;
	}
	void CopyFrom(STextAttributes* pAttributes) {
		m_csFontName = pAttributes->m_csFontName;
		m_colorText = pAttributes->m_colorText;
		m_colorBK = pAttributes->m_colorBK;
		m_Angle = pAttributes->m_Angle;
		m_fontHeight = pAttributes->m_fontHeight;
		m_fontWidth = pAttributes->m_fontWidth;
		m_Format = pAttributes->m_Format;
		m_Transparent = pAttributes->m_Transparent;
		m_fontWeight = pAttributes->m_fontWeight;
	}
	void CopyTo(STextAttributes* pAttributes) {
		pAttributes->m_csFontName = m_csFontName;
		pAttributes->m_colorText = m_colorText;
		pAttributes->m_colorBK = m_colorBK;
		pAttributes->m_Angle = m_Angle;
		pAttributes->m_fontHeight = m_fontHeight;
		pAttributes->m_fontWidth = m_fontWidth;
		pAttributes->m_Format = m_Format;
		pAttributes->m_Transparent = m_Transparent;
		pAttributes->m_fontWeight = m_fontWeight;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags);
	static void LoadSettings(STextAttributes* pAttrb);
	static void SaveSettings(STextAttributes* pAttrb);
};

//----------- Rectangular Hole ------------------------

struct SRectHoleAttributes {
	COLORREF m_colorLine;
	double m_LineWidth;
	double m_HoleHeight;
	double m_HoleWidth; 
	SRectHoleAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_LineWidth = 0.01;
		m_HoleHeight = 0.25;
		m_HoleWidth = 0.2500;
	}
	void CopyFrom(SRectHoleAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_LineWidth = pAttributes->m_LineWidth;
		m_HoleHeight = pAttributes->m_HoleHeight;
		m_HoleWidth = pAttributes->m_HoleWidth;
	}
	void CopyTo(SRectHoleAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_HoleHeight = m_HoleHeight;
		pAttributes->m_HoleWidth = m_HoleWidth;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SRectHoleAttributes* pAttrb);
	static void SaveSettings(SRectHoleAttributes* pAttrb);
};
//-------------- Round Hole ---------------------------

struct SRoundHoleAttributes {
	COLORREF m_colorLine;
	double m_LineWidth;
	double m_HoleRadius;  
	SRoundHoleAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_LineWidth = 0.01;
		m_HoleRadius = 0.2500;
	}
	void CopyFrom(SRoundHoleAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_LineWidth = pAttributes->m_LineWidth;
		m_HoleRadius = pAttributes->m_HoleRadius;
	}
	void CopyTo(SRoundHoleAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_HoleRadius = m_HoleRadius;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SRoundHoleAttributes* pAttrb);
	static void SaveSettings(SRoundHoleAttributes* pAttrb);
};
//-------- Round Hole with 1 flat Hole ---------------------

struct SRndHole1FlatAttributes {
	COLORREF m_colorLine;
	double m_LineWidth;
	double m_HoleRadius; 
	double m_FlatDistanceFromCenter;
	SRndHole1FlatAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_LineWidth = 0.01;
		m_HoleRadius = 0.2500;
		m_FlatDistanceFromCenter = 0.1000;
	}
	void CopyFrom(SRndHole1FlatAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_LineWidth = pAttributes->m_LineWidth;
		m_HoleRadius = pAttributes->m_HoleRadius;
		m_FlatDistanceFromCenter = pAttributes->m_FlatDistanceFromCenter;
	}
	void CopyTo(SRndHole1FlatAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_HoleRadius = m_HoleRadius;
		pAttributes->m_FlatDistanceFromCenter = m_FlatDistanceFromCenter;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SRndHole1FlatAttributes* pAttrb);
	static void SaveSettings(SRndHole1FlatAttributes* pAttrb);
};

//---------Round Hole with 2 flats Hole --------------------

struct SRndHole2FlatAttributes
{
	COLORREF m_colorLine;
	double m_LineWidth;
	double m_HoleRadius; //increments of 0.0001 inches 
	double m_FlatDistanceFromCenter;
	SRndHole2FlatAttributes() {
		m_colorLine = RGB(0, 0, 0);
		m_LineWidth = 0.01;
		m_HoleRadius = 0.2500;
		m_FlatDistanceFromCenter = 0.1000;
	}
	void CopyFrom(SRndHole2FlatAttributes* pAttributes) {
		m_colorLine = pAttributes->m_colorLine;
		m_LineWidth = pAttributes->m_LineWidth;
		m_HoleRadius = pAttributes->m_HoleRadius;
		m_FlatDistanceFromCenter = pAttributes->m_FlatDistanceFromCenter;
	}
	void CopyTo(SRndHole2FlatAttributes* pAttributes) {
		pAttributes->m_colorLine = m_colorLine;
		pAttributes->m_LineWidth = m_LineWidth;
		pAttributes->m_HoleRadius = m_HoleRadius;
		pAttributes->m_FlatDistanceFromCenter = m_FlatDistanceFromCenter;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SRndHole2FlatAttributes* pAttrb);
	static void SaveSettings(SRndHole2FlatAttributes* pAttrb);
};

struct SGridAttributes {
	COLORREF m_colorSnapLine;
	COLORREF m_colorHalfGrid;
	COLORREF m_colorMajorLine;
	int m_SnapLineWidth;
	int m_MajorLineWidth;
	BOOL m_Snap_Lines_Dots;	//TRUE => Lines  FALSE => Dots
	BOOL m_Major_Lines_Dots; //TRUE => Lines  FALSE => Dots
	CDoubleSize m_SnapGridSpacing;
	CDoubleSize m_MajorGridSpacing;
	SGridAttributes() {
		m_colorSnapLine = RGB(255, 0, 0);
		m_colorHalfGrid = RGB(0, 255, 0);
		m_colorMajorLine = RGB(0, 128, 255);
		m_SnapLineWidth = 1;
		m_MajorLineWidth = 3;
		m_Snap_Lines_Dots = FALSE;	//dots
		m_Major_Lines_Dots = TRUE;	//lines
		m_SnapGridSpacing = { 0.125,0.125 };
		m_MajorGridSpacing = { 1.0,1.0 };
	}
	void CopyFrom(SGridAttributes* pAttributes) {
		m_colorSnapLine = pAttributes->m_colorSnapLine;
		m_colorHalfGrid = pAttributes->m_colorHalfGrid;
		m_colorMajorLine = pAttributes->m_colorMajorLine;
		m_SnapLineWidth = pAttributes->m_SnapLineWidth;
		m_MajorLineWidth = pAttributes->m_MajorLineWidth;
		m_Snap_Lines_Dots = pAttributes->m_Snap_Lines_Dots;
		m_Major_Lines_Dots = pAttributes->m_Major_Lines_Dots;
		m_SnapGridSpacing = pAttributes->m_SnapGridSpacing;
		m_MajorGridSpacing = pAttributes->m_MajorGridSpacing;
	}
	void CopyTo(SGridAttributes* pAttributes) {
		pAttributes->m_colorSnapLine = m_colorSnapLine;
		pAttributes->m_colorHalfGrid = m_colorHalfGrid;
		pAttributes->m_colorMajorLine = m_colorMajorLine;
		pAttributes->m_SnapLineWidth = m_SnapLineWidth;
		pAttributes->m_MajorLineWidth = m_MajorLineWidth;
		pAttributes->m_Snap_Lines_Dots = m_Snap_Lines_Dots;
		pAttributes->m_Major_Lines_Dots = m_Major_Lines_Dots;
		pAttributes->m_SnapGridSpacing = m_SnapGridSpacing;
		pAttributes->m_MajorGridSpacing = m_MajorGridSpacing;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SGridAttributes* pAttrb);
	static void SaveSettings(SGridAttributes* pAttrb);
};

//------------------ Rulers ----------------------
struct SRullerAttributes {
	int m_RulerSize;
	//---------------------------
	// Font
	//---------------------------
	CString m_csFontName;	// font name of major tick labels
	int m_FontSize;
	COLORREF m_colorBk;		// Ruler Background color
	COLORREF m_colorText;
	//---------------------------
	// Major Tick
	//---------------------------
	int m_MajTickLength;//length of the major ticks
	COLORREF m_colorMajorTick;
	//---------------------------
	// Half Tick
	//---------------------------
	int m_HalfTickLength;
	COLORREF m_colorHalfTick;
	//---------------------------
	// Snap Tick
	//---------------------------
	COLORREF m_colorTickMark;
	int m_TickLength;	//length of run of the mill ticks
	//---------------------------
	// Misc
	//---------------------------
	COLORREF m_colorCursor;
	COLORREF m_colorCorner;
	SRullerAttributes() {
		m_RulerSize = RULER_SIZE;
		m_TickLength = RULER_SNAP_TICK_LEN;
		m_MajTickLength = RULER_MAJOR_TICK_LEN;
		m_HalfTickLength = RULER_HALF_TICK_LEN;
		m_csFontName = _T("ARIAL");
		m_FontSize = RULER_FONT_HEIGHT;
		m_colorBk = RGB(0,16,32);		// Ruler Background color
		m_colorMajorTick = RGB(32, 192, 255);
		m_colorHalfTick = RGB(0, 255, 0);
		m_colorTickMark = RGB(255, 192, 32);
		m_colorCursor = RGB(255, 255, 32);
		m_colorText = RGB(255, 255, 255);
		m_colorCorner = RGB(0, 255, 0);
	}
	void CopyFrom(SRullerAttributes* pAttributes) {
		m_RulerSize = pAttributes->m_RulerSize;
		m_TickLength = pAttributes->m_TickLength;
		m_HalfTickLength = pAttributes->m_HalfTickLength;
		m_MajTickLength = pAttributes->m_MajTickLength;
		m_csFontName = pAttributes->m_csFontName;
		m_FontSize = pAttributes->m_FontSize;
		m_colorBk = pAttributes->m_colorBk;
		m_colorTickMark = pAttributes->m_colorTickMark;
		m_colorHalfTick = pAttributes->m_colorHalfTick;
		m_colorCursor = pAttributes->m_colorCursor;
		m_colorText = pAttributes->m_colorText;
		m_colorCorner = pAttributes->m_colorCorner;
	}
	void CopyTo(SRullerAttributes* pAttributes) {
		pAttributes->m_RulerSize = m_RulerSize;
		pAttributes->m_TickLength = m_TickLength;
		pAttributes->m_HalfTickLength = m_HalfTickLength;
		pAttributes->m_MajTickLength = m_MajTickLength;
		pAttributes->m_csFontName = m_csFontName;
		pAttributes->m_FontSize = m_FontSize;
		pAttributes->m_colorBk = m_colorBk;
		pAttributes->m_colorTickMark = m_colorTickMark;
		pAttributes->m_colorHalfTick = m_colorHalfTick;
		pAttributes->m_colorCursor = m_colorCursor;
		pAttributes->m_colorText = m_colorText;
		pAttributes->m_colorCorner = m_colorCorner;
	}
	DocFileParseToken Parse(DocFileParseToken Token, CLexer* pLex);
	void Save(FILE* pO, int Indent, int flags) {

	}
	static void LoadSettings(SRullerAttributes* pAttrb);
	static void SaveSettings(SRullerAttributes* pAttrb);
};