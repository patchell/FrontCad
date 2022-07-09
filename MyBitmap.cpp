/----------------------------------------
** CLASS CMyBitmap
**
** Derived from CBitmap, this class adds
** the functionality of being able to
** load a CBitmap from a file.
----------------------------------------/
#include "pch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBitmap::CMyBitmap():CBitmap()
{

}

CMyBitmap::~CMyBitmap()
{

}

BOOL CMyBitmap::LoadImageBitmap(CString csPath)
{
	//--------------------------------------------
	//	LoadBitmap
	// This function loads a bitmap from a
	// bmp file.
	//
	// parameter:
	//	csPath....file name of bitmap
	//--------------------------------------------
	DeleteObject();
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImageW(
		NULL, 
		csPath, 
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
	);
	return Attach(hBitmap);
}
