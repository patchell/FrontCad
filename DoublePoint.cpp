#include "pch.h"

CPoint CDoublePoint::ToPixelPoint(CDoublePoint &ULHC, CScale &PixelsPerInch)
{
    CPoint ResultPoint;
    CDoublePoint p = *this + -ULHC;
    p = p * PixelsPerInch;
    p.RoundToNearest(CDoubleSize(1.0,1.0));
    ResultPoint = p.ToCPoint();
    return ResultPoint;
}

CPoint CDoublePoint::ToCPoint()
{
    return CPoint(
        GETAPP.RoundDoubleToInt(dX),
        GETAPP.RoundDoubleToInt(dY)
    );
}
CDoublePoint CDoublePoint::Snap(CDoubleSize SnapSize, BOOL SnapEnabled)
{
    //-----------------------------------------------------
    // Snap
    // sets the position to nearest snap grid position
    //
    // Parameters:
    //	SnapSize.Number of units for the snap grid
    // for example, for a 1/4 inch grid, SnapUnits
    // would be 0.250
    //
    // Returns:
    //	Cursor position in real units for the view
    //-----------------------------------------------------
    double x_mod, x, y_mod, y;
    CDoublePoint Result;

    if (SnapEnabled || (SnapSize != CDoubleSize(0.0,0.0)))
    {
        x_mod = fmod(dX, SnapSize.dCX);
        if (x_mod < (SnapSize.dCX / 2.0))
            x = dX - x_mod;
        else
            x = dX + SnapSize.dCX - x_mod;
        y_mod = fmod(dY, SnapSize.dCY);
        if (y_mod < (SnapSize.dCY / 2.0))
            y = dY - y_mod;
        else
            y = dY + SnapSize.dCY - y_mod;
        dX = x;
        dY = y;
   }
     return *this;
}

BOOL CDoublePoint::Hover(CDoublePoint Point)
{
    return 0;
}

void CDoublePoint::FilledRect(
    CDC* pDC, 
    MODE mode, 
    CSize Offset,
    CScale Scale,
    COLORREF Border,
    COLORREF Fill
)
{

}

CDoubleRect CDoublePoint::EnclosePoint(UINT nPixels, CScale Scale)
{
    double Inches;
    CDoubleRect Result;

    Inches = double(nPixels) * Scale.m_ScaleX; //number of inches to get 10 pixels
    Result.SetPointsFromCenter(*this, *this + CDoubleSize(Inches, Inches), *this);
    return Result;
}

void CDoublePoint::Draw(CDC* pDC, MODE mode, CDoublePoint &ULHC, CScale &Scale)
{
    switch (mode.PointsMode)
    {
    case SelectedPointsMode::NOP:
        break;
    case SelectedPointsMode::POINT_BOTH_RECT_FILLED:
        EnclosePoint(10, Scale).Draw(pDC, mode, ULHC, Scale);
        break;
    case SelectedPointsMode::POINT_ENCLOSED_IN_RECT:
        break;
    case SelectedPointsMode::POINT_FILLED_RECT:
        break;
    }
}

BOOL CDoublePoint::MakeSelectionRect(CRect& rect, CScale& Scale, CDoublePoint& ULHC, double Margin)
{
    //----------------------------------------------------
    // MakeSelectionRect
    //----------------------------------------------------
    BOOL InView = TRUE;

    CPoint Point = ToPixelPoint(ULHC, Scale);
    return InView;
}

void CDoublePoint::Flip(CDoublePoint Pivot, Axis axis)
{
    double FlipSize;

    switch (axis)
    {
    case Axis::X:
        FlipSize = (Pivot.dX - dX) * 2;
        dX = dX - FlipSize;
        break;
    case Axis::Y:
        FlipSize = (Pivot.dY - dY) * 2;
        dY = dY - FlipSize;
        break;
    }
}

DocFileParseToken CDoublePoint::Parse(DocFileParseToken Token, CLexer* pLex, DocFileParseToken TypeToken)
{
    Token = pLex->Point(DocFileParseToken::POINT, *this, Token);
    return Token;
}

