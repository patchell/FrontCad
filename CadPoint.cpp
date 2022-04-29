#include "pch.h"

CCadPoint::CCadPoint()
{
	dX = 0.0;
	dY = 0.0;
}

CCadPoint::CCadPoint(double x, double y)
{
	dX = x;
	dY = y;
}

CCadPoint::~CCadPoint()
{
}

BOOL CCadPoint::Create(CCadObject* pRefObj)
{
	OnCreate(pRefObj);
	return TRUE;
}

BOOL CCadPoint::Destroy(CCadObject* pDependentObject)
{
	CCadObject* pObj;
	BOOL rV = TRUE;

	if (GetDependentChildrenHead())
	{
		pObj = GetDependentChildrenHead();
		while (pObj)
		{
			//------------------------------
			// Go up the chain of dependent
			// children and prepare them
			//-----------------------------
			pObj->Destroy(this);
			
		}
	}
	return rV;
}

void CCadPoint::Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale)
{
}
