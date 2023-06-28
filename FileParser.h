#pragma once

class CFrontCadDoc;

class CFileParser
{
	CLexer m_Lex;
public:
	CFileParser();
	virtual ~CFileParser();
	BOOL Create(CString& csInputFileName);
	CLexer* GetLexer() { return &m_Lex; }
	BOOL Accept(CLexer::Tokens LookaHead, CLexer::Tokens Token);
	CLexer::Tokens Expect(CLexer::Tokens Token, CLexer::Tokens LookaHead);
	//-------------------------------------
	// Parsing Primative Utillity Methods
	//-------------------------------------
	CLexer::Tokens Color(
		CLexer::Tokens TargetToken,	//type of color object 
		COLORREF& ColorParam,		//Color Value to set 
		CLexer::Tokens Token		//Lookahead Token
	);
	CLexer::Tokens IntValue(
		CLexer::Tokens TargetToken,	//type of color object 
		int& IntegerValue,			//Integer Value to set 
		CLexer::Tokens Token		//Lookahead Token
	);
	CLexer::Tokens UINTValue(
		CLexer::Tokens TargetToken,	//type of color object 
		UINT& UINTValue,			//Integer Value to set 
		CLexer::Tokens Token		//Lookahead Token
	);
	CLexer::Tokens DoubleValue(
		CLexer::Tokens TargetToken,		//type of color object 
		double& IntegerValue,			//Double Value to set 
		CLexer::Tokens Token			//Lookahead Token
	);
	CLexer::Tokens StringValue(
		CLexer::Tokens TargetToken,		//type of color object 
		CString& csS,
		CLexer::Tokens Token		//Lookahead Token
	);
	//----------------------------------------------------
	// Parsing Methods
	//----------------------------------------------------
	BOOL ParseLibFile(CCadLibrary** ppLib);
	BOOL ParseDrawFile(CCadDrawing* pDrawing);
	CLexer::Tokens DrawFileAttributes(CLexer::Tokens  Token, CCadObject* pO);
	CLexer::Tokens DrawFileAttributes2(CLexer::Tokens  Token, CCadObject* pO);
	CLexer::Tokens DrawFileAttributes3(CLexer::Tokens  Token, CCadObject* pO);
//	CLexer::Tokens PrintRect(CLexer::Tokens  Token, CCadObject* pO);
	//----------------------------------------
	// File Save Utils
	//----------------------------------------
	static char* SaveString(char* s, int n, CLexer::Tokens  Token, char* string);
	static char* SaveString(char* s, int n, CLexer::Tokens  Token, CString& csString);
	static char* SaveSize(char* s, int n, CLexer::Tokens  Token, CDoubleSize& size);
	static char* SaveColor(char* s, int n, CLexer::Tokens  Token, COLORREF color);
	static char* SaveIntValue(char* s, int n, CLexer::Tokens  Token, int& value);
	static char* SaveUINTValue(char* s, int n, CLexer::Tokens  Token, UINT& value);
	static char* SaveDoubleValue(char* s, int n, CLexer::Tokens Token, double& value);
};

