#include "pch.h"

CGrid::CGrid()
{
	m_ZoomFactor = GRID_100PPI;
	//-------------------------------
	m_SnapGridOn = TRUE;
	//-------------------------------
	m_GridShowSnap = TRUE;
	m_SnapLineStyle = TRUE;	//FALSE = dots. TRUE = LINES
	//-------------------------------
	m_GridShowMajor = TRUE;
	m_MajorLineStyle = TRUE;	//FALSE = dots  TRUE = lines
}

CGrid::~CGrid()
{
}

void CGrid::Draw(
	CDC* pDC,
	MODE mode,
	DOUBLEPOINT& ULHC,
	CScale& Scale,
	CRect& rectClient
)
{
	//------------------------------------------------
	// DrawGrid
	//		This method draws the grid (guides)
	// on the view.
	// parameters:
	//		pDC.......pointer to device context
	//-------------------------------------------------
	if ((IsSnapGridShowing() && !IsSnapALine()) || (IsMajorGridShowing() && !IsMajorALine()))
	{
		DrawSnapDots(pDC, mode, ULHC, Scale, rectClient );
		if (IsMajorGridShowing() && IsMajorALine())
			DrawMajLines(pDC, mode, ULHC, Scale, rectClient );
	}
	else if (IsSnapGridShowing() && IsMajorGridShowing())
	{
		DrawSnapLines(pDC, mode, ULHC, Scale, rectClient );
		DrawMajLines(pDC, mode, ULHC, Scale, rectClient);
	}
	else if (IsMajorGridShowing())
		DrawMajLines(pDC, mode, ULHC, Scale, rectClient);
}

void CGrid::DrawSnapDots(
	CDC* pDC,
	MODE mode,
	DOUBLEPOINT& ULHC,
	CScale& Scale,
	CRect& rectClient
)
{
	//-------------------------------------------
	// DrawSnapDots
	//		Draw the grid in all dots.  The
	// major grid and the snap grid will be
	// drawn in their very own colors
	// 
	// parameters:
	//	pDC.........Device context to draw to
	//	mode........Drawing mode (not used)
	//	Offset......Not really used
	//	Scale.......Drawing surface scale
	//	rectClient..Client rectangle
	//	ULHC........Value of Upper Left Hand Corner
	//-------------------------------------------
	int i, j;
	int x, y;
	int Nx, Ny;
	int H, W;
	CDoubleSize SnapGrdSz;
	CCadPoint DotPoint;
	double X, Y, dTemp;
	int GLT_X, GLT_Y;	// Grid line types
	CSize PixelSpacing;

	SnapGrdSz = GetSnapGrid() * Scale;
	W = rectClient.Width();
	H = rectClient.Height();
	Nx = GETAPP.RoundDoubleToInt(double(W) / (GetPixelsPerInch().GetScaleX() * GetSnapGrid().dCX)) + 1;
	Ny = GETAPP.RoundDoubleToInt(double(H) / (GetPixelsPerInch().GetScaleY() * GetSnapGrid().dCY)) + 1;

	PixelSpacing = GetSnapGrid().ToPixelSize(Scale);
	for (i = 0; i < Nx; ++i)
	{
		X = double(i) * GetSnapGrid().dCX + ULHC.dX;
		GLT_X = GetGridLineType(X, Axis::X);
		dTemp = GETAPP.RoundToNearset((X - ULHC.dX) * Scale.GetScaleX(), 1.0);
		x = GETAPP.RoundDoubleToInt(dTemp);
		for (j = 0; j < Ny; ++j)
		{
			Y = double(j) * GetSnapGrid().dCY + ULHC.dY;
			GLT_Y = GetGridLineType(Y, Axis::Y);
			dTemp = GETAPP.RoundToNearset((Y - ULHC.dY) * Scale.GetScaleY(), 1.0);
			y = GETAPP.RoundDoubleToInt(dTemp);

			if ((GLT_X == GRID_SNAP || GLT_X == GRID_HALF) &&
				(GLT_Y == GRID_SNAP || GLT_Y == GRID_HALF) &&
				!(GLT_X == GRID_MAJOR || GLT_Y == GRID_MAJOR)
			)
			{
				//---------------------------------
				// If both grid lines (HORZ and
				// VERT) are Snap lines And if the
				// Grid spacing Is Greater than 12 
				// pixels and if Major lines are 
				// disabled and Snap lines are
				// enabled then draw a dot
				//---------------------------------
				if (PixelSpacing.cx > 12.0 && IsSnapGridShowing())
				{
					if (GLT_Y == GRID_HALF || GLT_X == GRID_HALF)
					{
						pDC->SetPixel(x, y, GetHalfLineColor());
					}
					else
					{
						pDC->SetPixel(x, y, GetSnapLineColor());
					}
				}
			}
			else if (GLT_X == GRID_MAJOR || GLT_Y == GRID_MAJOR)
			{
				//---------------------------------
				// If either grid line (HORZ or
				// VERT) are Major lines And if 
				//  Major lines are 
				// dots  and Major lines are
				// enabled then draw a dot
				//---------------------------------
				if (!IsMajorALine() && IsMajorGridShowing())
					pDC->SetPixel(x, y, GetMajLineColor());
				else
					pDC->SetPixel(x, y, GetMajLineColor());
			}
		}
	}
}

