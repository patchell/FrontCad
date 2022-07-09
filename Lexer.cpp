#include "pch.h"

KEYWORD KeyWords[] =
{
	{DocFileParseToken::DOUBLERECT,"DOUBLERECT"},
	{DocFileParseToken::LINE,"LINE"},
	{DocFileParseToken::RECT,"RECT"},
	{DocFileParseToken::ELIPSE,"ELIPSE"},
	{DocFileParseToken::ROUNDRECT,"ROUNDRECT"},
	{DocFileParseToken::POLYGON,"POLYGON"},
	{DocFileParseToken::LIBPART,"LIBPART"},
	{DocFileParseToken::HOLEROUND,"HOLEROUND"},
	{DocFileParseToken::HOLERECT,"HOLERECT"},
	{DocFileParseToken::HOLERND1FLAT,"HOLERND1FLAT"},
	{DocFileParseToken::HOLERND2FLAT,"HOLDRND2FLAT"},
	{DocFileParseToken::TEXT,"TEXT"},
	{DocFileParseToken::ARC,"ARC"},
	{DocFileParseToken::ARCCENTERED,"ARCCENT"},
	{DocFileParseToken::BITMAP,"BITMAP"},
	{DocFileParseToken::ARROW,"ARROW"},
	{DocFileParseToken::ORIGIN,"ORIGIN"},
	{DocFileParseToken::DIMENSION,"DIMENSION"},
	{DocFileParseToken::COLOR,"COLOR"},
	{ DocFileParseToken::BKCOLOR ,"COLORBK"},
	{ DocFileParseToken::FILLCOLOR , "FILLCOLOR"},
	{ DocFileParseToken::WIDTH , "WIDTH"},
	{ DocFileParseToken::POINT ,"POINT"},
	{ DocFileParseToken::SIZE ,"SIZE"},
	{ DocFileParseToken::DRAWING , "DRAWING"},
	{ DocFileParseToken::RED , "RED"},
	{ DocFileParseToken::GREEN, "GREEN"},
	{ DocFileParseToken::BLUE , "BLUE"},
	{ DocFileParseToken::RADIUS,"RADIUS"},
	{ DocFileParseToken::VERTEX , "VERTEX"},
	{ DocFileParseToken::FONT ,"FONT"},
	{ DocFileParseToken::WEIGHT ,"WEIGHT"},
	{ DocFileParseToken::HEIGHT ,"HEIGHT"},
	{ DocFileParseToken::ANGLE ,"ANGLE"},
	{ DocFileParseToken::TRANSPARENTToken ,"TRANSPARENT"},
	{DocFileParseToken::DOUBLERECT,"DOUBLERECT"},
	{DocFileParseToken::SHAPE,"SHAPE"},
	{DocFileParseToken::START,"START"},
	{DocFileParseToken::END,"END"},
	{DocFileParseToken::CENTER,"CENTER"},
	{DocFileParseToken::ARROW_TIP,"ARROWTIP"},
	{DocFileParseToken::ARROW_END,"ARROWEND"},
	{ DocFileParseToken::ATTRIB_ARC,"ATTR_ARC"},
	{ DocFileParseToken::ATTRIB_ARCANGLE,"ATTR_ARCANGLE"},
	{ DocFileParseToken::ATTRIB_ARCCENTER,"ATTR_ARCCENT"},
	{ DocFileParseToken::ATTRIB_ARROW,"ATTR_ARROW"},
	{ DocFileParseToken::ATTRIB_BITMAP,"ATTR_BITMAP"},
	{ DocFileParseToken::ATTRIB_DIMENSION,"ATTR_DIMENSION"},
	{ DocFileParseToken::ATTRIB_ELLIPSE,"ATTR_ELLIPSE"},
	{ DocFileParseToken::ATTRIB_HOLERECT,"ATTR_HOLERECT"},
	{ DocFileParseToken::ATTRIB_HOLERND1F,"ATTR_HOLERND1F"},
	{ DocFileParseToken::ATTRIB_HOLERND2F,"ATTR_HOLERND2F"},
	{ DocFileParseToken::ATTRIB_HOLERND,"ATTR_HOLERND"},
	{ DocFileParseToken::ATTRIB_LINE,"ATTR_LINE"},
	{ DocFileParseToken::ATTRIB_ORIGIN,"ATTR_ORIGIN"},
	{ DocFileParseToken::ATTRIB_POLYGON,"ATTR_POLYGON"},
	{ DocFileParseToken::ATTRIB_RECT,"ATTR_RECT"},
	{ DocFileParseToken::ATTRIB_RNDRECT,"ATTR_RNDRECT"},
	{ DocFileParseToken::ATTRIB_TEXT,"ATTR_TEXT"},
	{ DocFileParseToken::TEXTCOLOR,"TEXTCOLOR"},
	{ DocFileParseToken::LINECOLOR,"LINECOLOR"},
	{ DocFileParseToken::FORMAT,"FORMAT"},
	//------------------------------------------
	{ DocFileParseToken::NUM,"<number>"},
	{ DocFileParseToken::STRING,"<string>"},
	{DocFileParseToken(0),0}
};

