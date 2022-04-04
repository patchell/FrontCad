// MyBitmap.h: interface for the CMyBitmap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMyBitmap : public CBitmap  
{
public:
	CMyBitmap();
	virtual ~CMyBitmap();
	BOOL LoadImageBitmap(CString csPath);
};
