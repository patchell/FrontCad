#include "pch.h"

CCadPoint::CCadPoint()
{
}

CCadPoint::CCadPoint(double x, double y)
{
	dX = x;
	dY = y;
}

CCadPoint::~CCadPoint()
{
}

BOOL CCadPoint::Destroy(CCadObject* pDependentObject)
{
	CCadObject* pObj;

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
}

void CCadPoint::Draw(CDC* pDC, MODE mode, CSize Offset, CScale Scale)
{
}