KEYWORD CLexer::TokenStrings[10];
BOOL CLexer::InitTokenStringsFlag = TRUE;
UINT CLexer::CurrentTokenString = 0;

void CLexer::InitTokenStrings()
{
	for (size_t i = 0; i < 10; i++)
	{
		TokenStrings[i].m_pName = new char[128];
		TokenStrings[i].m_Token = DocFileParseToken::ERRORToken;
	}
}

const char* CLexer::TokenToString(DocFileParseToken Token)
{
	return CLexer::ConvertTokenToString(Token)->m_pName;
}

KEYWORD* CLexer::ConvertTokenToString(DocFileParseToken Token)
{
	KEYWORD* pStringToken = 0;
	int i;
	int loop = 1;

	//-------------------------------------
	// if it hasn't been initialized yet,
	// initialize the heap of "Keywords"
	//-------------------------------------

	if (CLexer::InitTokenStringsFlag == TRUE)
	{
		CLexer::InitTokenStringsFlag = FALSE;
		CLexer::InitTokenStrings();
	}
	if (10 < ++CLexer::CurrentTokenString)
		CLexer::CurrentTokenString = 0;

	pStringToken = &CLexer::TokenStrings[CLexer::CurrentTokenString];
	if (CLexer::CurrentTokenString >= 10)
		CLexer::CurrentTokenString = 0;

	if (Token < DocFileParseToken::ERRORToken)
	{
		sprintf_s((char*)pStringToken->m_pName, 128, "%d", int(Token));
		pStringToken->m_Token = Token;
	}
	else if (int(Token) < 256)
	{
		if (isprint(int(Token)))
		{
			sprintf_s((char*)pStringToken->m_pName, 128, "%c", int(Token));
			pStringToken->m_Token = Token;
		}
		else
		{
			strcpy_s((char*)pStringToken->m_pName, 128, "<not printable>");
		}
		pStringToken->m_Token = Token;
	}
	else       // Find the token in the token table
	{
		for (i = 0; KeyWords[i].m_pName && loop; ++i)
		{
			if (Token == KeyWords[i].m_Token)
			{
				strcpy_s((char*)pStringToken->m_pName, 128, KeyWords[i].m_pName);
				pStringToken->m_Token = KeyWords[i].m_Token;
				loop = 0;
			}
		}
	}
	return pStringToken;
}

CLexer::CLexer()
{
	m_LexIndex = 0;
	m_Col = 0;
	m_Line = 1;
	m_UnGetBuff = 0;
	for (int i = 0; i < 256; ++i)
		m_LexBuff[i] = 0;
	m_pIN = 0;
}

CLexer::CLexer(CString &path)
{
	m_LexIndex = 0;
	m_Col = 0;
	m_Line = 1;
	m_UnGetBuff = 0;
	for (int i = 0; i < 256; ++i)
		m_LexBuff[i] = 0;
	_wfopen_s(&m_pIN, path, _T("r"));
	
	if (m_pIN == NULL)
		::MessageBox(NULL, path, _T("ERROR:Can\'t Open"), MB_ICONHAND | MB_OK);
}

CLexer::CLexer(FILE *pIn)
{
	m_LexIndex = 0;
	m_UnGetBuff = 0;
	m_pIN = pIn;
	m_Col = 0;
	m_Line = 1;
	for (int i = 0; i < 256; ++i)
		m_LexBuff[i] = 0;
}

CLexer::~CLexer()
{
}

int CLexer::GetChar()
{
	int rV;

	if (m_UnGetBuff)
	{
		rV = m_UnGetBuff;
		m_UnGetBuff = 0;
	}
	else
	{
		rV = fgetc(m_pIN);
	}
	++m_Col;
	return rV;
}

int CLexer::UnGetChar(int c)
{
	m_UnGetBuff = c;
	if(m_Col)--m_Col;
	return 0;
}

DocFileParseToken CLexer::LookUp(const char *pKW)
{
	DocFileParseToken rV = DocFileParseToken ( - 1);

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

DocFileParseToken CLexer::Accept(DocFileParseToken LookaHead, DocFileParseToken Token)
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
	DocFileParseToken rV;

	if ( LookaHead == Token)
		rV = Lex();
	else
		rV = DocFileParseToken::ERRORToken;
	return rV;
}

