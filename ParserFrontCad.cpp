#include "pch.h"

CParserFrontCad::CParserFrontCad()
{
}

CParserFrontCad::~CParserFrontCad()
{
}

BOOL CParserFrontCad::Create(CArchive& ar)
{
	m_pLexer = new CLexerFrontCad;
	return m_pLexer->Create(ar);
}

BOOL CParserFrontCad::Parse(
	CCadObject* pDrawing
)
{
	BOOL Success = TRUE;

	try
	{
		pDrawing->Parse(this);
	}
	catch (SException& XCept)
	{
		Success = FALSE;
		MessageBox(NULL, XCept.ErrorString, _T("Error"), MB_OK);
	}
	return Success;
}


void CParserFrontCad::DrawFileAttributes(
	CCadObject* pO
)
{
	//--------------------------------------------------------
	// DrawFileAttributes	-> '(' DrawFileAttributes1 ')'
	//						-> .
	//--------------------------------------------------------
	Expect(Token('('));
	DrawFileAttributes2(pO);
	Expect(Token(')'));
}

void CParserFrontCad::DrawFileAttributes2(
	CCadObject* pO
)
{
	//--------------------------------------------------------------------
	//	DrawFileAttributes1	-> DrawingAttributes3 DrawingAttributes2;
	// 
	//	DrawingAttributes2	-> ',' DrawingAttributes3 DrawingAttributes2
	//						-> .
	//--------------------------------------------------------------------
	BOOL Loop = TRUE;

	DrawFileAttributes3(pO);
	while (Loop)
	{
		switch (GetLookaHead())
		{
		case Token(','):
			Expect(Token(','));
			DrawFileAttributes3(pO);
			break;
		default:
			Loop = FALSE;
			break;
		}
	}
}

void CParserFrontCad::DrawFileAttributes3(CCadObject* pO)
{
	//-------------------------------------------------------------
	//	DrawFileAttributes3	-> COLOR '(' NUM ',' NUM ',' NUM ')'
	//						-> SCALE '(' NUM ')'
	//						-> .
	//-------------------------------------------------------------
	switch (GetLookaHead())
	{
	case Token::BACKGROUND_COLOR:
		Color(Token::BACKGROUND_COLOR,((CCadDrawing*)pO)->GetBkColor());
		break;
	case Token::SCALE:
		Expect(Token::SCALE);
		Expect(Token('('));
		Expect(Token::NUMDOUBLE);
		Expect(Token(')'));
		break;
	}
}


