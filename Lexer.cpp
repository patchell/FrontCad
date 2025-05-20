#include "pch.h"

CLexer::CLexer()
{
	m_UngetBuffer = 0;
	m_pLexBuff = new char[LEXER_LEX_BUFF_SIZE];	// Buffer for the current token string
	memset(m_pLexBuff, 0, LEXER_LEX_BUFF_SIZE);
	m_LexBuffIndex = 0;		// index into ^
	m_Line = 0;
	m_Col = 0;
	m_LexValueInt = 0;
	m_pFileBuffeer = 0;	// Buffer the source file is stored in
	m_InFileSize = 0;	// Size of the buffer
	m_FileIndex = 0;	// Current index into file buffer
}

CLexer::~CLexer()
{
	if (m_pLexBuff)
		delete[] m_pLexBuff;
}

BOOL CLexer::Create(CArchive& ar)
{
	BOOL rV = TRUE;

	m_InFileSize = (int)ar.GetFile()->GetLength();
	m_pFileBuffeer = new char[m_InFileSize];
	ar.Read(m_pFileBuffeer, m_InFileSize);
	return rV;
}


Token CLexer::LookUp(const char* pKW)
{
	Token rV = Token(-1);

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


const char* CLexer::TokenLookup(Token TokenVal)
{
	const char* pRs = "Huh?";

	int i, loop;
	for (i = 0, loop = 1; GetKEYWORD(i)->m_pName && loop; ++i)
	{
		if (GetKEYWORD(i)->m_Token == TokenVal)
		{
			loop = 0;
			pRs = GetKEYWORD(i)->m_pName;
		}
	}
	return pRs;
}


int CLexer::LexGet()
{
	int c = EOF;

	if (m_pFileBuffeer && (m_FileIndex < m_InFileSize))
		c = m_pFileBuffeer[m_FileIndex++];
	else if (m_FileIndex == m_InFileSize)
	{
		c = EOF;
	}
	m_Col++;
	return c;
}

void CLexer::LexUnGet(int c)
{
	if ((m_FileIndex > 0) && (c != EOF))
	{
		--m_FileIndex;
		if (m_Col > 0)
			m_Col--;
	}
}

