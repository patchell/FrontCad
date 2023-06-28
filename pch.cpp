// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

SException Exception;

// if you love a klugue and you're happy, clap your hands
DOUBLEPOINT DOUBLEPOINT::Raw(
	CPoint p,	//point in client view (pixels)
	DOUBLEPOINT& LLHC,	//Upper Left Hand Corner offset
	SCALE Scale	//Inches per pixel
)
{
	//-------------------------------------
	// converts a point in the client
	// view into a double point
	// 
	// 
	//-------------------------------------
	dX = double(p.x) * Scale.dSX + LLHC.dX;
	dY = double(p.y) * Scale.dSY + LLHC.dY;
	return *this;
}

DOUBLEPOINT DOUBLEPOINT::LLHCfromPixelPoint(CPoint point, double cX, double cY)
{
	//--------------------------------------------------
	// LLHCfromPixelPoint
	// Does the oposite of ToPixel Point. "this" defines
	// the double point value where the point of
	// interest is.
	// 
	// point.....The pixel in the window to map to
	// cX,cY.....Scale factor in InchesPerPixel
	//--------------------------------------------------
	DOUBLEPOINT LLHC;

	LLHC.dX = dX - double(point.x) * cX;
	LLHC.dY = dY - double(point.y) * cY ;
	LLHC.Print("**** LLHC ****");
	return LLHC;
}

CPoint DOUBLEPOINT::ToPixelPoint(DOUBLEPOINT& LLHC,  double cX, double cY)
{
	CPoint Result;

	Result.x = GETAPP.RoundDoubleToInt((dX - LLHC.dX) * cX);
	Result.y = GETAPP.RoundDoubleToInt((dY - LLHC.dY) * cY);
	return Result;
}

//-------------------------------------------
// SnapOrderList methods
//-------------------------------------------

void SnapOrderList::AddSnapAtHead(SnapOrder* pSO)
{
	if (GetHead() == 0)
	{
		SetHead(pSO);
		SetTail(pSO);;
	}
	else
	{
		pSO->SetNext(GetHead());
		GetHead()->SetPrev(pSO);
		SetHead(pSO);
	}
	++m_nNumberOfObjects;
}

void SnapOrderList::AddSnapAtTail(SnapOrder* pSO)
{
	if (GetHead() == 0)
	{
		SetHead(pSO);
		SetTail(pSO);
	}
	else
	{
		pSO->SetPrev(GetTail());
		GetTail()->SetNext(pSO);
		SetTail(pSO);
	}
	++m_nNumberOfObjects;
}

BOOL SnapOrderList::InsertSnap(SnapOrder* pPrevSO, SnapOrder* pSO)
{
	BOOL rV = TRUE;

	if(m_nNumberOfObjects == 0 && pPrevSO)
	{ 
		rV = FALSE; //This makes no sense
	}
	else if (m_nNumberOfObjects == 0 && !pPrevSO)
	{
		//-----------------------------
		// This makes little sense
		// But we can work with it
		//-----------------------------
		SetHead(pSO);
		SetTail(pSO);
	}
	else if (pPrevSO == GetHead())
	{
		pSO->SetNext(GetHead()->GetNext());
		pSO->SetPrev(GetHead());
		GetHead()->GetNext()->SetPrev(pSO);
		GetHead()->SetNext(pSO);
	}
	else if (pPrevSO == GetTail())
	{
		GetTail()->SetNext(pSO);
		pSO->SetPrev(GetTail());
		SetTail(pSO);
	}
	else
	{
		pSO->SetPrev(pPrevSO);
		pSO->SetNext(pPrevSO->GetNext());
		pPrevSO->GetNext()->SetPrev(pSO);
		pPrevSO->SetNext(pSO);
	}
	if(rV)
		++m_nNumberOfObjects;
	return rV;
}

SnapOrder* SnapOrderList::Remove(SnapOrder* pSO, BOOL bDelete)
{
	if (pSO == GetHead())
	{
		SetHead(pSO->GetNext());
		if (GetHead())
			GetHead()->SetPrev(0);
		else
			SetTail(0);
	}
	else if (pSO == GetTail())
	{
		SetTail(pSO->GetPrev());
		if (GetTail())
			GetTail()->SetNext(0);
		else
			SetHead(0);
	}
	else
	{
		pSO->GetNext()->SetPrev(pSO->GetPrev());
		pSO->GetPrev()->SetNext(pSO->GetNext());
	}
	pSO->SetNext(0);
	pSO->SetPrev(0);
	--m_nNumberOfObjects;
	if (bDelete)
	{
		delete pSO;
		pSO = 0;
	}
	return pSO;
}

