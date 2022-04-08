#pragma once


//---------------------------------------------------------
// ruler types
//----------------------------------------------------------
constexpr auto RT_VERTICAL = 0;
constexpr auto RT_HORIZONTAL = 1;

// Ruler View Messages
constexpr auto RW_VSCROLL = 1;
constexpr auto RW_HSCROLL = 2;
constexpr auto RW_POSITION = 3;
constexpr auto RW_ZOOM = 4;
constexpr auto RW_SETLABELS = 5;
constexpr auto RW_INIT = 6;
constexpr auto RW_SHOW = 7;
//----------------------------------

class CRulerInfo
{
	//-----------------------------------------
	// MajorTickSpacing
	//		This is the spacing, in inches,
	// between the major ticks.  For Example
	// 1.00 Inches
	// As it turns out, this is the same
	// things as the MajorGridSpacing
	// 
	// MinorTickSpacing
	//		This is the spacing, in inches,
	// between the minor ticks.  For example
	// 0.125 Inches (1/8 inch)  This, as
	// it turns out IS identicle to the snap
	// grid
	//-----------------------------------------
	CGrid* m_pGrid;
	//----------------------------------
	CDoublePoint m_UpperLeft;
	CDoublePoint m_CursorPosition;
	CSize m_ScrollOffset;
	CSize m_szClient;
	//--------------------------------
	BOOL m_Show;
	SRullerAttributes m_Attributes;
	//--------------------------------
	BOOL m_RulersReady;
public:
	CRulerInfo();
	~CRulerInfo();
	void SetScrollOffset(CSize offset) { m_ScrollOffset = offset; }
	CSize GetScrollOffset() { return m_ScrollOffset; }
	void SetUpperLeft(CDoublePoint uL) { m_UpperLeft = uL; }
	CDoublePoint GetUpperLeft() { return m_UpperLeft; }
	void SetCursorPos(CDoublePoint CP) { m_CursorPosition = CP; }
	CDoublePoint GetCursorPos() { return m_CursorPosition; }
	void ShowRulers(BOOL Show) { m_Show = Show; }
	BOOL AreRulersShowing() { return m_Show; }
	CSize GetClientSize() { return m_szClient; }
	void SetClientSize(CSize sz) { m_szClient = sz; }
	BOOL AreFulersReady() { return m_RulersReady; }
	void SetRulersReady(BOOL b) { m_RulersReady = b; }
	//-------------------------------
	// Ruler Attributes
	//-------------------------------
	SRullerAttributes& GetAttributes() { return m_Attributes; }
	SRullerAttributes* GetPtrToAttributes() { return &m_Attributes; }
	//----------------- Tick Marks ------------------------------
	void SetTickLength(int TL) { GetAttributes().m_TickLength = TL; }
	int GetTickeLength() { return GetAttributes().m_TickLength; }
	void SetTickMarkColor(COLORREF c) { GetAttributes().m_colorTickMark = c; }
	COLORREF GetTickMarkColor() { return GetAttributes().m_colorTickMark; }
	//----------------- Major Ticks -----------------------------
	void SetMajorTickLength(int MTL) { GetAttributes().m_MajTickLength = MTL; }
	int GetMajorTickLength() { return GetAttributes().m_MajTickLength; }
	void SetMajorTickColor(COLORREF c) { GetAttributes().m_colorMajorTick = c; }
	COLORREF GetMajorTickColor() { return GetAttributes().m_colorMajorTick; }
	//---------------------- Half Ticks ---------------------------
	void SetHalfTickColor(COLORREF c) { GetAttributes().m_colorHalfTick = c; }
	COLORREF GetHalfTickColor() { return GetAttributes().m_colorHalfTick; }
	void SetHalfTickLength(int l) { GetAttributes().m_HalfTickLength = l; }
	int GetHalfTickLength() { return GetAttributes().m_HalfTickLength; }
	//--------------------- Background ----------------------------
	void SetBackGroundColor(COLORREF c) { GetAttributes().m_colorBk = c; }
	COLORREF GetBackGroundColor() { return GetAttributes().m_colorBk; }
	//------------------=-- Position Cursor --------------------------
	void SetCursorColor(COLORREF c) { GetAttributes().m_colorCursor = c; }
	COLORREF GetCursorColor() { return GetAttributes().m_colorCursor;}
	//-------------------- Text -----------------------------------
	void SetTextColor(COLORREF c) { GetAttributes().m_colorText = c; }
	COLORREF GetTextColor() { return GetAttributes().m_colorText; }
	//------------------- Corner Color -------------------------
	void SetCornerColor(COLORREF c) { GetAttributes().m_colorCorner = c; }
	COLORREF GetCornerColor() { return GetAttributes().m_colorCorner; }
	//------------------------------------------------------
	// Grid
	// The Grid and the Ruler markings sort of overlap.
	// If some of this looks confusing, that is because
	// I am confused
	//------------------------------------------------------
	CGrid* GetGrid() { return m_pGrid; }
	void SetGrid(CGrid* pG) { m_pGrid = pG; }
	void SetSnapGrid(CDoubleSize snap) { GetGrid()->SetSnapGrid(snap); }
	CDoubleSize GetSnapGrid() { return GetGrid()->GetSnapGrid(); }
	void SetMajorGrid(CDoubleSize MajGrd) { GetGrid()->SetMajorGrid(MajGrd); }
	CDoubleSize GetMajorGrid() { return GetGrid()->GetMajorGrid(); }
	void EnableSnap(BOOL en) { GetGrid()->EnableSnap(en); }// snap grid enabled
	BOOL IsSnapOn() { return GetGrid()->IsSnapOn(); }
	void ShowMajorGrid(BOOL en) { GetGrid()->ShowMajorGrid(en); }
	BOOL IsMajorGirdShowing() { return GetGrid()->IsMajorGridShowing(); }
	void ShowSnapGrid(BOOL en) { GetGrid()->ShowMajorGrid(en); }
	BOOL IsSnapGridShowing() { return GetGrid()->IsSnapGridShowing(); }
	//-------------------------------------------------
	// Grid Attributes
	//-------------------------------------------------
	void SetGridSnapLineColor(COLORREF c) { GetGrid()->SetSnapLineColor(c); }
	COLORREF GetGridSnapLineColor() { return GetGrid()->GetSnapLineColor(); }
	void SetGridMajLineColor(COLORREF c) { GetGrid()->SetMajLineColor( c); }
	COLORREF GetGridMajLineColor() { return GetGrid()->GetMajLineColor(); }
	void SetGridSnapLineWidth(int w) { GetGrid()->SetSnapLineWidth(w); }
	int GetGridSnapLineWidth() { return GetGrid()->GetSnapLineWidth(); }
	void SetGridMajLineWidth(int w) { GetGrid()->SetMajLineWidth(w); }
	int GetGridMajLineWidth() { return GetGrid()->GetMajLineWidth(); }
};


