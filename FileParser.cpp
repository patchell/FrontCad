#include "pch.h"

CFileParser::CFileParser()
{
	//----------------------------------------
	// Init Lexer Attributes
	//----------------------------------------
	m_LexIndex = 0;
	m_Col = 0;
	m_Line = 1;
	m_LastCol = 0;
	m_UnGetBuff = 0;
	m_LexValueInt = 0;
	m_LexValueDouble = 0.0;
	for (int i = 0; i < 256; ++i)
		m_LexBuff[i] = 0;
	for (int i = 0; i < 2048; ++i)
		m_InputBuffer[i] = 0;
	m_BufferPosition = 0;
	m_NumberOfCharsInBuffer = 0;
	m_EOF = FALSE;
}

CFileParser::~CFileParser()
{
}

BOOL CFileParser::Create(CString& csInputFileName)
{
	BOOL rV = TRUE;

	m_LexIndex = 0;
	m_Col = 0;
	m_Line = 1;
	m_UnGetBuff = 0;
	for (int i = 0; i < 256; ++i)
		m_LexBuff[i] = 0;

	return rV;
}

BOOL CFileParser::Accept(CFile* pcfInFile, int LookaHead, int Token)
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

int CFileParser::Expect(CFile* pcfInFile, int Token, int LookaHead)
{
	int rV = TOKEN_ERRORToken;

	if (Accept(pcfInFile, LookaHead, Token))
	{
		rV = Lex(pcfInFile);
	}
	else
	{
		//--------------------------------------
		// Houston, we have a syntax error
		//--------------------------------------
		Exception.ErrorString.Format(
			_T("Error::Line %d  Column %d\nExpected %hs\nFound %hs"),
			GetLine(),
			GetCol(),
			CString(TokenLookup(LookaHead)).GetBuffer(),
			CString(TokenLookup(Token)).GetBuffer()
		);
		throw(Exception);
	}
	return rV;
}


