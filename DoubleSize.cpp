#include "pch.h"

CDoubleSize::CDoubleSize(CSize szSize)
{
    double InchesX,InchesY;
    double Scale = GETVIEW->GetGrid().GetInchesPerPixel().GetScaleX();
    InchesX = Scale * double(szSize.cx);
    InchesY = Scale * double(szSize.cy);
    dCX = InchesX;
    dCY = InchesY;
}

void CDoubleSize::Parse(
	CParser* pParser,	// pointer to parser
	Token TypeToken// Token type to save object as
)
{
}

double CDoubleSize::Angle(BOOL DegreesMode)
{
    //------------------------------------
    // Angle
    //  Get the angle that is defined by
    // a vector that goes through the 
    // origine (of this object) and a
    // point that is defined as the size
    // 
    // parameter:
    // DegreesMode.....0=>Radians  1=>Degrees
    //------------------------------------

    double Angle;

    Angle = atan2(dCY, dCX);    //radians
    if(DOUBLESIZE_DEGREES == DegreesMode)
        Angle = Angle * 180.0 / GETAPP.GetPi(); //degrees
    return Angle;
}

CSize CDoubleSize::ToPixelSize(CScale& Scale) const
{
    //------------------------------
    // ToPixelSize
    // 
    // Parameters:
    //  Scale....Pixels per Inch
    //------------------------------
    int cx, cy;

    cx = GETAPP.RoundDoubleToInt(dCX * Scale.GetScaleX());
    cy = GETAPP.RoundDoubleToInt(dCY * Scale.GetScaleY());
    return CSize(cx,cy);
}

CDoubleSize::operator CPoint() const
{
    return CPoint(
        GETAPP.RoundDoubleToInt(dCX),
        GETAPP.RoundDoubleToInt(dCY)
    );
}
