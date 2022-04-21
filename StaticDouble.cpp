// StaticDouble.cpp : implementation file
//

#include "pch.h"


// CStaticDouble

IMPLEMENT_DYNAMIC(CStaticDouble, CStatic)

CStaticDouble::CStaticDouble()
{

}

CStaticDouble::~CStaticDouble()
{
}


BEGIN_MESSAGE_MAP(CStaticDouble, CStatic)
END_MESSAGE_MAP()



// CStaticDouble message handlers




void CStaticDouble::SetDoubleValue(double v)
{
    //----------------------------------------
    // Displays a double value in a CStatic
    // Object
    //----------------------------------------
    CString csValue;

    csValue.Format(_T("%8.3lf"), v);
    SetWindowTextW(csValue);
}