void CGrid::DrawMajLines(
	CDC* pDC,
	MODE mode,
	DOUBLEPOINT& ULHC,
	CScale& Scale,
	CRect& rectClient
)
{
	//--------------------------------------------------------------
	// DrawMajLines
	//		This method only draws the major grid lines.
	// parameters:
	//	pDC.........Device context to draw to
	//	mode........Drawing mode (not used)
	//	Offset......Not really used
	//	Scale.......Drawing surface scale
	//	rectClient..Client rectangle
	//	ULHC........Value of Upper Left Hand Corner
	//-------------------------------------------
	int i;
	int Nx, Ny;
	int H, W;
	CPen Pen, * oldPen;
	double Y,X;
	int x, y;
	double IntPart;

	//---------------------------------------
	W = rectClient.Width();
	H = rectClient.Height();
	Nx = GETAPP.RoundDoubleToInt(double(W) / (GetPixelsPerInch().GetScaleX() * GetMajorGrid().dCX)) + 1;
	Ny = GETAPP.RoundDoubleToInt(double(H) / (GetPixelsPerInch().GetScaleY() * GetMajorGrid().dCY)) + 1;
	//----------------------------------------------------------
	// Need to find out where the first major lines start
	//---------------------------------------------------------
	//------------------- Vertical Lines ---------------------

	if (GetMajorGrid().dCX * Scale.GetScaleX() > 12)
	{
		Pen.CreatePen(PS_SOLID, GetMajLineWidth(), GetMajLineColor());
		oldPen = pDC->SelectObject(&Pen);
		//-----------------------------
		// Vertical grid lines
		//----------------------------
		for (i = 0; i < Nx; ++i)
		{
			X = double(i + 1) * GetMajorGrid().dCX - modf(ULHC.dX,&IntPart);
			x = GETAPP.RoundDoubleToInt(X * Scale.GetScaleX());
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, H - 1);
		}
		//---------------------------------------
		// Horizontal Lines
		//--------------------------------------
		for (i = 0; i < Ny; ++i)
		{
			Y = double(i) * GetMajorGrid().dCY - modf(ULHC.dY, &IntPart);
			y = GETAPP.RoundDoubleToInt(Y * Scale.GetScaleX());
			pDC->MoveTo(0, y);
			pDC->LineTo(W - 1, y);
		}
		pDC->SelectObject(oldPen);
	}
}

