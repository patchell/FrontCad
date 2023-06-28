#include "pch.h"

CFileParser::CFileParser()
{
}

CFileParser::~CFileParser()
{
}

BOOL CFileParser::Create(CString& csInputFileName)
{
	m_Lex.Create(csInputFileName);
	return TRUE;
}

BOOL CFileParser::Accept(CLexer::Tokens LookaHead, CLexer::Tokens Token)
{
	//--------------------------------------------
	//** Accept
	//**
	//** this Method is used to match
	//** the token with the current
	//** Lookahead token.  If they match, get the
	//** next token.
	//**
	//** parameter:
	//**	LookAHead...Look Ahead Token
	//**	Token..expected token
	//**	pDoc....pointer to document instance
	//**
	//** Returns:Next token, or ERROR on error
	//--------------------------------------------/
	BOOL rV = FALSE;

	if (LookaHead == Token)
		rV = TRUE;
	return rV;
}

CLexer::Tokens CFileParser::Expect(CLexer::Tokens Token, CLexer::Tokens LookaHead)
{
	CLexer::Tokens rV = CLexer::Tokens::ERRORToken;

	if (Accept(LookaHead, Token))
	{
		rV = m_Lex.Lex();
	}
	else
	{
		//--------------------------------------
		// Houston, we have a syntax error
		//--------------------------------------
		Exception.ErrorString.Format(
			_T("Error::Line %d  Column %d\nExpected %s\nFound %s"),
			m_Lex.GetLine(),
			m_Lex.GetCol(),
			CString(m_Lex.TokenLookup(LookaHead)).GetBuffer(),
			CString(m_Lex.TokenLookup(Token)).GetBuffer()
		);
		throw(Exception);
	}
	return rV;
}


BOOL CFileParser::ParseLibFile(CCadLibrary** pLib)
{
	BOOL Sucess = TRUE;
	CLexer::Tokens Token;


	try
	{
		Token = GetLexer()->Lex();
		switch (Token)
		{
		case CLexer::Tokens::LIBFILE:
			*pLib = new CCadLibrary;
			(*pLib)->Create();
			Token = (*pLib)->Parse(Token, this);
			break;
		default:
			Exception.ErrorString.Format(_T("Not a Library File"));
			throw Exception;
			break;
		}
	}
	catch (SException& XCept)
	{
		Sucess = FALSE;
		MessageBox(NULL, XCept.ErrorString, _T("Error"), MB_OK);
	}
	return Sucess;
}

BOOL CFileParser::ParseDrawFile(CCadDrawing* pDrawing)
{
	BOOL Sucess = TRUE;
	CLexer::Tokens Token;
	CCadOrigin* pOrg;

	try
	{
		Token = GetLexer()->Lex();
		switch (Token)
		{
		case CLexer::Tokens::DRAWFILE:
			pOrg = new CCadOrigin;
			pOrg->Create(pDrawing, CCadObject::SubTypes::ORIGIN_LOCATION);
			Token = pDrawing->Parse(Token, this);
			break;
		default:
			break;
		}
	}
	catch (SException& XCept)
	{
		Sucess = FALSE;
		MessageBox(NULL, XCept.ErrorString, _T("Error"), MB_OK);
	}
	return Sucess;
}

CLexer::Tokens CFileParser::DrawFileAttributes(CLexer::Tokens Token, CCadObject* pO)
{
	//--------------------------------------------------------
	// DrawFileAttributes	-> '(' DrawFileAttributes1 ')'
	//						-> .
	//--------------------------------------------------------
	Token = Expect(CLexer::Tokens('('), Token);
	Token = DrawFileAttributes2(Token, pO);
	Token = Expect(CLexer::Tokens(')'), Token);
	return Token;
}


CLexer::Tokens CFileParser::DrawFileAttributes2(CLexer::Tokens Token, CCadObject* pO)
{
	//--------------------------------------------------------------------
	//	DrawFileAttributes1	-> DrawingAttributes3 DrawingAttributes2;
	// 
	//	DrawingAttributes2	-> ',' DrawingAttributes3 DrawingAttributes2
	//						-> .
	//--------------------------------------------------------------------
	BOOL Loop = TRUE;

	Token = DrawFileAttributes3(Token, pO);
	while (Loop)
	{
		switch (Token)
		{
		case ',':
			Token = Expect(CLexer::Tokens(','), Token);
			Token = DrawFileAttributes3(Token, pO);
			break;
		default:
			Loop = FALSE;
			break;
		}
	}
	return Token;
}

CLexer::Tokens CFileParser::DrawFileAttributes3(CLexer::Tokens Token, CCadObject* pO)
{
	//-------------------------------------------------------------
	//	DrawFileAttributes3	-> COLOR '(' NUM ',' NUM ',' NUM ')'
	//						-> SCALE '(' NUM ')'
	//						-> .
	//-------------------------------------------------------------
	switch (Token)
	{
	case CLexer::Tokens::BACKGROUND_COLOR:
		Token = Color(Token, ((CCadDrawing*)pO)->m_BkColor, Token);
		break;
	case CLexer::Tokens::SCALE:
		Token = Expect(CLexer::Tokens::SCALE, Token);
		Token = Expect(CLexer::Tokens('('), Token);
		Token = Expect(CLexer::Tokens::NUMDOUBLE, Token);
		Token = Expect(CLexer::Tokens(')'), Token);
		break;
	}
	return Token;
}

