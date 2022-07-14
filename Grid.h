#pragma once

constexpr auto GRID_SNAP = 1;
constexpr auto GRID_HALF = 2;
constexpr auto GRID_MAJOR = 3;

constexpr auto GRID_1000PPI = 0;
constexpr auto GRID_500PPI = 1;
constexpr auto GRID_200PPI = 2;
constexpr auto GRID_100PPI = 3;
constexpr auto GRID_50PPI = 4;
constexpr auto GRID_20PPI = 5;
constexpr auto GRID_10PPI = 6;

class CGrid
{
	//-------------Flags------------------
	BOOL m_SnapGridOn;		//snap grid is off
	BOOL m_GridShowMajor;			//enables the grid on the scrren
	BOOL m_MajorLineStyle;	//True = Lines, FALSE = Dots
	BOOL m_GridShowSnap;	// Show the snap grid
	BOOL m_SnapLineStyle;	//True = Lines, FALSE = Dots
	SGridAttributes m_Attributes;
	int m_ZoomFactor;	//screen magnification factor
//---------------------------------
// OK, what is the deal with these
// scale/zoom factiors?
//--------------------------------
	inline static double ScaleFactor[MAX_ZOOM] = {
		//-----------------------------------
		// These are the ratios between
		// adjacent pairs of the entries in
		// the Scale array 
		//-----------------------------------
		2.0,	//0
		2.5,	//1
		2.0,	//2
		2.0,	//3
		2.5,	//4
		2.0,	//5
		2.0		//6
	};
	inline static CScale PixelsPerInch[MAX_ZOOM] = {
		1000.0,		//1000 pixles per inch
		500.0		,	//500 pixels per inch
		200.0,		//200 pixels per inch
		100.0,		//100 pixels per inch
		50.0,		// 50 pixels per inch
		20.0,		// 20 pixels per inch
		10.0		// 10 pixels per inch
	};
	inline static CScale InchesPerPixel[MAX_ZOOM] = {
		0.001,	//.001 inches per pixel
		0.002,	//.002 inches per pixel
		0.005,	//.005 inches per pixel
		0.01,	//.01 inches per pixel
		0.02,	//.02 inches per pixel
		0.05,	//.05 inches per pixel
		0.1		//.1 inches per pixel
	};
public:
	CGrid();
	~CGrid();
	void Draw(
		CDC* pDC,
		MODE mode,
		DOUBLEPOINT ULHC,
		CScale& Scale,
		CRect& rectClient
	);
	void DrawSnapDots(
		CDC* pDC,
		MODE mode,
		DOUBLEPOINT ULHC,
		CScale Scale,
		CRect& rectClient
	);
	void DrawMajLines(
		CDC* pDC,
		MODE mode,
		DOUBLEPOINT ULHC,
		CScale Scale,
		CRect& rectClient
	);
	void DrawSnapLines(
		CDC* pDC,
		MODE mode,
		DOUBLEPOINT ULHC,
		CScale& Scale,
		CRect& rectClient
	);
	int GetGridLineType(double LineV, Axis Type);
	BOOL EnableSnap(BOOL en) { 
		BOOL OldEnable = m_SnapGridOn;
		m_SnapGridOn = en;
		return OldEnable;
	}	//Enables/Disables the Snap Grid
	BOOL IsSnapOn() { return m_SnapGridOn; }
	void ShowMajorGrid(BOOL en) { m_GridShowMajor = en; }	//Show major Grid
	BOOL IsMajorGridShowing() { return m_GridShowMajor; }
	void SetMajorLineStyle(BOOL lS) { m_MajorLineStyle = lS; }
	BOOL IsMajorALine() { return m_MajorLineStyle; }
	void SetSnapLineStyle(BOOL lS) { m_SnapLineStyle = lS; }
	BOOL IsSnapALine() { return m_SnapLineStyle; }
	void ShowSnapGrid(BOOL en) { m_GridShowSnap = en; }	// Show Snap Grid
	BOOL IsSnapGridShowing() { return m_GridShowSnap; }
	int GetZoomFactor() { return m_ZoomFactor; }
	void SetZoomFactor(int Zf) { m_ZoomFactor = Zf; }
	BOOL ZoomOut() {	//returns true if zoom changed
		BOOL rV = FALSE;

		if (m_ZoomFactor < (MAX_ZOOM - 1))
		{
			rV = TRUE;
			++m_ZoomFactor;
		}
		return rV;
	}
	BOOL ZoomIn() {	//returns true if zoom changed
		BOOL rV = FALSE;

		if (m_ZoomFactor > 0)
		{
			--m_ZoomFactor;
			rV = TRUE;
		}
		return rV;
	}
	double GetScaleFactor() { return ScaleFactor[m_ZoomFactor]; }
	//--------------------------------------------------
	// Screen <-> Drawing (Pixels <-> Inches) Conversions ie Scaling
	//-------------------------------------------------
	CScale& GetInchesPerPixel() { return InchesPerPixel[m_ZoomFactor]; }
	CScale& GetPixelsPerInch() { return PixelsPerInch[m_ZoomFactor]; }
	//-------------------------------------------------
	// Grid Attributes
	//-------------------------------------------------
	SGridAttributes& GetAttributes() { return m_Attributes; }
	SGridAttributes* GetPtrToAttributes() { return &m_Attributes; }
	void SetSnapGrid(CDoubleSize SnGrid) { GetAttributes().m_SnapGridSpacing = SnGrid; }
	CDoubleSize& GetSnapGrid() { return GetAttributes().m_SnapGridSpacing; }
	void SetMajorGrid(CDoubleSize MajGrd) { GetAttributes().m_MajorGridSpacing = MajGrd; }
	CDoubleSize GetMajorGrid() { return GetAttributes().m_MajorGridSpacing; }
	void SetSnapLineColor(COLORREF c) { GetAttributes().m_colorSnapLine = c; }
	COLORREF GetSnapLineColor() { return GetAttributes().m_colorSnapLine; }
	void SetMajLineColor(COLORREF c) { GetAttributes().m_colorMajorLine = c; }
	COLORREF GetHalfLineColor() { return GetAttributes().m_colorHalfGrid; }
	void SetHalfLineColor(COLORREF c) { GetAttributes().m_colorHalfGrid = c; }
	COLORREF GetMajLineColor() { return GetAttributes().m_colorMajorLine; }
	void SetSnapLineWidth(int w) { GetAttributes().m_SnapLineWidth = w;}
	int GetSnapLineWidth() { return GetAttributes().m_SnapLineWidth; }
	void SetMajLineWidth(int w) { GetAttributes().m_MajorLineWidth = w; }
	int GetMajLineWidth() { return GetAttributes().m_MajorLineWidth; }
};

