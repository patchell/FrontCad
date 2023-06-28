#include "pch.h"

CLexer::CLexer()
{
	m_LexIndex = 0;
	m_Col = 0;
	m_Line = 1;
	m_UnGetBuff = 0;
	m_LexValueInt = 0;
	m_LexValueDouble = 0.0;
	for (int i = 0; i < 256; ++i)
		m_LexBuff[i] = 0;
	m_pIN = 0;
}

CLexer::~CLexer()
{
}

BOOL CLexer::Create(CString& path)
{
	char* s = new char[512];
	BOOL rV = TRUE;

	m_LexIndex = 0;
	m_Col = 0;
	m_Line = 1;
	m_UnGetBuff = 0;
	for (int i = 0; i < 256; ++i)
		m_LexBuff[i] = 0;
	
	fopen_s(&m_pIN, GETAPP.ConvertCStringToChar(s, path), "r");
	if (m_pIN == NULL)
	{
		::MessageBox(NULL, path, _T("ERROR:Can\'t Open"), MB_ICONHAND | MB_OK);
		rV = FALSE;
	}
	delete[] s;
	return rV;
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

CLexer::Tokens CLexer::LookUp(const char *pKW)
{
	CLexer::Tokens rV = CLexer::Tokens( - 1);

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

const char* CLexer::TokenLookup(Tokens TokenVal)
{
	const char*pRs = "Huh?";

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

BOOL CLexer::IsValidDigit(int c)
{
	BOOL rV = FALSE;
	if (isdigit(c) || c == '.')
		rV = TRUE;
	return rV;
}

BOOL CLexer::IsDoubleValue(int c)
{
	BOOL rV = FALSE;

	if ('.' == c)
		rV = TRUE;
	return rV;
}

CLexer::Tokens CLexer::Lex()
{
	int loop;
	CLexer::Tokens rV;
	BOOL DoubleValue;
	int c;
	CLexer::Tokens token;
	loop = 1;
	while (loop)
	{
		c = GetChar();
		switch (c)
		{
			case EOF:
				loop = 0;
				rV = CLexer::Tokens::EOFToken;
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
				rV = CLexer::Tokens::STRING;
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
				} while (IsValidDigit(c = GetChar()));
				UnGetChar(c);
				m_LexBuff[m_LexIndex] = 0;
				loop = 0;
				if (DoubleValue)
				{
					rV = CLexer::Tokens::NUMDOUBLE;
					m_LexValueInt = atof(m_LexBuff);
				}
				else
				{
					rV = CLexer::Tokens::NUMINT;
					m_LexValueInt = atoi(m_LexBuff);
				}
				break;
			case ',': 
				rV = CLexer::Tokens(',');
				loop = 0;
				break;
			case '(':
				rV = CLexer::Tokens('(');
				loop = 0;
				break;
			case ')':
				rV = CLexer::Tokens(')');
				loop = 0;
				break;
			case '{':
				rV = CLexer::Tokens('{');
				loop = 0;
				break;
			case '}':
				rV = CLexer::Tokens('}');
				loop = 0;
				break;
			case '[':
				rV = CLexer::Tokens('[');
				loop = 0;
				break;
			case ']':
				rV = CLexer::Tokens(']');
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
				if (token >= CLexer::Tokens::ERRORToken)
					rV = token;
				else
					rV = CLexer::Tokens::ERRORToken;
				loop = 0;
				break;
		}
	}
	return rV;
}