DocFileParseToken CLexer::Expect(DocFileParseToken Lookahead, DocFileParseToken Token)
{
	DocFileParseToken rV = DocFileParseToken::ERRORToken;

	if ((rV = Accept(Lookahead, Token)) == DocFileParseToken::ERRORToken)
	{
		//--------------------------------------
		// Houston, we have a syntax error
		//--------------------------------------
		m_csErrorString.Format(
			_T("Error::Line %d  Column %d\nExpected %s\nFound %s"),
			m_Line,
			m_Col,
			CString(TokenToString(Lookahead)).GetBuffer(),
			CString(TokenToString(Token)).GetBuffer()
		);
		throw(m_csErrorString);
	}
	return rV;
}

BOOL CLexer::IsValidDigit(int c)
{
	BOOL rV = FALSE;
	if (isdigit(c) || c == '.')
		rV = TRUE;
	return rV;
}

DocFileParseToken CLexer::Lex()
{
	int loop;
	DocFileParseToken rV;
	int c;
	DocFileParseToken token;
	loop = 1;
	while (loop)
	{
		c = GetChar();
		switch (c)
		{
			case EOF:
				loop = 0;
				rV = DocFileParseToken::EOFToken;
				break;
			case ' ': case '\t':	//whitespace
				break;
			case '\n':	//end of line and white space
				++m_Line;
				m_Col = 0;
				break;
			case '\"':	//string
				m_LexIndex = 0;
				while ((c = GetChar()) != '\"')
				{
					m_LexBuff[m_LexIndex++] = c;
				}
				m_LexBuff[m_LexIndex] = 0;
				rV = DocFileParseToken::STRING;
				loop = 0;
				break;
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':	case '-': //deccimal number
				m_LexIndex = 0;
				do
				{
					m_LexBuff[m_LexIndex++] = c;
				} while (IsValidDigit(c = GetChar()));
				UnGetChar(c);
				m_LexBuff[m_LexIndex] = 0;
				loop = 0;
				rV = DocFileParseToken::NUM;
				break;
			case ',': 
				rV = DocFileParseToken(',');
				loop = 0;
				break;
			case '(':
				rV = DocFileParseToken('(');
				loop = 0;
				break;
			case ')':
				rV = DocFileParseToken(')');
				loop = 0;
				break;
			case '{':
				rV = DocFileParseToken('{');
				loop = 0;
				break;
			case '}':
				rV = DocFileParseToken('}');
				loop = 0;
				break;
			case '[':
				rV = DocFileParseToken('[');
				loop = 0;
				break;
			case ']':
				rV = DocFileParseToken(']');
				loop = 0;
				break;
			default:	//keywords
				m_LexIndex = 0;
				m_LexBuff[m_LexIndex++] = c;
				while (isalnum(c = GetChar()))
				{
					m_LexBuff[m_LexIndex++] = c;
				}
				UnGetChar(c);
				m_LexBuff[m_LexIndex] = 0;
				token = LookUp(m_LexBuff);
				if (token >= DocFileParseToken::ERRORToken)
					rV = token;
				else
					rV = DocFileParseToken::ERRORToken;
				loop = 0;
				break;
		}
	}
	return rV;
}

//--------------------------------------------------
// Misc Parsing Methods that come in handy
//--------------------------------------------------

DocFileParseToken CLexer::Point(
	DocFileParseToken TypeToken, 
	CCadPoint &p, 
	DocFileParseToken LookAheadToken
)
{
	double x, y;

	LookAheadToken = Expect(LookAheadToken, TypeToken);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken('('));
	x = atof(m_LexBuff);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken::NUM);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken(','));
	y = atof(m_LexBuff);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken::NUM);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken(')'));
	p = CCadPoint(x, y);
	return LookAheadToken;
}

DocFileParseToken CLexer::DoubleSize(
	DocFileParseToken TypeToken, 
	CDoubleSize& sz, 
	DocFileParseToken LookAheadToken)
{
	double x, y;

	LookAheadToken = Expect(LookAheadToken, TypeToken);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken('('));
	x = atof(m_LexBuff);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken::NUM);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken(','));
	y = atof(m_LexBuff);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken::NUM);
	LookAheadToken = Expect(LookAheadToken, DocFileParseToken(')'));
	sz = CDoubleSize(x, y);
	return LookAheadToken;
}