void CDoublePoint::Save(FILE* pO, DocFileParseToken Token, int Indent, int flags)
{
    char* IndentString = new char[256];

    GETAPP.MkIndentString(IndentString, Indent);
    fprintf(pO, "%s%s(%lf,%lf)\n",
        IndentString,
        CLexer::TokenToString(Token),
        dX,
        dY
    );
    delete[] IndentString;
}

CDoublePoint CDoublePoint::MidPoint(CDoublePoint P)
{
    CDoublePoint Mid;
    CDoubleSize Len;

    Len = P - *this;
    Len = Len / 2.0;
    return *this + Len;
}

BOOL CDoublePoint::IsPointOnTarget(CDoublePoint p)
{
    BOOL Result = FALSE;
    CDoubleRect r;
    CScale Scale = GETVIEW()->GetGrid().GetInchesPerPixel();
    double Inches = 10.0 * Scale.GetScaleX();
    //--------------------------
    // would like a rectangle
    // with a 10 pixel margin
    //-------------------------
    CDoubleRect rect;
    rect.SetPointsFromCenter(*this, *this + CDoubleSize(Inches, Inches), *this);
    Result = rect.PointInRectangle(p);
    return Result;
}

double CDoublePoint::DistanceTo(CDoublePoint p)
{
    return  (* this - p).Magnitude();
}

CDoublePoint CDoublePoint::Rotate(CDoublePoint* pPivot, double Angle)
{
    double x, y, r;

    Angle = GETAPP.GetPi() * Angle / 180.0;

    r = CDoubleSize(dX - pPivot->dX, dY - pPivot->dY).Magnitude();
    x = r * cos(Angle) + pPivot->dX;
    y = r * sin(Angle) + pPivot->dY;
    return CDoublePoint(x,y);
}

int CDoublePoint::intGetX()
{
    return GETAPP.RoundDoubleToInt(dX);
}

int CDoublePoint::intGetY()
{
    return GETAPP.RoundDoubleToInt(dY);
}

CDoublePoint CDoublePoint::Rotate(CDoublePoint Pivot, double Angle)
{
    //----------------------------------
    // Rotate
    //  This method rotates this point
    // by an angle in degrees with
    // respect to another point
    //  parameters:
    // Pivot......point to rotate around
    // Angle......Angle to rotate by in degrees
    //  returns:
    // Rotated point
   //----------------------------------
    double Pi = GETAPP.GetPi();
    double RadAngle = Angle * Pi / 180.0;
    CDoubleSize Diff;
    double TheOtherAngle;
    double Mag;
    //-----------------------------------
    // First thing, figure out the with
    // respect to the "X" axis from the
    // pivot point
    //----------------------------------
    Diff = *this - Pivot;
    Mag = Diff.Magnitude();
    TheOtherAngle = Diff.Angle(DOUBLESIZE_RADIANS);
    dX = Mag * cos(RadAngle + TheOtherAngle) + Pivot.dX;
    dY = Mag * sin(RadAngle + TheOtherAngle) + Pivot.dY;
   return (*this);
}

CDoublePoint CDoublePoint::Rotate(
    CDoublePoint Pivot, 
    CDoublePoint NewPoint
)
{
    //-----------------------------------------
    // Rotate
    //  This method rotate a point radially
    // around a pivot point.  So the new
    // position will lie on the circle
    // whose radias is between the pivot
    // and the and the location of this
    // point. i.e. the radius is 
    // preserved.
    // 
    // parameters:
    //  Pivot.......popint to rotate around
    //  NewPoint....Defines the new angle
    //              to rotate by.  This point
    //              does not have to lie on the
    //              the circle defined the radius
    //              from Pivot to this point.
    // 
    // Return: Location of new point
    // Actions:Updates this point to new location
    //-----------------------------------------
   CDoubleSize Diff;

    Diff = Pivot - NewPoint;
    return Rotate(Pivot, Diff.Angle());
}

CDoublePoint CDoublePoint::RoundUpToNearest(CDoubleSize roundto)
{
    dX = GETAPP.RoundUpToNearest(dX, roundto.dCX);
    dY = GETAPP.RoundUpToNearest(dY, roundto.dCY);
    return *this;
}

CDoublePoint CDoublePoint::RoundToNearest(CDoubleSize roundto)
{
    dX = GETAPP.RoundToNearset(dX, roundto.dCX);
    dY = GETAPP.RoundToNearset(dY, roundto.dCY);
    return *this;
}
