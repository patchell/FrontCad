#include "pch.h"

//----------------------------------------------
// CRulerView
//----------------------------------------------

IMPLEMENT_DYNCREATE(CRulerView, CWnd)

CRulerView::CRulerView()
{
	m_pRulerInfo = 0;
	m_rulerType = RT_HORIZONTAL;
	m_OriginOffset = 0;
}

CRulerView::~CRulerView()
{
}

BEGIN_MESSAGE_MAP(CRulerView, CWnd)
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CRulerView::IsBackGroundValid(CSize szClient)
{
	BOOL rV = FALSE;
	BITMAP Bm;

	HBITMAP hBM = HBITMAP(m_RulerBackground);
	if (hBM)
	{
		if (m_RulerBackground.GetBitmap(&Bm))
		{
			if ((Bm.bmWidth == szClient.cx) && (Bm.bmHeight == szClient.cy))
				rV = TRUE;
		}
	}
	return rV;
}

void CRulerView::SetRulerType(UINT rulerType)
{
	m_rulerType = rulerType;
	if (RT_VERTICAL == rulerType)
	{
		m_Font.CreateFontW(
			16, 
			0, 
			900, 
			900, 
			FW_NORMAL, 
			0, 
			0, 
			0, 
			ANSI_CHARSET, 
			OUT_TT_PRECIS, 
			CLIP_TT_ALWAYS, 
			PROOF_QUALITY, 
			VARIABLE_PITCH | FF_ROMAN, 
			_T("Times New Roman")
		);

	}
	else    // if(RT_HORIZONTAL == rulerType)
	{
		m_Font.CreateFont(
			16, 
			0, 
			0, 
			0, 
			FW_NORMAL, 
			0, 
			0, 
			0, 
			ANSI_CHARSET, 
			OUT_TT_PRECIS, 
			CLIP_TT_ALWAYS, 
			PROOF_QUALITY, 
			VARIABLE_PITCH | FF_ROMAN, 
			_T("Times New Roman")
		);
	}
}

//---------------------------------------------
// CRulerView drawing
//---------------------------------------------

void CRulerView::Draw()
{
	CDC* pDC = GetWindowDC();
	CDC memDC;
	CBitmap TheMem, * pOldBM;;
	CRect rectClient;
	int w, h;

	GetClientRect(&rectClient);
	w = rectClient.Width();
	h = rectClient.Height();
	memDC.CreateCompatibleDC(pDC);
	if (HBITMAP(m_RulerBackground))
		m_RulerBackground.DeleteObject();
	m_RulerBackground.CreateCompatibleBitmap(
		pDC,
		w,
		h
	);
	pOldBM = memDC.SelectObject(&m_RulerBackground);
	DrawTicker(&memDC, rectClient);
	pDC->BitBlt(0, 0, w, h, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBM);
	ReleaseDC(pDC);
}

