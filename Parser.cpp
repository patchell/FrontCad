#include "pch.h"

CParser::CParser()
{
	m_pLexer = 0;
	m_LookaHeadToken = Token(0);
}

CParser::~CParser()
{
	if (m_pLexer)
		delete m_pLexer;
}

BOOL CParser::Create(CArchive& ar)
{
	BOOL rV = TRUE;

	return rV;
}

void CParser::Expect(Token ExpectedToken)
{
	if (Accept(ExpectedToken))
		m_LookaHeadToken = GetLexer()->Lex();
	else
	{

	}
}

BOOL CParser::Accept(Token ExpectedToken) const
{
	BOOL rV = FALSE;

	if (m_LookaHeadToken == ExpectedToken)
		rV = TRUE;
	return rV;
}

//------------------------------------------
// Utility Parsing Methods
//------------------------------------------

void CParser::Color(
	Token TargetToken,	//type of color object 
	COLORREF& ColorParam		//Color Value to set 
)
{
	int RED, BLUE, GREEN;

	Expect(TargetToken);
	Expect(Token('('));
	IntValue(Token::RED, RED);
	Expect(Token(','));
	IntValue(Token::GREEN, GREEN);
	Expect(Token(','));
	IntValue(Token::BLUE, BLUE);
	Expect(Token(')'));
	ColorParam = RGB(RED, GREEN, BLUE);
}

void CParser::IntValue(
	Token TargetToken,	//type of color object 
	int& IntegerValue			//Integer Value to set 
)
{
	Expect(TargetToken);
	Expect(Token('('));
	IntegerValue = GetLexer()->GetLexValueInt();
	Expect(Token::NUMINT);
	Expect(Token(')'));
}

void CParser::UINTValue(
	Token TargetToken,
	UINT& UINTValue
)
{
	Expect(TargetToken);
	Expect(Token('('));
	UINTValue = (UINT)GetLexer()->GetLexValueInt();
	Expect(Token::NUMINT);
	Expect(Token(')'));
}

void CParser::DoubleValue(
	Token TargetToken,
	double& DoubleValue
)
{
	Expect(TargetToken);
	Expect(Token('('));
	DoubleValue = GetLexer()->GetLexValueDouble();
	Expect(Token::NUMDOUBLE);
	Expect(Token(')'));
}

void CParser::StringValue(
	Token TargetToken,
	CString& csS
)
{
	Expect(TargetToken);
	Expect(Token('('));
	csS.Format(_T("%hs"), GetLexer()->GetLexBuffer());
	Expect(Token::STRING);
	Expect(Token(')'));
}


//-------------------------------------------------
// Save File Utility Methods
//-------------------------------------------------

int CParser::SaveString(
	char* s,
	int n,
	Token TokenValue,
	CString& csString
)
{
	char* pTempStr = new char[64];
	int l = 0;

	l = sprintf_s(s, n, "%s(%s)",
		CLexer::TokenLookup(TokenValue),
		GETAPP.ConvertCStringToChar(pTempStr, csString)
	);
	delete[] pTempStr;
	return l;
}

char* CParser::SaveStringS(
	char* s, 
	int n, 
	Token TokenValue, 
	CString& csString
)
{
	char* pTempStr = new char[64];

	sprintf_s(s, n, "%s(%s)",
		CLexer::TokenLookup(TokenValue),
		GETAPP.ConvertCStringToChar(pTempStr, csString)
	);
	delete[] pTempStr;
	return s;
}

int CParser::SaveSize(char* s, int n, Token TokenValue, CDoubleSize& size)
{
	int l = 0;

	l = sprintf_s(s, n, "%hs(%10.3lf,%10.3lf)",
		CLexer::TokenLookup(TokenValue),
		size.dCX,
		size.dCY
	);
	return l;
}

int CParser::SaveColor(char* s, int n, Token TokenValue, COLORREF color)
{
	int l = 0;

	l = sprintf_s(s, n, "%hs(RED(%d),GREEN(%d),BLUE(%d))",
		CLexer::TokenLookup(TokenValue),
		RED(color),
		GREEN(color),
		BLUE(color)
	);
	return l;
}

char* CParser::SaveColorS(char* s, int n, Token TokenValue, COLORREF color)
{
	sprintf_s(s, n, "%hs(RED(%d),GREEN(%d),BLUE(%d))",
		CLexer::TokenLookup(TokenValue),
		RED(color),
		GREEN(color),
		BLUE(color)
	);
	return s;
}

int CParser::SaveIntValue(char* s, int n, Token  TokenValue, int& value)
{
	int l;

	l = sprintf_s(s, n, "%hs(%d)",
		CLexer::TokenLookup(TokenValue),
		value
	);
	return l;
}

char* CParser::SaveIntValueS(char* s, int n, Token TokenValue, int& value)
{
	sprintf_s(s, n, "%hs(%d)",
		CLexer::TokenLookup(TokenValue),
		value
	);
	return s;
}

int CParser::SaveUINTValue(char* s, int n, Token TokenValue, UINT& value)
{
	int l = 0;

	l = sprintf_s(s, n, "%hs(%d)",
		CLexer::TokenLookup(TokenValue),
		value
	);
	return l;
}

char* CParser::SaveUINTValueS(char* s, int n, Token TokenValue, UINT& value)
{
	sprintf_s(s, n, "%hs(%d)",
		CLexer::TokenLookup(TokenValue),
		value
	);
	return s;
}

int CParser::SaveDoubleValue(char* s, int n, Token TokenValue, double& value)
{
	int l = 0;

	l = sprintf_s(s, n, "%hs(%10.3lf)",
		CLexer::TokenLookup(TokenValue),
		value
	);
	return l;
}

char* CParser::SaveDoubleValueS(char* s, int n, Token TokenValue, double& value)
{
	sprintf_s(s, n, "%hs(%10.3lf)",
		CLexer::TokenLookup(TokenValue),
		value
	);
	return s;
}

int CParser::SaveDoubleValue(char* s, int n, double& v)
{
	int l = 0;

	l = sprintf_s(s, n, "%10.7lf", v);
	return l;
}

char* CParser::SaveDoubleValueS(char* s, int n, double& v)
{
	sprintf_s(s, n, "%10.7lf", v);
	return s;
}
