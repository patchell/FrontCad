#include "pch.h"

CParserFrontCadLib::CParserFrontCadLib()
{
}

CParserFrontCadLib::~CParserFrontCadLib()
{
}

BOOL CParserFrontCadLib::Create(CArchive& ar)
{
	BOOL rV = TRUE;
	return rV;
}

BOOL CParserFrontCadLib::Parse(
	CCadLibrary** pLib
)
{
	BOOL Success = TRUE;
	Token LaToken;

	try
	{
		LaToken = GetLexer()->Lex();
		switch (LaToken)
		{
		case Token::LIBFILE:
			*pLib = new CCadLibrary;
			(*pLib)->Create();
			(*pLib)->Parse(this, Token::LIBFILE);
			break;
		default:
			Exception.ErrorString.Format(_T("Not a Library File"));
			throw Exception;
			break;
		}
	}
	catch (SException& XCept)
	{
		Success = FALSE;
		MessageBox(NULL, XCept.ErrorString, _T("Error"), MB_OK);
	}
	return Success;
}