void CRulerView::DrawTicker(
	CDC* pDC,			//Device Context to draw to
	CRect &rulerRect	//Client Rectangle
)
{
	//--------------------------------------------------------
	// DrawTicker
	//
	//	This function draws the ticks for the ruller.
	//
	// parameters:
	//	pDC...........pointer to the device context
	//	ruleRect......Rectangle that encloses the ruler
	//---------------------------------------------------------
	// First find the long dimension of ruler
	// For a horizontal ruler, this is the right side
	// For a Vertical ruler, this is the bottom side
	//----------------------------------------------------------
	CString csLabel;	//format the label in here
	double Inches;		//Total number of inches long
	int nSize;			// long dimension of the ruler
	CPen* pOldPen,snapPen, majPen;
	CFont* pOldFont;
	COLORREF OldBkColor;
	COLORREF OldTextColor;
	int oldMapMode;
	int oldTextAlign;
	double PixPerInch;	//ruler scaling factor
	int SnapSpacing = 0, MajorSpacing = 0;
	double SnapSpace, MajorSpace;;
	int nTotalTicks;
	double FirstTick;
	Axis AxisType;
	CSize TextSize;
	CRect rectClient;
	CBrush brushBK, * oldBrush;
	int AddLableEvery = 1;
	int LabelCount = 1;

	brushBK.CreateSolidBrush(RGB(64,0,64)/*GetRulersInfo()->GetBackGroundColor()*/);
	oldBrush = pDC->SelectObject(&brushBK);
	GetClientRect(&rectClient);
	pDC->FillRect(&rectClient, &brushBK);
	pDC->SelectObject(oldBrush);

	CScale RulerPixPerIn = GetRulersInfo()->GetGrid()->GetPixelsPerInch();
	switch (GetRulerType())
	{
	case RT_HORIZONTAL:
		PixPerInch = RulerPixPerIn.GetScaleX();
		nSize = GetRulersInfo()->GetClientSize().cx;
		Inches = double(nSize) / PixPerInch;
		SnapSpace = GetRulersInfo()->GetGrid()->GetSnapGrid().dCX;
		MajorSpace = GetRulersInfo()->GetGrid()->GetMajorGrid().dCX;
		nTotalTicks = GETAPP.RoundDoubleToInt(Inches / SnapSpace); //Inches/(Inches/Tick)		break;
		FirstTick = GETAPP.RoundUpToNearest(
			GetRulersInfo()->GetUpperLeft().dX,
			GetRulersInfo()->GetGrid()->GetSnapGrid().dCX
		);
		AxisType = Axis::X;
		SnapSpacing = GETAPP.RoundDoubleToInt(
			SnapSpace * GetRulersInfo()->GetGrid()->GetPixelsPerInch().GetScaleX());
		MajorSpacing = GETAPP.RoundDoubleToInt(
			MajorSpace * GetRulersInfo()->GetGrid()->GetPixelsPerInch().GetScaleX()
		);
		break;
	case RT_VERTICAL:
		PixPerInch = RulerPixPerIn.GetScaleY();
		nSize = GetRulersInfo()->GetClientSize().cy;
		Inches = double(nSize) / PixPerInch;
		SnapSpace = GetRulersInfo()->GetGrid()->GetSnapGrid().dCY;
		MajorSpace = GetRulersInfo()->GetGrid()->GetMajorGrid().dCY;
		nTotalTicks = GETAPP.RoundDoubleToInt(Inches / SnapSpace); //Inches/(Inches/Tick)		break;
		FirstTick = GETAPP.RoundUpToNearest(
			GetRulersInfo()->GetUpperLeft().dY,
			GetRulersInfo()->GetGrid()->GetSnapGrid().dCY
		);
		AxisType = Axis::Y;
		SnapSpacing = GETAPP.RoundDoubleToInt(
			SnapSpace * GetRulersInfo()->GetGrid()->GetPixelsPerInch().GetScaleY());
		MajorSpacing = GETAPP.RoundDoubleToInt(
			MajorSpace * GetRulersInfo()->GetGrid()->GetPixelsPerInch().GetScaleY()
		);
		break;
	}
	//----------------------------------------------------------
	//Create a List of Labels, draw ticks, etc
	//----------------------------------------------------------
	if (MajorSpacing < 13)
	{
		AddLableEvery = 7;
		LabelCount = 0;
	}
	else if (MajorSpacing < 25)
	{
		AddLableEvery = 3;
		LabelCount = 0;
	}
	else
	{
		AddLableEvery = 0;
		LabelCount = 0;

	}
	if (GetRulersInfo()->AreRulersShowing())
	{
		//-----------------------------------
		// Select the drawing objects 
		//-----------------------------------
		pOldFont = pDC->SelectObject(&m_Font);
		OldTextColor = pDC->SetTextColor(GetRulersInfo()->GetTextColor());
		OldBkColor = pDC->SetBkColor(GetRulersInfo()->GetBackGroundColor());
		majPen.CreatePen(
			PS_SOLID, 
			GetRulersInfo()->GetGridMajLineWidth(), 
			GetRulersInfo()->GetMajorTickColor()
		);
		snapPen.CreatePen(
			PS_SOLID, 
			GetRulersInfo()->GetGridMajLineWidth(), 
			GetRulersInfo()->GetTickMarkColor()
		);
		pOldPen = pDC->SelectObject(&snapPen);
		oldTextAlign = pDC->SetTextAlign(
			(GetRulerType() == RT_HORIZONTAL) ? (TA_RIGHT | TA_TOP) : (TA_LEFT | TA_TOP)
		);
		oldMapMode = pDC->SetMapMode(MM_TEXT);
		//------------------------------------
		// Draw all of the ticks and label
		// the major ticks
		//------------------------------------
		for (int i = 0; i <= nTotalTicks; i++)
	{
			//----------------------------
			// Are we at a Major Tick?
			//----------------------------
			double tick;
			int tickType;
			int x, y;
			CPoint p;

			tick = FirstTick + SnapSpace * double(i);
			tickType = GetRulersInfo()->GetGrid()->GetGridLineType(tick, AxisType);
			switch (GetRulerType())
			{
			case RT_HORIZONTAL:
				p = CDoublePoint(
					tick,
					0.0
				).ToPixelPoint(
					-GetRulersInfo()->GetScrollOffset(),
					GetRulersInfo()->GetGrid()->GetPixelsPerInch()
				);
				pDC->MoveTo(p);
				switch (tickType)
				{
				case GRID_MAJOR:
					if (LabelCount-- == 0)
					{
						pDC->SelectObject(&majPen);
						y = GetRulersInfo()->GetMajorTickLength();
						pDC->LineTo(p.x, y);
						if (i > 0)
						{
							csLabel.Format(_T("%6.2lf"), tick);
							TextSize = pDC->GetTextExtent(csLabel);
							pDC->TextOutW(p.x + TextSize.cx / 2, y, csLabel);
						}
						LabelCount = AddLableEvery;
					}
					break;
				case GRID_SNAP:
					if (SnapSpacing > 12)
					{
						pDC->SelectObject(&snapPen);
						y = GetRulersInfo()->GetTickeLength();
						pDC->LineTo(p.x, y);
					}
					break;
				}  // end of "switch (tickType)"
				break;
			case RT_VERTICAL:
				p = CDoublePoint(
					0.0,
					tick
				).ToPixelPoint(
					-GetRulersInfo()->GetScrollOffset(),
					GetRulersInfo()->GetGrid()->GetPixelsPerInch()
				);
				pDC->MoveTo(p);
				switch (tickType)
				{
				case GRID_MAJOR:
					if (LabelCount-- == 0)
					{
						pDC->SelectObject(&majPen);
						x = GetRulersInfo()->GetMajorTickLength();
						pDC->LineTo(x, p.y);
						if (i != 0)
						{
							csLabel.Format(_T("%6.2lf"), tick);
							TextSize = pDC->GetTextExtent(csLabel);
							pDC->TextOutW(x, p.y + TextSize.cx / 2, csLabel);
						}
						LabelCount = AddLableEvery;
					}
					break;
				case GRID_SNAP:
					if (SnapSpacing > 12.0)
					{
						pDC->SelectObject(&snapPen);
						x = GetRulersInfo()->GetTickeLength();
						pDC->LineTo(x, p.y);
					}						break;
				}	// end of "switch (tickType)"
				break;
			} // end of "switch (GetRulerType())"
		}	// end of for loop
		// restore DC objects
		pDC->SetMapMode(oldMapMode);
		pDC->SelectObject(pOldFont);
		pDC->SelectObject(pOldPen);
		pDC->SetTextAlign(oldTextAlign);
		pDC->SetTextColor(OldTextColor);
		pDC->SetBkColor(OldBkColor);
		pDC->SelectObject(pOldPen);
	}// (End of if bShowPos)
}

