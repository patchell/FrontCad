#pragma once

class CLexer
{
	CString m_csErrorString;
	int m_UnGetBuff;
	FILE *m_pIN;
	int m_LexIndex;
	char m_LexBuff[256];
	int m_Line;
	int m_Col;
	static KEYWORD TokenStrings[10];
	static BOOL InitTokenStringsFlag;
	static UINT CurrentTokenString;
public:
	inline char *GetLexBuff() { return m_LexBuff; }
public:
	CLexer();
	CLexer(CString& path);
	CLexer(FILE *pIn);
	virtual ~CLexer();
	static void InitTokenStrings();
	DocFileParseToken Lex();
	int GetChar();
	int UnGetChar(int c);
	DocFileParseToken LookUp(const char *pS);
	DocFileParseToken Accept(DocFileParseToken LookaHead, DocFileParseToken Token);
	DocFileParseToken Expect(DocFileParseToken LookaHead, DocFileParseToken Token);
	BOOL IsValidDigit(int c);
	void Error(CString csError);
	static const char *TokenToString(DocFileParseToken Token);
	static KEYWORD* ConvertTokenToString(DocFileParseToken Token);
	//-----------------------------------------------
	// parsing primintives
	//-----------------------------------------------
	DocFileParseToken Point(
		DocFileParseToken token,
		CDoublePoint &p, 
		DocFileParseToken lookaheadtoken
	);
	DocFileParseToken DoubleSize(
		DocFileParseToken token,
		CDoubleSize& sz,
		DocFileParseToken lookaheadtoken
	);
	DocFileParseToken Color(DocFileParseToken token,COLORREF &c, DocFileParseToken lookaheadtoken);
	DocFileParseToken ReadDoubleValue(
		DocFileParseToken typetoken, 
		double& v, 
		DocFileParseToken Token
	);
	DocFileParseToken ReadUINTDecimalValue(DocFileParseToken typetoken, UINT& v, DocFileParseToken Token);
	DocFileParseToken ReadDecimalValue(
		DocFileParseToken typetoken, 
		int &v, 
		DocFileParseToken Token
	);
	DocFileParseToken csString(
		DocFileParseToken typeToken, 
		CString& csS, 
		DocFileParseToken Token
	);
	DocFileParseToken String(
		DocFileParseToken typeToken, 
		char *&s, 
		DocFileParseToken Token
	);
	DocFileParseToken DoubleRect(
		DocFileParseToken TypeToken, 
		CDoubleRect& DoubleRect, 
		DocFileParseToken LookaheadToken
	);
	void UnexpectedToken(DocFileParseToken Token);
};