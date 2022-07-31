// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// if you love a klugue and you're happy, clap your hands

DOUBLEPOINT DOUBLEPOINT::ULHCfromPixelPoint(CPoint point, double cX, double cY)
{
	//--------------------------------------------------
	// ULHCfromPixelPoint
	// Does the oposite of ToPixel Point. "this" defines
	// the double point value where the point of
	// interest is.
	// 
	// point.....The pixel in the window to map to
	// cX,cY.....Scale factor in InchesPerPixel
	//--------------------------------------------------
	DOUBLEPOINT ULHC;

	ULHC.dX = dX - double(point.x) * cX;
	ULHC.dY = dY - double(point.y) * cY ;
	ULHC.Print("**** ULHC ****");
	return ULHC;
}

CPoint DOUBLEPOINT::ToPixelPoint(DOUBLEPOINT ULHC,  double cX, double cY)
{
	CPoint Result;

	Result.x = GETAPP.RoundDoubleToInt((dX - ULHC.dX) * cX);
	Result.y = GETAPP.RoundDoubleToInt((dY - ULHC.dY) * cY);
	return Result;
}
