#include "pch.h"

CLexerFrontCad::CLexerFrontCad()
{
}

CLexerFrontCad::~CLexerFrontCad()
{
}

BOOL CLexerFrontCad::Create(CArchive& ar)
{
	return CLexer::Create(ar);
}


Token CLexerFrontCad::Lex()
{
	int loop;
	Token rV;
	BOOL DoubleValue;
	int c;
	Token token;
	loop = 1;
	while (loop)
	{
		c = LexGet();
		switch (c)
		{
		case EOF:
			loop = 0;
			rV = Token::EOFToken;
			break;
		case ' ': case '\t':	//whitespace
			break;
		case '\n':	//end of line and white space
			++m_Line;
			m_Col = 0;
			break;
		case '\"':	//string
			m_LexBuffIndex = 0;
			while ((c = LexGet()) != '\"')
			{
				m_pLexBuff[m_LexBuffIndex++] = c;
			}
			m_pLexBuff[m_LexBuffIndex] = 0;
			rV = Token::STRING;
			loop = 0;
			break;
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':	case '-': //deccimal number
			m_LexBuffIndex = 0;
			DoubleValue = FALSE;
			do
			{
				m_pLexBuff[m_LexBuffIndex++] = c;
				DoubleValue |= IsDoubleValue(c);
			} while (IsValidDigit(c = LexGet()));
			LexUnGet(c);
			m_pLexBuff[m_LexBuffIndex] = 0;
			loop = 0;
			if (DoubleValue)
			{
				rV = Token::NUMDOUBLE;
				m_LexValueDouble = atof(m_pLexBuff);
			}
			else
			{
				rV = Token::NUMINT;
				m_LexValueInt = atoi(m_pLexBuff);
			}
			break;
		case ',':
			rV = Token(',');
			loop = 0;
			break;
		case '(':
			rV = Token('(');
			loop = 0;
			break;
		case ')':
			rV = Token(')');
			loop = 0;
			break;
		case '{':
			rV = Token('{');
			loop = 0;
			break;
		case '}':
			rV = Token('}');
			loop = 0;
			break;
		case '[':
			rV = Token('[');
			loop = 0;
			break;
		case ']':
			rV = Token(']');
			loop = 0;
			break;
		default:	//keywords
			m_LexBuffIndex = 0;
			m_pLexBuff[m_LexBuffIndex++] = c;
			while (isalnum(c = LexGet()))
			{
				m_pLexBuff[m_LexBuffIndex++] = c;
			}
			LexUnGet(c);
			m_pLexBuff[m_LexBuffIndex] = 0;
			token = LookUp(m_pLexBuff);
			if (token >= Token::ERRORToken)
				rV = token;
			else
				rV = Token::ERRORToken;
			loop = 0;
			break;
		}
	}
	return rV;
}