void CRulerView::DrawCursorPos(CDC* pDC, CDoublePoint NewPos)
{
	//----------------------------------
	// set the map mode right
	//----------------------------------
	int oldMapMode = pDC->SetMapMode(MM_TEXT);

	CRect clientRect;
	CPoint Point;
	CPen CursorPen, * oldPen;

	GetClientRect(&clientRect);
	Point = (NewPos + -GetRulersInfo()->GetUpperLeft()).ToPixelPoint(
		CSize(0,0),
		GetRulersInfo()->GetGrid()->GetPixelsPerInch()
	);
	CursorPen.CreatePen(PS_SOLID, 5, GetRulersInfo()->GetCursorColor());
	oldPen = pDC->SelectObject(&CursorPen);
	switch (GetRulerType())
	{
	case RT_HORIZONTAL:
		pDC->MoveTo(CPoint(Point.x, 0));
		pDC->LineTo(CPoint(Point.x, clientRect.Height() - 1));
		break;
	case RT_VERTICAL:
		pDC->MoveTo(CPoint(0, Point.y));
		pDC->LineTo(CPoint(clientRect.Width() - 1,Point.y));
		break;
	}
	pDC->SelectObject(oldPen);
	pDC->SetMapMode(oldMapMode);
}

/////////////////////////////////////////////////////////////////////////////
// CRulerView diagnostics

#ifdef _DEBUG
void CRulerView::AssertValid() const
{
	CWnd::AssertValid();
}

void CRulerView::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRulerView message handlers

BOOL CRulerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// create a brush using the same color as the background
	if (cs.lpszClass == NULL)
	{
		HBRUSH hBr = CreateSolidBrush(GetSysColor(COLOR_MENU));
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_BYTEALIGNWINDOW, NULL, hBr);
	}
	return CWnd::PreCreateWindow(cs);
}

void CRulerView::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	((CSplitterWnd*)GetParent())->SetActivePane(1, 1);
}

void CRulerView::Scroll()
{
}

void CRulerView::SetCursor()
{
	CDC* pDC = GetDC();
	CDC memDC,memDCcopy;
	CBitmap* oldBitmap, *oldBMcopy;
	CBitmap bmCopy;
	BITMAP bmBK;

	if (HBITMAP(m_RulerBackground))
	{
		//ruler background
		memDC.CreateCompatibleDC(pDC);
		oldBitmap = memDC.SelectObject(&m_RulerBackground);
		//copy of ruler background
		memDCcopy.CreateCompatibleDC(pDC);
		m_RulerBackground.GetBitmap(&bmBK);
		bmCopy.CreateBitmapIndirect(&bmBK);
		oldBMcopy = memDCcopy.SelectObject(&bmCopy);
		memDCcopy.BitBlt(0, 0, bmBK.bmWidth, bmBK.bmHeight, &memDC, 0, 0, SRCCOPY);
		DrawCursorPos(
			&memDCcopy,
			GetRulersInfo()->GetCursorPos()
		);
		pDC->BitBlt(0, 0, bmBK.bmWidth, bmBK.bmHeight, &memDCcopy, 0, 0,SRCCOPY);
		memDC.SelectObject(oldBitmap);
		memDCcopy.SelectObject(oldBMcopy);
	}
	ReleaseDC(pDC);
}

BOOL CRulerView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CRulerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	GetRulersInfo()->SetRulersReady(TRUE);
}