DocFileParseToken CLexer::Color(DocFileParseToken Token,COLORREF &c, DocFileParseToken LookaheadToken)
{
	int Red=0, Blue=0, Green=0;
	int loop = 1;

	LookaheadToken = Accept(LookaheadToken, Token);
	LookaheadToken = Expect(LookaheadToken, DocFileParseToken('('));
	while (loop)
	{
		switch (LookaheadToken)
		{
			case DocFileParseToken::RED:
				LookaheadToken = ReadDecimalValue(DocFileParseToken::RED, Red, LookaheadToken);
				break;
			case DocFileParseToken::GREEN:
				LookaheadToken = ReadDecimalValue(DocFileParseToken::GREEN, Green, LookaheadToken);
				break;
			case DocFileParseToken::BLUE:
				LookaheadToken = ReadDecimalValue(DocFileParseToken::BLUE, Blue, LookaheadToken);
				break;
			case DocFileParseToken(',') :
				LookaheadToken = Expect(LookaheadToken, DocFileParseToken(','));
				break;
			case DocFileParseToken(')'):
				loop = 0;
				LookaheadToken = Expect(LookaheadToken, DocFileParseToken(')'));
				break;
			default:
				UnexpectedToken(LookaheadToken);
				break;
		}

	}
	c = RGB(Red, Green, Blue);
	return LookaheadToken;
}

DocFileParseToken CLexer::ReadDoubleValue(
	DocFileParseToken typetoken,	// What this token represents
	double& v,
	DocFileParseToken Token	// Look Ahead Token
)
{
	Token = Accept(Token, typetoken);
	Token = Token, (DocFileParseToken('('), Token);
	v = atof(m_LexBuff);
	Token = Token, (DocFileParseToken::NUM, Token);
	Token = Token, (DocFileParseToken(')'), Token);
	return Token;
}

DocFileParseToken CLexer::ReadUINTDecimalValue(
	DocFileParseToken typetoken,	// What this token represents
	UINT& v,
	DocFileParseToken Token	// Look Ahead Token
)
{
	Token = Accept(Token, typetoken);
	Token = Token, (DocFileParseToken('('), Token);
	v = atoi(m_LexBuff);
	Token = Token, (DocFileParseToken::NUM, Token);
	Token = Token, (DocFileParseToken(')'), Token);
	return Token;
}

DocFileParseToken CLexer::ReadDecimalValue(
	DocFileParseToken typetoken,	// What this token represents
	int& v,
	DocFileParseToken Token	// Look Ahead Token
)
{
	Token = Accept(Token, typetoken);
	Token = Token, (DocFileParseToken('('), Token);
	v = atoi(m_LexBuff);
	Token = Token, (DocFileParseToken::NUM, Token);
	Token = Token, (DocFileParseToken(')'), Token);
	return Token;
}

DocFileParseToken CLexer::csString(
	DocFileParseToken typeToken,
	CString& csS,
	DocFileParseToken Token	// lookahead Token
)
{
	Token = Expect(Token, typeToken);
	Token = Expect(Token, DocFileParseToken('('));
	csS = CString(m_LexBuff);
	Token = Expect(Token, DocFileParseToken::STRING);
	Token = Expect(Token, DocFileParseToken(')'));
	return Token;
}

DocFileParseToken CLexer::String(
	DocFileParseToken typeToken, 
	char *&s, 
	DocFileParseToken Token	// lookahead Token
)
{
	Token = Expect(Token, typeToken);
	Token = Expect(Token, DocFileParseToken('('));
	strcpy_s(s, 256, m_LexBuff);
	Token = Expect(Token, DocFileParseToken::STRING);
	Token = Expect(Token, DocFileParseToken(')'));
	return Token;
}

DocFileParseToken CLexer::CadRect(
	DocFileParseToken TypeToken, 
	CCadRect& Rect, 
	DocFileParseToken LookaheadToken
)
{
	CCadPoint P[4];

	LookaheadToken = Expect(LookaheadToken, TypeToken);
	LookaheadToken = Expect(LookaheadToken, DocFileParseToken('('));
	LookaheadToken = Point(DocFileParseToken::POINT, P[0], LookaheadToken);
	LookaheadToken = Expect(LookaheadToken, DocFileParseToken(','));
	LookaheadToken = Point(DocFileParseToken::POINT, P[1], LookaheadToken);
	LookaheadToken = Expect(LookaheadToken, DocFileParseToken(','));
	LookaheadToken = Point(DocFileParseToken::POINT, P[2], LookaheadToken);
	LookaheadToken = Expect(LookaheadToken, DocFileParseToken(','));
	LookaheadToken = Point(DocFileParseToken::POINT, P[3], LookaheadToken);
	LookaheadToken = Expect(LookaheadToken, DocFileParseToken(')'));
//	DoubleRect.SetPoints(P);
	return LookaheadToken;
}

void CLexer::UnexpectedToken(DocFileParseToken Token)
{
	m_csErrorString.Format(
		_T("Unexpected Token %s\nLine %d  Col %d"),
		CString(TokenToString(Token)).GetBuffer(),
		m_Line,
		m_Col
	);
	throw(m_csErrorString);
}

void CLexer::Error(CString csEString)
{
	m_csErrorString.Format(_T("Error:Line %d Col %d\n%lS"), m_Line, m_Col, csEString.GetString());
	throw(m_csErrorString);
}