void CGrid::DrawSnapLines(CDC* pDC, MODE mode, DOUBLEPOINT& ULHC, CScale &Scale, CRect& rectClient)
{
	//--------------------------------------------------------------
	// DrawAllLines
	//		This method draws the major grid lines and the snap 
	// grid lines
	// 
	// parameters:
	//	pDC.........Device context to draw to
	//	mode........Drawing mode (not used)
	//	Offset......Not really used
	//	Scale.......Drawing surface scale
	//	rectClient..Client rectangle
	//	ULHC........Value of Upper Left Hand Corner
	//-------------------------------------------
	int i;
	int Nx, Ny;
	int H, W;
	CPen snapPen, halfPen, * oldPen;
	double X, Y;
	int x, y;
	int GridLineType;
	//-----------------------------------------------------
	W = rectClient.Width();
	H = rectClient.Height();
	Nx = GETAPP.RoundDoubleToInt(double(W) / (GetPixelsPerInch().GetScaleX() * GetSnapGrid().dCX));
	Ny = GETAPP.RoundDoubleToInt(double(H) / (GetPixelsPerInch().GetScaleY() * GetSnapGrid().dCY));
	//----------------------------------------------------------
	// Need to find out where the first snap lines start
	//---------------------------------------------------------
	if (GetSnapGrid().dCX * Scale.GetScaleX() > 12)
	{
		snapPen.CreatePen(PS_SOLID, GetSnapLineWidth(), GetSnapLineColor());
		halfPen.CreatePen(PS_SOLID, GetSnapLineWidth(), GetHalfLineColor());
		oldPen = pDC->SelectObject(&snapPen);
		for (i = 0; i < Nx; ++i)
		{
			X = double(i) * GetSnapGrid().dCX;
			x = GETAPP.RoundDoubleToInt(X * Scale.GetScaleX());
			GridLineType = GetGridLineType(X, Axis::X);
			switch (GridLineType)
			{
			case GRID_SNAP:
				pDC->SelectObject(&snapPen);
				break;
			case GRID_HALF:
				pDC->SelectObject(&halfPen);
				break;
			}
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, H - 1);
		}
		for (i = 0; i < Ny; ++i)
		{
			Y = double(i) * GetSnapGrid().dCY;
			y = GETAPP.RoundDoubleToInt(Y * Scale.GetScaleX());
			GridLineType = GetGridLineType(Y, Axis::Y);
			switch (GridLineType)
			{
			case GRID_SNAP:
				pDC->SelectObject(&snapPen);
				break;
			case GRID_HALF:
				pDC->SelectObject(&halfPen);
				break;
			}
			pDC->MoveTo(0, y);
			pDC->LineTo(W - 1, y);
		}
		pDC->SelectObject(oldPen);
	}
}

int CGrid::GetGridLineType(double LineV, Axis Type)
{
	double SnapSpacing, MajSpacing, intpart;
	int GridLineType = 0;	//none of the above
	const char* s;
	double SnapsPerMajor;

	switch (Type)
	{
	case Axis::X:
		SnapSpacing = GetSnapGrid().dCX;
		MajSpacing = GetMajorGrid().dCX;
		SnapsPerMajor = MajSpacing / SnapSpacing;
		s = "X";
		break;
	case Axis::Y:
		SnapSpacing = GetSnapGrid().dCY;
		MajSpacing = GetMajorGrid().dCY;
		SnapsPerMajor = MajSpacing / SnapSpacing;
		s = "Y";
		break;
	default:
		SnapSpacing = 1.0;
		MajSpacing = 1.0;
		SnapsPerMajor = MajSpacing / SnapSpacing;
		s = "N";
		break;
	}
	double ModPart;

	ModPart = modf(LineV / MajSpacing, &intpart);
	if (GETAPP.IsEqToZero(ModPart, 0.0001))
		GridLineType = GRID_MAJOR;
	else if (GETAPP.IsEqToZero(ModPart - 0.50, 0.0001))
		GridLineType = GRID_HALF;
	else
		GridLineType = GRID_SNAP;
	/*
	ModPart = modf(LineV / SnapSpacing, &intpart);
	if (GETAPP.IsEqToZero(ModPart, 0.0001))
	{
		GridLineType = GRID_SNAP; //SnapGrid Line
		double M = LineV / MajSpacing;
		M = modf(M, &intpart) * SnapsPerMajor;
		if (GETAPP.IsEqToZero(M, 0.001))
		{
			GridLineType = GRID_MAJOR;
			if (ModPart == 0.500)
				GridLineType = GRID_HALF;
		}
	}
	*/
	return GridLineType;
}