BOOL CFileParser::ParseLibFile(
	CFile* pcfInFile, 
	CCadLibrary** pLib
)
{
	BOOL Success = TRUE;
	int Token;

	try
	{
		Token = Lex(pcfInFile);
		switch (Token)
		{
		case TOKEN_LIBFILE:
			*pLib = new CCadLibrary;
			(*pLib)->Create();
			Token = (*pLib)->Parse(pcfInFile, Token, this);
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

BOOL CFileParser::ParseDrawFile(
	CFile* pcfInFile,
	CCadDrawing* pDrawing
)
{
	BOOL Success = TRUE;
	int Token;
	CCadOrigin* pOrg;

	try
	{
		Token = Lex(pcfInFile);
		switch (Token)
		{
		case TOKEN_DRAWFILE:
			pOrg = new CCadOrigin;
			pOrg->Create(pDrawing, CCadObject::SubTypes::ORIGIN_LOCATION);
			Token = pDrawing->Parse(pcfInFile, Token, this);
			break;
		default:
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

int CFileParser::DrawFileAttributes(
	CFile* pcfInFile,
	int Token, 
	CCadObject* pO
)
{
	//--------------------------------------------------------
	// DrawFileAttributes	-> '(' DrawFileAttributes1 ')'
	//						-> .
	//--------------------------------------------------------
	Token = Expect(pcfInFile,int('('), Token);
	Token = DrawFileAttributes2(pcfInFile,Token, pO);
	Token = Expect(pcfInFile,int(')'), Token);
	return Token;
}


int CFileParser::DrawFileAttributes2(
	CFile* pcfInFile,
	int Token, 
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

	Token = DrawFileAttributes3(pcfInFile, Token, pO);
	while (Loop)
	{
		switch (Token)
		{
		case ',':
			Token = Expect(pcfInFile, int(','), Token);
			Token = DrawFileAttributes3(pcfInFile, Token, pO);
			break;
		default:
			Loop = FALSE;
			break;
		}
	}
	return Token;
}

int CFileParser::DrawFileAttributes3(CFile* pcfInFile, int Token, CCadObject* pO)
{
	//-------------------------------------------------------------
	//	DrawFileAttributes3	-> COLOR '(' NUM ',' NUM ',' NUM ')'
	//						-> SCALE '(' NUM ')'
	//						-> .
	//-------------------------------------------------------------
	switch (Token)
	{
	case TOKEN_BACKGROUND_COLOR:
		Token = Color(pcfInFile, Token, ((CCadDrawing*)pO)->m_BkColor, Token);
		break;
	case TOKEN_SCALE:
		Token = Expect(pcfInFile, TOKEN_SCALE, Token);
		Token = Expect(pcfInFile, int('('), Token);
		Token = Expect(pcfInFile, TOKEN_NUMDOUBLE, Token);
		Token = Expect(pcfInFile, int(')'), Token);
		break;
	}
	return Token;
}

//------------------------------------------
// Utility Parsing Methods
//------------------------------------------

int CFileParser::Color(
	CFile* pcfInFile,
	int TargetToken,	//type of color object 
	COLORREF& ColorParam,		//Color Value to set 
	int Token		//Lookahead Token
)
{
	int RED, BLUE, GREEN;

	Token = Expect(pcfInFile, TargetToken, Token);
	Token = Expect(pcfInFile, int('('), Token);
	Token = IntValue(pcfInFile, TOKEN_RED, RED, Token);
	Token = Expect(pcfInFile, int(','), Token);
	Token = IntValue(pcfInFile, TOKEN_GREEN, GREEN, Token);
	Token = Expect(pcfInFile, int(','), Token);
	Token = IntValue(pcfInFile, TOKEN_BLUE, BLUE, Token);
	Token = Expect(pcfInFile, int(')'), Token);
	ColorParam = RGB(RED, GREEN, BLUE);
	return Token;
}

int CFileParser::IntValue(
	CFile* pcfInFile,
	int TargetToken,	//type of color object 
	int& IntegerValue,			//Integer Value to set 
	int Token		//Lookahead Token
)
{
	Token = Expect(pcfInFile, TargetToken, Token);
	Token = Expect(pcfInFile, int('('), Token);
	IntegerValue = GetIntLexValue();
	Token = Expect(pcfInFile, TOKEN_NUMINT, Token);
	Token = Expect(pcfInFile, int(')'), Token);
	return Token;
}

int CFileParser::UINTValue(
	CFile* pcfInFile,
	int TargetToken, 
	UINT& UINTValue, 
	int Token
)
{
	Token = Expect(pcfInFile, TargetToken, Token);
	Token = Expect(pcfInFile, int('('), Token);
	UINTValue = (UINT)GetIntLexValue();
	Token = Expect(pcfInFile, TOKEN_NUMINT, Token);
	Token = Expect(pcfInFile, int(')'), Token);
	return Token;
}

int CFileParser::DoubleValue(
	CFile* pcfInFile,
	int TargetToken, 
	double& DoubleValue, 
	int Token
)
{
	Token = Expect(pcfInFile, TargetToken, Token);
	Token = Expect(pcfInFile, int('('), Token);
	DoubleValue = GetDoubleLexValue();
	Token = Expect(pcfInFile, TOKEN_NUMDOUBLE, Token);
	Token = Expect(pcfInFile, int(')'), Token);
	return Token;
}

int CFileParser::StringValue(
	CFile* pcfInFile,
	int TargetToken, 
	CString& csS, 
	int Token
)
{
	Token = Expect(pcfInFile, TargetToken, Token);
	Token = Expect(pcfInFile, int('('), Token);
	csS.Format(_T("%hs"),GetLexBuff());
	Token = Expect(pcfInFile, TOKEN_STRING, Token);
	Token = Expect(pcfInFile, int(')'), Token);
	return Token;
}

//-------------------------------------------------
// Save File Utility Methods
//-------------------------------------------------

char* CFileParser::SaveString(char* s, int n, int Token, char* string)
{
	sprintf_s(s, n, "%hs(%hs)",
		CFileParser::TokenLookup(Token),
		string
	);
	return s;
}

char* CFileParser::SaveString(char* s, int n, int Token, CString& csString)
{
	char* pTempStr = new char[64];

	sprintf_s(s, n, "%hs(%hs)",
		TokenLookup(Token),
		GETAPP.ConvertCStringToChar(pTempStr, csString)
	);
	delete[] pTempStr;
	return s;
}

char* CFileParser::SaveSize(char* s, int n, int Token, CDoubleSize& size)
{
	sprintf_s(s, n, "%hs(%10.3lf,%10.3lf)",
		TokenLookup(Token),
		size.dCX,
		size.dCY
	);
	return s;
}

char* CFileParser::SaveColor(char* s, int n, int Token, COLORREF color)
{
	sprintf_s(s, n, "%hs(RED(%d),GREEN(%d),BLUE(%d))",
		TokenLookup(Token),
		RED(color),
		GREEN(color),
		BLUE(color)
	);
	return s;
}

char* CFileParser::SaveIntValue(char* s, int n, int  Token, int& value)
{
	sprintf_s(s, n, "%hs(%d)",
		TokenLookup(Token),
		value
	);
	return s;
}

char* CFileParser::SaveUINTValue(char* s, int n, int Token, UINT& value)
{
	sprintf_s(s, n, "%hs(%d)",
		TokenLookup(Token),
		value
	);
	return s;
}

char* CFileParser::SaveDoubleValue(char* s, int n, int Token, double& value)
{
	sprintf_s(s, n, "%hs(%10.3lf)",
		TokenLookup(Token),
		value
	);
	return s;
}

char* CFileParser::SaveDoubleValue(char* s, int n, double& v)
{
	sprintf_s(s, n, "%10.7lf", v);
	return s;
}

//------------------------------------------------
// Lexer Methods
//------------------------------------------------

int CFileParser::GetFromInput(CFile* pcfInFile)
{
	int rV = EOF;

	if (!m_EOF)
	{
		if (m_NumberOfCharsInBuffer == 0)
		{
			m_NumberOfCharsInBuffer = pcfInFile->Read(m_InputBuffer, 2048);
			if (m_NumberOfCharsInBuffer == 0)
				m_EOF = TRUE;
			m_BufferPosition = 0;
		}
		if (m_NumberOfCharsInBuffer)
		{
			rV = m_InputBuffer[m_BufferPosition];
			m_BufferPosition++;
			m_NumberOfCharsInBuffer--;
		}
	}
	return rV;
}

int CFileParser::GetChar(CFile* pcfInFile)
{
	int rV;

	if (m_UnGetBuff)
	{
		rV = m_UnGetBuff;
		m_UnGetBuff = 0;
		if (rV == '\n')
		{
			m_LastCol = m_Col;
			m_Col = 0;
			++m_Line;
		}
		else
		{
			m_LastCol = m_Col;
			++m_Col;
		}
	}
	else
	{
		rV = GetFromInput(pcfInFile);
		if (rV != EOF)
		{
			if (rV == '\n')
			{
				m_LastCol = m_Col;
				m_Col = 0;
				++m_Line;
			}
			else
			{
				m_LastCol = m_Col;
				++m_Col;
			}
		}
		
	}
	return rV;
}

int CFileParser::UnGetChar(int c)
{
	m_UnGetBuff = c;
	if (c == '\n')
	{
		m_Col = m_LastCol;
		if (m_LastCol)
			m_LastCol--;
		if (m_Line)
			m_Line--;
	}
	else
	{
		m_Col = m_LastCol;
		if (m_LastCol)
			--m_LastCol;
	}
	return 0;
}

int CFileParser::LookUp(const char* pKW)
{
	int rV = int(-1);

	int i, loop;
	for (i = 0, loop = 1; KeyWords[i].m_pName && loop; ++i)
	{
		if (strcmp(KeyWords[i].m_pName, pKW) == 0)
		{
			loop = 0;
			rV = KeyWords[i].m_Token;
		}
	}
	return rV;
}

const char* CFileParser::TokenLookup(int TokenVal)
{
	const char* pRs = "Huh?";

	int i, loop;
	for (i = 0, loop = 1; KeyWords[i].m_pName && loop; ++i)
	{
		if (KeyWords[i].m_Token == TokenVal)
		{
			loop = 0;
			pRs = KeyWords[i].m_pName;
		}
	}
	return pRs;
}

BOOL CFileParser::IsValidDigit(int c)
{
	BOOL rV = FALSE;
	if (isdigit(c) || c == '.')
		rV = TRUE;
	return rV;
}

BOOL CFileParser::IsDoubleValue(int c)
{
	BOOL rV = FALSE;

	if ('.' == c)
		rV = TRUE;
	return rV;
}

int CFileParser::Lex(CFile* pcfInFile)
{
	int loop;
	int rV;
	BOOL DoubleValue;
	int c;
	int token;
	loop = 1;
	while (loop)
	{
		c = GetChar(pcfInFile);
		switch (c)
		{
		case EOF:
			loop = 0;
			rV = TOKEN_EOFToken;
			break;
		case ' ': case '\t':	//whitespace
			break;
		case '\n':	//end of line and white space
			++m_Line;
			m_Col = 0;
			break;
		case '\"':	//string
			m_LexIndex = 0;
			while ((c = GetChar(pcfInFile)) != '\"')
			{
				m_LexBuff[m_LexIndex++] = c;
			}
			m_LexBuff[m_LexIndex] = 0;
			rV = TOKEN_STRING;
			loop = 0;
			break;
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':	case '-': //deccimal number
			m_LexIndex = 0;
			DoubleValue = FALSE;
			do
			{
				m_LexBuff[m_LexIndex++] = c;
				DoubleValue |= IsDoubleValue(c);
			} while (IsValidDigit(c = GetChar(pcfInFile)));
			UnGetChar(c);
			m_LexBuff[m_LexIndex] = 0;
			loop = 0;
			if (DoubleValue)
			{
				rV = TOKEN_NUMDOUBLE;
				m_LexValueDouble = atof(m_LexBuff);
			}
			else
			{
				rV = TOKEN_NUMINT;
				m_LexValueInt = atoi(m_LexBuff);
			}
			break;
		case ',':
			rV = int(',');
			loop = 0;
			break;
		case '(':
			rV = int('(');
			loop = 0;
			break;
		case ')':
			rV = int(')');
			loop = 0;
			break;
		case '{':
			rV = int('{');
			loop = 0;
			break;
		case '}':
			rV = int('}');
			loop = 0;
			break;
		case '[':
			rV = int('[');
			loop = 0;
			break;
		case ']':
			rV = int(']');
			loop = 0;
			break;
		default:	//keywords
			m_LexIndex = 0;
			m_LexBuff[m_LexIndex++] = c;
			while (isalnum(c = GetChar(pcfInFile)))
			{
				m_LexBuff[m_LexIndex++] = c;
			}
			UnGetChar(c);
			m_LexBuff[m_LexIndex] = 0;
			token = LookUp(m_LexBuff);
			if (token >= TOKEN_ERRORToken)
				rV = token;
			else
				rV = TOKEN_ERRORToken;
			loop = 0;
			break;
		}
	}
	return rV;
}
