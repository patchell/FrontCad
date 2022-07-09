// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// if you love a klugue and you're happy, clap your hands

CPoint DOUBLEPOINT::ToPixelPoint(DOUBLEPOINT ULHC,  double cX, double cY)
{
	CPoint Result;
	double y;

	y = ULHC.dX;
	Result.x = GETAPP.RoundDoubleToInt((dX + y) * cX);
	Result.y = GETAPP.RoundDoubleToInt((dY + ULHC.dY) * cY);
	return Result;
}