//------------------------------------------
// Utility Parsing Methods
//------------------------------------------

CLexer::Tokens CFileParser::Color(
	CLexer::Tokens TargetToken,	//type of color object 
	COLORREF& ColorParam,		//Color Value to set 
	CLexer::Tokens Token		//Lookahead Token
)
{
	int RED, BLUE, GREEN;

	Token = Expect(TargetToken, Token);
	Token = Expect(CLexer::Tokens('('), Token);
	Token = IntValue(CLexer::Tokens::RED, RED, Token);
	Token = Expect(CLexer::Tokens(','), Token);
	Token = IntValue(CLexer::Tokens::GREEN, GREEN, Token);
	Token = Expect(CLexer::Tokens(','), Token);
	Token = IntValue(CLexer::Tokens::BLUE, BLUE, Token);
	Token = Expect(CLexer::Tokens(')'), Token);
	ColorParam = RGB(RED, GREEN, BLUE);
	return Token;
}

CLexer::Tokens CFileParser::IntValue(
	CLexer::Tokens TargetToken,	//type of color object 
	int& IntegerValue,			//Integer Value to set 
	CLexer::Tokens Token		//Lookahead Token
)
{
	Token = Expect(TargetToken, Token);
	Token = Expect(CLexer::Tokens('('), Token);
	IntegerValue = GetLexer()->GetIntLexValue();
	Token = Expect(CLexer::Tokens::NUMINT, Token);
	Token = Expect(CLexer::Tokens(')'), Token);
	return Token;
}

CLexer::Tokens CFileParser::UINTValue(CLexer::Tokens TargetToken, UINT& UINTValue, CLexer::Tokens Token)
{
	Token = Expect(TargetToken, Token);
	Token = Expect(CLexer::Tokens('('), Token);
	UINTValue = (UINT)GetLexer()->GetIntLexValue();
	Token = Expect(CLexer::Tokens::NUMINT, Token);
	Token = Expect(CLexer::Tokens(')'), Token);
	return Token;
}

CLexer::Tokens CFileParser::DoubleValue(
	CLexer::Tokens TargetToken, 
	double& DoubleValue, 
	CLexer::Tokens Token
)
{
	Token = Expect(TargetToken, Token);
	Token = Expect(CLexer::Tokens('('), Token);
	DoubleValue = GetLexer()->GetDoubleLexValue();
	Token = Expect(CLexer::Tokens::NUMDOUBLE, Token);
	Token = Expect(CLexer::Tokens(')'), Token);
	return Token;
}

CLexer::Tokens CFileParser::StringValue(
	CLexer::Tokens TargetToken, 
	CString& csS, 
	CLexer::Tokens Token
)
{
	Token = Expect(TargetToken, Token);
	Token = Expect(CLexer::Tokens('('), Token);
	csS.Format(_T("%s"),GetLexer()->GetLexBuff());
	Token = Expect(CLexer::Tokens::STRING, Token);
	Token = Expect(CLexer::Tokens(')'), Token);
	return Token;
}

//-------------------------------------------------
// Save File Utility Methods
//-------------------------------------------------

char* CFileParser::SaveString(char* s, int n, CLexer::Tokens Token, char* string)
{
	sprintf_s(s, n, "%s(%s)",
		CLexer::TokenLookup(Token),
		string
	);
	return s;
}

char* CFileParser::SaveString(char* s, int n, CLexer::Tokens Token, CString& csString)
{
	char* pTempStr = new char[64];

	sprintf_s(s, n, "%s(%s)",
		CLexer::TokenLookup(Token),
		GETAPP.ConvertCStringToChar(pTempStr, csString)
	);
	delete[] pTempStr;
	return s;
}

char* CFileParser::SaveSize(char* s, int n, CLexer::Tokens Token, CDoubleSize& size)
{
	sprintf_s(s, n, "%s(%10.3lf,%10.3lf)",
		CLexer::TokenLookup(Token),
		size.dCX,
		size.dCY
	);
	return s;
}

char* CFileParser::SaveColor(char* s, int n, CLexer::Tokens Token, COLORREF color)
{
	sprintf_s(s, n, "%s(RED(%d),GREEN(%d),BLUE(%d))",
		CLexer::TokenLookup(Token),
		RED(color),
		GREEN(color),
		BLUE(color)
	);
	return s;
}

char* CFileParser::SaveIntValue(char* s, int n, CLexer::Tokens  Token, int& value)
{
	sprintf_s(s, n, "%s(%d)",
		CLexer::TokenLookup(Token),
		value
	);
	return s;
}

char* CFileParser::SaveUINTValue(char* s, int n, CLexer::Tokens Token, UINT& value)
{
	sprintf_s(s, n, "%s(%d)",
		CLexer::TokenLookup(Token),
		value
	);
	return s;
}

char* CFileParser::SaveDoubleValue(char* s, int n, CLexer::Tokens Token, double& value)
{
	sprintf_s(s, n, "%s(%10.3lf)",
		CLexer::TokenLookup(Token),
		value
	);
	return s;
}
